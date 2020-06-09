/*********************************************************************
    项目名称:MG82F6D64-DEMO
    作    者:YZY
			测试板使用 MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=12MHz, SysCLK=12MHz
	功能描述：
			设置P44,P45用于GPIO,禁止其OCD功能.
			设置P47用于GPIO,禁止其RST功能.
		注意事项：
			如果同时将P44,P45,P47用于GPIO,则将无法使用OCD-ICE烧录和仿真 
			要恢复OCD烧录仿真,需先用U1烧录不带此功能的程序. 

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
函数名称:   void EnP44P45GPIO(void)
功能描述:   设置P44P45用于GPIO, 禁止其OCD功能
		注意: 此设置后,P44/P45将无法用于OCD-ICE仿真, 
输入参数:   
返回参数:     
*************************************************************************************/
void EnP44P45GPIO(void)
{
	u8 x;
	bit bEA=EA;
	ISPCR=0x80;
	IFADRH=0x00;
	IFADRL=DCON0_P;
	IFMT=ISP_READ_P;
	SCMD=0x46;
	SCMD=0xB9;
	_nop_();
	x=IFD;
	x=x&(~OCDE_P);
	IFD=x;
	IFMT=ISP_WRITE_P;
	SCMD=0x46;
	SCMD=0xB9;
	_nop_();
	ISPCR=0x00;
	IFMT=ISP_STANBY;
	
}

/***********************************************************************************
函数名称:   void EnP47GPIO(void)
功能描述:   设置P47用于GPIO, 禁止其RST功能
输入参数:   
返回参数:     
*************************************************************************************/
void EnP47GPIO(void)
{
	u8 x;
	bit bEA=EA;
	ISPCR=0x80;
	IFADRH=0x00;
	IFADRL=DCON0_P;
	IFMT=ISP_READ_P;
	SCMD=0x46;
	SCMD=0xB9;
	_nop_();
	x=IFD;
	x=x&(~RSTIO_P);
	IFD=x;
	IFMT=ISP_WRITE_P;
	SCMD=0x46;
	SCMD=0xB9;
	_nop_();
	ISPCR=0x00;
	IFMT=ISP_STANBY;
	
}


/***********************************************************************************
函数名称:   void InitPort(void)
功能描述:   IO配置
输入参数:   
返回参数:     
*************************************************************************************/
void InitPort(void)
{
	PORT_SetP4OpenDrainPu(BIT4|BIT5|BIT7);				// 设置 P44,P45,P47 带上拉的开漏输出(准双向口)
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
    InitSystem();
	
	LED_G_0=0;LED_G_1=0;LED_R=0;
	DelayXms(2000);
	LED_G_0=1;LED_G_1=1;LED_R=1;

	// 2秒后再设置P44P45P47为IO口，
	// 重新上电，在这之前的2秒内，可以用OCD-ICE烧录
	EnP44P45GPIO();
	EnP47GPIO();
					
    while(1)
    {
		LED_G_0=0;P44=0;
		DelayXms(200);
		LED_G_0=1;P44=1;
		LED_G_1=0;P45=0;
		DelayXms(200);
		LED_G_1=1;P45=1;
		LED_R=0;P47=0;
		DelayXms(200);
		LED_R=1;P47=1;
    }
}

