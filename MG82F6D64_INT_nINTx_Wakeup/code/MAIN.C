/*********************************************************************
    项目名称:MG82F6D64-DEMO
    作    者:YZY
			测试板使用 MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=32MHz, SysCLK=32MHz
	功能描述：
			设置P10,P11,P16用于INT1,INT0,INT2,下降沿触发
			20s无中断触发,进入掉电模式
			INT1(P10),INT0(P15),INT2(P11),INT3(P16)低电平唤醒

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
#define MCU_SYSCLK		32000000
/*************************************************/
/*************************************************
定义CPU时钟 CpuClk (不大于36MHz)
	1) CpuCLK=SysCLK
	2) CpuClk=SysClk/2
*************************************************/
#define MCU_CPUCLK		(MCU_SYSCLK)
//#define MCU_CPUCLK		(MCU_SYSCLK/2)

#define LED_G_0		P55
#define LED_R		P56
#define LED_G_1		P57

u8 WakeUpSorce;
u8 INTCnt;

/***********************************************************************************
函数名称:   void INT_INT0(void)	
功能描述:INT0中断服务程序
输入参数:   
返回参数:     
*************************************************************************************/
void INT_INT0(void)		interrupt INT_VECTOR_INT0
{
	WakeUpSorce=0;
	INTCnt=0;
	LED_G_0=1;LED_G_1=1;LED_R=1;
}

/***********************************************************************************
函数名称:   void INT_INT1(void)	
功能描述:INT1中断服务程序
输入参数:   
返回参数:     
*************************************************************************************/
void INT_INT1(void)		interrupt INT_VECTOR_INT1
{
	WakeUpSorce=1;
	INTCnt=0;
	LED_G_0=1;LED_G_1=1;LED_R=1;
}

/***********************************************************************************
函数名称:   void INT_INT2(void)	
功能描述:INT2中断服务程序
输入参数:   
返回参数:     
*************************************************************************************/
void INT_INT2(void)		interrupt INT_VECTOR_INT2
{
	WakeUpSorce=2;
	INTCnt=0;
	LED_G_0=1;LED_G_1=1;LED_R=1;
}

/***********************************************************************************
函数名称:   void INT_INT3(void)	
功能描述:INT3中断服务程序
输入参数:   
返回参数:     
*************************************************************************************/
void INT_INT3(void)		interrupt INT_VECTOR_INT3
{
	WakeUpSorce=3;
	INTCnt=0;
	LED_G_0=1;LED_G_1=1;LED_R=1;
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
函数名称:   void InitPort(void)
功能描述:   IO配置
输入参数:   
返回参数:     
*************************************************************************************/
void InitPort(void)
{
	PORT_SetP5PushPull(BIT5|BIT6|BIT7);					// 设置P55,P56,P57为推挽输出(LED)
	PORT_SetP3QuasiBi(BIT0|BIT1|BIT3|BIT4|BIT5);		// 设置P30,P31,P33,P34,P35为准双向口
	PORT_SetP1OpenDrainPu(BIT0|BIT1|BIT5|BIT6);			// 设置P10,P11,P15,P16为开漏输出带上拉，做INT使用
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
	INT_EnINT0();							//	使能INT0中断
	INT_EnINT1();							//	使能INT1中断
	INT_EnINT2();							//	使能INT2中断
	INT_EnINT3();							//	使能INT3中断
	INT_EnAll();				// 开启总中断
}	

/***********************************************************************************
函数名称:   void InitINT0(void)
功能描述:   INT0初始化
输入参数:   
返回参数:     
*************************************************************************************/
void InitINT0(void)
{
	INT_SetINT0P10();						//设置INT0引脚P10
	INT_SetINT0_DetectEdge();				//设置INT0外部中断检测方式为边沿检测, 当进入掉电模式后,自动转为电平检测
	INT_SetINT0_DetectLowFalling();			//设置INT0外部中断检测方式为低电平或下降沿触发
	INT_SetINT0Filter_SysclkDiv6_x3();		//设置INT0滤波为（sysclk/6）*3

}

/***********************************************************************************
函数名称:   void InitINT1(void)
功能描述:   INT1初始化
输入参数:   
返回参数:     
*************************************************************************************/
void InitINT1(void)
{
	INT_SetINT1P15();						//设置INT1引脚P10
	INT_SetINT1_DetectEdge();				//设置INT1外部中断检测方式为边沿检测, 当进入掉电模式后,自动转为电平检测
	INT_SetINT1_DetectLowFalling();			//设置INT1外部中断检测方式为低电平或下降沿触发
	INT_SetINT1Filter_SysclkDiv6_x3();		//设置INT1滤波为（sysclk/6）*3

}

/***********************************************************************************
函数名称:   void InitINT2(void)
功能描述:   INT2初始化
输入参数:   
返回参数:     
*************************************************************************************/
void InitINT2(void)
{
	INT_SetINT2P11();						//设置INT2引脚P11
	INT_SetINT2_DetectEdge();				//设置INT2外部中断检测方式为边沿检测, 当进入掉电模式后,自动转为电平检测
	INT_SetINT2_DetectLowFalling();			//设置INT2外部中断检测方式为低电平或下降沿触发
	INT_SetINT2Filter_SysclkDiv6_x3();		//设置INT2滤波为（sysclk/6）*3

}

/***********************************************************************************
函数名称:   void InitINT3(void)
功能描述:   INT3初始化
输入参数:   
返回参数:     
*************************************************************************************/
void InitINT3(void)
{
	INT_SetINT3P16();							//设置INT3引脚P16
	INT_SetINT3_DetectEdge();				//设置INT3外部中断检测方式为边沿检测, 当进入掉电模式后,自动转为电平检测
	INT_SetINT3_DetectLowFalling();			//设置INT3外部中断检测方式为低电平或下降沿触发
	INT_SetINT3Filter_SysclkDiv6_x3();		//设置INT3滤波为（sysclk/6）*3

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
	InitINT0();
	InitINT1();
	InitINT2();
	InitINT3();
	InitInterrupt();
}

void main()
{
	u8 BakCKCON2;
	
	InitSystem();
	LED_G_0=0;LED_G_1=0;LED_R=0;
	DelayXms(2000);
	LED_G_0=1;LED_G_1=1;LED_R=1;
	INTCnt=0;
	WakeUpSorce=0;
	while(1)
	{

		if(WakeUpSorce == 0)
		{
			LED_G_0=!LED_G_0;
		}
		else if(WakeUpSorce == 1)
		{
			LED_R=!LED_R;
		}
		else if(WakeUpSorce == 2)
		{
			LED_G_1=!LED_G_1;
		}
		else if(WakeUpSorce == 3)
		{
			LED_G_0=!LED_G_0;
			LED_G_1=!LED_G_1;
		}
		DelayXms(100);
		INTCnt++;
		if(INTCnt >=200)
		{
			LED_G_0=1;LED_G_1=1;LED_R=1;

			BakCKCON2=IAP_ReadPPage(CKCON2_P);							// 备份CKCON2 (使用了PLL CKM倍频的情况下)
			IAP_WritePPage(CKCON2_P,BakCKCON2&(~(MCKS0_P|MCKS1_P)));	// MCK=OSCin (使用了PLL CKM倍频的情况下)
			
			POW_SetMode_PD();											// 使MCU进入掉电模式
			
			_nop_();
			DelayXus(100);												// 延时，让CKM稳定 (使用了PLL CKM倍频的情况下)
			IAP_WritePPage(CKCON2_P,BakCKCON2); 						// 恢复CKCON2,选择使用倍频 (使用了PLL CKM倍频的情况下)

			INT_DisAll();							// 禁止全局中断	
			if(WakeUpSorce == 0)
			{
				LED_G_0=0;
			}
			else if(WakeUpSorce == 1)
			{
				LED_R=0;
			}
			else if(WakeUpSorce == 2)
			{
				LED_G_1=0;
			}
			else if(WakeUpSorce == 3)
			{
				LED_G_1=0;
				LED_G_0=0;
			}
			DelayXms(1000);
			LED_G_0=1;LED_G_1=1;LED_R=1;
			INT_EnAll();							// 使能全局中断	
			INTCnt=0;
		}

	}

}


