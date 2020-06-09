/*********************************************************************
    项目名称:MG82F6D64-DEMO
			测试板使用 MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=12MHz, SysCLK=12MHz
	功能描述：
			使用TWSI0 (SCL/P25, SDA/P23)主模式中断处理测试
			使用TWSI1 (SCL/P42, SDA/P43)主模式中断处理测试
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

#define I2C_SCL		P25
#define I2C_SDA		P23

#define I2C_SLAVE_ADDR		0xA0

bit bES0;
u8 LedTime;
u8 Cnt100;

u8 TWI0TestTime;
u8 TWI1TestTime;

u8 TWI0OvTime;
u8 TWI1OvTime;



enum{
	EE_IDLE=0,
	EE_SEND_START,
	EE_SEND_SLA,
	EE_WRITE_ADDR_H,
	EE_WRITE_ADDR_L,
	EE_SEND_RESTART,
	EE_WRITE_DATA,
	EE_READ_DATA,
	EE_SEND_STOP
}EE_STATUS;

#define EE_SUCCESS		0x00

#define DATA_LENGTH          16
WordTypeDef TWI0TestAddr;
WordTypeDef TWI1TestAddr;

xdata u8 TWI0WriteBuf[DATA_LENGTH];
xdata u8 TWI0ReadBuf[DATA_LENGTH];
xdata u8 TWI1WriteBuf[DATA_LENGTH];
xdata u8 TWI1ReadBuf[DATA_LENGTH];

#define TWI0_TEST_INTERVAL_TIME	(400/100)			// *100ms	TWI0测试间隔时间
#define TWI1_TEST_INTERVAL_TIME	(700/100)			// *100ms	TWI1测试间隔时间

#define IIC_OVTIME_MAX		20						// *1ms IIC操作超时溢出时间


#define EE_MODE_WRITE		0
#define EE_MODE_READ		1

typedef struct 
{
	u8 EEMode;
	u8 EEStatus;
	u8 EEDeviceAddr;
	u16 EEDataAddr;
	u8 *EEDataBuf;
	u8 EEFlag;
	u8 EEDataInx;
	u8 EEDataLength;
}EEPROM_PROC_STRUCT;

idata EEPROM_PROC_STRUCT EE_TWI0,EE_TWI1;


u8 rand;

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
	if(TWI0OvTime!=0) TWI0OvTime--;
	if(TWI1OvTime!=0) TWI1OvTime--;
	Cnt100++;
	if(Cnt100>=100)
	{
		Cnt100=0;
		if(TWI0TestTime!=0) TWI0TestTime--;
		if(TWI1TestTime!=0) TWI1TestTime--;
	}
}

/***********************************************************************************
函数名称:   void INT_TWSI(void)
功能描述:TWI0 中断服务程序
		 
输入参数:   
返回参数:     
*************************************************************************************/
void INT_TWI0() interrupt INT_VECTOR_TWI0
{
	_push_(SFRPI);
	SFR_SetPage(0);

	if(SISTA==0x08)
	{ // Tx:Start
		SICON=SICON&(~STA);
		SIDAT=EE_TWI0.EEDeviceAddr&0xFE;
		EE_TWI0.EEStatus=EE_SEND_SLA;
	}
	else if(SISTA==0x18)
    { // Tx:SLA+W Rx:ACK
    	// To send RegAddr high
		SICON=SICON|(AA);
    	SIDAT=(u8)(EE_TWI0.EEDataAddr>>8);
		EE_TWI0.EEStatus=EE_WRITE_ADDR_H;
    }
    else if(SISTA==0x20)
    { // Tx:SLA+W Rx:NACK
		EE_TWI0.EEFlag=EE_TWI0.EEStatus+0x80;
		EE_TWI0.EEStatus=EE_SEND_STOP;
		SICON=SICON|(STO);
    }
    else if(SISTA==0x28)
    { // Tx:DAT Rx:ACK
		if(EE_TWI0.EEStatus==EE_WRITE_ADDR_H)
		{
			// To send RegAddr LOW
			SICON=SICON|(AA);
			SIDAT=(u8)(EE_TWI0.EEDataAddr);
			EE_TWI0.EEStatus=EE_WRITE_ADDR_L;
		}
		else if(EE_TWI0.EEStatus==EE_WRITE_ADDR_L)
		{
			// Send RegAddr Done
			if(EE_TWI0.EEMode==EE_MODE_READ)
			{
				// Read mode: To Tx ReStart;
				SICON=SICON|(STA);
				EE_TWI0.EEStatus=EE_SEND_RESTART;
			}
			else
			{
				// Write mode: To Tx Data;
				SICON=SICON|(AA);
				EE_TWI0.EEStatus=EE_WRITE_DATA;
				EE_TWI0.EEDataInx=0;
				SIDAT=EE_TWI0.EEDataBuf[EE_TWI0.EEDataInx];
				EE_TWI0.EEDataInx++;
			}
		}
		else if(EE_TWI0.EEStatus==EE_WRITE_DATA)
		{
			if(EE_TWI0.EEDataInx>=EE_TWI0.EEDataLength)
			{
				SICON=SICON|(STO);
				EE_TWI0.EEFlag= EE_SUCCESS;
				EE_TWI0.EEStatus=EE_SEND_STOP;
			}
			else
			{
				SICON=SICON|(AA);
				SIDAT=EE_TWI0.EEDataBuf[EE_TWI0.EEDataInx];
				EE_TWI0.EEDataInx++;
			}
		}
		else
		{
			SICON=SICON|(STO);
			EE_TWI0.EEFlag=EE_TWI0.EEStatus+0x80;
			EE_TWI0.EEStatus=EE_SEND_STOP;
		}
    }
    else if(SISTA==0x30)
    { // Tx:DAT Rx:NACK
		EE_TWI0.EEFlag= EE_TWI0.EEStatus+0x80;
		if(EE_TWI0.EEStatus==EE_WRITE_DATA)
		{
			if(EE_TWI0.EEDataInx==EE_TWI0.EEDataLength)
			{
				EE_TWI0.EEFlag= EE_SUCCESS;
			}
		}
		SICON=SICON|(STO);
		EE_TWI0.EEStatus=EE_SEND_STOP;
    }
	else if(SISTA==0x10)
	{ // Tx:ReStart
		SICON=SICON&(~STA);
		SIDAT=EE_TWI0.EEDeviceAddr|0x01;
		EE_TWI0.EEStatus=EE_SEND_SLA;
	}
    else if(SISTA==0x40)
    { // Tx:SLA+R Rx:ACK
		SICON=SICON|(AA);
		EE_TWI0.EEDataInx=0;
		EE_TWI0.EEStatus=EE_READ_DATA;
    }
    else if(SISTA==0x48)
    { // Tx:SLA+R Rx:NACK
		EE_TWI0.EEFlag=EE_TWI0.EEStatus+0x80;
		EE_TWI0.EEStatus=EE_SEND_STOP;
		SICON=SICON|(STO);
    }
    else if(SISTA==0x50)
    { // Rx:DAT Tx:ACK
		if(EE_TWI0.EEStatus==EE_READ_DATA)
		{
			if(EE_TWI0.EEDataInx>=EE_TWI0.EEDataLength)
			{
				SICON=SICON|(STO);
				EE_TWI0.EEFlag= EE_SUCCESS;
				EE_TWI0.EEStatus=EE_SEND_STOP;
			}
			else
			{
				EE_TWI0.EEDataBuf[EE_TWI0.EEDataInx]=SIDAT;
				EE_TWI0.EEDataInx++;
				if(EE_TWI0.EEDataInx>=EE_TWI0.EEDataLength-1)
				{
					SICON=SICON&(~AA);
				}
				else
				{
					SICON=SICON|(AA);
				}
			}
		}
		else
		{
			EE_TWI0.EEFlag=EE_TWI0.EEStatus+0x80;
			EE_TWI0.EEStatus=EE_SEND_STOP;
			SICON=SICON|(STO);
		}
    }
    else if(SISTA==0x58)
    { // Rx:DAT Tx:NACK
		EE_TWI0.EEFlag= EE_TWI0.EEStatus+0x80;
		if(EE_TWI0.EEStatus==EE_READ_DATA)
		{
			EE_TWI0.EEDataBuf[EE_TWI0.EEDataInx]=SIDAT;
			EE_TWI0.EEDataInx++;
			if(EE_TWI0.EEDataInx==EE_TWI0.EEDataLength)
			{
				EE_TWI0.EEFlag= EE_SUCCESS;
			}
		}
		SICON=SICON|(STO);
		EE_TWI0.EEStatus=EE_SEND_STOP;
    }
	TWI0OvTime=IIC_OVTIME_MAX;
	SICON=SICON&(~SI);
	_pop_(SFRPI);
}


/***********************************************************************************
函数名称:   void INT_TWSI1(void)
功能描述:TWI1 中断服务程序
		 
输入参数:   
返回参数:     
*************************************************************************************/
void INT_TWI1() interrupt INT_VECTOR_TWI1
{
	_push_(SFRPI);
	SFR_SetPage(1);

	if(SI1STA==0x08)
	{ // Tx:Start
		SI1CON=SI1CON&(~STA1);
		SI1DAT=EE_TWI1.EEDeviceAddr&0xFE;
		EE_TWI1.EEStatus=EE_SEND_SLA;
	}
	else if(SI1STA==0x18)
    { // Tx:SLA+W Rx:ACK
    	// To send RegAddr high
		SI1CON=SI1CON|(AA1);
    	SI1DAT=(u8)(EE_TWI1.EEDataAddr>>8);
		EE_TWI1.EEStatus=EE_WRITE_ADDR_H;
    }
    else if(SI1STA==0x20)
    { // Tx:SLA+W Rx:NACK
		EE_TWI1.EEFlag=EE_TWI1.EEStatus+0x80;
		EE_TWI1.EEStatus=EE_SEND_STOP;
		SI1CON=SI1CON|(STO1);
    }
    else if(SI1STA==0x28)
    { // Tx:DAT Rx:ACK
		if(EE_TWI1.EEStatus==EE_WRITE_ADDR_H)
		{
			// To send RegAddr LOW
			SI1CON=SI1CON|(AA1);
			SI1DAT=(u8)(EE_TWI1.EEDataAddr);
			EE_TWI1.EEStatus=EE_WRITE_ADDR_L;
		}
		else if(EE_TWI1.EEStatus==EE_WRITE_ADDR_L)
		{
			// Send RegAddr Done
			if(EE_TWI1.EEMode==EE_MODE_READ)
			{
				// Read mode: To Tx ReStart;
				SI1CON=SI1CON|(STA1);
				EE_TWI1.EEStatus=EE_SEND_RESTART;
			}
			else
			{
				// Write mode: To Tx Data;
				SI1CON=SI1CON|(AA1);
				EE_TWI1.EEStatus=EE_WRITE_DATA;
				EE_TWI1.EEDataInx=0;
				SI1DAT=EE_TWI1.EEDataBuf[EE_TWI1.EEDataInx];
				EE_TWI1.EEDataInx++;
			}
		}
		else if(EE_TWI1.EEStatus==EE_WRITE_DATA)
		{
			if(EE_TWI1.EEDataInx>=EE_TWI1.EEDataLength)
			{
				SI1CON=SI1CON|(STO1);
				EE_TWI1.EEFlag= EE_SUCCESS;
				EE_TWI1.EEStatus=EE_SEND_STOP;
			}
			else
			{
				SI1CON=SI1CON|(AA1);
				SI1DAT=EE_TWI1.EEDataBuf[EE_TWI1.EEDataInx];
				EE_TWI1.EEDataInx++;
			}
		}
		else
		{
			SI1CON=SI1CON|(STO1);
			EE_TWI1.EEFlag=EE_TWI1.EEStatus+0x80;
			EE_TWI1.EEStatus=EE_SEND_STOP;
		}
    }
    else if(SI1STA==0x30)
    { // Tx:DAT Rx:NACK
		EE_TWI1.EEFlag= EE_TWI1.EEStatus+0x80;
		if(EE_TWI1.EEStatus==EE_WRITE_DATA)
		{
			if(EE_TWI1.EEDataInx==EE_TWI1.EEDataLength)
			{
				EE_TWI1.EEFlag= EE_SUCCESS;
			}
		}
		SI1CON=SI1CON|(STO1);
		EE_TWI1.EEStatus=EE_SEND_STOP;
    }
	else if(SI1STA==0x10)
	{ // Tx:ReStart
		if(EE_TWI1.EEMode==EE_MODE_READ)
		{ 
			SI1CON=SI1CON&(~STA1);
			SI1DAT=EE_TWI1.EEDeviceAddr|0x01;
			EE_TWI1.EEStatus=EE_SEND_SLA;
		}
		else
		{
			EE_TWI1.EEFlag=EE_TWI1.EEStatus+0x80;
			EE_TWI1.EEStatus=EE_SEND_STOP;
			SI1CON=SI1CON|(STO1);
		}
	}
    else if(SI1STA==0x40)
    { // Tx:SLA+R Rx:ACK
		SI1CON=SI1CON|(AA1);
		EE_TWI1.EEDataInx=0;
		EE_TWI1.EEStatus=EE_READ_DATA;
    }
    else if(SI1STA==0x48)
    { // Tx:SLA+R Rx:NACK
		EE_TWI1.EEFlag=EE_TWI1.EEStatus+0x80;
		EE_TWI1.EEStatus=EE_SEND_STOP;
		SI1CON=SI1CON|(STO1);
    }
    else if(SI1STA==0x50)
    { // Rx:DAT Tx:ACK
		if(EE_TWI1.EEStatus==EE_READ_DATA)
		{
			if(EE_TWI1.EEDataInx>=EE_TWI1.EEDataLength)
			{
				SI1CON=SI1CON|(STO1);
				EE_TWI1.EEFlag= EE_SUCCESS;
				EE_TWI1.EEStatus=EE_SEND_STOP;
			}
			else
			{
				EE_TWI1.EEDataBuf[EE_TWI1.EEDataInx]=SI1DAT;
				EE_TWI1.EEDataInx++;
				if(EE_TWI1.EEDataInx>=EE_TWI1.EEDataLength-1)
				{
					SI1CON=SI1CON&(~AA1);
				}
				else
				{
					SI1CON=SI1CON|(AA1);
				}
			}
		}
		else
		{
			EE_TWI1.EEFlag=EE_TWI1.EEStatus+0x80;
			EE_TWI1.EEStatus=EE_SEND_STOP;
			SI1CON=SI1CON|(STO1);
		}
    }
    else if(SI1STA==0x58)
    { // Rx:DAT Tx:NACK
		EE_TWI1.EEFlag= EE_TWI1.EEStatus+0x80;
		if(EE_TWI1.EEStatus==EE_READ_DATA)
		{
			EE_TWI1.EEDataBuf[EE_TWI1.EEDataInx]=SI1DAT;
			EE_TWI1.EEDataInx++;
			if(EE_TWI1.EEDataInx==EE_TWI1.EEDataLength)
			{
				EE_TWI1.EEFlag= EE_SUCCESS;
			}
		}
		SI1CON=SI1CON|(STO1);
		EE_TWI1.EEStatus=EE_SEND_STOP;
    }
	TWI1OvTime=IIC_OVTIME_MAX;
	SI1CON=SI1CON&(~SI1);
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
函数名称:   void InitTWI0(void)
功能描述:TWI0初始化
输入参数:   
返回参数:     
*************************************************************************************/
void InitTWI0()
{
	TWI0_Clear();
	TWI0_SetClock(TWI0_CLK_SYSCLK_32);
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
	TWI1_SetClock(TWI0_CLK_SYSCLK_32);
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
	INT_EnTWI0();						// 使能TWI0中断
	INT_EnTWI1();						// 使能TWI1中断
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
函数名称:   void TWI0_StartWriteBuf(u8 DevAddr,u16 RegStartAddr,u8 *pByte)
功能描述:
		启动通过TWI0将数据发送到从机
输入参数: 
		u8 DevAddr: 从机地址
		u16 RegStartAddr: 从机数据的起始地址
		u8 *pBuf: 待发送数据的缓存地址
		u8 Len: 待发送的数据个数
返回参数:
		无
*************************************************************************************/
void TWI0_StartWriteBuf(u8 DevAddr,u16 RegStartAddr,u8 *pBuf,u8 Len)
{
	
	EE_TWI0.EEMode=EE_MODE_WRITE;
	EE_TWI0.EEDataBuf=pBuf;
	EE_TWI0.EEDeviceAddr=DevAddr;
	EE_TWI0.EEDataAddr=RegStartAddr;
	EE_TWI0.EEDataLength=Len;
	EE_TWI0.EEStatus=EE_SEND_START;
	// start TWI0
	TWI0OvTime=IIC_OVTIME_MAX;
	TWI0_SendSTART();	
}


/***********************************************************************************
函数名称:   void TWI0_StartReadBuf(u8 DevAddr,u16 RegStartAddr,u8 *pByte)
功能描述:
		启动通过TWI0读取从机的数据
输入参数: 
		u8 DevAddr: 从机地址
		u16 RegStartAddr: 从机待读取数据的起始地址
		u8 *pBuf: 保存读取数据的缓存地址
		u8 Len: 待读取的数据个数
返回参数:
		无
*************************************************************************************/
void TWI0_StartReadBuf(u8 DevAddr,u16 RegStartAddr,u8 *pBuf,u8 Len)
{
	EE_TWI0.EEMode=EE_MODE_READ;
	EE_TWI0.EEDataBuf=pBuf;
	EE_TWI0.EEDeviceAddr=DevAddr;
	EE_TWI0.EEDataAddr=RegStartAddr;
	EE_TWI0.EEDataLength=Len;
	EE_TWI0.EEStatus=EE_SEND_START;
	// start TWI0
	TWI0OvTime=IIC_OVTIME_MAX;
	TWI0_SendSTART();	
}


/***********************************************************************************
函数名称:   void TWI1_StartWriteBuf(u8 DevAddr,u16 RegStartAddr,u8 *pByte)
功能描述:
		启动通过TWI1将数据发送到从机
输入参数: 
		u8 DevAddr: 从机地址
		u16 RegStartAddr: 从机数据的起始地址
		u8 *pBuf: 待发送数据的缓存地址
		u8 Len: 待发送的数据个数
返回参数:
		无
*************************************************************************************/
void TWI1_StartWriteBuf(u8 DevAddr,u16 RegStartAddr,u8 *pBuf,u8 Len)
{
	
	EE_TWI1.EEMode=EE_MODE_WRITE;
	EE_TWI1.EEDataBuf=pBuf;
	EE_TWI1.EEDeviceAddr=DevAddr;
	EE_TWI1.EEDataAddr=RegStartAddr;
	EE_TWI1.EEDataLength=Len;
	EE_TWI1.EEStatus=EE_SEND_START;
	// start TWI1
	TWI1OvTime=IIC_OVTIME_MAX;
	TWI1_SendSTART();	
}


/***********************************************************************************
函数名称:   void TWI1_StartReadBuf(u8 DevAddr,u16 RegStartAddr,u8 *pByte)
功能描述:
		启动通过TWI1读取从机的数据
输入参数: 
		u8 DevAddr: 从机地址
		u16 RegStartAddr: 从机待读取数据的起始地址
		u8 *pBuf: 保存读取数据的缓存地址
		u8 Len: 待读取的数据个数
返回参数:
		无
*************************************************************************************/
void TWI1_StartReadBuf(u8 DevAddr,u16 RegStartAddr,u8 *pBuf,u8 Len)
{
	EE_TWI1.EEMode=EE_MODE_READ;
	EE_TWI1.EEDataBuf=pBuf;
	EE_TWI1.EEDeviceAddr=DevAddr;
	EE_TWI1.EEDataAddr=RegStartAddr;
	EE_TWI1.EEDataLength=Len;
	EE_TWI1.EEStatus=EE_SEND_START;
	// start TWI1
	TWI1OvTime=IIC_OVTIME_MAX;
	TWI1_SendSTART();	
}


void TWI0_TestProc(void)
{
	u8 i;
	
	if(EE_TWI0.EEStatus==EE_IDLE)
	{
		if(TWI0TestTime==0)
		{
			if(EE_TWI0.EEMode!=EE_MODE_WRITE)
			{ // 开始写测试
				SendStr("\nTWI0 Write EEPROM Start...");
				SendHex(TWI0TestAddr.B.BHigh);
				SendHex(TWI0TestAddr.B.BLow);
				i=0;
				for(i=0;i<DATA_LENGTH;i++)
				{
					TWI0WriteBuf[i]=i+rand;
					i++;
				}
				TWI0_StartWriteBuf(I2C_SLAVE_ADDR, TWI0TestAddr.W, &TWI0WriteBuf[0],DATA_LENGTH);
			}
			else
			{
				SendStr("\nTWI0 Read EEPROM Start...");
				SendHex(TWI0TestAddr.B.BHigh);
				SendHex(TWI0TestAddr.B.BLow);
				TWI0_StartReadBuf(I2C_SLAVE_ADDR, TWI0TestAddr.W, &TWI0ReadBuf[0],DATA_LENGTH);
				TWI0TestAddr.W=TWI0TestAddr.W+DATA_LENGTH;
				if(TWI0TestAddr.W>=0x1000)
				{
					TWI0TestAddr.W=0;
				}
			}
		}
	}
	else if(EE_TWI0.EEStatus==EE_SEND_STOP)
	{
		TWI0TestTime=TWI0_TEST_INTERVAL_TIME;
		if(EE_TWI0.EEFlag != EE_SUCCESS)
		{
			SendStr("\nTWI0 Err: 0x");
			SendHex(EE_TWI0.EEFlag);
		}
		else
		{
			if(EE_TWI0.EEMode==EE_MODE_READ)
			{
				for(i=0;i<DATA_LENGTH;i++)
				{
					if(TWI0WriteBuf[i]!=TWI0ReadBuf[i]) break;
				}
				if(i>=DATA_LENGTH)
				{// 校验成功
					LED_G_0=0;
					SendStr("\nTWI0 WR Success!!");
					DelayXms(50);
					LED_G_0=1;
				}
				else
				{// 校验失败
					SendStr("\nTWI0 Verify Fail!! 0x");
					SendHex(i);
				}
			}	
			else
			{
				SendStr("\nTWI0 Wrtie Done!");
				DelayXms(5);
				TWI0TestTime=0;
			}
		}
		EE_TWI0.EEStatus=EE_IDLE;
	}
	else
	{
		if(TWI0OvTime==0)
		{ // 
			SendStr("\nTWI0 Fail-->TWI0_Reset..");
			InitTWI0();
			EE_TWI0.EEStatus=EE_IDLE;
			EE_TWI0.EEMode=EE_MODE_READ;
			TWI0TestTime=TWI0_TEST_INTERVAL_TIME;
		}
	}
}

void TWI1_TestProc(void)
{
	u8 i;
	
	if(EE_TWI1.EEStatus==EE_IDLE)
	{
		if(TWI1TestTime==0)
		{
			if(EE_TWI1.EEMode!=EE_MODE_WRITE)
			{ // 开始写测试
				SendStr("\nTWI1 Write EEPROM Start...");
				SendHex(TWI1TestAddr.B.BHigh);
				SendHex(TWI1TestAddr.B.BLow);
				i=0;
				for(i=0;i<DATA_LENGTH;i++)
				{
					TWI1WriteBuf[i]=i+rand;
					i++;
				}
				TWI1_StartWriteBuf(I2C_SLAVE_ADDR, TWI1TestAddr.W, &TWI1WriteBuf[0],DATA_LENGTH);
			}
			else
			{
				SendStr("\nTWI1 Read EEPROM Start...");
				SendHex(TWI1TestAddr.B.BHigh);
				SendHex(TWI1TestAddr.B.BLow);
				TWI1_StartReadBuf(I2C_SLAVE_ADDR, TWI1TestAddr.W, &TWI1ReadBuf[0],DATA_LENGTH);
				TWI1TestAddr.W=TWI1TestAddr.W+DATA_LENGTH;
				if(TWI1TestAddr.W>=0x1000)
				{
					TWI1TestAddr.W=0;
				}
			}
		}
	}
	else if(EE_TWI1.EEStatus==EE_SEND_STOP)
	{
		TWI1TestTime=TWI1_TEST_INTERVAL_TIME;
		if(EE_TWI1.EEFlag != EE_SUCCESS)
		{
			SendStr("\nTWI1 Err: 0x");
			SendHex(EE_TWI1.EEFlag);
		}
		else
		{
			if(EE_TWI1.EEMode==EE_MODE_READ)
			{
				for(i=0;i<DATA_LENGTH;i++)
				{
					if(TWI1WriteBuf[i]!=TWI1ReadBuf[i]) break;
				}
				if(i>=DATA_LENGTH)
				{// 校验成功
					LED_G_1=0;
					SendStr("\nTWI1 WR Success!!");
					DelayXms(50);
					LED_G_1=1;
				}
				else
				{// 校验失败
					SendStr("\nTWI1 Verify Fail!! 0x");
					SendHex(i);
				}
			}	
			else
			{
				SendStr("\nTWI1 Wrtie Done!");
				DelayXms(5);
				TWI1TestTime=0;
			}
		}
		EE_TWI1.EEStatus=EE_IDLE;
	}
	else
	{
		if(TWI1OvTime==0)
		{ // 
			SendStr("\nTWI1 Fail-->TWI1_Reset..");
			InitTWI1();
			EE_TWI1.EEStatus=EE_IDLE;
			EE_TWI1.EEMode=EE_MODE_READ;
			TWI1TestTime=TWI1_TEST_INTERVAL_TIME;
		}
	}
}

void main()
{
    InitSystem();
	
 	SendStr("\nStart");
	LED_G_0=0;LED_G_1=0;LED_R=0;
	DelayXms(1000);
	LED_G_0=1;LED_G_1=1;LED_R=1;

	rand = 0x80;

	TWI0TestAddr.W=0x0000;
	EE_TWI0.EEStatus=EE_IDLE;
	EE_TWI0.EEMode=EE_MODE_READ;
	TWI0TestTime=TWI0_TEST_INTERVAL_TIME;

	TWI1TestAddr.W=0x0000;
	EE_TWI1.EEStatus=EE_IDLE;
	EE_TWI1.EEMode=EE_MODE_READ;
	TWI1TestTime=TWI1_TEST_INTERVAL_TIME;
	
    while(1)
    {
		rand++;
    	if(LedTime==0)
    	{
			LED_R = ~LED_R;
			LedTime=200;
		}
		TWI0_TestProc();
		TWI1_TestProc();

    }
}


