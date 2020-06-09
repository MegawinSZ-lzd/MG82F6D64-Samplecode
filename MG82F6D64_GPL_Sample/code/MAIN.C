/*********************************************************************
    项目名称:MG82F6D64-DEMO
    作    者:YZY
			测试板使用 MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=12MHz, SysCLK=12MHz
	功能描述：
			GPL 位序改变
			GPL 对指定数据的CRC计算(CCIT16 CRC16 0x1021)		
			GPL 对Flash自动计算CRC
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


code u8 TestBuf[16]={0xAA,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};


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
功能描述:获取计数值
输入参数:  
返回参数:     
*************************************************************************************/
u32 GetCounter()
{
	DWordTypeDef dwTime;
	TM_GlobalSTOP(TIMER0_GLOBAL_BIT|TIMER1_GLOBAL_BIT);
	dwTime.B.BHigh=TH0;
	dwTime.B.BMHigh=TL0;
	dwTime.B.BMLow=TH1;
	dwTime.B.BLow=TL1;
	return dwTime.DW;
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
	TM_SetT1Clock_SYSCLKDiv12();			// 设置T1时钟源为 SYSCLK/12
	TM_SetT1Gate_Disable();
	TM_SetT1LowByte(0);						// 设置T1低8位
	TM_SetT1HighByte(0);					// 设置T1高8位
	//TM_EnableT1();						// 使能T1
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
	PORT_SetP5PushPull(BIT5|BIT6|BIT7);					// set P55,P56,P57 as Push Pull(LED)
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
	InitTimer0();
	InitTimer1();
}

/***********************************************************************************
函数名称:   u16 CRC16_CCIT(u8 *ptr,u16 len)
功能描述:软件方法进行CRC16计算
输入参数:   
		 u8 *pBuf: 待计算CRC缓存的起始地址
		 u16 len: 待计算CRC缓存的长度
返回参数:     
		 u16: CRC16值
*************************************************************************************/
u16 CRC16_CCIT(u8 *ptr,u16 len)
{
	u16 wCRCin = 0x0000;
	u16 wCPoly = 0x1021;
	u16 wChar=0;
	u16 i;
	u8 x;
	for(i=0;i<len;i++)
	{
		wChar=(ptr[i])&0x00FF;
		wCRCin=wCRCin^(wChar<<8);
		for(x=0;x<8;x++)
		{
			if((wCRCin&0x8000)!=0)
			{
				wCRCin=wCRCin<<1;
				wCRCin=wCRCin^wCPoly;
			}
			else
			{
				wCRCin=wCRCin<<1;
			}
		}
	}
    return wCRCin;
}


/***********************************************************************************
函数名称:   u16 GPL_CRC(u8 *pBuf,u8 len)
功能描述:CRC计算
输入参数:   
		 u8 *pBuf: 待计算CRC缓存的起始地址
		 u8 len: 待计算CRC缓存的长度
返回参数:     
		 u16: CRC16值
*************************************************************************************/
u16 GPL_CRC(u8 *pBuf,u8 len)
{
	WordTypeDef CRCValue;
	u8 i;
	// 设置CRC Seed
	GPL_CRC_WriteCRCSeedH(0x00);
	GPL_CRC_WriteCRCSeedL(0x00);
	
	for(i=0;i<len;i++)
	{
		GPL_CRC_WriteCRCData(pBuf[i]);
	}
	GPL_CRC_ReadCRCResultH(CRCValue.B.BHigh);
	GPL_CRC_ReadCRCResultL(CRCValue.B.BLow);
	return CRCValue.W;
}

/***********************************************************************************
函数名称:   u16 GPL_AutoFlashCRC(u16 StartAddr,u16 EndAddr)
功能描述:读取P页特殊功能器内容
输入参数:   
		 u16 StartAddr: CRC计算起始地址
		 u16 EndAddr: CRC计算结束地址(FLASH页面的最后一个字节)
		 			  如: ....0x33FF,0x35FF,0x37FF,0x39FF,0x3BFF,0x3DFF,0x3FFF.
返回参数:     
		 u16: CRC16
*************************************************************************************/
u16 GPL_AutoFlashCRC(u16 StartAddr,u16 EndAddr)
{
	WordTypeDef CRCValue;
	bit bEA=EA;
	// 设置CRC Seed
	GPL_CRC_WriteCRCSeedH(0x00);
	GPL_CRC_WriteCRCSeedL(0x00);

	GPL_CRC_CRC0DA_DataIn();
	
	// 设置CRC结束地址, 由IAPLB决定
	EA = 0; 					//关中断
	IFADRH = 0; 				//IFADRH必须为0
	IFADRL= IAPLB_P;			//送P页地址;
	IFD= (EndAddr>>8)&0xFE;		//送P页内容
	IFMT = ISP_WRITE_P;
	ISPCR = 0x80;				//设置等待时间, 允许ISP/IAP操作
	SCMD = 0x46;
	SCMD = 0xB9;
	nop();
	IFMT=0;
	ISPCR = 0;					//清ISP/IAP特殊寄存器，防止误操作
	// 设置起始地址
	IFADRH = StartAddr/256; 	//高地址
	IFADRL= StartAddr%256;		//低地址
	IFMT = ISP_AUTO_FLASH_CRC;
	ISPCR = 0x80;				//设置等待时间, 允许ISP/IAP操作
	SCMD = 0x46;
	SCMD = 0xB9;
	nop();
	GPL_CRC_ReadCRCResultH(CRCValue.B.BHigh);
	GPL_CRC_ReadCRCResultL(CRCValue.B.BLow);
	IFMT=0;
	ISPCR = 0;					//清ISP/IAP特殊寄存器，防止误操作
	EA = bEA;					//恢复中断
	return CRCValue.W;
}

void main()
{
	WordTypeDef CRCValue;
	u8 TestBorev;
	DWordTypeDef dwTime;
	unsigned char volatile code *pCode;

	InitSystem();

	SendStr("\nStart GPL Test!");

	LED_G_1=0;LED_R=0;
	DelayXms(1000);
	LED_G_1=1;LED_R=1;	

	// 测试位序变换
	TestBorev=0x46;
	SendStr("\nBorev old:0x");
	SendHex(TestBorev);
	GPL_BOREV(TestBorev);
	SendStr("\nBorev new:0x");
	SendHex(TestBorev);

	
	// 读取0x0000~0x03FF的CRC16值
	ResetCounter();
	CRCValue.W=GPL_AutoFlashCRC(0x0000, (1024-1));
	dwTime.DW=GetCounter();
	SendStr("\nHW 1KFlash CRC16 Value:0x");
	SendHex(CRCValue.B.BHigh);
	SendHex(CRCValue.B.BLow);
	SendStr("; Time:");
	SendDW2DEC(dwTime.DW);

	// 读取0x0000~0x03FF的CRC16值
	pCode=0x0000;
	ResetCounter();
	CRCValue.W=CRC16_CCIT(pCode, 1024);
	dwTime.DW=GetCounter();
	SendStr("\nSW 1KFlash CRC16 Value:0x");
	SendHex(CRCValue.B.BHigh);
	SendHex(CRCValue.B.BLow);
	SendStr("; Time:");
	SendDW2DEC(dwTime.DW);
	
	// 计算TestBuf[16]的CRC16值
	ResetCounter();
	CRCValue.W=GPL_CRC(TestBuf,16);
	dwTime.DW=GetCounter();
	SendStr("\nHW TestBuf[16] CRC16 Value:0x");
	SendHex(CRCValue.B.BHigh);
	SendHex(CRCValue.B.BLow);
	SendStr("; Time:");
	SendDW2DEC(dwTime.DW);
	
	// 软件计算TestBuf[16]的CRC16值
	ResetCounter();
	CRCValue.W=CRC16_CCIT(TestBuf,16);
	dwTime.DW=GetCounter();
	SendStr("\nSoft TestBuf[16] CRC16 Value:0x");
	SendHex(CRCValue.B.BHigh);
	SendHex(CRCValue.B.BLow);
	SendStr("; Time:");
	SendDW2DEC(dwTime.DW);
	while(1)
	{
		DelayXms(100);
		LED_G_0 = !LED_G_0;
	}

}


