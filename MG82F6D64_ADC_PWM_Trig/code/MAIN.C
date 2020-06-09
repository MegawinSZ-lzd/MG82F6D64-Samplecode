/*********************************************************************
    项目名称:MG82F6D64-DEMO
    作    者:YZY
			测试板使用 MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=32MHz, SysCLK=32MHz
	功能描述：
			使用PCA0溢出触发ADC转换, ADC值通过DMA发送到XRAM
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
#define MCU_SYSCLK		32000000
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


#define PWM_MIN			(0*4)
#define PWM_MAX			(1024*4)
#define PWM_3_4			(768*4)
#define PWM_2_4			(512*4)
#define PWM_1_4			(256*4)
#define PWM_LOW			(62*4)
#define PWM_HINT		(64*4)

#define PCA_RELOAD		(PWM_MAX)

#define PCA_C           (65536)       	

#define PCA_CL(x)		(u8)((65536-(x))%256) 
#define PCA_CH(x)     	(u8)((65536-(x))/256)          

#define TEST_ADC_DATA_CNT		16

xdata WordTypeDef TestBuf[TEST_ADC_DATA_CNT];
u8 TestBufLen;
xdata WordTypeDef ADCBuf[TEST_ADC_DATA_CNT];
u8 ADCBufLen;
u8 ChannelInx;
bit bAdcDMAFlag;

/***********************************************************************************
函数名称:   void SendByte(u8 tByte)
功能描述:通过UART发送一个字节
输入参数: 
		u8 tByte: 待发送的数据
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
函数名称:   void SendStr(u8* PStr)
功能描述:通过UART用字符串
输入参数: 
		u8* PStr: 字符串地址  
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
功能描述:通过UART用ASCII码发16进制的数
输入参数: 
		u8 ToSend: 待发送的16进制数  
返回参数:

*************************************************************************************/
void SendHex(u8 ToSend)
{
	u8 x;
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
函数名称:   void INT_PCA(void)
功能描述:PCA 中断服务程序
		 
输入参数:   
返回参数:     
*************************************************************************************/
void INT_PCA(void) interrupt INT_VECTOR_PCA
{
	_push_(SFRPI);
	SFRPI=0;
	if(CF)
	{
		CF=0;
		LED_R=!LED_R;
	}
	_pop_(SFRPI);
}



/***********************************************************************************
函数名称:   void INT_DMA(void)
功能描述:DMA 中断服务程序
		 
输入参数:   
返回参数:     
*************************************************************************************/
void INT_DMA(void) interrupt INT_VECTOR_DMA
{
	u8 i;
	_push_(SFRPI);
	SFRPI=0;
	LED_G_1=!LED_G_1;
	bAdcDMAFlag=TRUE;
	DMACR0=DMACR0&(~DCF0);		// 清中断标志位
	for(i=0;i<(TEST_ADC_DATA_CNT);i++)
	{
		ADCBuf[i].B.BHigh=TestBuf[i].B.BLow;
		ADCBuf[i].B.BLow=TestBuf[i].B.BHigh;
	}
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
	UART0_SetRxTxP30P31();				// 设置RXD/TXD 为P30/P31
	// 设置S0BRG Value
	UART0_SetS0BRGValue(S0BRG_BRGRL_115200_2X_32000000_1T);
	
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
	PORT_SetP1AInputOnly(BIT0|BIT1|BIT5);			// 设置P10(AIN0),P11(AIN1),P15(AIN5)为仅模拟输入
	PORT_SetP2PushPull(BIT2|BIT4|BIT6);		// 设置P22(CEX0),P24(CEX2),P26(CEX4)为推挽输出
	PORT_SetP3PushPull(BIT3|BIT4|BIT5);				// 设置P33(CEX1),P34(CEX3),P35(CEX5)为推挽输出
}

/***********************************************************************************
函数名称:   void InitPWM(void)
功能描述:   PWM初始化
		PCA时钟: 96MHz
		CH0(P22)/CH1(P33),CH2(P24)/CH3(P34),CH4(P17)/CH5(P35)
		3对互补,死区,中心对齐,分辨率为2048
		PWM频率: 96MHz/(2048*4)=11.718KHz
输入参数:   
返回参数:     
*************************************************************************************/
void InitPWM(void)
{
	PCA_SetCLOCK_CKMIX16();			// 96MHz
	
	PCA_CH0_SetMode_PWM();
	PCA_CH1_SetMode_PWM();
	PCA_CH2_SetMode_PWM();
	PCA_CH3_SetMode_PWM();
	PCA_CH4_SetMode_PWM();
	PCA_CH5_SetMode_PWM();

	PCA_CH0_SetPWM_16Bit();
	PCA_CH1_SetPWM_16Bit();
	PCA_CH2_SetPWM_16Bit();
	PCA_CH3_SetPWM_16Bit();
	PCA_CH4_SetPWM_16Bit();
	PCA_CH5_SetPWM_16Bit();

	PCA_CH01_SetPWM_EnBufferMode();		// CH0,CH1 使能缓冲模式
	PCA_CH23_SetPWM_EnBufferMode();		// CH2,CH3 使能缓冲模式
	PCA_CH45_SetPWM_EnBufferMode();		// CH4,CH5 使能缓冲模式
	PCA_CH01_SetPWM_EnDeadTime();		// CH0,CH1 使能死区
	PCA_CH23_SetPWM_EnDeadTime();		// CH2,CH3 使能死区
	PCA_CH45_SetPWM_EnDeadTime();		// CH2,CH3 使能死区

	PCA_SetPWM_CenterAligned();			// 中心对齐
	PCA_SetCenterAligned_EnVar();		// 中心对齐下使能可变解析度
	PCA_SetCenterAligned_CFonTop();		// 中心对齐下CF在顶部部置位

	PCA_SetDeadTimePreScaler_SysclkDiv8();	// 设置死区预分频 SysClk/8
	PCA_SetDeadTimePeriod(4);			// 设置死区时间 4*(1/PreScaler)=4*(1/(Sysclk/8))=32/Sysclk

	PCA_SetCounter(PCA_C-PCA_RELOAD);
	PCA_SetCounterReload(PCA_C-PCA_RELOAD);

	// 设置PWM占空比比较值
	PCA_CH0_SetValue(PCA_CH(PWM_1_4),PCA_CL(PWM_1_4));
	PCA_CH1_SetValue(PCA_CH(PWM_1_4),PCA_CL(PWM_1_4));
	PCA_CH2_SetValue(PCA_CH(PWM_2_4),PCA_CL(PWM_2_4));
	PCA_CH3_SetValue(PCA_CH(PWM_2_4),PCA_CL(PWM_2_4));
	PCA_CH4_SetValue(PCA_CH(PWM_3_4),PCA_CL(PWM_3_4));
	PCA_CH5_SetValue(PCA_CH(PWM_3_4),PCA_CL(PWM_3_4));

	// 使能PWM输出
	PCA_SetPWM0_EnOutput();					
	PCA_SetPWM1_EnOutput();
	PCA_SetPWM2_EnOutput();
	PCA_SetPWM3_EnOutput();
	PCA_SetPWM4_EnOutput();
	PCA_SetPWM5_EnOutput();

	PCA_SetCEX0CEX2CEX4_P22P24P26();	// 设置CEX0:P22,CEX2:P24,CEX4:P26
	PCA_SetCEX1CEX3CEX5_P33P34P35();	// 设置CEX1:P33,CEX2:P34,CEX4:P35
	PCA_CF_EnInterrupt();				// 使能CF中断

	PCA_EnPCACounter();					// 使能PCA计数器, 
		
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
	ADC_SetClock_SYSCLKDiv2();						// ADC转换时钟为 SYSCLK       转换率= 32M/2/30=533.3K
	ADC_SetMode_PCA0Overflow();						// ADC启动模式, PCA0溢出触发
	ADC_SetADCData_12Bit();							// ADC数据位数为12位
	ADC_SetRightJustified();						// ADC转换数据右对齐
	ADC_SetChannel_AIN0();							// ADC设置通道0
	ChannelInx=0;
}

/***********************************************************************************
函数名称:   void InitDMA_ADC0_TO_XRAM(void)
功能描述:DMA初始化 ADC0->XRAM
输入参数:   
返回参数:     
*************************************************************************************/
void InitDMA_ADC0_TO_XRAM()
{
	DMA_SetSource_ADC0();									// 设置来源为 ADC0
	DMA_SetDestination_XRAM();								// 设置目标为 XRAM
	DMA_SetXRAMStartaddr((u16)(&TestBuf[0].BUF[0]));		// 设置目标XRAM起始地址 
	DMA_SetXRAMStartaddrRL((u16)(&TestBuf[0].BUF[0]));		// 设置目标XRAM重载起始地址
	DMA_SetCount(65536-(TEST_ADC_DATA_CNT*2));				// 设置计数值
	DMA_SetCountRL(65536-(TEST_ADC_DATA_CNT*2));			// 设置重载计数值
	DMA_SetTrig_Software();									// 设置触发方式为软件触发
	DMA_EnDCF0Int();										// 使能计数溢出中断
	DMA_EnLoop();											// 使能循环模式
	DMA_Enable();											// 使能DMA
}

/***********************************************************************************
函数名称:   void InitInterrupt(void)
功能描述:   中断初始化
输入参数:   
返回参数:     
*************************************************************************************/
void InitInterrupt(void)
{
	INT_EnDMA();											// 使能DMA中断
	INT_EnPCA();											// 使能PCA中断
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
	InitUart0_S0BRG();
	InitADC();
	InitPWM();
	InitDMA_ADC0_TO_XRAM();
	InitInterrupt();

}


void main()
{
	u8 i,cnt;
	WordTypeDef wTemp;
	
    InitSystem();

	LED_G_0=0;LED_G_1=0;LED_R=0;
	DelayXms(1000);
	LED_G_0=1;LED_G_1=1;LED_R=1;
	
	
	DMA_Start();
	ADC_SoftStart();
	INT_EnAll();

	cnt=0;
    while(1)
    {
		if(bAdcDMAFlag)
		{
			bAdcDMAFlag=0;
			SendStr("\nADC:");
			wTemp.W=0;
			for(i=0;i<(TEST_ADC_DATA_CNT);i++)
			{
				wTemp.W=wTemp.W+ADCBuf[i].W;
			}
			wTemp.W=wTemp.W>>4;;
			SendHex(wTemp.B.BHigh);
			SendHex(wTemp.B.BLow);

			cnt++;
			if(cnt>=200)
			{
				cnt=0;
				LED_G_0=!LED_G_0;
			}
		}

    }
}



