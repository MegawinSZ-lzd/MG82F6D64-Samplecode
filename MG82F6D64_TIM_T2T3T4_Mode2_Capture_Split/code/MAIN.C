/*********************************************************************
    项目名称:MG82F6D64-DEMO
			测试板使用 MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=32MHz, SysCLK=32MHz
		功能描述：
			定义T2为Mode2:捕获模式,分割模式
			定义T3为Mode2:捕获模式,分割模式
			定义T4为Mode2:捕获模式,分割模式
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

#define TIMER_12T_1ms_TH	((65536-(u16)(float)(1000*((float)(MCU_SYSCLK)/(float)(12000000)))) /256) 			
#define TIMER_12T_1ms_TL	((65536-(u16)(float)(1000*((float)(MCU_SYSCLK)/(float)(12000000)))) %256)

#define IO_TEST_0		P52
#define IO_TEST_1		P53
#define IO_TEST_2		P54
#define IO_TEST_3		P55
#define IO_TEST_4		P56
#define IO_TEST_5		P57

#define LED_G_0			P55
#define LED_R			P56
#define LED_G_1			P57

#define CAPTURE_BUF_SIZE	8
bit bT2Capture;
xdata u8 T2CaptureData[CAPTURE_BUF_SIZE];
u8 T2CaptureCnt;

bit bT3Capture;
xdata u8 T3CaptureData[CAPTURE_BUF_SIZE];
u8 T3CaptureCnt;

bit bT4Capture;
xdata u8 T4CaptureData[CAPTURE_BUF_SIZE];
u8 T4CaptureCnt;


/***********************************************************************************
函数名称:   void INT_T2(void)
功能描述:T2 中断服务程序
输入参数:   
返回参数:     
*************************************************************************************/
void INT_T2(void) interrupt INT_VECTOR_T2
{
	_push_(SFRPI);
	SFR_SetPage(0);			// TF2,EXF2仅在0页操作, 所以需设置页面为0, 而且在中断里需保护SFRPI
	if(TF2)
	{
		IO_TEST_0=!IO_TEST_0;
		TF2=0;
	}
	if(EXF2)
	{
		EXF2=0;
		if(bT2Capture==0)
		{
			T2CaptureData[T2CaptureCnt]=RCAP2H;
			T2CaptureCnt++;
			if(T2CaptureCnt>=CAPTURE_BUF_SIZE)
			{
				bT2Capture=TRUE;
			}
		}
	}
	if(RCLK_TF2L)
	{
		IO_TEST_3=!IO_TEST_3;
		RCLK_TF2L=0;
	}
	_pop_(SFRPI);
}

/***********************************************************************************
函数名称:   void INT_T3(void)
功能描述:T3 中断服务程序
输入参数:   
返回参数:     
*************************************************************************************/
void INT_T3(void) interrupt INT_VECTOR_T3
{
	_push_(SFRPI);
	SFR_SetPage(1);				// TF3仅在1页操作
	if(TF3)
	{
		TF3=0;
		SFR_SetPage(0);			// P5仅在Page 0
		IO_TEST_1=!IO_TEST_1;
	}
	SFR_SetPage(1);				// EXF3仅在1页操作
	if(EXF3)
	{
		EXF3=0;
		if(bT3Capture==0)
		{
			T3CaptureData[T3CaptureCnt]=RCAP3H;
			T3CaptureCnt++;
			if(T3CaptureCnt>=CAPTURE_BUF_SIZE)
			{
				bT3Capture=TRUE;
			}
		}
	}
	SFR_SetPage(1);				// TF3L仅在1页操作
	if(TF3L)
	{
		TF3L=0;
		SFR_SetPage(0); 		// P5仅在Page 0
		IO_TEST_4=!IO_TEST_4;
	}
	_pop_(SFRPI);
}

/***********************************************************************************
函数名称:   void INT_T4(void)
功能描述:T4 中断服务程序
输入参数:   
返回参数:     
*************************************************************************************/
void INT_T4(void) interrupt INT_VECTOR_T4
{
	_push_(SFRPI);
	SFR_SetPage(2);				// TF4仅在2页操作
	if(TF4)
	{
		TF4=0;
		SFR_SetPage(0);			// P5仅在Page 0
		IO_TEST_2=!IO_TEST_2;
	}
	SFR_SetPage(2);				// EXF4仅在2页操作
	if(EXF4)
	{
		EXF4=0;
		if(bT4Capture==0)
		{
			T4CaptureData[T4CaptureCnt]=RCAP4H;
			T4CaptureCnt++;
			if(T4CaptureCnt>=CAPTURE_BUF_SIZE)
			{
				bT4Capture=TRUE;
			}
		}
	}
	SFR_SetPage(2);				// TF4L仅在2页操作
	if(TF4L)
	{
		TF4L=0;
		SFR_SetPage(0); 		// P5仅在Page 0
		IO_TEST_5=!IO_TEST_5;
	}
	_pop_(SFRPI);
}

/***********************************************************************************

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
函数名称:   void InitTimer2(void)
功能描述:Timer2初始化设置	
输入参数:   
返回参数:     
*************************************************************************************/
void InitTimer2(void)
{
	TM_SetT2Mode_Capture();					// 设置T2模式2为捕获模式
	TM_SetT2Clock_SYSCLKDiv12();			// 设置T2时钟源为 SYSCLK/12
	TM_SetTL2Clock_SYSCLKDiv12();			// 设置T2L时钟源为 SYSCLK/12
	TM_EnableAccessTF2L();					// 使能访问TF2L和TF2L中断
	TM_DisableTR2LC();						// 禁用TR2LC以防止TH2重载时清除TL2
	TM_EnT2Split();							// 使能T2分割模式
	
	TM_SetT2LowByte(0);						// 设置T2低8位				
	TM_SetT2RLLowByte(0x40);				// 设置重载值低8位		  ---T2L溢出频率为 SYSCLK/12/(256-0x40)

	TM_SetT2HighByte(0);					// 设置T2高8位
	TM_SetT2RLHighByte(0);					// 设置重载值高8位

	TM_SetT2Capture_T2EXPin();				// 使能T2外部中断为T2EX引脚
	TM_SetT2CKOT2EX_P10P11();				// 使能T2EX引脚为P11
	TM_EnableT2EX_DetectFalling();			// 使能T2EX引脚检测下降沿

	TM_EnableT2();							// 使能T2
	TM_EnableTL2();							// 使能TL2
}

/***********************************************************************************
函数名称:   void InitTimer3(void)
功能描述:Timer3初始化设置
输入参数:   
返回参数:     
*************************************************************************************/
void InitTimer3(void)
{
	TM_SetT3Mode_Capture();					// 设置T3模式2为捕获模式
	TM_SetT3Clock_SYSCLKDiv12();			// 设置T3时钟源为 SYSCLK/12
	TM_SetTL3Clock_SYSCLKDiv12();			// 设置T3L时钟源为 SYSCLK/12
	
	
	TM_SetT3LowByte(0);						// 设置T3低8位				
	TM_SetT3RLLowByte(0x80);				// 设置重载值低8位		  ---T3L溢出频率为 SYSCLK/12/(256-0x80)

	TM_SetT3HighByte(0);					// 设置T3高8位
	TM_SetT3RLHighByte(0);					// 设置重载值高8位

	TM_SetT3Capture_T3EXPin();				// 使能T3外部中断为T3EX引脚
	TM_SetT3CKOT3EX_P33P32();				// 设置T3/T3CKO:P33,T3EX:P32
	TM_EnableT3EX_DetectFalling();			// 使能T3EX引脚检测下降沿
	TM_EnT3Split();							// 使能T3分割模式
	
	TM_DisableTR3LC();						// 禁用TR3LC以防止TH3重载时清除TL3

	TM_EnableT3();							// 使能T3
	TM_EnableTL3();							// 使能TL3

}

/***********************************************************************************
函数名称:   void InitTimer4(void)
功能描述:Timer4初始化设置
输入参数:   
返回参数:     
*************************************************************************************/
void InitTimer4(void)
{
	TM_SetT4Mode_Capture();					// 设置T4模式2为捕获模式
	TM_SetT4Clock_SYSCLKDiv12();			// 设置T4时钟源为 SYSCLK/12
	TM_SetTL4Clock_SYSCLKDiv12();			// 设置T4L时钟源为 SYSCLK/12
	
	
	TM_SetT4LowByte(0);						// 设置T4低8位				
	TM_SetT4RLLowByte(0x80);				// 设置重载值低8位		  ---T4L溢出频率为 SYSCLK/12/(256-0x80)

	TM_SetT4HighByte(0);					// 设置T4高8位
	TM_SetT4RLHighByte(0);					// 设置重载值高8位

	TM_SetT4Capture_T4EXPin();				// 使能T4外部中断为T4EX引脚
	TM_SetT4CKOT4EX_P36P37();				// 设置T4/T4CKO:P36,T4EX:P37
	TM_EnableT4EX_DetectFalling();			// 使能T4EX引脚检测下降沿
	TM_EnT4Split();							// 使能T4分割模式
	
	TM_DisableTR4LC();						// 禁用TR4LC以防止TH4重载时清除TL4

	TM_EnableT4();							// 使能T4
	TM_EnableTL4();							// 使能TL4

}

/***********************************************************************************
函数名称:   void InitInterrupt(void)
功能描述:InitInterrupt初始化设置
输入参数:   
返回参数:     
*************************************************************************************/
void InitInterrupt(void)
{
	INT_EnTIMER2();
	INT_EnTIMER2L();						// 使能TF2L中断
	INT_EnTIMER3();
	INT_EnTIMER3L();						// 使能TF3L中断
	INT_EnTIMER4();
	INT_EnTIMER4L();						// 使能TF4L中断
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
	PORT_SetP1OpenDrainPu(BIT1);										// 设置P11为开漏带上拉，用于T2EX引脚
	PORT_SetP3QuasiBi(BIT2|BIT3|BIT6|BIT7);								// 设置P32,P37为准双向口，用于T3EX,T4EX引脚
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
	UART0_SetS0BRGValue(S0BRG_BRGRL_57600_2X_32000000_1T);
	
	UART0_EnS0BRG();					// 开启S0BRG时钟
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
	
	InitTimer2();			
	InitTimer3();			
	InitTimer4();
	
	InitInterrupt();		// 中断初始化
	
	INT_EnAll();			// 使能全局中断

}



void main()
{
	
	u8 i;
	
    InitSystem();
	
 	LED_G_0=0;LED_G_1=0;LED_R=0;
	DelayXms(1000);
 	LED_G_0=1;LED_G_1=1;LED_R=1;

					
    while(1)
    {
    	DelayXms(100);
    	if(bT2Capture)
    	{
			SendStr("\nT2 Caputre:");
			for(i=0;i<CAPTURE_BUF_SIZE;i++)
			{
				SendByte('0');
				SendByte('x');
				SendHex(T2CaptureData[i]);
				SendByte(' ');
			}
			T2CaptureCnt=0;
			bT2Capture=0;
    	}
		if(bT3Capture)
		{
			SendStr("\nT3 Caputre:");
			for(i=0;i<CAPTURE_BUF_SIZE;i++)
			{
				SendByte('0');
				SendByte('x');
				SendHex(T3CaptureData[i]);
				SendByte(' ');
			}
			T3CaptureCnt=0;
			bT3Capture=0;
		}
		if(bT4Capture)
		{
			SendStr("\nT4 Caputre:");
			for(i=0;i<CAPTURE_BUF_SIZE;i++)
			{
				SendByte('0');
				SendByte('x');
				SendHex(T4CaptureData[i]);
				SendByte(' ');
			}
			T4CaptureCnt=0;
			bT4Capture=0;
		}
    }
}


