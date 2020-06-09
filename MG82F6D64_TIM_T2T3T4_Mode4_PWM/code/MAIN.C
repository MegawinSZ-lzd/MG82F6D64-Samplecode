/*********************************************************************
    ��Ŀ����:MG82F6D64-DEMO
			���԰�ʹ�� MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=12MHz, SysCLK=12MHz
		����������
			����T2ΪMode4:8λPWMģʽ�������P10
			����T3ΪMode4:8λPWMģʽ�������P33
			����T3ΪMode4:8λPWMģʽ�������P36
		
		ע�����
			ռ�ձ��޷�����100%,��Ҫ�����ϴ���

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
#define IO_TEST_3		P55
#define IO_TEST_4		P56
#define IO_TEST_5		P57

#define LED_G_0			P55
#define LED_R			P56
#define LED_G_1			P57

#define IO_T2_PWM_OUT	P10
#define IO_T3_PWM_OUT	P33
#define IO_T4_PWM_OUT	P36

#define T2_PWM_STEP_MAX	256		// ���� 2~256
#define T3_PWM_STEP_MAX	100		// ���� 2~256
#define T4_PWM_STEP_MAX	200		// ���� 2~256

#define T2_PWM_DUTY_0	((256-T2_PWM_STEP_MAX)+0)	
#define T2_PWM_DUTY_10	((256-T2_PWM_STEP_MAX)+(10*T2_PWM_STEP_MAX)/100)	
#define T2_PWM_DUTY_30	((256-T2_PWM_STEP_MAX)+(30*T2_PWM_STEP_MAX)/100)	
#define T2_PWM_DUTY_60	((256-T2_PWM_STEP_MAX)+(60*T2_PWM_STEP_MAX)/100)	
#define T2_PWM_DUTY_80	((256-T2_PWM_STEP_MAX)+(80*T2_PWM_STEP_MAX)/100)	

#define T3_PWM_DUTY_0	((256-T3_PWM_STEP_MAX)+0)	
#define T3_PWM_DUTY_10	((256-T3_PWM_STEP_MAX)+(10*T3_PWM_STEP_MAX)/100)	
#define T3_PWM_DUTY_30	((256-T3_PWM_STEP_MAX)+(30*T3_PWM_STEP_MAX)/100)	
#define T3_PWM_DUTY_60	((256-T3_PWM_STEP_MAX)+(60*T3_PWM_STEP_MAX)/100)	
#define T3_PWM_DUTY_80	((256-T3_PWM_STEP_MAX)+(80*T3_PWM_STEP_MAX)/100)	

#define T4_PWM_DUTY_0	((256-T4_PWM_STEP_MAX)+0)	
#define T4_PWM_DUTY_10	((256-T4_PWM_STEP_MAX)+(10*T4_PWM_STEP_MAX)/100)	
#define T4_PWM_DUTY_30	((256-T4_PWM_STEP_MAX)+(30*T4_PWM_STEP_MAX)/100)	
#define T4_PWM_DUTY_60	((256-T4_PWM_STEP_MAX)+(60*T4_PWM_STEP_MAX)/100)	
#define T4_PWM_DUTY_80	((256-T4_PWM_STEP_MAX)+(80*T4_PWM_STEP_MAX)/100)	

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
	TM_SetT2Mode_8bitPWM();							// ����T2ģʽΪ8λPWMģʽ
	TM_SetT2Clock_SYSCLKDiv12();					// ����T2ʱ��ԴΪ SYSCLK/12
	
	TM_SetT2LowByte(T2_PWM_DUTY_10);				// ����T2��8λ	  			
	TM_SetT2RLLowByte(T2_PWM_DUTY_10);				// ��������ֵ��8λ	  
	
	TM_SetT2HighByte(256-T2_PWM_STEP_MAX);			// ����T2��8λ
	TM_SetT2RLHighByte(256-T2_PWM_STEP_MAX);		// ��������ֵ��8λ

	TM_EnableT2();									// ʹ��T2

	//TM_EnableT2CKO_Invert();
	TM_SetT2CKOT2EX_P10P11();						// ����T2CKO�����P10
	TM_DisT2CKO();									// ��ֹT2CKO���
}

/***********************************************************************************
��������:   void InitTimer3(void)
��������:Timer3��ʼ������
�������:   
���ز���:     
*************************************************************************************/
void InitTimer3(void)
{
	TM_SetT3Mode_8bitPWM();							// ����T3ģʽΪ8λPWMģʽ
	TM_SetT3Clock_SYSCLKDiv12();					// ����T3ʱ��ԴΪ SYSCLK/12
	
	
	TM_SetT3LowByte(T3_PWM_DUTY_10);				// ����T3��8λ				
	TM_SetT3RLLowByte(T3_PWM_DUTY_10);				// ��������ֵ��8λ

	TM_SetT3HighByte(256-T3_PWM_STEP_MAX);			// ����T3��8λ
	TM_SetT3RLHighByte(256-T3_PWM_STEP_MAX);		// ��������ֵ��8λ


	TM_EnableT3();									// ʹ��T3
	TM_SetT3CKOT3EX_P33P32();						// ����T2CKO�����P33
	//TM_EnableT3CKO_Invert();
	TM_DisT3CKO();									// ��ֹT3CKO���
}

/***********************************************************************************
��������:   void InitTimer4(void)
��������:Timer4��ʼ������
�������:   
���ز���:     
*************************************************************************************/
void InitTimer4(void)
{
	TM_SetT4Mode_8bitPWM();							// ����T4ģʽΪ8λPWMģʽ
	TM_SetT4Clock_SYSCLKDiv12();					// ����T4ʱ��ԴΪ SYSCLK/12
	
	
	TM_SetT4LowByte(T4_PWM_DUTY_10);				// ����T4��8λ				
	TM_SetT4RLLowByte(T4_PWM_DUTY_10);				// ��������ֵ��8λ

	TM_SetT4HighByte(256-T4_PWM_STEP_MAX);			// ����T4��8λ
	TM_SetT4RLHighByte(256-T4_PWM_STEP_MAX);		// ��������ֵ��8λ


	TM_EnableT4();									// ʹ��T4
	TM_SetT4CKOT4EX_P36P37();						// ����T2CKO�����P36
	//TM_EnableT4CKO_Invert();
	TM_DisT4CKO();									// ��ֹT4CKO���
}

/***********************************************************************************
��������:   void InitPort(void)
��������:   IO����
�������:   
���ز���:     
*************************************************************************************/
void InitPort(void)
{

	IO_T2_PWM_OUT=1;
	IO_T3_PWM_OUT=1;
	IO_T4_PWM_OUT=1;
	
	PORT_SetP5PushPull(BIT2|BIT3|BIT4|BIT5|BIT6|BIT7);					// ����P52,P53,P54,P55,P56,P57Ϊ�������
	PORT_SetP1PushPull(BIT0);											// ����P10Ϊ�������������T2CKO(PWM)����
	PORT_SetP3PushPull(BIT3|BIT6);										// ����P33,P36Ϊ�������������T3CKO(PWM),T4CKO(PWM)����
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
	
	InitTimer2();			
	InitTimer3();	
	InitTimer4();


}


void main()
{
	
    InitSystem();
	
 	LED_G_0=0;LED_R=0;LED_G_1=0;
	DelayXms(1000);
 	LED_G_0=1;LED_R=1;LED_G_1=1;

					
    while(1)
    {
    	LED_R=0;
    	LED_G_1=0;
    	TM_SetT2LowByte(T2_PWM_DUTY_0);
    	TM_SetT3LowByte(T3_PWM_DUTY_0);
    	TM_SetT4LowByte(T4_PWM_DUTY_0);
    	TM_SetT2RLLowByte(T2_PWM_DUTY_0);
    	TM_SetT3RLLowByte(T3_PWM_DUTY_0);
    	TM_SetT4RLLowByte(T4_PWM_DUTY_0);
    	DelayXms(1);
    	TM_EnT2CKO();
    	TM_EnT3CKO();
    	TM_EnT4CKO();
    	LED_G_1=1;
    	DelayXms(2);

    	LED_G_1=0;
    	TM_SetT2RLLowByte(T2_PWM_DUTY_10);
    	TM_SetT3RLLowByte(T3_PWM_DUTY_10);
    	TM_SetT4RLLowByte(T4_PWM_DUTY_10);
    	LED_G_1=1;
    	DelayXms(2);

    	LED_G_1=0;
    	TM_SetT2RLLowByte(T2_PWM_DUTY_30);
    	TM_SetT3RLLowByte(T3_PWM_DUTY_30);
    	TM_SetT4RLLowByte(T4_PWM_DUTY_30);
    	LED_G_1=1;
    	DelayXms(2);

    	LED_G_1=0;
    	TM_SetT2RLLowByte(T2_PWM_DUTY_60);
    	TM_SetT3RLLowByte(T3_PWM_DUTY_60);
    	TM_SetT4RLLowByte(T4_PWM_DUTY_60);
    	LED_G_1=1;
    	DelayXms(2);

    	LED_G_1=0;
    	TM_SetT2RLLowByte(T2_PWM_DUTY_80);
    	TM_SetT3RLLowByte(T3_PWM_DUTY_80);
    	TM_SetT4RLLowByte(T4_PWM_DUTY_80);
    	LED_G_1=1;
    	DelayXms(2);

    	LED_G_1=0;
    	TM_SetT2RLLowByte(0xFF);
    	TM_SetT3RLLowByte(0xFF);
    	TM_SetT4RLLowByte(0xFF);
    	LED_G_1=1;
    	DelayXms(2);

    	// ���Duty 100% 
    	LED_G_1=0;			
		TM_DisT2CKO();		
		TM_DisT3CKO();
		TM_DisT4CKO();
		IO_T2_PWM_OUT=0;
		IO_T3_PWM_OUT=0;
		IO_T4_PWM_OUT=0;
    	LED_G_1=1;
    	DelayXms(2);

		// ֹͣ���
    	LED_R=1;
    	LED_G_1=0;
		TM_DisT2CKO();
		TM_DisT3CKO();
		TM_DisT4CKO();
		IO_T2_PWM_OUT=1;
		IO_T3_PWM_OUT=1;
		IO_T4_PWM_OUT=1;
    	LED_G_1=1;
		DelayXms(10);
    	
    }
}

