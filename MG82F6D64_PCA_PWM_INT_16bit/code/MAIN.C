/*********************************************************************
    项目名称:MG82F6D64-DEMO
			测试板使用 MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=12MHz, SysCLK=12MHz
	功能描述：
			CPUCLK=SYSCLK (32M)
			PCA时钟: SYSCLK
			CH0(P22),CH1(P33),CH2(P24),CH3(P34),CH4(P26),CH5(P35),PWM6(P62),PWM7(P63)
			在PCA中断里修改占空比
			边沿对齐,分辨率为1000
			PWM频率: PCAClk/(1000)=12MHz/1000=12KHz

			P10作为输入控制PWM输出，P10=1: 有输出， P10=0: 无输出
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

#define IO_TEST_0		P52
#define IO_TEST_1		P53
#define IO_TEST_2		P54
#define IO_TEST_3		P55
#define IO_TEST_4		P56
#define IO_TEST_5		P57

#define LED_G_0		P55
#define LED_R		P56
#define LED_G_1		P57


#define PWM_MIN			(0*1)
#define PWM_MAX			(1000*1)
#define PWM_3_4			(750*1)
#define PWM_2_4			(500*1)
#define PWM_1_4			(250*1)
#define PWM_LOW			(40*1)
#define PWM_HINT		(50*1)

#define PCA_RELOAD		(PWM_MAX)

#define PCA_C           (65536)       	

#define PCA_CL(x)		(u8)((65536-(x))%256) 
#define PCA_CH(x)     	(u8)((65536-(x))/256)          

idata WordTypeDef wDuty[8];
bit bDutyChange;
u8 DutyFlag;

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
		LED_R=!LED_R;
		CF=0;
		// Todo...
		// ......
		if(bDutyChange)
		{
			duty.W=PCA_C-wDuty[0].W;
			PCA_CH0_SetValue(duty.B.BHigh,duty.B.BLow);
			duty.W=PCA_C-wDuty[1].W;
			PCA_CH1_SetValue(duty.B.BHigh,duty.B.BLow);
			duty.W=PCA_C-wDuty[2].W;
			PCA_CH2_SetValue(duty.B.BHigh,duty.B.BLow);
			duty.W=PCA_C-wDuty[3].W;
			PCA_CH3_SetValue(duty.B.BHigh,duty.B.BLow);
			duty.W=PCA_C-wDuty[4].W;
			PCA_CH4_SetValue(duty.B.BHigh,duty.B.BLow);
			duty.W=PCA_C-wDuty[5].W;
			PCA_CH5_SetValue(duty.B.BHigh,duty.B.BLow);
			duty.W=PCA_C-wDuty[6].W;
			PCA_CH6_SetValue(duty.B.BHigh,duty.B.BLow);
			duty.W=PCA_C-wDuty[7].W;
			PCA_CH7_SetValue(duty.B.BHigh,duty.B.BLow);
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
	PORT_SetP5PushPull(BIT2|BIT3|BIT4|BIT5|BIT6|BIT7);				// 设置P55,P56,P57为推挽输出(LED)
	PORT_SetP2PushPull(BIT2|BIT4|BIT6);				// 设置P22(CEX0),P24(CEX2),P26(CEX4)为推挽输出
	PORT_SetP3PushPull(BIT3|BIT4|BIT5);				// 设置P33(CEX1),P34(CEX3),P35(CEX5)为推挽输出
	PORT_SetP6PushPull(BIT2|BIT3);					// 设置P62(PWM6),P63(PWM7)为推挽输出

	P10=1;
	PORT_SetP1OpenDrainPu(BIT0);					// 设置P10为开漏带上拉，用于控制输入
}


/***********************************************************************************
函数名称:   void InitPCA_PWM(void)
功能描述:   PCA初始化
		PCA时钟: 12MHz
		CH0(P22),CH1(P33),CH2(P24),CH3(P34),CH4(P26),CH5(P35),PWM6(P62),PWM7(P63)
		边沿对齐,分辨率为1000
		PWM频率: 12MHz/(1000)=12KHz
输入参数:   
返回参数:     
*************************************************************************************/
void InitPCA_PWM(void)
{
	PCA_SetCLOCK_SYSCLK();			// PCA时钟为SysClk = 12MHz
	
	PCA_CH0_SetMode_PWM();
	PCA_CH1_SetMode_PWM();
	PCA_CH2_SetMode_PWM();
	PCA_CH3_SetMode_PWM();
	PCA_CH4_SetMode_PWM();
	PCA_CH5_SetMode_PWM();
	PCA_CH6_SetMode_PWM();
	PCA_CH7_SetMode_PWM();

	PCA_CH0_SetPWM_16Bit();
	PCA_CH1_SetPWM_16Bit();
	PCA_CH2_SetPWM_16Bit();
	PCA_CH3_SetPWM_16Bit();
	PCA_CH4_SetPWM_16Bit();
	PCA_CH5_SetPWM_16Bit();
	PCA_CH6_SetPWM_16Bit();
	PCA_CH7_SetPWM_16Bit();


	PCA_SetPWM_EdgeAligned();			// 边沿对齐

	PCA_SetCounter(PCA_C-PCA_RELOAD);
	PCA_SetCounterReload(PCA_C-PCA_RELOAD);

	// 设置PWM占空比比较值
	PCA_CH0_SetValue(PCA_CH(PWM_MIN),PCA_CL(PWM_MIN));
	PCA_CH1_SetValue(PCA_CH(PWM_MIN),PCA_CL(PWM_MIN));
	PCA_CH2_SetValue(PCA_CH(PWM_MIN),PCA_CL(PWM_MIN));
	PCA_CH3_SetValue(PCA_CH(PWM_MIN),PCA_CL(PWM_MIN));
	PCA_CH4_SetValue(PCA_CH(PWM_MIN),PCA_CL(PWM_MIN));
	PCA_CH5_SetValue(PCA_CH(PWM_MIN),PCA_CL(PWM_MIN));
	PCA_CH6_SetValue(PCA_CH(PWM_MIN),PCA_CL(PWM_MIN));
	PCA_CH7_SetValue(PCA_CH(PWM_MIN),PCA_CL(PWM_MIN));

	// 禁止PWM输出
	PCA_SetPWM0_DisOutput();					
	PCA_SetPWM1_DisOutput();
	PCA_SetPWM2_DisOutput();					
	PCA_SetPWM3_DisOutput();
	PCA_SetPWM4_DisOutput();					
	PCA_SetPWM5_DisOutput();
	PCA_SetPWM6_DisOutput();					
	PCA_SetPWM7_DisOutput();

	PCA_SetPOEn_PWMCycle();				// 禁止/使能PWM输出功能对齐到PWM周期

	PCA_SetCEX0CEX2CEX4_P22P24P26();	// 设置CEX0:P22,CEX2:P24,CEX4:P26
	PCA_SetCEX1CEX3CEX5_P33P34P35();	// 设置CEX1:P33,CEX3:P34,CEX5:P35
	PCA_SetPWM6PWM7_P62P63();			// 设置PWM6:P62,PWM7:P63

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

}


void main()
{
	u8 i,x,cnt;
	bit bOutFlag;
    InitSystem();

	LED_G_0=0;LED_R=0;LED_G_0=0;
	DelayXms(1000);
	LED_G_0=1;LED_R=1;LED_G_1=1;

	INT_EnAll();
	

	wDuty[0].W=PWM_MIN;
	wDuty[1].W=PWM_LOW;
	wDuty[2].W=PWM_1_4;
	wDuty[3].W=PWM_2_4;
	wDuty[4].W=PWM_3_4;
	wDuty[5].W=PWM_2_4;
	wDuty[6].W=PWM_1_4;
	wDuty[7].W=PWM_MAX;
	
	DutyFlag=0x00;
	bOutFlag=FALSE;
	
	while(1)
    {
    	DelayXms(1);
    	cnt++;
    	if(cnt>=200)
    	{
			cnt=0;
	    	LED_G_1=!LED_G_1;
	    	if(bOutFlag==TRUE)
	    	{
				while(bDutyChange==TRUE);		//等待之前的数据更改完成
		    	x=0x01;
		    	for(i=0;i<8;i++)
		    	{
					if((DutyFlag&x)==0)
					{
						wDuty[i].W=wDuty[i].W+20;
						if(wDuty[i].W >= PWM_MAX)
						{
							wDuty[i].W = PWM_MAX;
							DutyFlag=DutyFlag|x;
						}
					}
					else
					{
						if(wDuty[i].W < 21)
						{
							wDuty[i].W = PWM_MIN;
							DutyFlag=DutyFlag&(~x);
						}
						else
						{
							wDuty[i].W=wDuty[i].W-20;
						}
					}
					x=x<<1;
		    	}
		    	bDutyChange=TRUE;
	    	}
    	}
    	if(P10==1)
    	{
			if(bOutFlag==FALSE)
			{
				bOutFlag=TRUE;
				IO_TEST_1=0;
				// 使能PWM输出
				PCA_SetPWM0_EnOutput(); 				
				PCA_SetPWM1_EnOutput();
				PCA_SetPWM2_EnOutput(); 				
				PCA_SetPWM3_EnOutput();
				PCA_SetPWM4_EnOutput(); 				
				PCA_SetPWM5_EnOutput();
				PCA_SetPWM6_EnOutput(); 				
				PCA_SetPWM7_EnOutput();
			}
    	}
    	else
    	{
			if(bOutFlag==TRUE)
			{
				bOutFlag=FALSE;
				IO_TEST_1=1;
				// 禁止PWM输出
				PCA_SetPWM0_DisOutput();					
				PCA_SetPWM1_DisOutput();
				PCA_SetPWM2_DisOutput();					
				PCA_SetPWM3_DisOutput();
				PCA_SetPWM4_DisOutput();					
				PCA_SetPWM5_DisOutput();
				PCA_SetPWM6_DisOutput();					
				PCA_SetPWM7_DisOutput();
			}
    	}
    }
}

