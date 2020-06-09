/*********************************************************************
    项目名称:MG82F6D64-DEMO
			测试板使用 MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=12MHz, SysCLK=12MHz
		功能描述：
			定义T2为Mode4:8位PWM模式，输出到P10
			定义T3为Mode4:8位PWM模式，输出到P33
			定义T3为Mode4:8位PWM模式，输出到P36
		
		注意事项：
			占空比无法做到100%,需要软件上处理

    建立时间:
    修改日志:

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
定义系统时钟SysClk (MAX.50MHz)
可选: 
	11059200,12000000,
	22118400,24000000,
	29491200,32000000,
	44236800,48000000
*************************************************/
#define MCU_SYSCLK		12000000
/*************************************************/
/*************************************************
定义CPU时钟 CpuClk (MAX.36MHz)
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

#define T2_PWM_STEP_MAX	256		// 步数 2~256
#define T3_PWM_STEP_MAX	100		// 步数 2~256
#define T4_PWM_STEP_MAX	200		// 步数 2~256

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
函数名称:void DelayXus(u8 xUs)
功能描述:延时程序，单位为us
输入参数:u8 Us -> *1us  (1~255)
返回参数:     
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
函数名称:void DelayXms(u16 xMs)
功能描述:延时程序，单位为ms
输入参数:u16 xMs -> *1ms  (1~65535)
返回参数:     
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
函数名称:   void InitTimer2(void)
功能描述:Timer2初始化设置	
输入参数:   
返回参数:     
*************************************************************************************/
void InitTimer2(void)
{
	TM_SetT2Mode_8bitPWM();							// 设置T2模式为8位PWM模式
	TM_SetT2Clock_SYSCLKDiv12();					// 设置T2时钟源为 SYSCLK/12
	
	TM_SetT2LowByte(T2_PWM_DUTY_10);				// 设置T2低8位	  			
	TM_SetT2RLLowByte(T2_PWM_DUTY_10);				// 设置重载值低8位	  
	
	TM_SetT2HighByte(256-T2_PWM_STEP_MAX);			// 设置T2高8位
	TM_SetT2RLHighByte(256-T2_PWM_STEP_MAX);		// 设置重载值高8位

	TM_EnableT2();									// 使能T2

	//TM_EnableT2CKO_Invert();
	TM_SetT2CKOT2EX_P10P11();						// 设置T2CKO输出到P10
	TM_DisT2CKO();									// 禁止T2CKO输出
}

/***********************************************************************************
函数名称:   void InitTimer3(void)
功能描述:Timer3初始化设置
输入参数:   
返回参数:     
*************************************************************************************/
void InitTimer3(void)
{
	TM_SetT3Mode_8bitPWM();							// 设置T3模式为8位PWM模式
	TM_SetT3Clock_SYSCLKDiv12();					// 设置T3时钟源为 SYSCLK/12
	
	
	TM_SetT3LowByte(T3_PWM_DUTY_10);				// 设置T3低8位				
	TM_SetT3RLLowByte(T3_PWM_DUTY_10);				// 设置重载值低8位

	TM_SetT3HighByte(256-T3_PWM_STEP_MAX);			// 设置T3高8位
	TM_SetT3RLHighByte(256-T3_PWM_STEP_MAX);		// 设置重载值高8位


	TM_EnableT3();									// 使能T3
	TM_SetT3CKOT3EX_P33P32();						// 设置T2CKO输出到P33
	//TM_EnableT3CKO_Invert();
	TM_DisT3CKO();									// 禁止T3CKO输出
}

/***********************************************************************************
函数名称:   void InitTimer4(void)
功能描述:Timer4初始化设置
输入参数:   
返回参数:     
*************************************************************************************/
void InitTimer4(void)
{
	TM_SetT4Mode_8bitPWM();							// 设置T4模式为8位PWM模式
	TM_SetT4Clock_SYSCLKDiv12();					// 设置T4时钟源为 SYSCLK/12
	
	
	TM_SetT4LowByte(T4_PWM_DUTY_10);				// 设置T4低8位				
	TM_SetT4RLLowByte(T4_PWM_DUTY_10);				// 设置重载值低8位

	TM_SetT4HighByte(256-T4_PWM_STEP_MAX);			// 设置T4高8位
	TM_SetT4RLHighByte(256-T4_PWM_STEP_MAX);		// 设置重载值高8位


	TM_EnableT4();									// 使能T4
	TM_SetT4CKOT4EX_P36P37();						// 设置T2CKO输出到P36
	//TM_EnableT4CKO_Invert();
	TM_DisT4CKO();									// 禁止T4CKO输出
}

/***********************************************************************************
函数名称:   void InitPort(void)
功能描述:   IO配置
输入参数:   
返回参数:     
*************************************************************************************/
void InitPort(void)
{

	IO_T2_PWM_OUT=1;
	IO_T3_PWM_OUT=1;
	IO_T4_PWM_OUT=1;
	
	PORT_SetP5PushPull(BIT2|BIT3|BIT4|BIT5|BIT6|BIT7);					// 设置P52,P53,P54,P55,P56,P57为推挽输出
	PORT_SetP1PushPull(BIT0);											// 设置P10为推挽输出，用于T2CKO(PWM)引脚
	PORT_SetP3PushPull(BIT3|BIT6);										// 设置P33,P36为推挽输出，用于T3CKO(PWM),T4CKO(PWM)引脚
}

/***********************************************************************************
函数名称:   void InitClock(void)
功能描述:   时钟初始化	
输入参数:   
返回参数:     
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

	// P60 输出 MCK/4
	//CLK_P60OC_MCKDiv4();
}

/***********************************************************************************
函数名称:   void InitSystem(void)
功能描述:   系统初始化
输入参数:   
返回参数:     
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

    	// 输出Duty 100% 
    	LED_G_1=0;			
		TM_DisT2CKO();		
		TM_DisT3CKO();
		TM_DisT4CKO();
		IO_T2_PWM_OUT=0;
		IO_T3_PWM_OUT=0;
		IO_T4_PWM_OUT=0;
    	LED_G_1=1;
    	DelayXms(2);

		// 停止输出
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


