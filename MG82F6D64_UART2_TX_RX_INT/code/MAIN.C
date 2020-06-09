/*********************************************************************
    项目名称:MG82F6D64-DEMO
    作    者:YZY
			测试板使用 MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=12MHz, SysCLK=12MHz
	功能描述：
			UART2中断发送及接收
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

#define UART2_RX_BUFF_SIZE   32   		 //字节
#define UART2_TX_BUFF_SIZE   32   		 //字节
xdata u8 RcvBuf[UART2_RX_BUFF_SIZE];
u8 Uart2RxIn =0;
u8 Uart2RxOut =0;
xdata u8 TxBuf[UART2_TX_BUFF_SIZE];
u8 Uart2TxIn =0;
u8 Uart2TxOut =0;
bit bUart2TxFlag;

u8 LedTime;

/***********************************************************************************
函数名称:   void INT_UART2(void)
功能描述:UART2 中断服务程序
		 
输入参数:   
返回参数:     
*************************************************************************************/
void INT_UART2(void) interrupt INT_VECTOR_UART2
{
	_push_(SFRPI);		   //保护SFRPI寄存器值

	SFR_SetPage(3);		   /*设置3页寄存器*/  //S2CON仅3页操作
	if(TI2)					//发送中断内容
	{
	   TI2 = 0;	   
		if(Uart2TxIn==Uart2TxOut)
		{
			bUart2TxFlag=FALSE;
		}
		else
		{
			S2BUF=TxBuf[Uart2TxOut];
			bUart2TxFlag=TRUE;
			Uart2TxOut++;
			if(Uart2TxOut>=UART2_TX_BUFF_SIZE)
			{
				Uart2TxOut=0;
			}
		}
	}
	if(RI2)					//接收中断内容
	{
		RI2 = 0;				//清除中断标志
		RcvBuf[Uart2RxIn] = S2BUF;
		Uart2RxIn++;
		if(Uart2RxIn >=UART2_RX_BUFF_SIZE)
		{
			Uart2RxIn =0;
		}
		
	}
	_pop_(SFRPI);		   //恢复SFRPI寄存器值
}

/***********************************************************************************
函数名称:   void INT_T0(void)
功能描述:T0 中断服务程序
		 间隔1ms
输入参数:   
返回参数:     
*************************************************************************************/
void INT_T0(void) interrupt INT_VECTOR_T0
{
	TH0=TIMER_12T_1ms_TH;
	TL0=TIMER_12T_1ms_TL;

	if(LedTime!=0) LedTime--;
}

/***********************************************************************************
函数名称:void Uart2SendByte(u8 tByte)
功能描述:Uart2发送数据
输入参数:u8 tByte: 待发送的数据
返回参数:     
*************************************************************************************/
void Uart2SendByte(u8 tByte)
{
	u8 i;
	
	if(bUart2TxFlag==FALSE)
	{
		Uart2TxOut=0;
		Uart2TxIn=1;
		TxBuf[0]=tByte;
		SFR_SetPage(3);
        TI2=1;
        SFR_SetPage(0);
	}
	else
	{
		i=Uart2TxIn;
		TxBuf[i]=tByte;
		i++;
		if(i>=UART2_TX_BUFF_SIZE)
		{
			i=0;
		}
		while(i==Uart2TxOut)
		{
		}
		INT_DisUART2();
		Uart2TxIn=i;
		INT_EnUART2();
	}
}

/***********************************************************************************
函数名称:void Uart2SendStr(u8* PStr)
功能描述:Uart2发送字符串
输入参数: u8* PStr:字符串起始地址
返回参数:     
*************************************************************************************/
void Uart2SendStr(u8* PStr)
{
	while(*PStr != 0)
	{
		Uart2SendByte(*PStr);
		PStr ++;
	}
}

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
函数名称:   void InitUart2(void)
功能描述:   Uart2初始化(波特率9600)、RX:P34 TX:P35
输入参数:   
返回参数:     
*************************************************************************************/
void InitUart2(void)
{
	UART2_SetMode8bitUARTVar();								// 8位 可变波特率

	UART2_EnS2BRG();										//使能S1BRG波特率发生器
	UART2_SetBaudRateX2();									//使能双倍波特率
	UART2_SetRxTxP32P33();									//设置串口使用引脚：RX:P32 TX:P33
	UART2_EnReception();									//使能串口接收功能
	UART2_SetS2BRGSelSYSCLK();								//设置S1BRG时钟源为系统时钟：1T

	UART2_SetS2BRGValue(S2BRG_BRGRL_9600_2X_12000000_1T);	//设置S2BRG值及重载值：9600波特率，双倍波特率，12MHz系统时钟，1TS2BRG时钟源
	
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
	PORT_SetP3QuasiBi(BIT0|BIT1|BIT3|BIT4|BIT5);		// 设置P30,P31,P33,P34,P35为准双向口
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
函数名称:   void InitInterrupt(void)
功能描述:   中断初始化
输入参数:   
返回参数:     
*************************************************************************************/
void InitInterrupt(void)
{
	INT_EnTIMER0();			//	使能T0中断
	INT_EnUART2();			//	使能串口中断
	INT_EnAll();			//  使能全局中断
	
}	

/***********************************************************************************
函数名称:   void InitTimer0(void)
功能描述:Timer0初始化设置
		 定义T0为16位定时器,时钟为Sysclk/12 
输入参数:   
返回参数:     
*************************************************************************************/
void InitTimer0(void)
{
	TM_SetT0Mode_1_16BIT_TIMER();			// 设置T0模式为16位模式
	TM_SetT0Clock_SYSCLKDiv12();			// 设置T0时钟源为 SYSCLK/12
	TM_SetT0Gate_Disable();	
				
	TM_SetT0LowByte(TIMER_12T_1ms_TL);		// 设置T0低8位
	TM_SetT0HighByte(TIMER_12T_1ms_TH);		// 设置T0高8位

	TM_EnableT0();							// 使能T0
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
	InitTimer0();
	InitUart2();
	InitInterrupt();	
}

void main()
{
	InitSystem();

	
	Uart2RxIn=0;
	Uart2RxOut=0;
	Uart2TxIn=0;
	Uart2TxOut=0;
	bUart2TxFlag=0;
	
	LED_G_0=0;
	DelayXms(1000);
	LED_G_0=1;
	DelayXms(500);
	
	Uart2SendStr("Start!\n");


	while(1)
	{
		if(LedTime==0)
		{
			LedTime=100;
		     LED_G_0 = !LED_G_0;
		}
		if(Uart2RxIn != Uart2RxOut)
		{
			Uart2SendByte(RcvBuf[Uart2RxOut]);
			Uart2RxOut++;
			if(Uart2RxOut>=UART2_RX_BUFF_SIZE)
			{
				Uart2RxOut=0;
			}
		}

	}

}


