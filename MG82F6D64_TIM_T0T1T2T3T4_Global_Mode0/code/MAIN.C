/*********************************************************************
    项目名称:MG82F6D64-DEMO
			测试板使用 MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=32MHz, SysCLK=32MHz
		功能描述：
			全局控制T0、T1、T2、T3,T4定时器 	
			
		注意事项：

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

#define LED_G_0			P55
#define LED_R			P56
#define LED_G_1			P57

/***********************************************************************************
函数名称:   void INT_T0(void)
功能描述:T0 中断服务程序
		 间隔1ms
输入参数:   
返回参数:     
*************************************************************************************/
void INT_T0(void) interrupt INT_VECTOR_T0
{

	IO_TEST_0=!IO_TEST_0;			//TF0在进入中断时会被硬件自动清掉，因此不需要if(TF0)
}

/***********************************************************************************
函数名称:   void INT_T0(void)
功能描述:T0 中断服务程序
		 间隔1ms
输入参数:   
返回参数:     
*************************************************************************************/
void INT_T1(void) interrupt INT_VECTOR_T1
{
	IO_TEST_1=!IO_TEST_1;				//TF1在进入中断时会被硬件自动清掉，因此不需要if(TF1)
}

/***********************************************************************************
函数名称:   void INT_T2(void)
功能描述:T2 中断服务程序
		 间隔1ms
输入参数:   
返回参数:     
*************************************************************************************/
void INT_T2(void) interrupt INT_VECTOR_T2
{
	_push_(SFRPI);
	SFR_SetPage(0);			// TF2,EXF2仅在0页操作, 所以需设置页面为0, 而且在中断里需保护SFRPI
	if(TF2)
	{
		TF2=0;
		IO_TEST_2=!IO_TEST_2;
	}
	_pop_(SFRPI);
}

/***********************************************************************************
函数名称:   void INT_T0(void)
功能描述:T3 中断服务程序
		 间隔1ms
输入参数:   
返回参数:     
*************************************************************************************/
void INT_T3(void) interrupt INT_VECTOR_T3
{
	_push_(SFRPI);
	SFR_SetPage(1);				// TF3,EXF3仅在1页操作, 所以需设置页面为1, 而且在中断里需保护SFRPI
	if(TF3)						
	{
		TF3=0;
		SFR_SetPage(0);			// P55仅在Page 0
		LED_G_0=!LED_G_0;
	}
	_pop_(SFRPI);				
}

/***********************************************************************************
函数名称:   void INT_T4(void)
功能描述:T4 中断服务程序
		 间隔1ms
输入参数:   
返回参数:     
*************************************************************************************/
void INT_T4(void) interrupt INT_VECTOR_T4
{
	_push_(SFRPI);
	SFR_SetPage(2);				// TF4,EXF4仅在2页操作, 所以需设置页面为2, 而且在中断里需保护SFRPI
	if(TF4)						
	{
		TF4=0;
		SFR_SetPage(0);			// P56仅在Page 0
		LED_R=!LED_R;
	}
	_pop_(SFRPI);				
}

/***********************************************************************************


/*************************************************
函数名称:     void DelayXus(u16 xUs)
功能描述:   	延时程序，单位为us
调用函数:        
输入参数:     u8 Us -> *1us  (1~255)
输出参数:     
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
函数名称:     void DelayXms(u16 xMs)
功能描述:     延时程序，单位为ms
输入参数:     u16 xMs -> *1ms  (1~65535)
输出参数:     
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
函数名称:   void InitTimer0(void)
功能描述:Timer0初始化设置	
输入参数:   
返回参数:     
*************************************************************************************/
void InitTimer0(void)
{
	TM_SetT0Mode_0_8BIT_PWM();				// 设置T0模式0为8位PWM模式
	TM_SetT0Clock_SYSCLKDiv12();			// 设置T0时钟源为 SYSCLK/12
	TM_SetT0Gate_Disable();

	TM_SetT0LowByte(0);						// 设置T0低8位
	TM_SetT0HighByte(0);					// 设置T0高8位
//	TM_EnableT0();							// 使能T0
}

/***********************************************************************************
函数名称:   void InitTimer1(void)
功能描述:Timer1初始化设置
输入参数:   
返回参数:     
*************************************************************************************/
void InitTimer1(void)
{
	TM_SetT1Mode_0_8BIT_PWM();				// 设置T1模式0为8位PWM模式
	TM_SetT1Clock_SYSCLKDiv12();			// 设置T1时钟源为 SYSCLK/12
	TM_SetT1Gate_Disable();

	TM_SetT1LowByte(0);						// 设置T1低8位
	TM_SetT1HighByte(0);					// 设置T1高8位
//	TM_EnableT1();							// 使能T1
}

/***********************************************************************************
函数名称:   void InitTimer2(void)
功能描述:Timer2初始化设置	
输入参数:   
返回参数:     
*************************************************************************************/
void InitTimer2(void)
{
	TM_SetT2Mode_AutoRLandExI();			// 设置T2模式0为16位自动重载和外部中断模式
	TM_SetT2Clock_SYSCLKDiv12();			// 设置T2时钟源为 SYSCLK/12
	
	TM_SetT2HighByte(HIBYTE(65536-1000));			// 设置T4高8位
	TM_SetT2LowByte(LOBYTE(65536-1000));			// 设置T4低8位
	TM_SetT2RLHighByte(HIBYTE(65536-1000));			// 设置重载值高8位
	TM_SetT2RLLowByte(LOBYTE(65536-1000));			// 设置重载值低8位

//	TM_EnableT2();							// 使能T2


}

/***********************************************************************************
函数名称:   void InitTimer3(void)
功能描述:Timer3初始化设置
输入参数:   
返回参数:     
*************************************************************************************/
void InitTimer3(void)
{
	TM_SetT3Mode_AutoRLandExI();			// 设置T3模式0为16位自动重载和外部中断模式
	TM_SetT3Clock_SYSCLKDiv12();			// 设置T3时钟源为 SYSCLK/12
	
	TM_SetT3HighByte(HIBYTE(65536-1200));			// 设置T4高8位
	TM_SetT3LowByte(LOBYTE(65536-1200));			// 设置T4低8位
	TM_SetT3RLHighByte(HIBYTE(65536-1200));			// 设置重载值高8位
	TM_SetT3RLLowByte(LOBYTE(65536-1200));			// 设置重载值低8位
	
//	TM_EnableT3();							// 使能T3

}

/***********************************************************************************
函数名称:   void InitTimer4(void)
功能描述:Timer4初始化设置
输入参数:   
返回参数:     
*************************************************************************************/
void InitTimer4(void)
{
	TM_SetT4Mode_AutoRLandExI();			// 设置T4模式0为16位自动重载和外部中断模式
	TM_SetT4Clock_SYSCLKDiv12();			// 设置T4时钟源为 SYSCLK/12
	
	TM_SetT4HighByte(HIBYTE(65536-1400));			// 设置T4高8位
	TM_SetT4LowByte(LOBYTE(65536-1400));			// 设置T4低8位
	TM_SetT4RLHighByte(HIBYTE(65536-1400));			// 设置重载值高8位
	TM_SetT4RLLowByte(LOBYTE(65536-1400));			// 设置重载值低8位
	
//	TM_EnableT4();							// 使能T4

}


/***********************************************************************************
函数名称:   void InitInterrupt(void)
功能描述:InitInterrupt初始化设置
输入参数:   
返回参数:     
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
函数名称:   void InitPort(void)
功能描述:   IO配置
输入参数:   
返回参数:     
*************************************************************************************/
void InitPort(void)
{
	PORT_SetP5PushPull(BIT2|BIT3|BIT4|BIT5|BIT6|BIT7);					// 设置P52,P53,P54,P55,P56,P57为推挽输出
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

	InitTimer0();			
	InitTimer1();
	InitTimer2();			
	InitTimer3();	
	InitTimer4();	
	

	InitInterrupt();		// 中断初始化
	
	INT_EnAll();			// 使能全局中断

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
		// 全局使能定时器0、1、2、3、4
		TM_GlobalStart(TIMER0_GLOBAL_BIT|TIMER1_GLOBAL_BIT|TIMER2_GLOBAL_BIT|TIMER3_GLOBAL_BIT|TIMER4_GLOBAL_BIT);
    	DelayXms(200);
    	LED_G_1=1;
		// 全局禁止定时器0、1、2、3、4
		TM_GlobalSTOP(TIMER0_GLOBAL_BIT|TIMER1_GLOBAL_BIT|TIMER2_GLOBAL_BIT|TIMER3_GLOBAL_BIT|TIMER4_GLOBAL_BIT);
		DelayXms(200);
		// 全局重载定时器0、1、2、3、4
		TM_GlobalReload(TIMER0_GLOBAL_BIT|TIMER1_GLOBAL_BIT|TIMER2_GLOBAL_BIT|TIMER3_GLOBAL_BIT|TIMER4_GLOBAL_BIT);
    }
}

