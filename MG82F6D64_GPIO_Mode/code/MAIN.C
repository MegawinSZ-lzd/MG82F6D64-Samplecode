/*********************************************************************
    项目名称:MG82F6D64-DEMO
    作    者:YZY
			测试板使用 MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=12MHz, SysCLK=12MHz
	功能描述：
			设置IO口模式
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
函数名称:   void InitPort(void)
功能描述:   IO配置
输入参数:   
返回参数:     
*************************************************************************************/
void InitPort(void)
{
//	PORT_SetP0AInputOnly(BIT0|BIT1);				// 设置P00,P01为仅模拟输入口
//	PORT_SetP0OpenDrain(BIT0|BIT1);					// 设置P00,P01为开漏带上拉输出模式
//	PORT_SetP0OpenDrainPu(BIT0|BIT1);				// 设置P00,P01为开漏输出模式
//	PORT_SetP0PushPull(BIT0|BIT1);					// 设置P00,P01为推挽输出模式

//	PORT_SetP1AInputOnly(BIT0|BIT1);				// 设置P10,P11 为仅模拟输入口
//	PORT_SetP1OpenDrain(BIT0|BIT1);					// 设置P10,P11为开漏带上拉输出模式
//	PORT_SetP1OpenDrainPu(BIT0|BIT1);				// 设置P10,P11为开漏输出模式
//	PORT_SetP1PushPull(BIT0|BIT1);					// 设置P10,P11为推挽输出模式

//	PORT_SetP2AInputOnly(BIT2|BIT4);				// 设置P22,P24 为仅模拟输入口
//	PORT_SetP2OpenDrain(BIT2|BIT4);					// 设置P22,P24 为开漏带上拉输出模式
//	PORT_SetP2OpenDrainPu(BIT2|BIT4);				// 设置P22,P24 为开漏输出模式
//	PORT_SetP2PushPull(BIT2|BIT4);					// 设置P22,P24 为推挽输出模式

//	PORT_SetP3InputOnly(BIT3);						/ 设置P33 为仅输入口
//	PORT_SetP3OpenDrain(BIT3|BIT4|BIT5);			// 设置P33,P34,P35 为开漏输出模式
//	PORT_SetP3QuasiBi(BIT0|BIT1|BIT3|BIT4|BIT5);	// 设置P30,P31,P33,P34,P35 为准双向模式
//	PORT_SetP3PushPull(BIT5);						// 设置P35 为推挽输出模式

//	PORT_SetP4AInputOnly(BIT4|BIT5);				// 设置P44,P45 为仅模拟输入口
//	PORT_SetP4OpenDrain(BIT4|BIT5);					// 设置P44,P45 为开漏带上拉输出模式
//	PORT_SetP4OpenDrainPu(BIT4|BIT5);				// 设置P44,P45 为开漏输出模式
//	PORT_SetP4PushPull(BIT4|BIT5);					// 设置P44,P45 为推挽输出模式

//	PORT_SetP5AInputOnly(BIT0|BIT1);				// 设置P50,P51 为仅模拟输入口
//	PORT_SetP5OpenDrain(BIT0|BIT1);					// 设置P50,P51 为开漏带上拉输出模式
//	PORT_SetP5penDrainPu(BIT0|BIT1);				// 设置P50,P51 为开漏输出模式
//	PORT_SetP5ushPull(BIT0|BIT1);					// 设置P50,P51 为推挽输出模式

//	PORT_SetP6AInputOnly(BIT0|BIT1);				// 设置P60,P61 为仅模拟输入口
//	PORT_SetP6OpenDrain(BIT0|BIT1);				// 设置P60,P61 为开漏带上拉输出模式
//	PORT_SetP6OpenDrainPu(BIT0|BIT1);				// 设置P60,P61 为开漏输出模式
//	PORT_SetP6PushPull(BIT0|BIT1);				// 设置P60,P61 为推挽输出模式

//	PORT_SetP7AInputOnly(BIT0|BIT1);				// 设置P70,P71 为仅模拟输入口
//	PORT_SetP7OpenDrain(BIT0|BIT1); 				// 设置P70,P71 为开漏带上拉输出模式
//	PORT_SetP7penDrainPu(BIT0|BIT1);				// 设置P70,P71 为开漏输出模式
//	PORT_SetP7ushPull(BIT0|BIT1);					// 设置P70,P71 为推挽输出模式

	PORT_SetP5PushPull(BIT5|BIT6|BIT7);					// 设置P55,P56,P57为推挽输出(LED)
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
	
}

void main()
{
	u8 i;
    InitSystem();
	LED_G_1=0;LED_R=0;LED_G_0=0;
	DelayXms(1000);
	LED_G_1=1;LED_R=1;LED_G_0=1;

    while(1)
    {
		LED_R=!LED_R;
		for(i=0;i<50;i++)
		{
			LED_G_0=1;
			DelayXus(i*2);
			LED_G_0=0;
			DelayXus(i*2);
		}
		LED_G_0=1;
		DelayXms(500);
		for(i=0;i<50;i++)
		{
			LED_G_1=1;
			DelayXus(100);
			DelayXus(i*2);
			LED_G_1=0;
			DelayXus(100);
			DelayXus(i*2);
		}
			LED_G_1=1;
		DelayXms(500);

    }
}


