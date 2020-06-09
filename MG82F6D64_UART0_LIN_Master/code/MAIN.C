/*********************************************************************
    ��Ŀ����:MG82F6D64-DEMO
			���԰�ʹ�� MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=12MHz, SysCLK=12MHz
	����������
			����0��LIN Master
	ע�����

    ����ʱ��:
    �޸���־:
    
*********************************************************************/
#define _MAIN_C

#include <Intrins.h>
#include <Absacc.h>

#include <Stdio.h>  // for printf

#include ".\include\REG_MG82F6D64.H"
#include ".\include\Type.h"
#include ".\include\API_Macro_MG82F6D64.H"
#include ".\include\API_Uart_BRGRL_MG82F6D64.H"


/*************************************************
����ϵͳʱ��SysClk (MAX.50MHz)
��ѡ: 
	11059200,12000000,
	22118400,24000000,
	29491200,32000000,
	44236800,48000000
*************************************************/
#define MCU_SYSCLK		12000000
/*************************************************/
/*************************************************
����CPUʱ�� CpuClk (MAX.36MHz)
	1) CpuCLK=SysCLK
	2) CpuClk=SysClk/2
*************************************************/
#define MCU_CPUCLK		(MCU_SYSCLK)
//#define MCU_CPUCLK		(MCU_SYSCLK/2)

#define TIMER_1T_1ms_TH	((65536-(u16)(float)(1000*((float)(MCU_SYSCLK)/(float)(1000000)))) /256) 			
#define TIMER_1T_1ms_TL	((65536-(u16)(float)(1000*((float)(MCU_SYSCLK)/(float)(1000000)))) %256)

#define TIMER_12T_1ms_TH	((65536-(u16)(float)(1000*((float)(MCU_SYSCLK)/(float)(12000000)))) /256) 			
#define TIMER_12T_1ms_TL	((65536-(u16)(float)(1000*((float)(MCU_SYSCLK)/(float)(12000000)))) %256)

#define LED_G_0			P55
#define LED_R			P56
#define LED_G_1			P57

#define LIN_TXD			P31
#define LIN_RXD			P30

#define LIN_TIME_MAX				10

#define LIN_SYNC_BYTE				0x55

#define LIN_AUTO_BAUDRATE_FLAG		0		// 0->��ֹ�Զ�У׼������             1->ʹ���Զ�У׼������			

#define LIN_PID_MASTER_TX_MSG0		0x10		
#define LIN_PID_MASTER_TX_MSG1		0x11	

#define LIN_PID_MASTER_RX_MSG0		0x21
#define LIN_PID_MASTER_RX_MSG1		0x31




// ����LIN״̬
typedef enum
{
  LIN_IDLE_BREAK=0,
  LIN_SYNC,
  LIN_PID,
  LIN_MASTER_RX_DATA,
  LIN_MASTER_RX_CHECKSUM,
  LIN_MASTER_RX_DONE,
  LIN_MASTER_TX_DATA,
  LIN_MASTER_TX_CHECKSUM,
  LIN_MASTER_TX_DONE,
  LIN_ERR
}LIN_State;

// ����LINģʽ
typedef enum
{
  	LIN_MODE_MASTER_TX=0,
	LIN_MODE_MASTER_RX,
	LIN_MODE_SLAVE_TX,
	LIN_MODE_SLAVE_RX
}LIN_Mode;

// ����֡�ṹ��
typedef struct
{
	LIN_Mode Mode;
  	LIN_State State;
  	u8 PID ;
  	u8 Data[8];
  	u8 DataLength;
  	u8 DataInx;
  	u8 Check;
} LIN_Frame ;

#define BIT(A,B)      ((A>>B)&0x01)   // A Ϊ���� 

idata LIN_Frame LINFrame;


u8 LedTime;
u8 LINOverTime;

void LIN_Master_StartFrame(void);
u8 LIN_CalcParity(u8 id);
u8 LIN_CalcChecksum(u8 id,u8 *lindata,u8 length);

/***********************************************************************************
��������:   void INT_UART0(void)
��������:UART0 �жϷ������
		 
�������:   
���ز���:     
*************************************************************************************/
void INT_UART0(void) interrupt INT_VECTOR_UART0
{
	_push_(SFRPI);
	SFR_SetPage(0);
	if(RI0==1)
	{
		if(LINFrame.State==LIN_MASTER_RX_DATA)
		{ // Rx data
			
			LINFrame.Data[LINFrame.DataInx] = S0BUF;
			LINFrame.DataInx++;
			if(LINFrame.DataInx>=LINFrame.DataLength)
			{
				LINFrame.State++;
			}

		}
		else if(LINFrame.State==LIN_MASTER_RX_CHECKSUM)
		{ // Rx checksum
			LINFrame.Check=S0BUF;
			LINFrame.State++;
			
		}
		else if(LINFrame.State==LIN_MASTER_RX_DONE)
		{ // Rx done
		
		}
		else
		{
			LINFrame.State=LIN_ERR;
		}
		RI0 = 0;
	}
	else
	{
		TI0 = 0;
		
		if(LINFrame.State==LIN_IDLE_BREAK)
		{
			if((S0CFG1&SBF0)!=0)
			{ // Send break end
				UART0_LIN_ClrSBF0();
				S0BUF=LIN_SYNC_BYTE;		// Send SYNC
				LINFrame.State++;
			}
		}
		/**/
		else if(LINFrame.State==LIN_SYNC)
		{// Send SYNC end
			LINFrame.State++;
			S0BUF=LINFrame.PID;				// Send PID
		}
		/**/
		else if(LINFrame.State==LIN_PID)
		{// Send PID end 
			if(LINFrame.Mode == LIN_MODE_MASTER_RX)
			{
				LINFrame.DataInx=0;
				LINFrame.State= LIN_MASTER_RX_DATA;
				UART0_LIN_SetRX();			// ����Ϊ LIN RXģʽ
				
			}
			else if(LINFrame.Mode == LIN_MODE_MASTER_TX)
			{// Start send DATA
				S0BUF = LINFrame.Data[0];
				LINFrame.DataInx=1;
				LINFrame.State= LIN_MASTER_TX_DATA;
			}
			else
			{ 
				LINFrame.State=LIN_ERR;
			}
		}
		else if(LINFrame.State==LIN_MASTER_TX_DATA)
		{
			
			S0BUF=LINFrame.Data[LINFrame.DataInx];
			LINFrame.DataInx++;
			if(LINFrame.DataInx>=LINFrame.DataLength)
			{
				LINFrame.State++;
			}

		}
		else if(LINFrame.State==LIN_MASTER_TX_CHECKSUM)
		{
			S0BUF=LINFrame.Check;
			LINFrame.State++;
		}
		else if(LINFrame.State==LIN_MASTER_TX_DONE)
		{
		}
		else
		{
			LINFrame.State=LIN_ERR;
		}
	}
	_pop_(SFRPI);
}

/***********************************************************************************
��������:   void INT_T0(void)
��������:T0 �жϷ������
		 ���1ms
�������:   
���ز���:     
*************************************************************************************/
void INT_T0(void) interrupt INT_VECTOR_T0
{
	TH0=TIMER_12T_1ms_TH;
	TL0=TIMER_12T_1ms_TL;

	if(LedTime!=0) LedTime--;
	if(LINOverTime!=0) LINOverTime--;
}


/*************************************************
��������:     void DelayXus(u16 xUs)
��������:   	��ʱ���򣬵�λΪus
�������:     u8 Us -> *1us  (1~255)
�������:     
*************************************************/
void DelayXus(u8 xUs)
{
	while(xUs!=0)
	{
#if (MCU_CPUCLK>=11059200)
		_nop_();
#endif
#if (MCU_CPUCLK>=14745600)
		_nop_();
		_nop_();
		_nop_();
		_nop_();
#endif
#if (MCU_CPUCLK>=16000000)
		_nop_();
#endif

#if (MCU_CPUCLK>=22118400)
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
#endif
#if (MCU_CPUCLK>=24000000)
		_nop_();
		_nop_();
#endif		
#if (MCU_CPUCLK>=29491200)
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
#endif
#if (MCU_CPUCLK>=32000000)
		_nop_();
		_nop_();
#endif

		xUs--;
	}
}

/*************************************************
��������:     void DelayXms(u16 xMs)
��������:     ��ʱ���򣬵�λΪms
�������:     u16 xMs -> *1ms  (1~65535)
�������:     
*************************************************/
void DelayXms(u16 xMs)
{
	while(xMs!=0)
	{
		CLRWDT();
		DelayXus(200);
		DelayXus(200);
		DelayXus(200);
		DelayXus(200);
		DelayXus(200);
		xMs--;
		
	}
}


/***********************************************************************************
��������:   void InitUart0_LIN(void)
��������:   Uart0��ʼ�� Ϊ LIN
�������:   
���ز���:     
*************************************************************************************/
void InitUart0_LIN(void)
{
	
	UART0_SetAccess_S0CR1();			// ����SFR 0xB9��ַ�Ƿ���S0CR1
	UART0_SetModeLIN();					// ���ô���LIN ģʽ				
	UART0_EnReception();				// �������չ���
	UART0_SetBRGFromS0BRG();			// ���ò�������ԴΪ S0BRG
	UART0_SetS0BRGBaudRateX2();			// ����2x������
	UART0_SetS0BRGSelSYSCLK();			// ����S0BRGʱ������ΪSysclk

	// ����S0BRG Value
	UART0_SetS0BRGValue(S0BRG_BRGRL_19200_2X_12000000_1T);
	
	UART0_EnS0BRG();					// ����S0BRGʱ��
	
	UART0_LIN_TxSYNC_16Bit();			// ���� TX break 16bit
	
}

/***********************************************************************************
��������:   void InitTimer0(void)
��������:Timer0��ʼ������
		 ����T0Ϊ16λ��ʱ��,ʱ��ΪSysclk/192 6us~393216us	
�������:   
���ز���:     
*************************************************************************************/
void InitTimer0(void)
{
	TM_SetT0Mode_1_16BIT_TIMER();			// ����T0ģʽΪ16λģʽ
	TM_SetT0Clock_SYSCLKDiv12();					// ����T0ʱ��ԴΪ SYSCLK/12
	TM_SetT0Gate_Disable();

	TM_SetT0LowByte(TIMER_12T_1ms_TL);		// ����T0��8λ
	TM_SetT0HighByte(TIMER_12T_1ms_TH);		// ����T0��8λ

	TM_EnableT0();							// ʹ��T0
}


/***********************************************************************************
��������:   void InitInterrupt(void)
��������:   �жϳ�ʼ��
�������:   
���ز���:     
*************************************************************************************/
void InitInterrupt(void)
{
//	INT_EnUART0();				// ʹ��T0�ж�
	INT_EnTIMER0();				// ʹ��T0�ж�
}	

/***********************************************************************************
��������:   void InitPort(void)
��������:   IO����
�������:   
���ز���:     
*************************************************************************************/
void InitPort(void)
{
	PORT_SetP5PushPull(BIT2|BIT3|BIT4|BIT5|BIT6|BIT7);					// ����P52,P53,P54,P55,P56,P57Ϊ�������
	PORT_SetP3QuasiBi(BIT0|BIT1|BIT3|BIT4|BIT5);						// ���� P30,P31,P33,P34,P35 ��Ϊ׼˫���
}

/***********************************************************************************
��������:   void InitClock(void)
��������:   ʱ�ӳ�ʼ��	
�������:   
���ز���:     
*************************************************************************************/
void InitClock(void)
{
#if (MCU_SYSCLK==11059200)
#if (MCU_CPUCLK==MCU_SYSCLK)
	// SysClk=11.0592MHz CpuClk=11.0592MHz
	CLK_SetCKCON0(IHRCO_110592MHz|CPUCLK_SYSCLK_DIV_1|SYSCLK_MCKDO_DIV_1);
	
#else
	// SysClk=11.0592MHz CpuClk=5.5296MHz
	CLK_SetCKCON0(IHRCO_110592MHz|CPUCLK_SYSCLK_DIV_2|SYSCLK_MCKDO_DIV_1);
#endif
#endif

#if (MCU_SYSCLK==12000000)
#if (MCU_CPUCLK==MCU_SYSCLK)
	// SysClk=12MHz CpuClk=12MHz
	CLK_SetCKCON0(IHRCO_12MHz|CPUCLK_SYSCLK_DIV_1|SYSCLK_MCKDO_DIV_1);
	
#else
	// SysClk=12MHz CpuClk=6MHz
	CLK_SetCKCON0(IHRCO_12MHz|CPUCLK_SYSCLK_DIV_2|SYSCLK_MCKDO_DIV_1);
#endif
#endif

#if (MCU_SYSCLK==22118400)
#if (MCU_CPUCLK==MCU_SYSCLK)
	// SysClk=22.1184MHz CpuClk=22.1184MHz
	CLK_SetCKCON0(IHRCO_110592MHz|CPUCLK_SYSCLK_DIV_1|SYSCLK_MCKDO_DIV_1|ENABLE_CKM|CKM_OSCIN_DIV_2);
	DelayXus(100);
	// IHRCO, MCK=CKMIx4, OSCin=IHRCO
	CLK_SetCKCON2(ENABLE_IHRCO|MCK_CKMI_X4|OSCIn_IHRCO);
#else
	// SysClk=22.1184MHz CpuClk=11.0592MHz
	CLK_SetCKCON0(IHRCO_110592MHz|CPUCLK_SYSCLK_DIV_2|SYSCLK_MCKDO_DIV_1|ENABLE_CKM|CKM_OSCIN_DIV_2);
	DelayXus(100);
	// IHRCO, MCK=CKMIx4, OSCin=IHRCO
	CLK_SetCKCON2(ENABLE_IHRCO|MCK_CKMI_X4|OSCIn_IHRCO);
#endif
#endif

#if (MCU_SYSCLK==24000000)
#if (MCU_CPUCLK==MCU_SYSCLK)
	// SysClk=24MHz CpuClk=24MHz
	CLK_SetCKCON0(IHRCO_12MHz|CPUCLK_SYSCLK_DIV_1|SYSCLK_MCKDO_DIV_1|ENABLE_CKM|CKM_OSCIN_DIV_2);
	DelayXus(100);
	// IHRCO, MCK=CKMIx4, OSCin=IHRCO
	CLK_SetCKCON2(ENABLE_IHRCO|MCK_CKMI_X4|OSCIn_IHRCO);
#else
	// SysClk=24MHz CpuClk=12MHz
	CLK_SetCKCON0(IHRCO_12MHz|CPUCLK_SYSCLK_DIV_2|SYSCLK_MCKDO_DIV_1|ENABLE_CKM|CKM_OSCIN_DIV_2);
	DelayXus(100);
	// IHRCO, MCK=CKMIx4, OSCin=IHRCO
	CLK_SetCKCON2(ENABLE_IHRCO|MCK_CKMI_X4|OSCIn_IHRCO);
#endif
#endif

#if (MCU_SYSCLK==29491200)
#if (MCU_CPUCLK==MCU_SYSCLK)
	// Cpuclk high speed
	CLK_SeCpuCLK_HighSpeed();
	// SysClk=29.491200MHz CpuClk=29.491200MHz
	CLK_SetCKCON0(IHRCO_110592MHz|CPUCLK_SYSCLK_DIV_1|SYSCLK_MCKDO_DIV_1|ENABLE_CKM|CKM_OSCIN_DIV_2);
	DelayXus(100);
	// IHRCO, MCK=CKMIx5.33, OSCin=IHRCO
	CLK_SetCKCON2(ENABLE_IHRCO|MCK_CKMI_X533|OSCIn_IHRCO);
#else
	// SysClk=29.491200MHz CpuClk=14.7456MHz
	CLK_SetCKCON0(IHRCO_110592MHz|CPUCLK_SYSCLK_DIV_2|SYSCLK_MCKDO_DIV_1|ENABLE_CKM|CKM_OSCIN_DIV_2);
	DelayXus(100);
	// IHRCO, MCK=CKMIx5.33, OSCin=IHRCO
	CLK_SetCKCON2(ENABLE_IHRCO|MCK_CKMI_X533|OSCIn_IHRCO);
#endif
#endif

#if (MCU_SYSCLK==32000000)
#if (MCU_CPUCLK==MCU_SYSCLK)
	// Cpuclk high speed
	CLK_SeCpuCLK_HighSpeed();
	// SysClk=32MHz CpuClk=32MHz
	CLK_SetCKCON0(IHRCO_12MHz|CPUCLK_SYSCLK_DIV_1|SYSCLK_MCKDO_DIV_1|ENABLE_CKM|CKM_OSCIN_DIV_2);
	DelayXus(100);
	// IHRCO, MCK=CKMIx5.33, OSCin=IHRCO
	CLK_SetCKCON2(ENABLE_IHRCO|MCK_CKMI_X533|OSCIn_IHRCO);
#else
	// SysClk=32MHz CpuClk=16MHz
	CLK_SetCKCON0(IHRCO_12MHz|CPUCLK_SYSCLK_DIV_2|SYSCLK_MCKDO_DIV_1|ENABLE_CKM|CKM_OSCIN_DIV_2);
	DelayXus(100);
	// IHRCO, MCK=CKMIx5.33, OSCin=IHRCO
	CLK_SetCKCON2(ENABLE_IHRCO|MCK_CKMI_X533|OSCIn_IHRCO);
#endif
#endif

#if (MCU_SYSCLK==36000000)
#if (MCU_CPUCLK==MCU_SYSCLK)
	// Cpuclk high speed
	CLK_SeCpuCLK_HighSpeed();
	// CKMIx6,x8,x12
	CLK_SetCKM_x6x8x12();	
	// SysClk=36MHz CpuClk=18MHz
	CLK_SetCKCON0(IHRCO_12MHz|CPUCLK_SYSCLK_DIV_1|SYSCLK_MCKDO_DIV_1|ENABLE_CKM|CKM_OSCIN_DIV_2);
	DelayXus(100);
	// IHRCO, MCK=CKMIx6, OSCin=IHRCO
	CLK_SetCKCON2(ENABLE_IHRCO|MCK_CKMI_X4_X6|OSCIn_IHRCO);
#else
	// CKMIx6,x8,x12
	CLK_SetCKM_x6x8x12();	
	// SysClk=36MHz CpuClk=18MHz
	CLK_SetCKCON0(IHRCO_12MHz|CPUCLK_SYSCLK_DIV_2|SYSCLK_MCKDO_DIV_1|ENABLE_CKM|CKM_OSCIN_DIV_2);
	DelayXus(100);
	// IHRCO, MCK=CKMIx6, OSCin=IHRCO
	CLK_SetCKCON2(ENABLE_IHRCO|MCK_CKMI_X4_X6|OSCIn_IHRCO);
#endif
#endif


#if (MCU_SYSCLK==44236800)
	// SysClk=44.2368MHz CpuClk=22.1184MHz
	CLK_SetCKCON0(IHRCO_110592MHz|CPUCLK_SYSCLK_DIV_1|SYSCLK_MCKDO_DIV_1|ENABLE_CKM|CKM_OSCIN_DIV_2);
	DelayXus(100);
	// IHRCO, MCK=CKMIx8, OSCin=IHRCO
	CLK_SetCKCON2(ENABLE_IHRCO|MCK_CKMI_X8|OSCIn_IHRCO);
#endif

#if (MCU_SYSCLK==48000000)
	// SysClk=48MHz CpuClk=24MHz
	CLK_SetCKCON0(IHRCO_12MHz|CPUCLK_SYSCLK_DIV_2|SYSCLK_MCKDO_DIV_1|ENABLE_CKM|CKM_OSCIN_DIV_2);
	DelayXus(100);
	// IHRCO, MCK=CKMIx8, OSCin=IHRCO
	CLK_SetCKCON2(ENABLE_IHRCO|MCK_CKMI_X8|OSCIn_IHRCO);
#endif

	// P60 ��� MCK/4
	//CLK_P60OC_MCKDiv4();
}


/***********************************************************************************
��������:   void InitSystem(void)
��������:   ϵͳ��ʼ��
�������:   
���ز���:     
*************************************************************************************/
void InitSystem(void)
{
	InitPort();
	InitClock();
	InitTimer0();
	InitUart0_LIN();
	InitInterrupt();
}


/*
*************************************************************************************
*  Function
*
*************************************************************************************

/***********************************************************************************
��������:   void LIN_Master_StartFrame(void)
��������:   LIN Master ��ʼһ��֡��ͨ��
�������:   
���ز���:     
*************************************************************************************/
void LIN_Master_StartFrame(void)
{
	UART0_LIN_SetTX();
	UART0_LIN_ClrTXER0();
	UART0_LIN_ClrSBF0();
	TI0=0;
	UART0_LIN_SendSYNC();
	S0BUF=0x55;
}

/***********************************************************************************
��������:	uint8 LIN_CalcParity(uint8 id)
��������: LIN��żУ�����
�������:	
���ز���:	  
*************************************************************************************/
u8 LIN_CalcParity(u8 id)
{
	u8 parity, p0,p1;
	parity=id; 
	p0=(BIT(parity,0)^BIT(parity,1)^BIT(parity,2)^BIT(parity,4))<<6;     //żУ��λ
	p1=(!(BIT(parity,1)^BIT(parity,3)^BIT(parity,4)^BIT(parity,5)))<<7;  //��У��λ
	parity|=(p0|p1);
	return parity;
}


/***********************************************************************************
��������:	u8 LIN_CalcChecksum(uint8 id,uint8 *data,uint8 length)
��������: LINУ��ͼ���
�������:	
���ز���:	  
*************************************************************************************/
u8 LIN_CalcChecksum(u8 id,u8 *lindata,u8 length)
{
	u8 i;
	u16 check_sum = 0;
	//0x3C 0x3D ʹ�ñ�׼��У��
	if(id != 0x3c && id != 0x7d)  //ʹ����ǿ��У��
	{
	 	check_sum  = id ;
	} 
	else //ʹ�ñ�׼У��
	{
	 	check_sum = 0 ;
	}
	for (i = 0; i < length; i++) 
	{    
	 	 check_sum += *(lindata++);
	  
	  	if (check_sum > 0xFF)      //��λ
	  	{
	  		check_sum -= 0xFF;
	  	}
	}
	return (~check_sum);  //ȡ��
}




/*
*************************************************************************************
*/ 

/***********************************************************************************
��������:	void TestMasterTxMsgToSlave(u8 uPID)
��������: ���Է������ݵ��ӻ�
�������:	u8 uPID: PID ->0x00~0x3F
���ز���:	  
*************************************************************************************/
void TestMasterTxMsgToSlave(u8 uPID)
{
	u8 i;

	// Master Tx Test
	LINFrame.Mode=LIN_MODE_MASTER_TX;
	LINFrame.PID=LIN_CalcParity(uPID);
	LINFrame.DataInx=0;
	LINFrame.DataLength=8;						// �������ݳ���Ϊ8 Byte
	for(i=0;i<8;i++)
	{
		LINFrame.Data[i]=i+0x30+uPID;			//  ���ò�������
	}
	LINFrame.Check=LIN_CalcChecksum(LINFrame.PID, &LINFrame.Data[0],LINFrame.DataLength);
	LINFrame.State=LIN_IDLE_BREAK;
	LIN_Master_StartFrame();
	TI0=0;RI0=0;
	INT_EnUART0();		// ʹ�� UART0�ж�
	
	LINOverTime=20;
	while (LINOverTime!=0)
	{
		if((LINFrame.State==LIN_ERR)||(LINFrame.State==LIN_MASTER_TX_DONE))
		{
		  break;
		}
	}
	INT_DisUART0(); 	// ��ֹ UART0�ж�
	if(LINFrame.State==LIN_MASTER_TX_DONE)
	{// ���ͳɹ�
		LED_R=0;
	}
	DelayXms(2);
	LED_R=1;
}

/***********************************************************************************
��������:	void TestMasterRxMsgFromSlave(u8 uPID)
��������: ���Զ�ȡ�ӻ�������
�������:	u8 uPID: PID ->0x00~0x3F
���ز���:	  
*************************************************************************************/
void TestMasterRxMsgFromSlave(u8 uPID)
{
	u8 i;
	// Master Rx Test
	LINFrame.Mode=LIN_MODE_MASTER_RX;
	LINFrame.PID=LIN_CalcParity(uPID);
	LINFrame.DataInx=0;
	LINFrame.DataLength=8;
	for(i=0;i<8;i++)
	{
		LINFrame.Data[i]=0x00;
	}
	LINFrame.State=LIN_IDLE_BREAK;
	LIN_Master_StartFrame();
	TI0=0;RI0=0;
	INT_EnUART0();		// ʹ�� UART0�ж�
	
	LINOverTime=20;
	while (LINOverTime!=0)
	{
		if((LINFrame.State==LIN_ERR)||(LINFrame.State==LIN_MASTER_RX_DONE))
		{
		  break;
		}
	}
	INT_DisUART0(); 	// ��ֹ UART0�ж�
	if(LINFrame.State==LIN_MASTER_RX_DONE)
	{// ���ճɹ�
		// У������
		if(LINFrame.Check == LIN_CalcChecksum(LINFrame.PID, &LINFrame.Data[0], LINFrame.DataLength))
		{
			LED_R=0;
		}
	}
	DelayXms(2);
	LED_R=1;
}

void main()
{
	u8 uPID;
	
	InitSystem();
	
	LED_G_0=0;LED_R=0;LED_G_1=0;
	DelayXms(1000);
	LED_G_0=1;LED_R=1;LED_G_1=1;	

	INT_EnAll();		// ʹ��ȫ���ж�

	uPID=0x00;
	
	while(1)
    {
    	DelayXms(50);
		if(uPID<0x20)
		{
			LED_G_0=!LED_G_0;
			LED_G_1=1;
			TestMasterTxMsgToSlave(uPID);
		}
		else
		{
			LED_G_1=!LED_G_1;
    		LED_G_0=1;
			TestMasterRxMsgFromSlave(uPID);
		}
		uPID++;
		if(uPID>=0x40)
		{
			uPID=0x00;
		}
		
    }

}

