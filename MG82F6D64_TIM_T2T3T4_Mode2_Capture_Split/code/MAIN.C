/*********************************************************************
    ��Ŀ����:MG82F6D64-DEMO
			���԰�ʹ�� MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=32MHz, SysCLK=32MHz
		����������
			����T2ΪMode2:����ģʽ,�ָ�ģʽ
			����T3ΪMode2:����ģʽ,�ָ�ģʽ
			����T4ΪMode2:����ģʽ,�ָ�ģʽ
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
#define MCU_SYSCLK		32000000
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

#define IO_TEST_0		P52
#define IO_TEST_1		P53
#define IO_TEST_2		P54
#define IO_TEST_3		P55
#define IO_TEST_4		P56
#define IO_TEST_5		P57

#define LED_G_0			P55
#define LED_R			P56
#define LED_G_1			P57

#define CAPTURE_BUF_SIZE	8
bit bT2Capture;
xdata u8 T2CaptureData[CAPTURE_BUF_SIZE];
u8 T2CaptureCnt;

bit bT3Capture;
xdata u8 T3CaptureData[CAPTURE_BUF_SIZE];
u8 T3CaptureCnt;

bit bT4Capture;
xdata u8 T4CaptureData[CAPTURE_BUF_SIZE];
u8 T4CaptureCnt;


/***********************************************************************************
��������:   void INT_T2(void)
��������:T2 �жϷ������
�������:   
���ز���:     
*************************************************************************************/
void INT_T2(void) interrupt INT_VECTOR_T2
{
	_push_(SFRPI);
	SFR_SetPage(0);			// TF2,EXF2����0ҳ����, ����������ҳ��Ϊ0, �������ж����豣��SFRPI
	if(TF2)
	{
		IO_TEST_0=!IO_TEST_0;
		TF2=0;
	}
	if(EXF2)
	{
		EXF2=0;
		if(bT2Capture==0)
		{
			T2CaptureData[T2CaptureCnt]=RCAP2H;
			T2CaptureCnt++;
			if(T2CaptureCnt>=CAPTURE_BUF_SIZE)
			{
				bT2Capture=TRUE;
			}
		}
	}
	if(RCLK_TF2L)
	{
		IO_TEST_3=!IO_TEST_3;
		RCLK_TF2L=0;
	}
	_pop_(SFRPI);
}

/***********************************************************************************
��������:   void INT_T3(void)
��������:T3 �жϷ������
�������:   
���ز���:     
*************************************************************************************/
void INT_T3(void) interrupt INT_VECTOR_T3
{
	_push_(SFRPI);
	SFR_SetPage(1);				// TF3����1ҳ����
	if(TF3)
	{
		TF3=0;
		SFR_SetPage(0);			// P5����Page 0
		IO_TEST_1=!IO_TEST_1;
	}
	SFR_SetPage(1);				// EXF3����1ҳ����
	if(EXF3)
	{
		EXF3=0;
		if(bT3Capture==0)
		{
			T3CaptureData[T3CaptureCnt]=RCAP3H;
			T3CaptureCnt++;
			if(T3CaptureCnt>=CAPTURE_BUF_SIZE)
			{
				bT3Capture=TRUE;
			}
		}
	}
	SFR_SetPage(1);				// TF3L����1ҳ����
	if(TF3L)
	{
		TF3L=0;
		SFR_SetPage(0); 		// P5����Page 0
		IO_TEST_4=!IO_TEST_4;
	}
	_pop_(SFRPI);
}

/***********************************************************************************
��������:   void INT_T4(void)
��������:T4 �жϷ������
�������:   
���ز���:     
*************************************************************************************/
void INT_T4(void) interrupt INT_VECTOR_T4
{
	_push_(SFRPI);
	SFR_SetPage(2);				// TF4����2ҳ����
	if(TF4)
	{
		TF4=0;
		SFR_SetPage(0);			// P5����Page 0
		IO_TEST_2=!IO_TEST_2;
	}
	SFR_SetPage(2);				// EXF4����2ҳ����
	if(EXF4)
	{
		EXF4=0;
		if(bT4Capture==0)
		{
			T4CaptureData[T4CaptureCnt]=RCAP4H;
			T4CaptureCnt++;
			if(T4CaptureCnt>=CAPTURE_BUF_SIZE)
			{
				bT4Capture=TRUE;
			}
		}
	}
	SFR_SetPage(2);				// TF4L����2ҳ����
	if(TF4L)
	{
		TF4L=0;
		SFR_SetPage(0); 		// P5����Page 0
		IO_TEST_5=!IO_TEST_5;
	}
	_pop_(SFRPI);
}

/***********************************************************************************

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
��������:void SendByte(u8 tByte)
��������:����0��������
�������: u8 tByte: �����͵�����
���ز���:     
*************************************************************************************/
void SendByte(u8 tByte)
{
	bit bES0;
	bES0=ES0;
	ES0=0;
	S0BUF=tByte;
	while(TI0==0);
	TI0=0;
	ES0=bES0;
}



/***********************************************************************************
��������:void SendStr(u8* PStr)
��������:�����ַ���
�������: u8* PStr:�ַ�����ʼ��ַ
���ز���:     
*************************************************************************************/
void SendStr(u8* PStr)
{
	while(*PStr != 0)
	{
		SendByte(*PStr);
		PStr ++;
	}
}

/***********************************************************************************
��������:   void SendHex(u8 ToSend)
��������:���ַ���ʽ����16������
�������: u8 ToSend: �����͵�16����  
���ز���:     
*************************************************************************************/
void SendHex(u8 ToSend)
{
	u8 x;
//	SendByte(' ');
//	SendByte('0');
//	SendByte('x');
	x = ToSend >> 4;
	if(x < 10)
	{
		SendByte('0'+x);
	}
	else
	{
		SendByte('A'-10+x);
	}
	x= ToSend&0x0F;
	if(x < 10)
	{
		SendByte('0'+x);
	}
	else
	{
		SendByte('A'-10+x);
	}
}

/***********************************************************************************
��������:   void InitTimer2(void)
��������:Timer2��ʼ������	
�������:   
���ز���:     
*************************************************************************************/
void InitTimer2(void)
{
	TM_SetT2Mode_Capture();					// ����T2ģʽ2Ϊ����ģʽ
	TM_SetT2Clock_SYSCLKDiv12();			// ����T2ʱ��ԴΪ SYSCLK/12
	TM_SetTL2Clock_SYSCLKDiv12();			// ����T2Lʱ��ԴΪ SYSCLK/12
	TM_EnableAccessTF2L();					// ʹ�ܷ���TF2L��TF2L�ж�
	TM_DisableTR2LC();						// ����TR2LC�Է�ֹTH2����ʱ���TL2
	TM_EnT2Split();							// ʹ��T2�ָ�ģʽ
	
	TM_SetT2LowByte(0);						// ����T2��8λ				
	TM_SetT2RLLowByte(0x40);				// ��������ֵ��8λ		  ---T2L���Ƶ��Ϊ SYSCLK/12/(256-0x40)

	TM_SetT2HighByte(0);					// ����T2��8λ
	TM_SetT2RLHighByte(0);					// ��������ֵ��8λ

	TM_SetT2Capture_T2EXPin();				// ʹ��T2�ⲿ�ж�ΪT2EX����
	TM_SetT2CKOT2EX_P10P11();				// ʹ��T2EX����ΪP11
	TM_EnableT2EX_DetectFalling();			// ʹ��T2EX���ż���½���

	TM_EnableT2();							// ʹ��T2
	TM_EnableTL2();							// ʹ��TL2
}

/***********************************************************************************
��������:   void InitTimer3(void)
��������:Timer3��ʼ������
�������:   
���ز���:     
*************************************************************************************/
void InitTimer3(void)
{
	TM_SetT3Mode_Capture();					// ����T3ģʽ2Ϊ����ģʽ
	TM_SetT3Clock_SYSCLKDiv12();			// ����T3ʱ��ԴΪ SYSCLK/12
	TM_SetTL3Clock_SYSCLKDiv12();			// ����T3Lʱ��ԴΪ SYSCLK/12
	
	
	TM_SetT3LowByte(0);						// ����T3��8λ				
	TM_SetT3RLLowByte(0x80);				// ��������ֵ��8λ		  ---T3L���Ƶ��Ϊ SYSCLK/12/(256-0x80)

	TM_SetT3HighByte(0);					// ����T3��8λ
	TM_SetT3RLHighByte(0);					// ��������ֵ��8λ

	TM_SetT3Capture_T3EXPin();				// ʹ��T3�ⲿ�ж�ΪT3EX����
	TM_SetT3CKOT3EX_P33P32();				// ����T3/T3CKO:P33,T3EX:P32
	TM_EnableT3EX_DetectFalling();			// ʹ��T3EX���ż���½���
	TM_EnT3Split();							// ʹ��T3�ָ�ģʽ
	
	TM_DisableTR3LC();						// ����TR3LC�Է�ֹTH3����ʱ���TL3

	TM_EnableT3();							// ʹ��T3
	TM_EnableTL3();							// ʹ��TL3

}

/***********************************************************************************
��������:   void InitTimer4(void)
��������:Timer4��ʼ������
�������:   
���ز���:     
*************************************************************************************/
void InitTimer4(void)
{
	TM_SetT4Mode_Capture();					// ����T4ģʽ2Ϊ����ģʽ
	TM_SetT4Clock_SYSCLKDiv12();			// ����T4ʱ��ԴΪ SYSCLK/12
	TM_SetTL4Clock_SYSCLKDiv12();			// ����T4Lʱ��ԴΪ SYSCLK/12
	
	
	TM_SetT4LowByte(0);						// ����T4��8λ				
	TM_SetT4RLLowByte(0x80);				// ��������ֵ��8λ		  ---T4L���Ƶ��Ϊ SYSCLK/12/(256-0x80)

	TM_SetT4HighByte(0);					// ����T4��8λ
	TM_SetT4RLHighByte(0);					// ��������ֵ��8λ

	TM_SetT4Capture_T4EXPin();				// ʹ��T4�ⲿ�ж�ΪT4EX����
	TM_SetT4CKOT4EX_P36P37();				// ����T4/T4CKO:P36,T4EX:P37
	TM_EnableT4EX_DetectFalling();			// ʹ��T4EX���ż���½���
	TM_EnT4Split();							// ʹ��T4�ָ�ģʽ
	
	TM_DisableTR4LC();						// ����TR4LC�Է�ֹTH4����ʱ���TL4

	TM_EnableT4();							// ʹ��T4
	TM_EnableTL4();							// ʹ��TL4

}

/***********************************************************************************
��������:   void InitInterrupt(void)
��������:InitInterrupt��ʼ������
�������:   
���ز���:     
*************************************************************************************/
void InitInterrupt(void)
{
	INT_EnTIMER2();
	INT_EnTIMER2L();						// ʹ��TF2L�ж�
	INT_EnTIMER3();
	INT_EnTIMER3L();						// ʹ��TF3L�ж�
	INT_EnTIMER4();
	INT_EnTIMER4L();						// ʹ��TF4L�ж�
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
	PORT_SetP1OpenDrainPu(BIT1);										// ����P11Ϊ��©������������T2EX����
	PORT_SetP3QuasiBi(BIT2|BIT3|BIT6|BIT7);								// ����P32,P37Ϊ׼˫��ڣ�����T3EX,T4EX����
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
��������:   void InitUart0(void)
��������:   Uart0��ʼ��
		 ʹ��S0BRG��Ϊ��������Դ
�������:   
���ز���:     
*************************************************************************************/
void InitUart0_S0BRG(void)
{
	
	UART0_SetAccess_S0CR1();			// ����SFR 0xB9��ַ�Ƿ���S0CR1
	UART0_SetMode8bitUARTVar();			// 8λ �ɱ䲨����
	UART0_EnReception();				// ʹ�ܽ���
	UART0_SetBRGFromS0BRG();			// ���ò�������ԴΪ S0BRG
	UART0_SetS0BRGBaudRateX2();			// ����2x������
	UART0_SetS0BRGSelSYSCLK();			// ����S0BRGʱ������ΪSysclk

	// ����S0BRG Value
	UART0_SetS0BRGValue(S0BRG_BRGRL_57600_2X_32000000_1T);
	
	UART0_EnS0BRG();					// ����S0BRGʱ��
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
	InitUart0_S0BRG();
	
	InitTimer2();			
	InitTimer3();			
	InitTimer4();
	
	InitInterrupt();		// �жϳ�ʼ��
	
	INT_EnAll();			// ʹ��ȫ���ж�

}



void main()
{
	
	u8 i;
	
    InitSystem();
	
 	LED_G_0=0;LED_G_1=0;LED_R=0;
	DelayXms(1000);
 	LED_G_0=1;LED_G_1=1;LED_R=1;

					
    while(1)
    {
    	DelayXms(100);
    	if(bT2Capture)
    	{
			SendStr("\nT2 Caputre:");
			for(i=0;i<CAPTURE_BUF_SIZE;i++)
			{
				SendByte('0');
				SendByte('x');
				SendHex(T2CaptureData[i]);
				SendByte(' ');
			}
			T2CaptureCnt=0;
			bT2Capture=0;
    	}
		if(bT3Capture)
		{
			SendStr("\nT3 Caputre:");
			for(i=0;i<CAPTURE_BUF_SIZE;i++)
			{
				SendByte('0');
				SendByte('x');
				SendHex(T3CaptureData[i]);
				SendByte(' ');
			}
			T3CaptureCnt=0;
			bT3Capture=0;
		}
		if(bT4Capture)
		{
			SendStr("\nT4 Caputre:");
			for(i=0;i<CAPTURE_BUF_SIZE;i++)
			{
				SendByte('0');
				SendByte('x');
				SendHex(T4CaptureData[i]);
				SendByte(' ');
			}
			T4CaptureCnt=0;
			bT4Capture=0;
		}
    }
}

