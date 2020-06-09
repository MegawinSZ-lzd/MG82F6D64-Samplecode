/*********************************************************************
    项目名称:MG82F6D64-DEMO
			测试板使用 MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=32MHz, SysCLK=32MHz
	功能描述：
			PCA时钟: 96MHz
			CH0(P22)/CH1(P33),CH2(P24)/CH3(P34),CH4(P26)/CH5(P35)
			3对互补,死区,中心对齐,分辨率为2048
			PWM频率: 96MHz/(2048*2)=23.437KHz
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

#define LED_G_0		P55
#define LED_R		P56
#define LED_G_1		P57



#define PWM_MIN			(0*2)
#define PWM_MAX			(1024*2)
#define PWM_3_4			(768*2)
#define PWM_2_4			(512*2)
#define PWM_1_4			(256*2)
#define PWM_LOW			(62*2)
#define PWM_HINT		(64*2)

#define PCA_RELOAD		(PWM_MAX)

#define PCA_C           (65536)       	

#define PCA_CL(x)		(u8)((65536-(x))%256) 
#define PCA_CH(x)     	(u8)((65536-(x))/256)          

idata WordTypeDef wDuty[3];
bit bDutyChange;

/***********************************************************************************
函数名称:   void INT_PCA(void)
功能描述:PCA 中断服务程序
		 
输入参数:   
返回参数:     
*************************************************************************************/
void INT_PCA(void) interrupt INT_VECTOR_PCA
{
	WordTypeDef duty;
	_push_(SFRPI);
	SFRPI=0;
	if(CF)
	{
		CF=0;
		LED_R=!LED_R;
		// Todo...
		if(bDutyChange==TRUE)
		{
			// 设置CH0/CH1重载值, 重载值在CH1上
			duty.W=PCA_C-wDuty[0].W;
			PCA_CH1_SetValue(duty.B.BHigh,duty.B.BLow);
			// 设置CH2/CH3重载值, 重载值在CH3上
			duty.W=PCA_C-wDuty[1].W;
			PCA_CH3_SetValue(duty.B.BHigh,duty.B.BLow);
			// 设置CH4/CH5重载值, 重载值在CH5上
			duty.W=PCA_C-wDuty[2].W;
			PCA_CH5_SetValue(duty.B.BHigh,duty.B.BLow);
			bDutyChange=FALSE;
		}
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
函数名称:   void InitPort(void)
功能描述:   IO配置
输入参数:   
返回参数:     
*************************************************************************************/
void InitPort(void)
{
	PORT_SetP5PushPull(BIT5|BIT6|BIT7);				// 设置P55,P56,P57为推挽输出(LED)
	PORT_SetP2PushPull(BIT2|BIT4|BIT6);				// 设置P22(CEX0),P24(CEX2),P26(CEX4)为推挽输出
	PORT_SetP3PushPull(BIT3|BIT4|BIT5);				// 设置P33(CEX1),P34(CEX3),P35(CEX5)为推挽输出
}


/***********************************************************************************
函数名称:   void InitPCA_PWM(void)
功能描述:   PWM初始化
		PCA时钟: 96MHz
		CH0(P22)/CH1(P33),CH2(P24)/CH3(P34),CH4(P17)/CH5(P35)
		3对互补,死区,中心对齐,分辨率为2048
		PWM频率: 96MHz/(2048*2)=23.437KHz
输入参数:   
返回参数:     
*************************************************************************************/
void InitPCA_PWM(void)
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
	PCA_SetCenterAligned_CFonTop();		// 中心对齐下CF在顶部置位

	PCA_SetDeadTimePreScaler_SysclkDiv8();	// 设置死区预分频 SysClk/8
	PCA_SetDeadTimePeriod(4);				// 设置死区时间 4*(1/PreScaler)=4*(1/(Sysclk/8))=32/Sysclk

	PCA_SetCounter(PCA_C-PCA_RELOAD);
	PCA_SetCounterReload(PCA_C-PCA_RELOAD);

	// 设置PWM占空比比较值
	PCA_CH0_SetValue(PCA_CH(PWM_MIN),PCA_CL(PWM_MIN));
	PCA_CH1_SetValue(PCA_CH(PWM_MIN),PCA_CL(PWM_MIN));
	PCA_CH2_SetValue(PCA_CH(PWM_MIN),PCA_CL(PWM_MIN));
	PCA_CH3_SetValue(PCA_CH(PWM_MIN),PCA_CL(PWM_MIN));
	PCA_CH4_SetValue(PCA_CH(PWM_MIN),PCA_CL(PWM_MIN));
	PCA_CH5_SetValue(PCA_CH(PWM_MIN),PCA_CL(PWM_MIN));

	// 禁止PWM输出
	PCA_SetPWM0_EnOutput();					
	PCA_SetPWM1_EnOutput();
	PCA_SetPWM2_EnOutput();
	PCA_SetPWM3_EnOutput();
	PCA_SetPWM4_EnOutput();
	PCA_SetPWM5_EnOutput();

	PCA_SetCEX0CEX2CEX4_P22P24P26();	// 设置CEX0:P22,CEX2:P24,CEX4:P26
	PCA_SetCEX1CEX3CEX5_P33P34P35();	// 设置CEX1:P33,CEX3:P34,CEX5:P35

	PCA_CF_EnInterrupt();				// 使能CF中断

	
	
	PCA_EnPCACounter();					// 使能PCA计数器, 
		
}



/***********************************************************************************
函数名称:   void InitInterrupt(void)
功能描述:   中断初始化
输入参数:   
返回参数:     
*************************************************************************************/
void InitInterrupt(void)
{
	INT_EnPCA();					// 使能PCA中断
	
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
	InitClock();
	InitPort();
	InitPCA_PWM();
	InitInterrupt();

	INT_EnAll();
}


void main()
{
	
    InitSystem();
    
	LED_G_0=0;LED_R=0;LED_G_1=0;
    DelayXms(1000);
	LED_G_0=1;LED_R=1;LED_G_1=1;

	wDuty[0].W=PWM_MIN;
	wDuty[1].W=PWM_2_4;
	wDuty[2].W=PWM_MAX;

	
	while(1)
    {
    	LED_G_0=!LED_G_0;
    	DelayXms(200);
		while(bDutyChange==TRUE);		//等待之前的数据更改完成
		if(wDuty[0].W>=PWM_MAX)
		{
			wDuty[0].W=PWM_MIN;
		}
		else
		{
			wDuty[0].W=wDuty[0].W+16;
		}
		if(wDuty[1].W>=PWM_MAX)
		{
			wDuty[1].W=PWM_MIN;
		}
		else
		{
			wDuty[1].W=wDuty[1].W+16;
		}
		if(wDuty[2].W>=PWM_MAX)
		{
			wDuty[2].W=PWM_MIN;
		}
		else
		{
			wDuty[2].W=wDuty[2].W+16;
		}
		bDutyChange=TRUE;

    }
}

