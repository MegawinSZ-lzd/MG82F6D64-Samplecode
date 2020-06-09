/*********************************************************************
    项目名称:MG82F6D64-DEMO
    作    者:YZY
			测试板使用 MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=12MHz, SysCLK=12MHz
	功能描述：
			RTC时钟为XTAL 32.768KHz
			RTC Clock 预分频 32768
			RTC 重载值 (64-1)=63
			RTC溢出频率为 32768/32768/1 = 1Hz
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

/***********************************************************************************
函数名称:   void INT_SF(void)
功能描述:SF(系统标志位)中断服务程序
		 RTC,WDTF,BOD0F,BOD1F
输入参数:   
返回参数:     
*************************************************************************************/
void INT_SF(void) interrupt INT_VECTOR_SF
{
	if((PCON1&WDTF)!=0)
	{
		PCON1=PCON1|WDTF;
	}
	if((PCON1&BOF0)!=0)
	{
		PCON1=PCON1|BOF0;
	}
	if((PCON1&BOF1)!=0)
	{
		PCON1=PCON1|BOF1;
	}
	if((PCON1&RTCF)!=0)
	{
		PCON1=PCON1|RTCF;
		LED_R=!LED_R;
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
函数名称:   u8 IAP_ReadPPage(u8 PsfrAddr)
功能描述:读取P页特殊功能器内容
输入参数:   
		 u8 PsfrAddr: P页地址
返回参数:     
		 u8: P页内容
*************************************************************************************/
u8 IAP_ReadPPage(u8 PsfrAddr)
{
	bit bEA=EA;
	EA = 0; 					//关中断
	IFADRH = 0; 				//IFADRH必须为0
	IFADRL= PsfrAddr;			//送P页地址;
	IFMT = ISP_READ_P;
	ISPCR = 0x80;				//设置等待时间, 允许ISP/IAP操作
	SCMD = 0x46;
	SCMD = 0xB9;
	nop();
	IFMT=0;
	ISPCR = 0;					//清ISP/IAP特殊寄存器，防止误操作
	EA = bEA;					//恢复中断
	return IFD;
}


/***********************************************************************************
函数名称:void IAP_WritePPage(u8 PsfrAddr,u8 PsfrData)
功能描述:写P页特殊功能器
输入参数:   
		 u8 PsfrAddr: P页地址,u8 PsfrData:  P页内容
返回参数:     
*************************************************************************************/
void IAP_WritePPage(u8 PsfrAddr,u8 PsfrData)
{
	bit bEA=EA;
	EA = 0; 					//关中断
	IFADRH = 0; 				//IFADRH必须为0
	IFADRL= PsfrAddr;			//送P页地址;
	IFD= PsfrData;				//送P页内容
	IFMT = ISP_WRITE_P;
	ISPCR = 0x80;				//设置等待时间, 允许ISP/IAP操作
	SCMD = 0x46;
	SCMD = 0xB9;
	nop();
	IFMT=0;
	ISPCR = 0;					//清ISP/IAP特殊寄存器，防止误操作
	EA = bEA;					//恢复中断
}


/***********************************************************************************
函数名称:   void InitRTC(void)
功能描述:RTC 初始化设置	
		RTC时钟为ILRCO 32KHz
		RTC Clock 预分频 32
		RTC 重载值 (64-10)=54
		RTC溢出频率为 32K/32/10=100Hz
输入参数:   
返回参数:     
*************************************************************************************/
void InitRTC(void)
{
	u8 x;
	x=IAP_ReadPPage(CKCON2_P);
	x=(x&0x3F)|(ENABLE_XTAL|GAIN_FOR_32K);
	IAP_WriteByte(CKCON2_P,x);
	while(CKCON1 & XTOR == 0x00);		// 等待XTAL准备就绪
	
	RTC_SetClock_XTAL2_ECKI_P60();		// 选择RTC时钟为 XTAL 32.768KHz
	RTC_SetClock_Div_32768();			// 选择RTC Clock 预分频 32768
	RTC_SetReload(64-1);				// RTC 重载值 (64-1)=63
	RTC_SetCounter(64-1);	
	RTC_ClearRTCF();					// 清RTCF
	RTC_Enable();						// 使能RTC
	
}

/***********************************************************************************
函数名称:   void InitInterrupt(void)
功能描述:InitInterrupt初始化设置
输入参数:   
返回参数:     
*************************************************************************************/
void InitInterrupt(void)
{
	INT_EnSF_RTC();			// 使能RTC中断,必须再使能系统标志中断
	INT_EnSF();				// 使能系统标志中断

	
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
	InitRTC();			
	InitInterrupt();		// 中断初始化
	
	INT_EnAll();			// 使能全局中断

}

void main()
{
    InitSystem();
	
 	LED_G_0=0;LED_G_1=0;LED_R=0;
	DelayXms(1000);
 	LED_G_0=1;LED_G_1=1;LED_R=1;
					
    while(1)
    {
    	DelayXms(100);
    	LED_G_0=!LED_G_0;
    }

}


