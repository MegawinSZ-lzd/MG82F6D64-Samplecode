/*********************************************************************
    项目名称:MG82F6D64-DEMO
    作    者:YZY
			测试板使用 MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=12MHz, SysCLK=12MHz
	功能描述：
			使用TWSI0 (SCL/P25, SDA/P23)主模式测试
			使用TWSI1 (SCL/P42, SDA/P43)主模式测试
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

#define TIMER_12T_1ms_TH	((65536-(u16)(float)(1000*((float)(MCU_SYSCLK)/(float)(12000000)))) /256) 			
#define TIMER_12T_1ms_TL	((65536-(u16)(float)(1000*((float)(MCU_SYSCLK)/(float)(12000000)))) %256)


#define LED_G_0		P55
#define LED_R		P56
#define LED_G_1		P57

#define TWI0_SCL		P25
#define TWI0_SDA		P23

#define TWI1_SCL		P42
#define TWI1_SDA		P43

#define SLAVE_ADDRESS_A0	0xA0
#define SLAVE_ADDRESS_B0	0xB0
#define TEST_BUF_SIZE	16

u8	TWI0OvTime;
u8	TWI1OvTime;

u8 rand;

WordTypeDef TWI0TestAddr;
WordTypeDef TWI1TestAddr;

bit bES0;
u8 LedTime;
xdata u8 WriteBuf[TEST_BUF_SIZE];
xdata u8 ReadBuf[TEST_BUF_SIZE];
u8 TestBufLen;

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
	if(TWI0OvTime!=0) TWI0OvTime--;
	if(TWI1OvTime!=0) TWI1OvTime--;
	if(LedTime!=0) LedTime--;
}

/***********************************************************************************
函数名称:   void SendByte(u8 tByte)
功能描述:通过UART发送一个字节
输入参数: 
		u8 tByte: 待发送的数据
返回参数:

*************************************************************************************/
void SendByte(u8 tByte)
{
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
	UART0_SetS0BRGValue(S0BRG_BRGRL_57600_2X_12000000_1T);

	UART0_EnS0BRG();					// 开启S0BRG时钟
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

	TM_SetT0LowByte(TIMER_12T_1ms_TL);						// 设置T0低8位
	TM_SetT0HighByte(TIMER_12T_1ms_TH);					// 设置T0高8位

	TM_EnableT0();							// 使能T0
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
	PORT_SetP2OpenDrainPu(BIT3|BIT5);					// 设置P23,P25为开漏带上拉(TWI0:SCL/SDA)
	PORT_SetP4OpenDrainPu(BIT2|BIT3);					// 设置P42,P43为开漏带上拉(TWI1:SCL/SDA)
}

/***********************************************************************************
函数名称:   void InitTWI0(void)
功能描述:TWI0初始化
输入参数:   
返回参数:     
*************************************************************************************/
void InitTWI0()
{
	TWI0_Clear();
	TWI0_SetClock(TWI0_CLK_SYSCLK_128);
	TWI0_SetSCLSDAP25P23();
	TWI0_Enable();
}

/***********************************************************************************
函数名称:   void InitTWI1(void)
功能描述:TWI1初始化
输入参数:   
返回参数:     
*************************************************************************************/
void InitTWI1()
{
	TWI1_Clear();
	TWI1_SetClock(TWI1_CLK_SYSCLK_128);
	TWI1_SetSCLSDAP42P43();
	TWI1_Enable();
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
	INT_EnTIMER0();						// 使能T0中断
	INT_EnAll();						// 使能全局中断
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
	InitTWI0();
	InitTWI1();
	InitTimer0();
	InitUart0_S0BRG();
	InitInterrupt();
}

/***********************************************************************************
函数名称:   u8 TWI0_WriteBuf(u8 DevAddr,u16 RegStartAddr,u8 *pByte,u8 Len)
功能描述:
		通过TWI0将数据发送到从机
输入参数: 
		u8 DevAddr: 从机地址
		u16 RegStartAddr: 从机数据的起始地址
		u8 *pBuf: 待发送数据的缓存地址
		u8 Len: 待发送的数据个数
返回参数:
		u8: 0->成功, 其它->失败
*************************************************************************************/
u8 TWI0_WriteBuf(u8 DevAddr,u16 RegStartAddr,u8 *pBuf,u8 Len)
{
	u8 i;
	u8 Flag;
	Flag=1;

	SICON |=STA;						// 发送START
	TWI0OvTime=5;
	SICON &=~SI;			
	while((SICON&SI)!=SI){if(TWI0OvTime==0) goto TWI0_WRITE_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值
	SICON &=~STA;

	SICON=SICON|(AA);	   

	Flag++;
	SIDAT = DevAddr&0xFE;				// 发送器件地址 
	TWI0OvTime=5;
	SICON &=~SI;			
	while((SICON&SI)!=SI){if(TWI0OvTime==0) goto TWI0_WRITE_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值

	Flag++;
	SIDAT = HIBYTE(RegStartAddr);		// 发送地址 高8位
	TWI0OvTime=5;
	SICON &=~SI;			
	while((SICON&SI)!=SI){if(TWI0OvTime==0) goto TWI0_WRITE_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值

	Flag++;
	SIDAT = LOBYTE(RegStartAddr);		// 发送地址 低8位
	TWI0OvTime=5;
	SICON &=~SI;			
	while((SICON&SI)!=SI){if(TWI0OvTime==0) goto TWI0_WRITE_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值


	Flag++;
	i=0;
	while(i<Len)
	{
		if(i==(Len-1))
		{
			SICON=SICON&(~AA);	   
		}
		else
		{
			SICON=SICON|(AA);	   
		}
		SIDAT=pBuf[i];			// 发送数据
		TWI0OvTime=5;
		SICON &=~SI;			
		while((SICON&SI)!=SI){if(TWI0OvTime==0) goto TWI0_WRITE_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值
		i++;
	}
	
	Flag++;
	SICON |= STO;				// 发送STOP
	TWI0OvTime=5;
	SICON &=~SI;			
	while((SICON&STO)==STO){if(TWI0OvTime==0) goto TWI0_WRITE_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值
	SICON &=~STO;

	SICON = SICON &(~SI);		// 清中断标志

	return 0;
	
TWI0_WRITE_ERR:
	return Flag;
}


/***********************************************************************************
函数名称:   u8 TWI0_ReadBuf(u8 DevAddr,u16 RegStartAddr,u8 *pBuf,u8 Len)
功能描述:
		通过TWI0读取从机的数据
输入参数: 
		u8 DevAddr: 从机地址
		u16 RegStartAddr: 从机待读取数据的起始地址
		u8 *pBuf: 保存读取数据的缓存地址
		u8 Len: 待读取的数据个数
返回参数:
		u8: 0->成功, 其它->失败
*************************************************************************************/
u8 TWI0_ReadBuf(u8 DevAddr,u16 RegStartAddr,u8 *pBuf,u8 Len)
{
	u8 i;
	u8 Flag;
	Flag=1;
	
	SICON |=STA;				// 发送START
	TWI0OvTime=5;
	SICON &=~SI;			
	while((SICON&SI)!=SI){if(TWI0OvTime==0) goto TWI0_READ_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值
	SICON &=~STA;

	Flag++;
	SICON=SICON|(AA);	   

	SIDAT = DevAddr&0xFE;		// 发送器件地址 
	TWI0OvTime=5;
	SICON &=~SI;			
	while((SICON&SI)!=SI){if(TWI0OvTime==0) goto TWI0_READ_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值

	Flag++;
	SIDAT = HIBYTE(RegStartAddr);		// 发送地址 高8位
	TWI0OvTime=5;
	SICON &=~SI;			
	while((SICON&SI)!=SI){if(TWI0OvTime==0) goto TWI0_READ_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值

	Flag++;
	SIDAT = LOBYTE(RegStartAddr);		// 发送地址 低8位
	TWI0OvTime=5;
	SICON &=~SI;			
	while((SICON&SI)!=SI){if(TWI0OvTime==0) goto TWI0_READ_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值

	Flag++;
	SICON |= STA;				// 再次发送START
	TWI0OvTime=5;
	SICON &=~SI;			
	while((SICON&SI)!=SI){if(TWI0OvTime==0) goto TWI0_READ_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值
	SICON &=~STA;

	Flag++;
	SIDAT = DevAddr|0x01;		// 发送器件地址 ,进入读模式
	TWI0OvTime=5;
	SICON &=~SI;			
	while((SICON&SI)!=SI){if(TWI0OvTime==0) goto TWI0_READ_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值

	Flag++;
	i=0;
	while(i<Len)
	{
		if(i==(Len-1))
		{
			SICON=SICON&(~AA);	   
		}
		else
		{
			SICON=SICON|(AA);	   
		}
		TWI0OvTime=5;
		SICON &=~SI;			
		while((SICON&SI)!=SI){if(TWI0OvTime==0) goto TWI0_READ_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值
		pBuf[i] = SIDAT; 			// 读取数据
		i++;
	}
	
	Flag++;
	SICON |= STO;				// 发送STOP
	TWI0OvTime=5;
	SICON &=~SI;			
	while((SICON&STO)==STO){if(TWI0OvTime==0) goto TWI0_READ_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值
	SICON &=~STO;

	SICON = SICON &(~SI);		// 清中断标志

	return 0;
	
TWI0_READ_ERR:
	SICON |= STO;				// 发送STOP
	SICON = SICON &(~SI);		// 清中断标志
	return Flag;
}

/***********************************************************************************
函数名称:   u8 TWI1_WriteBuf(u8 DevAddr,u16 RegStartAddr,u8 *pByte,u8 Len)
功能描述:
		通过TWI1将数据发送到从机
输入参数: 
		u8 DevAddr: 从机地址
		u16 RegStartAddr: 从机数据的起始地址
		u8 *pBuf: 待发送数据的缓存地址
		u8 Len: 待发送的数据个数
返回参数:
		u8: 0->成功, 其它->失败
*************************************************************************************/
u8 TWI1_WriteBuf(u8 DevAddr,u16 RegStartAddr,u8 *pBuf,u8 Len)
{
	u8 i;
	u8 Flag;
	Flag=1;

	SFR_SetPage(1);

	SI1CON |=STA1;						// 发送START
	TWI1OvTime=5;
	SI1CON &=~SI1;			
	while((SI1CON&SI1)!=SI1){if(TWI1OvTime==0) goto TWI1_WRITE_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值
	SI1CON &=~STA;

	SI1CON=SI1CON|(AA1);	   

	Flag++;
	SI1DAT = DevAddr&0xFE;				// 发送器件地址 
	TWI1OvTime=5;
	SI1CON &=~SI1;			
	while((SI1CON&SI1)!=SI1){if(TWI1OvTime==0) goto TWI1_WRITE_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值

	Flag++;
	SI1DAT = HIBYTE(RegStartAddr);		// 发送地址 高8位
	TWI1OvTime=5;
	SI1CON &=~SI1;			
	while((SI1CON&SI1)!=SI1){if(TWI1OvTime==0) goto TWI1_WRITE_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值

	Flag++;
	SI1DAT = LOBYTE(RegStartAddr);		// 发送地址 低8位
	TWI1OvTime=5;
	SI1CON &=~SI1;			
	while((SI1CON&SI1)!=SI1){if(TWI1OvTime==0) goto TWI1_WRITE_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值


	Flag++;
	i=0;
	while(i<Len)
	{
		if(i==(Len-1))
		{
			SI1CON=SI1CON&(~AA1);	   
		}
		else
		{
			SI1CON=SI1CON|(AA1);	   
		}
		SI1DAT=pBuf[i];					// 发送数据
		TWI1OvTime=5;
		SI1CON &=~SI1;			
		while((SI1CON&SI1)!=SI1){if(TWI1OvTime==0) goto TWI1_WRITE_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值
		i++;
	}
	
	Flag++;
	SI1CON |= STO1;						// 发送STOP
	TWI1OvTime=5;
	SI1CON &=~SI1;			
	while((SI1CON&STO1)==STO1){if(TWI1OvTime==0) goto TWI1_WRITE_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值
	SI1CON &=~STO1;

	SI1CON = SI1CON &(~SI1);			// 清中断标志
	SFR_SetPage(0);
	return 0;

TWI1_WRITE_ERR:
	SI1CON |= STO1;						// 发送STOP
	SI1CON = SI1CON &(~SI1);			// 清中断标志
	SFR_SetPage(0);
	return Flag;

}


/***********************************************************************************
函数名称:   u8 TWI1_ReadBuf(u8 DevAddr,u16 RegStartAddr,u8 *pBuf,u8 Len)
功能描述:
		通过TWI1读取从机的数据
输入参数: 
		u8 DevAddr: 从机地址
		u16 RegStartAddr: 从机待读取数据的起始地址
		u8 *pBuf: 保存读取数据的缓存地址
		u8 Len: 待读取的数据个数
返回参数:
		u8: 0->成功, 其它->失败
*************************************************************************************/
u8 TWI1_ReadBuf(u8 DevAddr,u16 RegStartAddr,u8 *pBuf,u8 Len)
{
	u8 i;
	u8 Flag;
	Flag=1;

	SFR_SetPage(1);
	
	SI1CON |=STA1;				// 发送START
	TWI1OvTime=5;
	SI1CON &=~SI1;			
	while((SI1CON&SI1)!=SI1){if(TWI1OvTime==0) goto TWI1_READ_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值
	SI1CON &=~STA1;

	SI1CON=SI1CON|(AA1);	   

	Flag++;
	SI1DAT = DevAddr&0xFE;		// 发送器件地址 
	TWI1OvTime=5;
	SI1CON &=~SI1;			
	while((SI1CON&SI1)!=SI1){if(TWI1OvTime==0) goto TWI1_READ_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值

	Flag++;
	SI1DAT = HIBYTE(RegStartAddr);		// 发送地址 高8位
	TWI1OvTime=5;
	SI1CON &=~SI1;			
	while((SI1CON&SI1)!=SI1){if(TWI1OvTime==0) goto TWI1_READ_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值

	Flag++;
	SI1DAT = LOBYTE(RegStartAddr);		// 发送地址 低8位
	TWI1OvTime=5;
	SI1CON &=~SI1;			
	while((SI1CON&SI1)!=SI1){if(TWI1OvTime==0) goto TWI1_READ_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值

	Flag++;
	SI1CON |= STA1;				// 再次发送START
	TWI1OvTime=5;
	SI1CON &=~SI1;			
	while((SI1CON&SI1)!=SI1){if(TWI1OvTime==0) goto TWI1_READ_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值
	SI1CON &=~STA1;

	Flag++;
	SI1DAT = DevAddr|0x01;		// 发送器件地址 ,进入读模式
	TWI1OvTime=5;
	SI1CON &=~SI1;			
	while((SI1CON&SI1)!=SI1){if(TWI1OvTime==0) goto TWI1_READ_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值

	Flag++;
	i=0;
	while(i<Len)
	{
		if(i==(Len-1))
		{
			SI1CON=SI1CON&(~AA1);	   
		}
		else
		{
			SI1CON=SI1CON|(AA1);	   
		}
		TWI1OvTime=5;
		SI1CON &=~SI1;			
		while((SI1CON&SI1)!=SI1){if(TWI1OvTime==0) goto TWI1_READ_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值
		pBuf[i] = SI1DAT; 			// 读取数据
		i++;
	}
	
	Flag++;
	SI1CON |= STO1;				// 发送STOP
	TWI1OvTime=5;
	SI1CON &=~SI1;			
	while((SI1CON&STO1)==STO1){if(TWI1OvTime==0) goto TWI1_READ_ERR;}		// 等待发送完成, 如果时间溢出则返回失败值
	SI1CON &=~STO1;

	SI1CON = SI1CON &(~SI1);		// 清中断标志
	SFR_SetPage(0);
	return 0;
	
TWI1_READ_ERR:
	SI1CON |= STO1; 					// 发送STOP
	SI1CON = SI1CON &(~SI1);			// 清中断标志
	SFR_SetPage(0);
	return Flag;
}


void TWI0_TestProc(void)
{
	u8 S0Ret;
	u8 i;
	
	SendStr("\nTWI0 Test ");
	SendHex(TWI0TestAddr.B.BHigh);
	SendHex(TWI0TestAddr.B.BLow);
	
	for(i=0;i<TEST_BUF_SIZE;i++)
	{
		WriteBuf[i]=0x78+i+rand;
	}
	
	S0Ret=TWI0_WriteBuf(SLAVE_ADDRESS_A0, TWI0TestAddr.W, &WriteBuf[0],TEST_BUF_SIZE);
	if(S0Ret==0)
	{
		DelayXms(5); // EEPROM 写时间
		S0Ret=TWI0_ReadBuf(SLAVE_ADDRESS_A0,TWI0TestAddr.W,&ReadBuf[0],TEST_BUF_SIZE);
		if(S0Ret==0)
		{
			for(i=0;i<TEST_BUF_SIZE;i++)
			{
				if(WriteBuf[i] != ReadBuf[i]) break;
			}
			if(i<TEST_BUF_SIZE)
			{
				SendStr("...Err--Verify:");
				SendHex(i);
				SendByte(0x20);
				SendHex(WriteBuf[i]);
				SendByte(0x20);
				SendHex(ReadBuf[i]);
			}
			else
			{
				LED_G_0=0;
				SendStr("...SUCCESS!");
				DelayXms(50);
				LED_G_0=1;
			}
		}
		else
		{
			SendStr("...Err--R:");
			SendHex(S0Ret);
		}
	}
	else
	{
		SendStr("...Err--W:");
		SendHex(S0Ret);
	}
	if(S0Ret!=0)
	{
		// 出错,重置TWI0
		InitTWI0();
	
	}
	TWI0TestAddr.W=TWI0TestAddr.W+TEST_BUF_SIZE;
	if(TWI0TestAddr.W>=0x1000)
	{
		TWI0TestAddr.W=0;
	}
}

void TWI1_TestProc(void)
{
	u8 S0Ret;
	u8 i;
	
	SendStr("\nTWI1 Test ");
	SendHex(TWI1TestAddr.B.BHigh);
	SendHex(TWI1TestAddr.B.BLow);
	
	for(i=0;i<TEST_BUF_SIZE;i++)
	{
		WriteBuf[i]=0x78+i+rand;
	}
	
	S0Ret=TWI1_WriteBuf(SLAVE_ADDRESS_A0, TWI1TestAddr.W, &WriteBuf[0],TEST_BUF_SIZE);
	if(S0Ret==0)
	{
		DelayXms(5); // EEPROM 写时间
		S0Ret=TWI1_ReadBuf(SLAVE_ADDRESS_A0,TWI1TestAddr.W,&ReadBuf[0],TEST_BUF_SIZE);
		if(S0Ret==0)
		{
			for(i=0;i<TEST_BUF_SIZE;i++)
			{
				if(WriteBuf[i] != ReadBuf[i]) break;
			}
			if(i<TEST_BUF_SIZE)
			{
				SendStr("...Err--Verify:");
				SendHex(i);
				SendByte(0x20);
				SendHex(WriteBuf[i]);
				SendByte(0x20);
				SendHex(ReadBuf[i]);
			}
			else
			{
				LED_G_1=0;
				SendStr("...SUCCESS!");
				DelayXms(50);
				LED_G_1=1;
			}
		}
		else
		{
			SendStr("...Err--R:");
			SendHex(S0Ret);
		}
	}
	else
	{
		SendStr("...Err--W:");
		SendHex(S0Ret);
	}
	if(S0Ret!=0)
	{
		// 出错,重置TWI1
		InitTWI1();
	}
	TWI1TestAddr.W=TWI1TestAddr.W+TEST_BUF_SIZE;
	if(TWI1TestAddr.W>=0x1000)
	{
		TWI1TestAddr.W=0;
	}
}


void main()
{
	
    InitSystem();
	
 	SendStr("\nStart");
	LED_G_0=0;LED_G_1=0;LED_R=0;
	DelayXms(1000);
	LED_G_0=1;LED_G_1=1;LED_R=1;

	rand = 0x00;
	TWI0TestAddr.W=0x0000;
	TWI1TestAddr.W=0x0000;

    while(1)
    {
		rand++;
    	if(LedTime==0)
    	{
			LED_R = ~LED_R;
			LedTime=200;
			TWI0_TestProc();
			TWI1_TestProc();
		}
    }
}


