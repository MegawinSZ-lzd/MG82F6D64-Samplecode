/*********************************************************************
    ��Ŀ����:MG82F6D64-DEMO
			���԰�ʹ�� MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=32MHz, SysCLK=32MHz
		����������
			ȫ�ֿ���T0��T1��T2��T3,T4��ʱ�� 	
			
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

#define IO_TEST_0		P52
#define IO_TEST_1		P53
#define IO_TEST_2		P54

#define LED_G_0			P55
#define LED_R			P56
#define LED_G_1			P57

/***********************************************************************************
��������:   void INT_T0(void)
��������:T0 �жϷ������
		 ���1ms
�������:   
���ز���:     
*************************************************************************************/
void INT_T0(void) interrupt INT_VECTOR_T0
{

	IO_TEST_0=!IO_TEST_0;			//TF0�ڽ����ж�ʱ�ᱻӲ���Զ��������˲���Ҫif(TF0)
}

/***********************************************************************************
��������:   void INT_T0(void)
��������:T0 �жϷ������
		 ���1ms
�������:   
���ز���:     
*************************************************************************************/
void INT_T1(void) interrupt INT_VECTOR_T1
{
	IO_TEST_1=!IO_TEST_1;				//TF1�ڽ����ж�ʱ�ᱻӲ���Զ��������˲���Ҫif(TF1)
}

/***********************************************************************************
��������:   void INT_T2(void)
��������:T2 �жϷ������
		 ���1ms
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
		IO_TEST_2=!IO_TEST_2;
	}
	_pop_(SFRPI);
}

/***********************************************************************************
��������:   void INT_T0(void)
��������:T3 �жϷ������
		 ���1ms
�������:   
���ز���:     
*************************************************************************************/
void INT_T3(void) interrupt INT_VECTOR_T3
{
	_push_(SFRPI);
	SFR_SetPage(1);				// TF3,EXF3����1ҳ����, ����������ҳ��Ϊ1, �������ж����豣��SFRPI
	if(TF3)						
	{
		TF3=0;
		SFR_SetPage(0);			// P55����Page 0
		LED_G_0=!LED_G_0;
	}
	_pop_(SFRPI);				
}

/***********************************************************************************
��������:   void INT_T4(void)
��������:T4 �жϷ������
		 ���1ms
�������:   
���ز���:     
*************************************************************************************/
void INT_T4(void) interrupt INT_VECTOR_T4
{
	_push_(SFRPI);
	SFR_SetPage(2);				// TF4,EXF4����2ҳ����, ����������ҳ��Ϊ2, �������ж����豣��SFRPI
	if(TF4)						
	{
		TF4=0;
		SFR_SetPage(0);			// P56����Page 0
		LED_R=!LED_R;
	}
	_pop_(SFRPI);				
}

/***********************************************************************************


/*************************************************
��������:     void DelayXus(u16 xUs)
��������:   	��ʱ���򣬵�λΪus
���ú���:        
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
��������:   void InitTimer0(void)
��������:Timer0��ʼ������	
�������:   
���ز���:     
*************************************************************************************/
void InitTimer0(void)
{
	TM_SetT0Mode_0_8BIT_PWM();				// ����T0ģʽ0Ϊ8λPWMģʽ
	TM_SetT0Clock_SYSCLKDiv12();			// ����T0ʱ��ԴΪ SYSCLK/12
	TM_SetT0Gate_Disable();

	TM_SetT0LowByte(0);						// ����T0��8λ
	TM_SetT0HighByte(0);					// ����T0��8λ
//	TM_EnableT0();							// ʹ��T0
}

/***********************************************************************************
��������:   void InitTimer1(void)
��������:Timer1��ʼ������
�������:   
���ز���:     
*************************************************************************************/
void InitTimer1(void)
{
	TM_SetT1Mode_0_8BIT_PWM();				// ����T1ģʽ0Ϊ8λPWMģʽ
	TM_SetT1Clock_SYSCLKDiv12();			// ����T1ʱ��ԴΪ SYSCLK/12
	TM_SetT1Gate_Disable();

	TM_SetT1LowByte(0);						// ����T1��8λ
	TM_SetT1HighByte(0);					// ����T1��8λ
//	TM_EnableT1();							// ʹ��T1
}

/***********************************************************************************
��������:   void InitTimer2(void)
��������:Timer2��ʼ������	
�������:   
���ز���:     
*************************************************************************************/
void InitTimer2(void)
{
	TM_SetT2Mode_AutoRLandExI();			// ����T2ģʽ0Ϊ16λ�Զ����غ��ⲿ�ж�ģʽ
	TM_SetT2Clock_SYSCLKDiv12();			// ����T2ʱ��ԴΪ SYSCLK/12
	
	TM_SetT2HighByte(HIBYTE(65536-1000));			// ����T4��8λ
	TM_SetT2LowByte(LOBYTE(65536-1000));			// ����T4��8λ
	TM_SetT2RLHighByte(HIBYTE(65536-1000));			// ��������ֵ��8λ
	TM_SetT2RLLowByte(LOBYTE(65536-1000));			// ��������ֵ��8λ

//	TM_EnableT2();							// ʹ��T2


}

/***********************************************************************************
��������:   void InitTimer3(void)
��������:Timer3��ʼ������
�������:   
���ز���:     
*************************************************************************************/
void InitTimer3(void)
{
	TM_SetT3Mode_AutoRLandExI();			// ����T3ģʽ0Ϊ16λ�Զ����غ��ⲿ�ж�ģʽ
	TM_SetT3Clock_SYSCLKDiv12();			// ����T3ʱ��ԴΪ SYSCLK/12
	
	TM_SetT3HighByte(HIBYTE(65536-1200));			// ����T4��8λ
	TM_SetT3LowByte(LOBYTE(65536-1200));			// ����T4��8λ
	TM_SetT3RLHighByte(HIBYTE(65536-1200));			// ��������ֵ��8λ
	TM_SetT3RLLowByte(LOBYTE(65536-1200));			// ��������ֵ��8λ
	
//	TM_EnableT3();							// ʹ��T3

}

/***********************************************************************************
��������:   void InitTimer4(void)
��������:Timer4��ʼ������
�������:   
���ز���:     
*************************************************************************************/
void InitTimer4(void)
{
	TM_SetT4Mode_AutoRLandExI();			// ����T4ģʽ0Ϊ16λ�Զ����غ��ⲿ�ж�ģʽ
	TM_SetT4Clock_SYSCLKDiv12();			// ����T4ʱ��ԴΪ SYSCLK/12
	
	TM_SetT4HighByte(HIBYTE(65536-1400));			// ����T4��8λ
	TM_SetT4LowByte(LOBYTE(65536-1400));			// ����T4��8λ
	TM_SetT4RLHighByte(HIBYTE(65536-1400));			// ��������ֵ��8λ
	TM_SetT4RLLowByte(LOBYTE(65536-1400));			// ��������ֵ��8λ
	
//	TM_EnableT4();							// ʹ��T4

}


/***********************************************************************************
��������:   void InitInterrupt(void)
��������:InitInterrupt��ʼ������
�������:   
���ز���:     
*************************************************************************************/
void InitInterrupt(void)
{
	INT_EnTIMER0();
	INT_EnTIMER1();
	INT_EnTIMER2();
	INT_EnTIMER3();
	INT_EnTIMER4();
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
	InitTimer1();
	InitTimer2();			
	InitTimer3();	
	InitTimer4();	
	

	InitInterrupt();		// �жϳ�ʼ��
	
	INT_EnAll();			// ʹ��ȫ���ж�

}



void main()
{
	
    InitSystem();
	
 	LED_G_0=0;LED_G_1=0;LED_R=0;
	DelayXms(1000);
 	LED_G_0=1;LED_G_1=1;LED_R=1;

					
    while(1)
    {
    	LED_G_1=0;
		// ȫ��ʹ�ܶ�ʱ��0��1��2��3��4
		TM_GlobalStart(TIMER0_GLOBAL_BIT|TIMER1_GLOBAL_BIT|TIMER2_GLOBAL_BIT|TIMER3_GLOBAL_BIT|TIMER4_GLOBAL_BIT);
    	DelayXms(200);
    	LED_G_1=1;
		// ȫ�ֽ�ֹ��ʱ��0��1��2��3��4
		TM_GlobalSTOP(TIMER0_GLOBAL_BIT|TIMER1_GLOBAL_BIT|TIMER2_GLOBAL_BIT|TIMER3_GLOBAL_BIT|TIMER4_GLOBAL_BIT);
		DelayXms(200);
		// ȫ�����ض�ʱ��0��1��2��3��4
		TM_GlobalReload(TIMER0_GLOBAL_BIT|TIMER1_GLOBAL_BIT|TIMER2_GLOBAL_BIT|TIMER3_GLOBAL_BIT|TIMER4_GLOBAL_BIT);
    }
}
