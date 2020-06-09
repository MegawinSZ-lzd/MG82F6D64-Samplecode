/*********************************************************************
    项目名称:MG82F6D64-DEMO
    作    者:YZY
			测试板使用 MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=32KHz, SysCLK=32KHz
	功能描述：
			设置时钟为 ILRCO 32KHz, P60输出MCK/4
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

#define LED_G_0		P55
#define LED_R		P56
#define LED_G_1		P57

/*************************************************
函数名称:     void DelayXms(u16 xMs)
描述:   		延时程序，单位为ms
调用函数:        
输入参数:     u16 xMs -> *1ms  (1~65535)
输出参数:     
返回:        		
其它:
*************************************************/
void DelayXms(u16 xMs)
{
	while(xMs!=0)
	{
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();

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
	PORT_SetP5PushPull(BIT5|BIT6|BIT7);					// 设置P55,P56,P57为推挽输出(LED)
	PORT_SetP6PushPull(BIT0);					    // 设置P60 为推挽输出模式

}

/***********************************************************************************
函数名称:   void InitClock_ILRCO(void)
功能描述:   初始化Clock配置
输入参数:   
返回参数:     
*************************************************************************************/
void InitClock_ILRCO(void)
{
	// P60 输出 MCK/4
	CLK_P60OC_MCKDiv4();			//32KHz/4=8KHz

	// IHRCO Enable, MCK=OSCin, OSCin=ILRCO
	CLK_SetCKCON2(ENABLE_IHRCO|MCK_OSCin|OSCIn_ILRCO);
	nop();
	// IHRCO Disable, MCK=OSCin, OSCin=ILRCO
	CLK_SetCKCON2(DISABLE_IHRCO|MCK_OSCin|OSCIn_ILRCO);

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
	InitClock_ILRCO();	
}

void main()
{
    InitSystem();
	
	LED_G_0=0;LED_G_1=0;LED_R=0;
	DelayXms(1000);
	LED_G_0=1;LED_G_1=1;LED_R=1;
					
    while(1)
    {
		LED_G_0=0;
		DelayXms(200);
		LED_G_0=1;
		LED_G_1=0;
		DelayXms(200);
		LED_G_1=1;
		LED_R=0;
		DelayXms(200);
		LED_R=1;

    }
}

