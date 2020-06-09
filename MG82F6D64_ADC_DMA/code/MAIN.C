/*********************************************************************
    项目名称:MG82F6D64-DEMO
    作    者:YZY
			测试板使用 MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=12MHz, SysCLK=12MHz
	功能描述：
			使用DMA读取ADC的值到XRAM
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

#define TEST_ADC_DATA_CNT		32

xdata WordTypeDef TestBuf[TEST_ADC_DATA_CNT];
u8 TestBufLen;

bit bAdcDMAFlag;

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

/***********************************************************************************
函数名称:   void INT_DMA(void)
功能描述:DMA 中断服务程序
		 
输入参数:   
返回参数:     
*************************************************************************************/
void INT_DMA(void) interrupt INT_VECTOR_DMA
{
	_push_(SFRPI);
	SFRPI=0;
	LED_R=!LED_R;
	bAdcDMAFlag=TRUE;
	DMACR0=DMACR0&(~DCF0);		// 清中断标志位
	_pop_(SFRPI);
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
	// Sets B.R. value
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
	PORT_SetP5PushPull(BIT5|BIT6|BIT7);					// 设置 P55,P56,P57作为推挽输出(LED)
	PORT_SetP3QuasiBi(BIT0|BIT1|BIT3|BIT4|BIT5);		// 设置 P30,P31,P33,P34,P35 作为准双向口
	PORT_SetP1AInputOnly(BIT0|BIT1|BIT5|BIT6|BIT7);	// 设置P10(AIN0),P11(AIN1),P15(AIN5),P16(AIN6),P17(AIN7)为仅模拟输入
}

/***********************************************************************************
函数名称:   void InitADC(void)
功能描述:   ADC初始化
输入参数:   
返回参数:     
*************************************************************************************/
void InitADC(void)
{
	ADC_Enable();									// 使能ADC
	ADC_SetClock_SYSCLK();						    // ADC转换时钟为 SYSCLK       转换率= 12M/30=600K
	ADC_SetMode_FreeRunning();						// ADC启动模式, 全速模式
	ADC_SetRightJustified();						// ADC转换数据右对齐
	ADC_SetADCData_12Bit();
	ADC_SetChannel_AIN0();							// ADC设置通道0
}

/***********************************************************************************
函数名称:   void InitDMA_ADC0_TO_XRAM(void)
功能描述:DMA初始化 ADC0->XRAM
输入参数:   
返回参数:     
*************************************************************************************/
void InitDMA_ADC0_TO_XRAM()
{
	DMA_SetSource_ADC0();
	DMA_SetDestination_XRAM();
	DMA_SetXRAMStartaddr((u16)(&TestBuf[0].BUF[0]));
	DMA_SetXRAMStartaddrRL((u16)(&TestBuf[0].BUF[0]));
	DMA_SetCount(65536-(TEST_ADC_DATA_CNT*2));
	DMA_SetCountRL(65536-(TEST_ADC_DATA_CNT*2));
	DMA_SetTrig_Software();
	DMA_EnDCF0Int();
	DMA_DisLoop();
	DMA_Enable();
}

/***********************************************************************************
函数名称:   void InitInterrupt(void)
功能描述:   中断初始化
输入参数:   
返回参数:     
*************************************************************************************/
void InitInterrupt(void)
{
	INT_EnDMA();					// 使能DMA中断
	
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
	InitUart0_S0BRG();
	InitADC();
	InitDMA_ADC0_TO_XRAM();
	InitInterrupt();

	INT_EnAll();
}


void main()
{
	u8 i,x;

    InitSystem();
	
	LED_G_0=0;LED_R=0;LED_G_1=0;
	DelayXms(1000);
	LED_G_0=1;LED_R=1;LED_G_1=1;

	DMA_Start();
	ADC_SoftStart();
	
    while(1)
    {
    	DelayXms(200);
  		LED_G_0 = !LED_G_0;

		if(bAdcDMAFlag)
		{
			bAdcDMAFlag=0;
			printf("\nADC:");
			for(i=0;i<(TEST_ADC_DATA_CNT);i++)
			{
				x=TestBuf[i].B.BHigh;
				TestBuf[i].B.BHigh=TestBuf[i].B.BLow;
				TestBuf[i].B.BLow=x;
				printf("%04X ",TestBuf[i].W);
			}
			DMA_Start();
		}

    }
}



