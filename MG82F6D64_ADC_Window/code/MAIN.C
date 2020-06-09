/*********************************************************************
    项目名称:MG82F6D64-DEMO
    作    者:YZY
			测试板使用 MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=12MHz, SysCLK=12MHz
	功能描述：
			ADC 窗口电压检测，并将采集ADC值输出到uart，
			若ADC值处于窗口阈值内，则会在串口输出信息。
			采集引脚：P10	   串口：P30 P31

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

#define LED_G_0		P55
#define LED_R		P56
#define LED_G_1		P57

bit bES0;
bit bADCFinish =0;
WordTypeDef wAdcValue;

#define TEST_ADC_BUF_SIZE	16
xdata u16 wAdcBuf[TEST_ADC_BUF_SIZE];
u8 AdcBufCnt;

/***********************************************************************************
函数名称:   void INT_ADC(void)
功能描述:ADC 中断服务程序
		 
输入参数:   
返回参数:     
*************************************************************************************/
void INT_ADC(void) interrupt INT_VECTOR_ADC
{
	
	if((ADCON0 & ADCWI) != 0)	//窗口中断
	{
		ADCON0 &= ~ADCWI;//清除窗口中断
		LED_R = ~LED_R;
		wAdcValue.B.BHigh = ADCDH;
   	 	wAdcValue.B.BLow = ADCDL;
		wAdcValue.W &= 0xFFF;
		wAdcBuf[AdcBufCnt]=wAdcValue.W;
		AdcBufCnt++;
		if(AdcBufCnt>=TEST_ADC_BUF_SIZE)
		{
			bADCFinish=TRUE;
			INT_DisADC();		// Buf收满,禁止ADC中断
		}
		
	}
	
}

/*************************************************
函数名称: char putchar (char c)   
功能描述: 用于printf 的串口输出程序
输入参数:     char c
输出参数:     
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
函数名称:   void InitUart0(void)
功能描述:   Uart0初始化
		 使用S0BRG作为波特率来源
输入参数:   
返回参数:     
*************************************************************************************/
void InitUart0_S0BRG(void)
{
	
	UART0_SetAccess_S0CR1();			// 设置SFR 0xB9地址是访问S0CR1
	UART0_SetMode8bitUARTVar();			// 8位 可变波特率
	UART0_EnReception();				// 使能接收
	UART0_SetBRGFromS0BRG();			// 设置波特率来源为 S0BRG
	UART0_SetS0BRGBaudRateX2();			// 设置2x波特率
	UART0_SetS0BRGSelSYSCLK();			// 设置S0BRG时钟输入为Sysclk
	UART0_SetRxTxP30P31();

	// 设置S0BRG Value
	UART0_SetS0BRGValue(S0BRG_BRGRL_9600_2X_12000000_1T);
	
	UART0_EnS0BRG();					// 开启S0BRG时钟
}

/***********************************************************************************
函数名称:   void InitPort(void)
功能描述:   IO配置
输入参数:   
返回参数:     
*************************************************************************************/
void InitPort(void)
{
	PORT_SetP5PushPull(BIT5|BIT6|BIT7);		// 设置 P55,P56,P57作为推挽输出(LED)
	PORT_SetP1AInputOnly(BIT0);				// 设置P10(AIN0)为仅模拟输入
}

/***********************************************************************************
函数名称:   void InitADC(void)
功能描述:   ADC初始化
输入参数:   
返回参数:     
*************************************************************************************/
void InitADC(void)
{
	ADC_Enable();						// 使能ADC
	ADC_SetClock_SYSCLK();			    // ADC转换时钟为 SYSCLK
	ADC_SetMode_FreeRunning();			// ADC全速运行
	ADC_SetADCData_12Bit();				// ADC数据位: 12bit			
	ADC_SetRightJustified();			// ADC转换数据右对齐

	ADC_SetWindow_WithIn();				// 设置在窗口内置位ADCWI
	ADC_SetWindow_High(0x700);			// 设置ADC窗口阈值高点
	ADC_SetWindow_Low(0x000);			// 设置ADC窗口阈值低点

	ADC_DisInterrupt_ADCI();			// 禁止ADC完成标志位ADCI是否产生中断
	ADC_EnInterrupt_ADCWI();			// 使能ADC窗口标志位ADCWI产生中断
	ADC_DisInterrupt_SMPF();			// 禁止ADC采样完成标志位SFPF产生中断
	
}

/***********************************************************************************
函数名称:   void InitInterrupt(void)
功能描述:   中断初始化
输入参数:   
返回参数:     
*************************************************************************************/
void InitInterrupt(void)
{
	INT_EnADC();						// 使能ADC中断
	INT_EnAll();						//  使能全局中断
	
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
	InitADC();
	InitUart0_S0BRG();
	InitInterrupt();
}

void main()
{
	u8 i;
	
	InitSystem();
	AdcBufCnt=0;
	bADCFinish=FALSE;

	printf("\nStart!");

	LED_G_0=0;LED_R=0;LED_G_1=0;
	DelayXms(1000);
	LED_G_0=1;LED_R=1;LED_G_1=1;

  	while(1)
  	{
		DelayXms(100);
		LED_G_0 = !LED_G_0;

		if(bADCFinish)
		{
			LED_R=1;
			printf("\nADC Win:");
			for(i=0;i<TEST_ADC_BUF_SIZE;i++)
			{
				printf("%04X ",wAdcBuf[i]);
			}
			bADCFinish=FALSE;
			INT_EnADC();	// 处理完数据,重新使能ADC中断
		}
			
    }
}


