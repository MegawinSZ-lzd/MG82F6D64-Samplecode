/*********************************************************************
    项目名称:MG82F6D64-DEMO
    作    者:LZD
			测试板使用 MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=12MHz, SysCLK=12MHz
	功能描述：
			设置WDT, 使能WDT中断 约1s
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

#define LED_G_0		P55
#define LED_R		P56
#define LED_G_1		P57

/***********************************************************************************
函数名称:   void INT_SF(void)
功能描述:SF(系统标志位)中断服务程序
		 RTC,WDTF,BOD0F,BOD1F
输入参数:   
返回参数:     
*************************************************************************************/
void INT_SF(void) interrupt INT_VECTOR_SF
{
	if((PCON1&WDTF)!=0)
	{
		PCON1=PCON1|WDTF;
		LED_R=!LED_R;
	}
	else if((PCON1&BOF0)!=0)
	{
		PCON1=PCON1|BOF0;
	}
	else if((PCON1&BOF1)!=0)
	{
		PCON1=PCON1|BOF1;
	}
	else if((PCON1&RTCF)!=0)
	{
		PCON1=PCON1|RTCF;
	}
}


/*************************************************
函数名称:     void DelayXus(u16 xUs)
功能描述:   	延时程序，单位为us
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
		//CLRWDT();
		DelayXus(200);
		DelayXus(200);
		DelayXus(200);
		DelayXus(200);
		DelayXus(200);
		xMs--;
		
	}
}

/***********************************************************************************
函数名称:   void InitPort(void)
功能描述:   IO配置
输入参数:   
返回参数:     
*************************************************************************************/
void InitPort(void)
{
	PORT_SetP5OpenDrainPu(BIT5|BIT6|BIT7);		// 设置 P55,P56,P57 作为开漏带上拉
}


/***********************************************************************************
函数名称:   void InitInterrupt(void)
功能描述:InitInterrupt初始化设置
输入参数:   
返回参数:     
*************************************************************************************/
void InitInterrupt(void)
{
	INT_EnSF_WDT();			// 使能WDT中断,必须再使能系统标志中断
	INT_EnSF();				// 使能系统标志中断

	
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
	InitInterrupt();

	INT_EnAll();
}



void main()
{
	u8 i;

	WDT_Clear();		// 清看门狗
	
    InitSystem();
    
	WDT_SetClock_32K_DIV_64_1024ms();
	WDT_Enable();		// 使能看门狗
	
	LED_G_0=0;LED_G_1=0;LED_R=0;
	for(i=0;i<10;i++)
	{
		DelayXms(100);
		WDT_Clear();		// 清看门狗
	}
	LED_G_0=1;LED_G_1=1;LED_R=1;

	while(1)
	{ //  未清看门狗, 将产生中断
		LED_G_0=!LED_G_0;
		DelayXms(100);
	}
}

