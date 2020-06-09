/*********************************************************************
    项目名称:MG82F6D64-DEMO
			测试板使用 MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=24MHz, SysCLK=24MHz
		功能描述：
			定义T2为Mode6:DutyCapture模式
			定义T3为Mode6:DutyCapture模式
			定义T3为Mode6:DutyCapture模式
		
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
#define MCU_SYSCLK		24000000
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

bit bT2Captureing;
bit bT2CaptureFirst;
bit bT2CaptureOk;
WordTypeDef T2CaptureFirst;
WordTypeDef T2CaptureSecond;

bit bT3Captureing;
bit bT3CaptureFirst;
bit bT3CaptureOk;
WordTypeDef T3CaptureFirst;
WordTypeDef T3CaptureSecond;

bit bT4Captureing;
bit bT4CaptureFirst;
bit bT4CaptureOk;
WordTypeDef T4CaptureFirst;
WordTypeDef T4CaptureSecond;

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
		TF2=0;
		IO_TEST_0=!IO_TEST_0;
		
	}
	if(EXF2)
	{
		EXF2=0;
		if(bT2Captureing==TRUE)
		{
			if(bT2CaptureFirst==FALSE)
			{
				bT2CaptureFirst=TRUE;
				T2CaptureFirst.B.BHigh=RCAP2H;
				T2CaptureFirst.B.BLow=RCAP2L;
			}
			else
			{
				bT2Captureing=FALSE;
				bT2CaptureOk=TRUE;
				T2CaptureSecond.B.BHigh=TH2;
				T2CaptureSecond.B.BLow=TL2;
			}
		}
		IO_TEST_3=!IO_TEST_3;
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
	if(TF2)
	{
		TF2=0;
		SFR_SetPage(0);			// P5仅在Page 0
		IO_TEST_1=!IO_TEST_1;
	}
	SFR_SetPage(1);				// EXF3仅在1页操作
	if(EXF3)
	{
		EXF3=0;
		if(bT3Captureing==TRUE)
		{
			if(bT3CaptureFirst==FALSE)
			{
				bT3CaptureFirst=TRUE;
				T3CaptureFirst.B.BHigh=RCAP3H;
				T3CaptureFirst.B.BLow=RCAP3L;
			}
			else
			{
				bT3Captureing=FALSE;
				bT3CaptureOk=TRUE;
				T3CaptureSecond.B.BHigh=TH3;
				T3CaptureSecond.B.BLow=TL3;
			}
		}
		SFR_SetPage(0);			// P5仅在Page 0
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
		IO_TEST_3=!IO_TEST_3;
	}
	SFR_SetPage(2);				// EXF4仅在2页操作
	if(EXF4)
	{
		EXF4=0;
		if(bT4Captureing==TRUE)
		{
			if(bT4CaptureFirst==FALSE)
			{
				bT4CaptureFirst=TRUE;
				T4CaptureFirst.B.BHigh=RCAP4H;
				T4CaptureFirst.B.BLow=RCAP4L;
			}
			else
			{
				bT4Captureing=FALSE;
				bT4CaptureOk=TRUE;
				T4CaptureSecond.B.BHigh=TH4;
				T4CaptureSecond.B.BLow=TL4;
			}
		}
		SFR_SetPage(0); 		// P5仅在Page 0
		IO_TEST_5=!IO_TEST_5;
	}
	_pop_(SFRPI);
}

/***********************************************************************************

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
函数名称:void DelayXus(u8 xUs)
功能描述:延时程序，单位为us
输入参数:u8 Us -> *1us  (1~255)
返回参数:     
*************************************************************************************/
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

/***********************************************************************************
函数名称:void DelayXms(u16 xMs)
功能描述:延时程序，单位为ms
输入参数:u16 xMs -> *1ms  (1~65535)
返回参数:     
*************************************************************************************/
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
函数名称:   void InitTimer2(void)
功能描述:Timer2初始化设置	
输入参数:   
返回参数:     
*************************************************************************************/
void InitTimer2(void)
{
	TM_SetT2Mode_DutyCapture();						// 设置T2模式为DutyCapture模式
	TM_SetT2Clock_SYSCLKDiv12();					// 设置T2时钟源为 SYSCLK/12
	
	TM_SetT2LowByte(0);								// 设置TL2	  			
	TM_SetT2RLLowByte(0);							// 设置RCAP2L  
	
	TM_SetT2HighByte(0);							// 设置TH2
	TM_SetT2RLHighByte(0);							// 设置RCAP2H

	TM_SetT2Capture_T2EXPin();						// 使能T2外部中断为T2EX引脚
	TM_SetT2CKOT2EX_P10P11();						// 使能T2EX引脚为P11
	TM_EnableT2EX_DetectFalling();					// 使能T2EX引脚检测下降沿
	TM_EnableT2EX_DetectRising();					// 使能T2EX引脚检测上升沿
	
}

/***********************************************************************************
函数名称:   void InitTimer3(void)
功能描述:Timer3初始化设置
输入参数:   
返回参数:     
*************************************************************************************/
void InitTimer3(void)
{
	TM_SetT3Mode_DutyCapture();						// 设置T3模式为DutyCapture模式
	TM_SetT3Clock_SYSCLKDiv12();					// 设置T3时钟源为 SYSCLK/12
	
	TM_SetT3LowByte(0);								// 设置TL3	  			
	TM_SetT3RLLowByte(0);							// 设置RCAP3L  
	
	TM_SetT3HighByte(0);							// 设置TH3
	TM_SetT3RLHighByte(0);							// 设置RCAP3H

	TM_SetT3Capture_T3EXPin();						// 使能T3外部中断为T3EX引脚
	TM_SetT3CKOT3EX_P33P32();						// 使能T3EX引脚为P32
	TM_EnableT3EX_DetectFalling();					// 使能T3EX引脚检测下降沿
	TM_EnableT3EX_DetectRising();					// 使能T3EX引脚检测上升沿
}

/***********************************************************************************
函数名称:   void InitTimer4(void)
功能描述:Timer4初始化设置
输入参数:   
返回参数:     
*************************************************************************************/
void InitTimer4(void)
{
	TM_SetT4Mode_DutyCapture();						// 设置T4模式为DutyCapture模式
	TM_SetT4Clock_SYSCLKDiv12();					// 设置T4时钟源为 SYSCLK/12
	
	TM_SetT4LowByte(0);								// 设置TL4	  			
	TM_SetT4RLLowByte(0);							// 设置RCAP4L  
	
	TM_SetT4HighByte(0);							// 设置TH4
	TM_SetT4RLHighByte(0);							// 设置RCAP4H

	TM_SetT4Capture_T4EXPin();						// 使能T4外部中断为T4EX引脚
	TM_SetT4CKOT4EX_P36P37();						// 使能T4EX引脚为P37
	TM_EnableT4EX_DetectFalling();					// 使能T4EX引脚检测下降沿
	TM_EnableT4EX_DetectRising();					// 使能T4EX引脚检测上升沿
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
	INT_EnTIMER3();
	INT_EnTIMER4();
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
	UART0_SetS0BRGValue(S0BRG_BRGRL_57600_2X_24000000_1T);
	
	UART0_EnS0BRG();					// 开启S0BRG时钟
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
	
	InitTimer2();			
	InitTimer3();	
	InitTimer4();

	InitInterrupt();		// 中断初始化
	
	INT_EnAll();			// 使能全局中断


}

void StartT2CaptureDuty(void)
{
	IO_TEST_0=!IO_TEST_0;
	IO_TEST_3=!IO_TEST_3;
	bT2CaptureFirst=FALSE;
	bT2CaptureOk=FALSE;
	bT2Captureing=TRUE;
	TM_SetT2HighByte(0);
	TM_SetT2LowByte(0);
	TM_EnableT2();
}

void StartT3CaptureDuty(void)
{
	IO_TEST_1=!IO_TEST_1;
	IO_TEST_4=!IO_TEST_4;
	bT3CaptureFirst=FALSE;
	bT3CaptureOk=FALSE;
	bT3Captureing=TRUE;
	TM_SetT3HighByte(0);
	TM_SetT3LowByte(0);
	TM_EnableT3();
}

void StartT4CaptureDuty(void)
{
	IO_TEST_2=!IO_TEST_2;
	IO_TEST_5=!IO_TEST_5;
	bT4CaptureFirst=FALSE;
	bT4CaptureOk=FALSE;
	bT4Captureing=TRUE;
	TM_SetT4HighByte(0);
	TM_SetT4LowByte(0);
	TM_EnableT4();
}

void main()
{
	float duty;
	float freq;
	
    InitSystem();
	
	printf("\nHello!!");

 	LED_G_0=0;LED_R=0;LED_G_1=0;
	DelayXms(1000);
 	LED_G_0=1;LED_R=1;LED_G_1=1;

	StartT2CaptureDuty();
	StartT3CaptureDuty();
	StartT4CaptureDuty();
					
    while(1)
    {
    	DelayXms(500);
    	if(bT2CaptureOk)
    	{
    		duty=(float)(((double)(T2CaptureFirst.W)*100L)/(float)T2CaptureSecond.W);
    		freq=(float)(1000)/((float)(T2CaptureSecond.W)/2);
			printf("\nT2 Caputre: Duty=%f,Freq=%fKHz",duty,freq);
			StartT2CaptureDuty();
    	}
		if(bT3CaptureOk)
		{
    		duty=(float)(((double)(T3CaptureFirst.W)*100L)/(float)T3CaptureSecond.W);
    		freq=(float)(1000)/((float)(T3CaptureSecond.W)/2);
			printf("\nT3 Caputre: Duty=%f,Freq=%fKHz",duty,freq);
			StartT3CaptureDuty();
		}
		if(bT4CaptureOk)
		{
    		duty=(float)(((double)(T4CaptureFirst.W)*100L)/(float)T4CaptureSecond.W);
    		freq=(float)(1000)/((float)(T4CaptureSecond.W)/2);
			printf("\nT4 Caputre: Duty=%f,Freq=%fKHz",duty,freq);
			StartT4CaptureDuty();
		}
    }
}


