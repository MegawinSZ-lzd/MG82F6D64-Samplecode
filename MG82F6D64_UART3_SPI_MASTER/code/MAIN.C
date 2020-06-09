/*********************************************************************
    项目名称:MG82F6D64-DEMO
    作    者:YZY
			测试板使用 MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=12MHz, SysCLK=12MHz
	功能描述：
			串口3，工作在模式4 (即SPI主机模式)
			SPICLK <-->TXD	P37
			MOSI	<-->RXD  P36
			MISO	<-->S0MI P65
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
定义系统时钟SysClk
可选: 
	11059200,12000000,
	22118400,24000000,
	29491200,32000000,
	44236800,48000000
*************************************************/
#define MCU_SYSCLK		12000000
/*************************************************/
/*************************************************
定义CPU时钟 CpuClk (不大于36MHz)
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

#define SPI_NSS		P34

sfr UARTSPICLK		= 0xBB;

// 选择SPICLK 初始电平
#define SPICLK_LOW	0	// SPICLK  初始为低电平
#define SPICLK_HIGH	1	// SPICLK  初始为低电平
#define SPI_CPOL  SPICLK_LOW	

// 选择数据传送顺序
#define LSB	0	// 低位在先
#define MSB	1	// 高位在先
#define SPI_DORD	MSB

// 选择SPI 的传送速度
#define SPI_CLK_SYSCLK_4	0	// SYSCLK/4
#define SPI_CLK_SYSCLK_12	1	// SYSCLK/12
#define SPI_CLOCK	SPI_CLK_SYSCLK_4

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
	PORT_SetP5PushPull(BIT5|BIT6|BIT7);
	PORT_SetP3PushPull(BIT4|BIT6|BIT7);			// 设置P34,P36,P37为推挽输出，做NSS,串口SPI使用
	PORT_SetP6OpenDrainPu(BIT5);				// 设置P66,做MISO
}

/***********************************************************************************
函数名称:   u8 Uart3SPITransceiver(u8 ToSend)
功能描述:   串口SPI主机发送函数
输入参数:   u8 ToSend
返回参数:     
*************************************************************************************/
u8 Uart3SPITransceiver(u8 ToSend)
{
	u8 x;
	SFR_SetPage(4);
	S3BUF=ToSend;
	while(TI3==0);
	TI3=0;
	x=S3BUF;
	SFR_SetPage(0);
	return x;
}

/***********************************************************************************
函数名称:   void InitUart3_SPI_Master(void)
功能描述:   Uart3初始化 SPI Master
输入参数:   
返回参数:     
*************************************************************************************/
void InitUart3_SPI_Master(void)
{
	P37=1;
	UART3_SetModeSPIMaster();			// 设置串口SPI主机模式
	UART3_SetMISOP65();					// 设置串口SPI的MISO引脚为P65
	UART3_SetRxTxP36P37();				// 设置串口SPI的RX（MOSI）,TX(SPI_CLK)引脚为P36,P37
#if (SPI_CLOCK == SPI_CLK_SYSCLK_4)
	UART3_SetSPIClock_SYSCLKDiv4();		// 设置SPI时钟为系统时钟的4分频
#else
	UART3_SetSPIClock_SYSCLKDiv12();	// 设置SPI时钟为系统时钟的12分频
#endif
#if (SPI_DORD==MSB)
	UART3_SetDataMSB();
#else
	UART3_SetDataLSB();
#endif

#if (SPI_CPOL==SPICLK_LOW)
	UARTSPICLK = 0x27;					// SPICLK  初始为低电平

#endif

	
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
	InitUart3_SPI_Master();
}

void main()
{
	InitSystem();
	LED_G_1=0;LED_R=0;
	DelayXms(1000);
	LED_G_1=1;LED_R=1;	
	while(1)
	{
		DelayXms(100);
		LED_G_0 = !LED_G_0;	
		SPI_NSS=0;
		Uart3SPITransceiver(0x05);	
		SPI_NSS=1;
	}

}


