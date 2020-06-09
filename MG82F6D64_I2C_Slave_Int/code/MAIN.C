/*********************************************************************
    项目名称:MG82F6D64-DEMO
			测试板使用 MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=12MHz, SysCLK=12MHz
	功能描述：
			TWI0 (SCL/P25, SDA/P23)从模式中断处理测试
			TWI1 (SCL/P42, SDA/P43)从模式中断处理测试
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

#define SLAVE_ADDRESS	0xA0
#define TEST_BUF_SIZE	16

#define	TWI_OV_TIME_MAX			20

u8	TWI0OvTime;
u8	TWI1OvTime;
bit bTWI0Error;
bit bTWI1Error;
bit bTWI0ADFlag;
bit bTWI1ADFlag;

bit bTWI0RXFlag;
bit bTWI1RXFlag;

WordTypeDef TWI0RegAddr;
WordTypeDef TWI1RegAddr;
xdata u8 TWI0Buf[TEST_BUF_SIZE];
xdata u8 TWI1Buf[TEST_BUF_SIZE];
u8 TWI0Inx;
u8 TWI1Inx;
bit bES0;
u8 LedTime;

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
	if(TWI0OvTime!=0) 
	{
		TWI0OvTime--;
		if(TWI0OvTime==0) bTWI0Error=TRUE;
	}
	if(TWI1OvTime!=0) 
	{
		TWI1OvTime--;
		if(TWI1OvTime==0) bTWI1Error=TRUE;
	}
	if(LedTime!=0) LedTime--;
}


/***********************************************************************************
函数名称:   void INT_TWI0(void)
功能描述:TWI0 中断服务程序
		 
输入参数:   
返回参数:     
*************************************************************************************/
void INT_TWI0() interrupt INT_VECTOR_TWI0
{
	_push_(SFRPI);
	SFR_SetPage(0);
	if(SISTA==0x80)
	{ // 0x80: 接收到数据,并且返回了ACK
		TWI0OvTime = TWI_OV_TIME_MAX;			// 重置IIC超时时间
		if(bTWI0ADFlag==0)
		{// 是Reg地址
			if(TWI0Inx==0)
			{
				TWI0RegAddr.B.BHigh=SIDAT;
				TWI0Inx++;
			}
			else 
			{
				TWI0RegAddr.B.BLow=SIDAT;
				bTWI0ADFlag=1;					// 接下来的数据是Data
				TWI0Inx=0;
			}
		}
		else
		{
			TWI0Buf[TWI0Inx]=SIDAT;				// 接收到的数据读取到缓存
			TWI0Inx++;
			if(TWI0Inx>=TEST_BUF_SIZE) 
			{
				bTWI0RXFlag=TRUE;
				TWI0Inx=0;
				TWI0OvTime = 0; 				// 数据满，超时时间置零，不再检查超时
			}
		}
	}
	else if((SISTA==0x60)||(SISTA==0x68))
	{// 0x60,0x68: 接收到本机从机地址+写操作,并且返回了ACK
		bTWI0ADFlag=0;					// 接下来的两个数据Reg地址
		TWI0Inx=0;
		TWI0OvTime = TWI_OV_TIME_MAX;			// 重置IIC超时时间
	}
	else if((SISTA==0xA8)||(SISTA==0xB0))
	{// 0x60,0x68: 接收到本机从机地址+读操作,并且返回了ACK
		TWI0Inx=0;						// 发送缓存索引清零
		goto _IIC_SET_SIDAT;
	}
	else if((SISTA==0xB8)||(SISTA==0xC0)||(SISTA==0xC8))
	{
        // 0xB8:  SIDAT里的数据已经发送出去,并且收到了ACK
		// 0xC0:  SIDAT里的数据已经发送出去,没有收到了ACK
		// 0xC8:  SIDAT里最后的数据已经发送出去,并且收到了ACK
_IIC_SET_SIDAT:
		TWI0OvTime = TWI_OV_TIME_MAX;			// 重置IIC超时时间
		if(TWI0Inx>=TEST_BUF_SIZE) 
		{
			TWI0Inx=0;
			TWI0OvTime = 0; 				// 数据满，超时时间置零，不再检查超时
		}
		SIDAT=TWI0Buf[TWI0Inx]; 				// 加载要发送的数据
		TWI0Inx++;
	}
	TWI0_SCL=0;
	SICON=SICON|(AA);						// 置AA
	SICON=SICON&(~SI);						// 清中断标志
	TWI0_SCL=1;

	_pop_(SFRPI);
}

/***********************************************************************************
函数名称:   void INT_TWI1(void)
功能描述:TWI1 中断服务程序
		 
输入参数:   
返回参数:     
*************************************************************************************/
void INT_TWI1() interrupt INT_VECTOR_TWI1
{
	_push_(SFRPI);
	SFR_SetPage(1);
	if(SI1STA==0x80)
	{ // 0x80: 接收到数据,并且返回了ACK
		TWI1OvTime = TWI_OV_TIME_MAX;			// 重置IIC超时时间
		if(bTWI1ADFlag==0)
		{// 是Reg地址
			if(TWI1Inx==0)
			{
				TWI1RegAddr.B.BHigh=SI1DAT;
				TWI1Inx++;
			}
			else 
			{
				TWI1RegAddr.B.BLow=SI1DAT;
				bTWI1ADFlag=1;					// 接下来的数据是Data
				TWI1Inx=0;
			}
		}
		else
		{
			TWI1Buf[TWI1Inx]=SI1DAT;				// 接收到的数据读取到缓存
			TWI1Inx++;
			if(TWI1Inx>=TEST_BUF_SIZE) 
			{
				bTWI1RXFlag=TRUE;
				TWI1Inx=0;
				TWI1OvTime = 0; 				// 数据满，超时时间置零，不再检查超时
			}
		}
	}
	else if((SI1STA==0x60)||(SI1STA==0x68))
	{// 0x60,0x68: 接收到本机从机地址+写操作,并且返回了ACK
		bTWI1ADFlag=0;					// 接下来的两个数据Reg地址
		TWI1Inx=0;
		TWI1OvTime = TWI_OV_TIME_MAX;			// 重置IIC超时时间
	}
	else if((SI1STA==0xA8)||(SI1STA==0xB0))
	{// 0x60,0x68: 接收到本机从机地址+读操作,并且返回了ACK
		TWI1Inx=0;						// 发送缓存索引清零
		goto _IIC_SET_SI1DAT;
	}
	else if((SI1STA==0xB8)||(SI1STA==0xC0)||(SI1STA==0xC8))
	{
        // 0xB8:  SIDAT里的数据已经发送出去,并且收到了ACK
		// 0xC0:  SIDAT里的数据已经发送出去,没有收到了ACK
		// 0xC8:  SIDAT里最后的数据已经发送出去,并且收到了ACK
_IIC_SET_SI1DAT:
		TWI1OvTime = TWI_OV_TIME_MAX;			// 重置IIC超时时间
		if(TWI1Inx>=TEST_BUF_SIZE) 
		{
			TWI1Inx=0;
			TWI1OvTime = 0; 					// 数据满，超时时间置零，不再检查超时
		}
		SI1DAT=TWI1Buf[TWI1Inx]; 				// 加载要发送的数据
		TWI1Inx++;
	}
	TWI1_SCL=0;
	SI1CON=SI1CON|(AA);						// 置AA
	SI1CON=SI1CON&(~SI1);					// 清中断标志
	TWI1_SCL=1;
	_pop_(SFRPI);
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
函数名称:   void InitTWI0_Slave(void)
功能描述:TWI0初始化 从机模式
输入参数:   
返回参数:     
*************************************************************************************/
void InitTWI0_Slave()
{
	TWI0_Clear();
	TWI0_SetClock(TWI0_CLK_SYSCLK_32);
	TWI0_SetSCLSDAP25P23();
	TWI0_Enable();
	TWI0_SetSlaveAddr(SLAVE_ADDRESS);
	TWI0_SendACK();

}

/***********************************************************************************
函数名称:   void InitTWI1_Slave(void)
功能描述:TWI1初始化 从机模式
输入参数:   
返回参数:     
*************************************************************************************/
void InitTWI1_Slave()
{
	TWI1_Clear();
	TWI1_SetClock(TWI1_CLK_SYSCLK_32);
	TWI1_SetSCLSDAP42P43();
	TWI1_Enable();
	TWI1_SetSlaveAddr(SLAVE_ADDRESS);
	TWI1_SendACK();

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
	INT_EnTIMER0();				// 使能T0中断
	INT_EnTWI0();				// 使能TWI0中断
	INT_EnTWI1();				// 使能TWI1中断
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
	InitTWI0_Slave();
	InitTWI1_Slave();
	InitTimer0();
	InitUart0_S0BRG();
	InitInterrupt();

	INT_EnAll();			//  使能全局中断
}

void main()
{
	u8 i;
    InitSystem();


 	SendStr("\nStart");
 	
	LED_G_0=0;LED_G_1=0;LED_R=0;
	DelayXms(1000);
	LED_G_0=1;LED_G_1=1;LED_R=1;

	TWI0OvTime = 0; 				// 超时时间置零，不检查超时
	TWI1OvTime = 0; 				// 超时时间置零，不检查超时

    while(1)
    {
    	if(LedTime==0)
    	{
			LED_R = !LED_R;
			if(LED_R==0)
			{
				LedTime=20;
			}
			else
			{
				LedTime=250;
			}
    	}
    	if(bTWI0RXFlag==TRUE)
    	{
			LED_G_0=0;LED_R=1;
			SendStr("\nTWI0 RX 0x");
			SendHex(TWI0RegAddr.B.BHigh);
			SendHex(TWI0RegAddr.B.BLow);
			for(i=0;i<TEST_BUF_SIZE;i++)
			{
				SendByte(' ');
				SendHex(TWI0Buf[i]);
			}
			bTWI0RXFlag=FALSE;
			LED_G_0=1;LED_R=1;
			LedTime=250;
    	}
    	if(bTWI1RXFlag==TRUE)
    	{
			LED_G_1=0;LED_R=1;
			SendStr("\nTWI1 RX 0x");
			SendHex(TWI1RegAddr.B.BHigh);
			SendHex(TWI1RegAddr.B.BLow);
			for(i=0;i<TEST_BUF_SIZE;i++)
			{
				SendByte(' ');
				SendHex(TWI1Buf[i]);
			}
			bTWI1RXFlag=FALSE;
			LED_G_1=1;LED_R=1;
			LedTime=250;
    	}
    	
    	if(bTWI0Error==TRUE)
    	{
    		bTWI0Error=FALSE;
			// 出错,重置TWI0
			LED_G_0=0;LED_R=0;
			InitTWI0_Slave();
			DelayXms(50);
			LED_G_0=1;LED_R=1;
			LedTime=250;
    	}
		if(bTWI1Error==TRUE)
		{
			bTWI1Error=FALSE;
			// 出错,重置TWI0
			LED_G_1=0;LED_R=0;
			InitTWI1_Slave();
			DelayXms(50);
			LED_G_1=1;LED_R=1;
			LedTime=250;
		}
    }
}


