/*********************************************************************
    ��Ŀ����:MG82F6D64-DEMO
			���԰�ʹ�� MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=24MHz, SysCLK=24MHz
		����������
			����T2ΪMode6:DutyCaptureģʽ �ָ�ģʽ
			����T3ΪMode6:DutyCaptureģʽ �ָ�ģʽ
			����T3ΪMode6:DutyCaptureģʽ �ָ�ģʽ
		
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
#define MCU_SYSCLK		24000000
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

bit bT2Captureing;
bit bT2CaptureFirst;
bit bT2CaptureOk;
u8 TH2Cnt;
WordTypeDef T2CaptureFirst;
WordTypeDef T2CaptureSecond;

bit bT3Captureing;
bit bT3CaptureFirst;
bit bT3CaptureOk;
u8 TH3Cnt;
WordTypeDef T3CaptureFirst;
WordTypeDef T3CaptureSecond;

bit bT4Captureing;
bit bT4CaptureFirst;
bit bT4CaptureOk;
u8 TH4Cnt;
WordTypeDef T4CaptureFirst;
WordTypeDef T4CaptureSecond;

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
		TF2=0;
		if(bT2Captureing==TRUE)
		{
			TH2Cnt++;
		}
		
	}
	if(EXF2)
	{
		EXF2=0;
		if(bT2Captureing==TRUE)
		{
			if(bT2CaptureFirst==FALSE)
			{
				bT2CaptureFirst=TRUE;
				T2CaptureFirst.B.BHigh=TH2Cnt;
				T2CaptureFirst.B.BLow=RCAP2H;
			}
			else
			{
				bT2Captureing=FALSE;
				bT2CaptureOk=TRUE;
				T2CaptureSecond.B.BHigh=TH2Cnt;
				T2CaptureSecond.B.BLow=TH2;
			}
		}
		IO_TEST_3=!IO_TEST_3;
	}
	if(RCLK_TF2L)
	{
		IO_TEST_0=!IO_TEST_0;
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
		if(bT3Captureing==TRUE)
		{
			TH3Cnt++;
		}
		
	}
	SFR_SetPage(1);				// EXF3����1ҳ����
	if(EXF3)
	{
		EXF3=0;
		if(bT3Captureing==TRUE)
		{
			if(bT3CaptureFirst==FALSE)
			{
				bT3CaptureFirst=TRUE;
				T3CaptureFirst.B.BHigh=TH3Cnt;
				T3CaptureFirst.B.BLow=RCAP3H;
			}
			else
			{
				bT3Captureing=FALSE;
				bT3CaptureOk=TRUE;
				T3CaptureSecond.B.BHigh=TH3Cnt;
				T3CaptureSecond.B.BLow=TH3;
			}
		}
		SFR_SetPage(0);			// P5����Page 0
		IO_TEST_4=!IO_TEST_4;
	}
	SFR_SetPage(1);				// TF3L����1ҳ����
	if(TF3L)
	{
		TF3L=0;
		SFR_SetPage(0); 		// P5����Page 0
		IO_TEST_1=!IO_TEST_1;
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
		if(bT4Captureing==TRUE)
		{
			TH4Cnt++;
		}
	}
	SFR_SetPage(2);				// EXF4����2ҳ����
	if(EXF4)
	{
		EXF4=0;
		if(bT4Captureing==TRUE)
		{
			if(bT4CaptureFirst==FALSE)
			{
				bT4CaptureFirst=TRUE;
				T4CaptureFirst.B.BHigh=TH4Cnt;
				T4CaptureFirst.B.BLow=RCAP4H;
			}
			else
			{
				bT4Captureing=FALSE;
				bT4CaptureOk=TRUE;
				T4CaptureSecond.B.BHigh=TH4Cnt;
				T4CaptureSecond.B.BLow=TH4;
			}
		}
		SFR_SetPage(0); 		// P5����Page 0
		IO_TEST_5=!IO_TEST_5;
	}
	SFR_SetPage(2);				// TF4L����2ҳ����
	if(TF4L)
	{
		TF4L=0;
		SFR_SetPage(0); 		// P5����Page 0
		IO_TEST_2=!IO_TEST_2;
	}
	_pop_(SFRPI);
}

/***********************************************************************************

/*************************************************
��������: char putchar (char c)   
��������: ����printf �Ĵ����������
�������:     char c
�������:     
*************************************************/
char putchar (char c)   
{      
	bit bES;
	bES=ES0;
    ES0=0;        
    S0BUF = c;        
    while(TI0==0);        
    TI0=0;        
    ES0=bES;        
    return 0;
}

/***********************************************************************************
��������:void DelayXus(u8 xUs)
��������:��ʱ���򣬵�λΪus
�������:u8 Us -> *1us  (1~255)
���ز���:     
*************************************************************************************/
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

/***********************************************************************************
��������:void DelayXms(u16 xMs)
��������:��ʱ���򣬵�λΪms
�������:u16 xMs -> *1ms  (1~65535)
���ز���:     
*************************************************************************************/
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
��������:   void InitTimer2(void)
��������:Timer2��ʼ������	
�������:   
���ز���:     
*************************************************************************************/
void InitTimer2(void)
{
	TM_SetT2Mode_DutyCapture();						// ����T2ģʽΪDutyCaptureģʽ
	TM_SetT2Clock_SYSCLKDiv12();					// ����T2ʱ��ԴΪ SYSCLK/12
	TM_SetTL2Clock_SYSCLKDiv12();					// ����T2Lʱ��ԴΪ SYSCLK/12
	TM_EnableAccessTF2L();							// ʹ�ܷ���TF2L��TF2L�ж�
	TM_DisableTR2LC();								// ����TR2LC�Է�ֹEXF2���TR2L
	TM_EnT2Split();									// ʹ��T2�ָ�ģʽ
	
	TM_SetT2LowByte(0);								// ����TL2	  			
	TM_SetT2RLLowByte(0);							// ����RCAP2L  
	
	TM_SetT2HighByte(0);							// ����TH2
	TM_SetT2RLHighByte(0);							// ����RCAP2H

	TM_SetT2Capture_T2EXPin();						// ʹ��T2�ⲿ�ж�ΪT2EX����
	TM_SetT2CKOT2EX_P10P11();						// ʹ��T2EX����ΪP11
	TM_EnableT2EX_DetectFalling();					// ʹ��T2EX���ż���½���
	TM_EnableT2EX_DetectRising();					// ʹ��T2EX���ż��������
	
	TM_EnableTL2();									// ʹ��TL2
	
}

/***********************************************************************************
��������:   void InitTimer3(void)
��������:Timer3��ʼ������
�������:   
���ز���:     
*************************************************************************************/
void InitTimer3(void)
{
	TM_SetT3Mode_DutyCapture();						// ����T3ģʽΪDutyCaptureģʽ
	TM_SetT3Clock_SYSCLKDiv12();					// ����T3ʱ��ԴΪ SYSCLK/12
	TM_SetTL3Clock_SYSCLKDiv12();					// ����T3Lʱ��ԴΪ SYSCLK/12
	TM_DisableTR3LC();								// ����TR3LC�Է�ֹEXF3���TR3L
	TM_EnT3Split();									// ʹ��T3�ָ�ģʽ
	
	TM_SetT3LowByte(0);								// ����TL3	  			
	TM_SetT3RLLowByte(0);							// ����RCAP3L  
	
	TM_SetT3HighByte(0);							// ����TH3
	TM_SetT3RLHighByte(0);							// ����RCAP3H

	TM_SetT3Capture_T3EXPin();						// ʹ��T3�ⲿ�ж�ΪT3EX����
	TM_SetT3CKOT3EX_P33P32();						// ʹ��T3EX����ΪP32
	TM_EnableT3EX_DetectFalling();					// ʹ��T3EX���ż���½���
	TM_EnableT3EX_DetectRising();					// ʹ��T3EX���ż��������

	
	TM_EnableTL3();									// ʹ��TL3
}

/***********************************************************************************
��������:   void InitTimer4(void)
��������:Timer4��ʼ������
�������:   
���ز���:     
*************************************************************************************/
void InitTimer4(void)
{
	TM_SetT4Mode_DutyCapture();						// ����T4ģʽΪDutyCaptureģʽ
	TM_SetT4Clock_SYSCLKDiv12();					// ����T4ʱ��ԴΪ SYSCLK/12
	TM_SetTL4Clock_SYSCLKDiv12();					// ����T4Lʱ��ԴΪ SYSCLK/12
	TM_DisableTR4LC();								// ����TR4LC�Է�ֹEXF4���TR4L
	TM_EnT4Split();									// ʹ��T4�ָ�ģʽ
	
	TM_SetT4LowByte(0);								// ����TL4	  			
	TM_SetT4RLLowByte(0);							// ����RCAP4L  
	
	TM_SetT4HighByte(0);							// ����TH4
	TM_SetT4RLHighByte(0);							// ����RCAP4H

	TM_SetT4Capture_T4EXPin();						// ʹ��T4�ⲿ�ж�ΪT4EX����
	TM_SetT4CKOT4EX_P36P37();						// ʹ��T4EX����ΪP37
	TM_EnableT4EX_DetectFalling();					// ʹ��T4EX���ż���½���
	TM_EnableT4EX_DetectRising();					// ʹ��T4EX���ż��������

	TM_EnableTL4();									// ʹ��TL4
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
	UART0_SetS0BRGValue(S0BRG_BRGRL_57600_2X_24000000_1T);
	
	UART0_EnS0BRG();					// ����S0BRGʱ��
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
	InitUart0_S0BRG();
	
	InitTimer2();			
	InitTimer3();	
	InitTimer4();

	InitInterrupt();		// �жϳ�ʼ��
	
	INT_EnAll();			// ʹ��ȫ���ж�


}

void StartT2CaptureDuty(void)
{
	IO_TEST_0=!IO_TEST_0;
	IO_TEST_3=!IO_TEST_3;
	bT2CaptureFirst=FALSE;
	bT2CaptureOk=FALSE;
	bT2Captureing=TRUE;
	TM_SetT2HighByte(0);
	TH2Cnt=0;
	TM_EnableT2();
}

void StartT3CaptureDuty(void)
{
	IO_TEST_1=!IO_TEST_1;
	IO_TEST_4=!IO_TEST_4;
	bT3CaptureFirst=FALSE;
	bT3CaptureOk=FALSE;
	bT3Captureing=TRUE;
	TM_SetT3HighByte(0);
	TH3Cnt=0;
	TM_EnableT3();
}

void StartT4CaptureDuty(void)
{
	IO_TEST_2=!IO_TEST_2;
	IO_TEST_5=!IO_TEST_5;
	bT4CaptureFirst=FALSE;
	bT4CaptureOk=FALSE;
	bT4Captureing=TRUE;
	TM_SetT4HighByte(0);
	TH4Cnt=0;
	TM_EnableT4();
}

void main()
{
	float duty;
	float freq;
	
    InitSystem();
	
 	LED_G_0=0;LED_R=0;LED_G_1=0;
	DelayXms(1000);
 	LED_G_0=1;LED_R=1;LED_G_1=1;

	StartT2CaptureDuty();
	StartT3CaptureDuty();
	StartT4CaptureDuty();
					
    while(1)
    {
    	DelayXms(500);
    	if(bT2CaptureOk)
    	{
    		duty=(float)(((double)(T2CaptureFirst.W)*100L)/(float)T2CaptureSecond.W);
    		freq=(float)(1000)/((float)(T2CaptureSecond.W)/2);
			printf("\nT2 Caputre: Duty=%f,Freq=%fKHz",duty,freq);
			StartT2CaptureDuty();
    	}
		if(bT3CaptureOk)
		{
    		duty=(float)(((double)(T3CaptureFirst.W)*100L)/(float)T3CaptureSecond.W);
    		freq=(float)(1000)/((float)(T3CaptureSecond.W)/2);
			printf("\nT3 Caputre: Duty=%f,Freq=%fKHz",duty,freq);
			StartT3CaptureDuty();
		}
		if(bT4CaptureOk)
		{
    		duty=(float)(((double)(T4CaptureFirst.W)*100L)/(float)T4CaptureSecond.W);
    		freq=(float)(1000)/((float)(T4CaptureSecond.W)/2);
			printf("\nT4 Caputre: Duty=%f,Freq=%fKHz",duty,freq);
			StartT4CaptureDuty();
		}
    }
}

