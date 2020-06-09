/*********************************************************************
    项目名称:MG82F6D64-DEMO
    作    者:YZY
			测试板使用 MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=24MHz, SysCLK=48MHz
	功能描述：
			SPI(nSS/P14,MOSI/P15,MISO/P16,SPICLK/P17)读写EN25Q32A测试
			SPI速率为24MHz
			包括了无DMA和有DMA的测试
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
#define MCU_SYSCLK		48000000
/*************************************************/
/*************************************************
定义CPU时钟 CpuClk (MAX.36MHz)
	1) CpuCLK=SysCLK
	2) CpuClk=SysClk/2
*************************************************/
//#define MCU_CPUCLK		(MCU_SYSCLK)
#define MCU_CPUCLK		(MCU_SYSCLK/2)

#define TIMER_1T_1ms_TH	((65536-(u16)(float)(1000*((float)(MCU_SYSCLK)/(float)(1000000)))) /256) 			
#define TIMER_1T_1ms_TL	((65536-(u16)(float)(1000*((float)(MCU_SYSCLK)/(float)(1000000)))) %256)

#define TIMER_12T_1ms_TH	((65536-(u16)(float)(1000*((float)(MCU_SYSCLK)/(float)(12000000)))) /256) 			
#define TIMER_12T_1ms_TL	((65536-(u16)(float)(1000*((float)(MCU_SYSCLK)/(float)(12000000)))) %256)

#define LED_G_0		P55
#define LED_R		P56
#define LED_G_1		P57

#define SPI_nSS		P14

// SPI
#define Dummy_Data					0xFF
#define FLASH_BLOCK_SIZE			(64L*1024L)					// 64K Byte

#define FLASH_TEST_START_ADDRESS	0x00000000
#define FLASH_TEST_SIZE_MAX			(1024L*1024L)				// 1M Byte

xdata u8 TestBuf[512];
u16 TestBufCnt;


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
函数名称:void SendByte(u8 tByte)
功能描述:串口0发送数据
输入参数: u8 tByte: 待发送的数据
返回参数:     
*************************************************************************************/
void SendByte(u8 tByte)
{
	bit bES0;
	bES0=ES0;
	ES0=0;
	S0BUF=tByte;
	while(TI0==0);
	TI0=0;
	ES0=bES0;
}



/***********************************************************************************
函数名称:void SendStr(u8* PStr)
功能描述:发送字符串
输入参数: u8* PStr:字符串起始地址
返回参数:     
*************************************************************************************/
void SendStr(u8* PStr)
{
	while(*PStr != 0)
	{
		SendByte(*PStr);
		PStr ++;
	}
}

/***********************************************************************************
函数名称:   void SendHex(u8 ToSend)
功能描述:用字符形式发送16进制数
输入参数: u8 ToSend: 待发送的16进制  
返回参数:     
*************************************************************************************/
void SendHex(u8 ToSend)
{
	u8 x;
//	SendByte(' ');
//	SendByte('0');
//	SendByte('x');
	x = ToSend >> 4;
	if(x < 10)
	{
		SendByte('0'+x);
	}
	else
	{
		SendByte('A'-10+x);
	}
	x= ToSend&0x0F;
	if(x < 10)
	{
		SendByte('0'+x);
	}
	else
	{
		SendByte('A'-10+x);
	}
}

/***********************************************************************************
函数名称:   void SendDW2DEC(u8 ToSend)
功能描述:发送10进制数
输入参数: u32 ToSend: 待发送的16进制 (4字节) 
返回参数:     
*************************************************************************************/
void SendDW2DEC(u32 ToSend)
{
	bit bFlag;
	u8 x;
	bFlag=0;
	x=ToSend/1000000000L;
	ToSend=ToSend%1000000000L;
	if(x!=0)
	{	bFlag=TRUE;
		SendByte('0'+x);
	}
	
	x=ToSend/100000000L;
	ToSend=ToSend%100000000L;
	if((x!=0)||(bFlag==TRUE))
	{	bFlag=TRUE;
		SendByte('0'+x);
	}
	
	x=ToSend/10000000L;
	ToSend=ToSend%10000000L;
	if((x!=0)||(bFlag==TRUE))
	{	bFlag=TRUE;
		SendByte('0'+x);
	}
	
	x=ToSend/1000000L;
	ToSend=ToSend%1000000L;
	if((x!=0)||(bFlag==TRUE))
	{	bFlag=TRUE;
		SendByte('0'+x);
	}
	
	x=ToSend/100000L;
	ToSend=ToSend%100000L;
	if((x!=0)||(bFlag==TRUE))
	{	bFlag=TRUE;
		SendByte('0'+x);
	}
	
	x=ToSend/10000L;
	ToSend=ToSend%10000L;
	if((x!=0)||(bFlag==TRUE))
	{	bFlag=TRUE;
		SendByte('0'+x);
	}
	
	x=ToSend/1000L;
	ToSend=ToSend%1000L;
	if((x!=0)||(bFlag==TRUE))
	{	bFlag=TRUE;
		SendByte('0'+x);
	}
	
	x=ToSend/100L;
	ToSend=ToSend%100L;
	if((x!=0)||(bFlag==TRUE))
	{	bFlag=TRUE;
		SendByte('0'+x);
	}
	
	x=ToSend/10L;
	if((x!=0)||(bFlag==TRUE))
	{	bFlag=TRUE;
		SendByte('0'+x);
	}
	
	x=ToSend%10L;
	SendByte('0'+x);
		
}

/***********************************************************************************
函数名称:   void ResetCounter()
功能描述:重置计数器
输入参数:  
返回参数:     
*************************************************************************************/
void ResetCounter()
{
	// 关闭T0,T1
	TM_GlobalSTOP(TIMER0_GLOBAL_BIT|TIMER1_GLOBAL_BIT);
	TM_SetT0HighByte(0);
	TM_SetT0LowByte(0);
	TM_SetT1HighByte(0);
	TM_SetT1LowByte(0);
	// 使能T0,T1
	TM_GlobalStart(TIMER0_GLOBAL_BIT|TIMER1_GLOBAL_BIT);
}
/***********************************************************************************
函数名称:   void SendCounter()
功能描述:输出计数
输入参数:  
返回参数:     
*************************************************************************************/
void SendCounter()
{
	DWordTypeDef dwTime;
	TM_GlobalSTOP(TIMER0_GLOBAL_BIT|TIMER1_GLOBAL_BIT);
	dwTime.B.BHigh=TH0;
	dwTime.B.BMHigh=TL0;
	dwTime.B.BMLow=TH1;
	dwTime.B.BLow=TL1;
	SendStr("\nTIME: ");
	SendDW2DEC(dwTime.DW);
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

	// 设置S0BRG Value
	UART0_SetS0BRGValue(S0BRG_BRGRL_115200_2X_48000000_1T);
	
	UART0_EnS0BRG();					// 开启S0BRG时钟
}

/***********************************************************************************
函数名称:   void InitTimer0(void)
功能描述:Timer0初始化设置
		 定义T0为16位定时器,时钟为Sysclk/192 6us~393216us	
输入参数:   
返回参数:     
*************************************************************************************/
void InitTimer0(void)
{
	TM_SetT0Mode_1_16BIT_TIMER();			// 设置T0模式为16位模式
	TM_SetT0Clock_T1OF();					// 设置T0时钟源为 T1OF
	TM_SetT0Gate_Disable();
	TM_SetT0LowByte(0);						// 设置T0低8位
	TM_SetT0HighByte(0);					// 设置T0高8位
	//TM_EnableT0();						// 使能T0
}

/***********************************************************************************
函数名称:   void InitTimer1(void)
功能描述:Timer1初始化设置
		 定义T1为16位定时器,时钟为Sysclk/48 1.5us~98304us 
输入参数:   
返回参数:     
*************************************************************************************/
void InitTimer1(void)
{
	TM_SetT1Mode_1_16BIT_TIMER();			// 设置T1模式为16位模式
	TM_SetT1Clock_SYSCLKDiv48();			// 设置T1时钟源为 SYSCLK/48
	TM_SetT1Gate_Disable();
	TM_SetT1LowByte(0);						// 设置T1低8位
	TM_SetT1HighByte(0);					// 设置T1高8位
	//TM_EnableT1();						// 使能T1
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
	PORT_SetP1PushPull(BIT5|BIT7);						// 设置P15(MOSI),P17(SPICLK)为推挽输出 
	PORT_SetP1OpenDrainPu(BIT6);	// 设置P16(MISO)为准双向口
	PORT_SetP1PushPull(BIT4);							// 设置P14为推挽输出 用于nSS
	PORT_P14P15P16P17_HighDrive();  
	PORT_P1_EnFastDrive(BIT4|BIT5|BIT6|BIT7);
}

/***********************************************************************************
函数名称:   void InitSPI_Master(void)
功能描述:   SPI初始化
输入参数:   
返回参数:     
*************************************************************************************/
void InitSPI_Master(void)
{
	
	SPI_Enable();									// 使能SPI
	SPI_SelectMASTERByMSTRbit();					// 设置MASTER
	SPI_SetClock(SPI_CLK_SYSCLK_4);					// 设置SPI时钟 SYSCLK/4 48M/4=12M
	SPI_SetCPOL_0();								// 设置SPICLK初始电平 CPOL=0 低电平
	SPI_SetDataMSB();								// 设置数据传送位序 MSB 高位在前
	SPI_SetCPHA_0();								// 设置采样和改变数据模式 CPHA=1 前沿改变数据,后沿采样
	SPI_SetUseP14P15P16P17P53P54();					// 设置IO nSS/P14,MOSI/P15,MISO/P16,SPICLK/P17

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
函数名称:   u8 SPITransceiver(u8 ToSend)
功能描述:   SPI主机模式收发数据
输入参数:   u8 SPI_DATA: 待发送的数据
返回参数:   u8 :收到的数据  
*************************************************************************************/
u8 SPITransceiver(u8 SPI_DATA)
{
	SPI_SendData(SPI_DATA);							// SPI 发送数据
	while(SPI_ChkCompleteFlag()==0);				// 等待SPI传送完成
	SPI_ClearCompleteFlag();						// SPI 清完成标志
	return SPI_GetData();							// 返回接收到的数据
}

/***********************************************************************************
函数名称:   void InitDMA_XRAM_TO_SPI(void)
功能描述:DMA初始化 通过SPI发送XRAM的数据 
输入参数:   
返回参数:     
*************************************************************************************/
void InitDMA_XRAM_TO_SPI()
{
	DMA_Disable();									// 停止DMA
	DMA_SetSource_XRAM();							// 设置来源为XRAM
	DMA_SetDestination_SPI0TX();					// 设置目标为SPI TX
	DMA_SetXRAMStartaddr((u16)(&TestBuf[0]));		// 设置XRAM的起始地址
	DMA_SetXRAMStartaddrRL((u16)(&TestBuf[0]));		// 设置XRAM额起始地址重载值
	DMA_SetCount(65536-256);						// 设置计数值 为256
	DMA_SetCountRL(65536-256);						// 设置计数重载值 为256
	DMA_SetTrig_Software();							// 设置软件触发启动DMA
	DMA_DisDCF0Int();								// 禁止DMA完成标志DCF0产生中断
	DMA_DisLoop();									// 禁止循环
	DMA_Enable();									// 使能DMA
}

/***********************************************************************************
函数名称:   void InitDMA_SPI_TO_XRAM(void)
功能描述:DMA初始化 通过SPI读取数据存储到XRAM
输入参数:   
返回参数:     
*************************************************************************************/
void InitDMA_SPI_TO_XRAM()
{
	DMA_Disable();									// 停止DMA
	DMA_SetSource_SPI0RX();							// 设置来源为SPI RX
	DMA_SetDestination_XRAM();						// 设置目标为XRAM
	DMA_SetXRAMStartaddr((u16)(&TestBuf[0]));		// 设置XRAM的起始地址
	DMA_SetXRAMStartaddrRL((u16)(&TestBuf[0]));		// 设置XRAM额起始地址重载值
	DMA_SetCount(65536-512);						// 设置计数值 为512
	DMA_SetCountRL(65536-512);						// 设置计数重载值 为512
	DMA_SetTrig_Software();							// 设置软件触发启动DMA
	DMA_DisDCF0Int();								// 禁止DMA完成标志DCF0产生中断
	DMA_DisLoop();									// 禁止循环
	DMA_Enable();									// 使能DMA
}

/*
*************************************************************************************
*  SPI FLASH EN25Q32A
*
*************************************************************************************
*/

/***********************************************************************************
函数名称:	u8 EN25Q32A_Flash_1ByteRead (u32 Address)
功能描述:	读取EN25Q32A指定地址的一个BYTE 

输入参数:	uint32_t Address: 24位指定地址 0x00000000 ~ 0xFFFFFFFF
返回参数: uint8_t 读取到的内容	  
*************************************************************************************/
u8 EN25Q32A_Flash_1ByteRead (u32 Address)
{ 
	u8 x;
	SPI_nSS = 0;
    
    /* Write command */
    SPITransceiver(0x03);
	
    /* Write address */
    SPITransceiver((u8)(Address>>16));
    SPITransceiver((u8)(Address>>8));
    SPITransceiver((u8)(Address));
    
    /* Read data */
    x=SPITransceiver(Dummy_Data);

	SPI_nSS = 1;

    return  x;
}


/***********************************************************************************
函数名称:	void EN25Q32A_Flash_MultiBytesRead (u32 Address, u8 *BufferAddreass, u32 Length)
功能描述:	读取EN25Q32A指定地址的多个BYTE 

输入参数:	  u32 Address: 24位指定地址 0x00000000 ~ 0xFFFFFFFF
		  u8 *BufferAddreass: 存储读取内容的缓存地址
		  u16 Length: 读取内容的长度
返回参数: 
*************************************************************************************/
void EN25Q32A_Flash_MultiBytesRead (u32 Address, u8 *BufferAddreass, u16 Length)
{
	SPI_nSS = 0;
    /* Write command */
    SPITransceiver(0x03);
	
    /* Write address */
    SPITransceiver((u8)(Address>>16));
    SPITransceiver((u8)(Address>>8));
    SPITransceiver((u8)(Address));
    
    /* Read data */
    while(Length !=0)
	{
		*BufferAddreass=SPITransceiver(Dummy_Data);
        BufferAddreass ++;
        Length--;
	}
	SPI_nSS = 1;
}

/***********************************************************************************
函数名称:	u32 EN25Q32A_Flash_Read_ID (void)
功能描述:	读取EN25Q32A ID 

输入参数:	
返回参数: uint32_t 读取到的ID 
*************************************************************************************/
u32 EN25Q32A_Flash_Read_ID (void)
{
	DWordTypeDef RDAT;
    
    SPI_nSS = 0;
    /* Write command */
    SPITransceiver(0x9F);
    
    /* Read ID */
    RDAT.B.BMHigh=SPITransceiver(Dummy_Data);
    RDAT.B.BMLow=SPITransceiver(Dummy_Data);
    RDAT.B.BLow=SPITransceiver(Dummy_Data);
	SPI_nSS  = 1;
    
    return RDAT.DW;
}

/***********************************************************************************
函数名称:	void EN25Q32A_Flash_Write_Enable (void)
功能描述: 使能写Flash

输入参数:	
返回参数: 
*************************************************************************************/
void EN25Q32A_Flash_Write_Enable(void)
{
	u8 RDAT;
	
    
  Re_Flash_Write_Enable:
    /* Write enable */
	SPI_nSS  = 0;
	SPITransceiver(0x06);
	SPI_nSS  = 1;

    /* Read status register */
	SPI_nSS  = 0;
	SPITransceiver(0x05);			//RDSR
   
	RDAT = SPITransceiver(Dummy_Data);                                  // Get received data

	SPI_nSS  = 1;
	
    /* Check WEL == 1 */
	if((RDAT & 0x02) == 0x00)
		goto Re_Flash_Write_Enable;
}

/***********************************************************************************
函数名称:	void EN25Q32A_Flash_Check_Busy(void)
功能描述:	检测EN25Q32A是否忙

输入参数:	
返回参数: 
*************************************************************************************/
void EN25Q32A_Flash_Check_Busy(void)
{
	u8 RDAT;
    
    
    /* Read status register */
	Re_Flash_Check_Busy:
	SPI_nSS  = 0;
	SPITransceiver(0x05);							//RDSR

	RDAT = SPITransceiver(Dummy_Data);                                  // Get received data
	
    /* Check erase or write complete */
	SPI_nSS  = 1;
	if((RDAT & 0x01) == 0x01)			//WIP, write in progress
		goto Re_Flash_Check_Busy;
}

/***********************************************************************************
函数名称:	void EN25Q32A_Flash_Page_Program(u32 Addr, u8 *DataSource, u16 Length)
功能描述:	EN25Q32A 写操作

输入参数:	u32 Addr: 写起始地址. 0x00000000 ~ 0xFFFFFFFF.
		  u8 *DataSource: 待写数据缓存起始地址. 
		  u16 Length: 待写数据长度, 最大256字节
返回参数: 
*************************************************************************************/ 
void EN25Q32A_Flash_Page_Program(u32 Addr, u8 *DataSource, u16 Length)
{
    EN25Q32A_Flash_Write_Enable();
    
	SPI_nSS  = 0;
	SPITransceiver( 0x02);                               // Write command 
	
    /* Write address */
    SPITransceiver((u8)(Addr>>16));
    SPITransceiver((u8)(Addr>>8));
    SPITransceiver((u8)(Addr));
	
	while(Length!=0)                                            // Write Data
	{
		SPITransceiver(*DataSource);
        DataSource++;
		Length--;
	}
	
	SPI_nSS  = 1;
   
	EN25Q32A_Flash_Check_Busy();                                       // Wait program complete
}

/***********************************************************************************
函数名称:	void EN25Q32A_Flash_Read_Start(u32 Addr)
功能描述:	EN25Q32A 启动读操作

输入参数:	u32Addr: 写起始地址. 0x00000000 ~ 0xFFFFFFFF.
返回参数: 
*************************************************************************************/ 
void EN25Q32A_Flash_Read_Start(u32 Addr)
{

	SPI_nSS  = 0;
   
    /* Write command */
    SPITransceiver(0x03);
	
    /* Write address */
    SPITransceiver((u8)(Addr>>16));
    SPITransceiver((u8)(Addr>>8));
    SPITransceiver((u8)(Addr));
    
	
}
/***********************************************************************************
函数名称:	void EN25Q32A_Flash_Read_Stop(void)
功能描述:	EN25Q32A 停止读操作

输入参数:	
返回参数: 
*************************************************************************************/ 
void EN25Q32A_Flash_Read_Stop()
{

	SPI_nSS  = 1;
}

/***********************************************************************************
函数名称:	void EN25Q32A_Flash_Page_Program_Start(u32 Addr)
功能描述:	EN25Q32A 启动页写操作

输入参数:	u32Addr: 写起始地址. 0x00000000 ~ 0xFFFFFFFF.
返回参数: 
*************************************************************************************/ 
void EN25Q32A_Flash_Page_Program_Start(u32 Addr)
{

    EN25Q32A_Flash_Write_Enable();
    
	SPI_nSS  = 0;
	SPITransceiver( 0x02);                               // Write command 
                
    /* Write address */
    SPITransceiver((u8)(Addr>>16));
    SPITransceiver((u8)(Addr>>8));
    SPITransceiver((u8)(Addr));
    
	
}

/***********************************************************************************
函数名称:	void EN25Q32A_Flash_Program_One_Word(u16 wData)
功能描述:	EN25Q32A 写一个WORD(16bit)

输入参数:	u16 wData: 待写的数据
返回参数: 
*************************************************************************************/ 
void EN25Q32A_Flash_Program_One_Word(u16 wData)

{
    SPITransceiver((u8)(wData>>8));
    SPITransceiver((u8)(wData));
}

/***********************************************************************************
函数名称:	void EN25Q32A_Flash_Page_Program_Stop(void)
功能描述:	EN25Q32A 停止页写操作

输入参数:	
返回参数: 
*************************************************************************************/ 
void EN25Q32A_Flash_Page_Program_Stop()
{

	SPI_nSS  = 1;
	EN25Q32A_Flash_Check_Busy();                                       // Wait program complete
}

/***********************************************************************************
函数名称:	void EN25Q32A_Flash_Block_Erase(u32 Addr)
功能描述:	EN25Q32A 快擦除操作

输入参数:	u32 Addr: 要擦除Block 的地址.

返回参数: 
*************************************************************************************/ 
void EN25Q32A_Flash_Block_Erase(u32 Addr)
{
    EN25Q32A_Flash_Write_Enable();
    
	SPI_nSS  = 0;
	SPITransceiver(0xD8);                               			// Block Erase command 
                
	
    /* Write address */
    SPITransceiver((u8)(Addr>>16));
    SPITransceiver((u8)(Addr>>8));
    SPITransceiver((u8)(Addr));
	
	SPI_nSS  = 1;
   
	EN25Q32A_Flash_Check_Busy();                                       // Wait Block Erase complete
}


/*
*************************************************************************************
*/ 


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
	InitSPI_Master();
	InitUart0_S0BRG();
	InitTimer0();
	InitTimer1();
}


void main()
{
	u32 i;
	
	DWordTypeDef dwTemp;
	
    InitSystem();
	
	SendStr("\nHello!");
	LED_G_1=0;LED_R=0;
	DelayXms(1000);
	LED_G_1=1;LED_R=1;


	
	dwTemp.DW=EN25Q32A_Flash_Read_ID();
	SendStr("\nEN25Q32A ID: ");
	SendHex(dwTemp.B.BMHigh);
	SendHex(dwTemp.B.BMLow);
	SendHex(dwTemp.B.BLow);

	// 无DMA测试
	SendStr("\nEN25Q32A Erase...");
	i=FLASH_TEST_SIZE_MAX/FLASH_BLOCK_SIZE;
	// 擦除Flash
	dwTemp.DW=FLASH_TEST_START_ADDRESS;
	ResetCounter();
	while(i!=0)
	{
		LED_R = !LED_R;
		EN25Q32A_Flash_Block_Erase(dwTemp.DW);
		dwTemp.DW=dwTemp.DW+FLASH_BLOCK_SIZE;
		i--;
	}
	SendCounter();
	for(i=0;i<256;i++)
	{
		TestBuf[i]=i;
	}
	// 写Flash
	SendStr("\nEN25Q32A Write 1M");
	i=FLASH_TEST_SIZE_MAX/256;		
	dwTemp.DW=FLASH_TEST_START_ADDRESS;
	ResetCounter();
	while(i!=0)
	{
		LED_R = !LED_R;
		EN25Q32A_Flash_Page_Program(dwTemp.DW,&TestBuf[0],256);
		dwTemp.DW=dwTemp.DW+256;
		i--;
	}
	SendCounter();
	// 读Flash
	SendStr("\nEN25Q32A Read 1M");
	i=FLASH_TEST_SIZE_MAX/512;		
	dwTemp.DW=FLASH_TEST_START_ADDRESS;
	ResetCounter();
	while(i!=0)
	{
		LED_R = !LED_R;
		EN25Q32A_Flash_MultiBytesRead(dwTemp.DW,&TestBuf[0],512);
		dwTemp.DW=dwTemp.DW+512;
		i--;
	}
	SendCounter();


	// 有DMA测试
	SendStr("\nEN25Q32A Erase...");
	i=FLASH_TEST_SIZE_MAX/FLASH_BLOCK_SIZE;
	// 擦除Flash
	dwTemp.DW=FLASH_TEST_START_ADDRESS;
	ResetCounter();
	while(i!=0)
	{
		LED_R = !LED_R;
		EN25Q32A_Flash_Block_Erase(dwTemp.DW);
		dwTemp.DW=dwTemp.DW+FLASH_BLOCK_SIZE;
		i--;
	}
	SendCounter();
	for(i=0;i<256;i++)
	{
		TestBuf[i]=i;
	}
	// 写Flash
	SendStr("\nDMA EN25Q32A Write 1M");
	i=FLASH_TEST_SIZE_MAX/256;		
	dwTemp.DW=FLASH_TEST_START_ADDRESS;
	InitDMA_XRAM_TO_SPI();					// 配置DMA,从XRAM传送256个数据到SPI
	ResetCounter();
	while(i!=0)
	{
		LED_R = !LED_R;
		EN25Q32A_Flash_Page_Program_Start(dwTemp.DW);
		DMA_ClearCompleteFlag();			// 清DMA完成标志
		DMA_Start();						// 启动DMA 开始传送256字节的数据
		while(DMA_ChkCompleteFlag()==0)		// 等待DMA完成
		{

		}
		while(SPI_ChkCompleteFlag()==0);	// 等待最后一个数据发送完成
		SPI_ClearCompleteFlag();			// 清SPI完成标志
		EN25Q32A_Flash_Page_Program_Stop();
		dwTemp.DW=dwTemp.DW+256;
		i--;
	}
	SendCounter();
	// 读Flash
	SendStr("\nDMA EN25Q32A Read 1M");
	i=FLASH_TEST_SIZE_MAX/512;		
	dwTemp.DW=FLASH_TEST_START_ADDRESS;
	InitDMA_SPI_TO_XRAM();					// 配置DMA,从SPI读取512个数据到XRAM
	ResetCounter();
	EN25Q32A_Flash_Read_Start(dwTemp.DW);
	while(i!=0)
	{
		LED_R = !LED_R;
		DMA_ClearCompleteFlag();			// 清DMA完成标志
		DMA_Start();						// 启动DMA 开始从SPI读取512字节的数据
		while(DMA_ChkCompleteFlag()==0)		// 等待DMA完成
		{

		}
		i--;
	}
	EN25Q32A_Flash_Read_Stop();				
	SendCounter();
	
	
    while(1)
    {
    	DelayXms(100);
		LED_G_1 = !LED_G_1;
    }
}


