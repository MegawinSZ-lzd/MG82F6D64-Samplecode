#ifndef _MACRO_MG82F6D64_H
#define _MACRO_MG82F6D64_H

#define SFR_SetPage(x)		SFRPI=x

/*
*************************************************************************************
*  INTERRUPT 中断 
*
*************************************************************************************
*/
//中断向量
#define INT_VECTOR_INT0		0
#define INT_VECTOR_T0		1
#define INT_VECTOR_INT1		2
#define INT_VECTOR_T1		3
#define INT_VECTOR_UART0	4
#define INT_VECTOR_T2		5
#define INT_VECTOR_INT2		6
#define INT_VECTOR_SPI		7
#define INT_VECTOR_ADC		8
#define INT_VECTOR_PCA		9
#define INT_VECTOR_SF		10
#define INT_VECTOR_KB		11
#define INT_VECTOR_TWI0		12
#define INT_VECTOR_AC0		13
#define INT_VECTOR_UART1	14
#define INT_VECTOR_INT3		15
#define INT_VECTOR_T3		16
#define INT_VECTOR_AC1		17
#define INT_VECTOR_DMA		18
#define	INT_VECTOR_AC2		19
#define INT_VECTOR_UART2	20
#define	INT_VECTOR_T4		21
#define	INT_VECTOR_UART3	22
#define	INT_VECTOR_TWI1		23

//中断使能/禁止
#define INT_EnAll()			EA=1		// 使能全局中断
#define INT_DisAll()		EA=0		// 禁止全局中断
#define INT_EnINT0()		EX0=1
#define INT_DisINT0()		EX0=0
#define INT_EnTIMER0()		ET0=1
#define INT_DisTIMER0()		ET0=0
#define INT_EnINT1()		EX1=1
#define INT_DisINT1()		EX1=0
#define INT_EnTIMER1()		ET1=1
#define INT_DisTIMER1()		ET1=0
#define INT_EnUART0()		ES0=1
#define INT_DisUART0()		ES0=0
#define INT_EnTIMER2()		ET2=1
#define INT_EnTIMER2L()		TCLK_TL2IE=1
#define INT_DisTIMER2L()	TCLK_TL2IE=0
#define INT_DisTIMER2()		ET2=0
#define INT_EnINT2()		EX2=1
#define INT_DisINT2()		EX2=0
#define INT_EnSPI()			EIE1=EIE1|ESPI
#define INT_DisSPI()		EIE1=EIE1&(~ESPI)
#define INT_EnADC()			EIE1=EIE1|EADC
#define INT_DisADC()		EIE1=EIE1&(~EADC)
#define INT_EnPCA()			EIE1=EIE1|EPCA
#define INT_DisPCA()		EIE1=EIE1&(~EPCA)
#define INT_EnSF()			EIE1=EIE1|ESF
#define INT_DisSF()			EIE1=EIE1&(~ESF)
#define INT_EnKBI()			EIE1=EIE1|EKB
#define INT_DisKBI()		EIE1=EIE1&(~EKB)
#define INT_EnTWI0()		EIE1=EIE1|ETWI0
#define INT_DisTWI0()		EIE1=EIE1&(~ETWI0)
#define INT_EnAC0()			EIE1=EIE1|EAC0
#define	INT_DisAC0()		EIE1=EIE1&(~EAC0)
#define INT_EnUART1()		EIE1=EIE1|ES1
#define INT_DisUART1()		EIE1=EIE1&(~ES1)
#define INT_EnINT3()		EX3=1
#define INT_DisINT3()		EX3=0
#define INT_EnTIMER3()		EIE2=EIE2|ET3
#define INT_DisTIMER3()		EIE2=EIE2&(~ET3)
#define INT_EnTIMER3L()		SFRPI=1;TL3IE=1;SFRPI=0
#define INT_DisTIMER3L()	SFRPI=1;TL3IE=0;SFRPI=0
#define INT_EnAC1()			EIE2=EIE2|EAC1
#define	INT_DisAC1()		EIE2=EIE2&(~EAC1)
#define INT_EnDMA()			EDMA=1
#define INT_DisDMA()		EDMA=0
#define INT_EnAC2()			EIE2=EIE2|EAC2
#define	INT_DisAC2()		EIE2=EIE2&(~EAC2)
#define INT_EnUART2()		EIE2=EIE2|ES2
#define INT_DisUART2()		EIE2=EIE2&(~ES2)
#define INT_EnTIMER4()		EIE2=EIE2|ET4
#define INT_DisTIMER4()		EIE2=EIE2&(~ET4)
#define INT_EnTIMER4L()		SFRPI=2;TL4IE=1;SFRPI=0
#define INT_DisTIMER4L()	SFRPI=2;TL4IE=0;SFRPI=0
#define INT_EnUART3()		EIE2=EIE2|ES3
#define INT_DisUART3()		EIE2=EIE2&(~ES3)
#define INT_EnTWI1()		EIE2=EIE2|ETWI1
#define INT_DisTWI1()		EIE2=EIE2&(~ETWI1)

#define INT_EnSF_WDT()		SFIE=SFIE|WDTFIE
#define INT_DisSF_WDT()		SFIE=SFIE&(~WDTFIE)
#define INT_EnSF_BOD0()		SFIE=SFIE|BOF0IE
#define INT_DisSF_BOD0()	SFIE=SFIE&(~BOF0IE)
#define INT_EnSF_BOD1()		SFIE=SFIE|BOF1IE
#define INT_DisSF_BOD1()	SFIE=SFIE&(~BOF1IE)
#define INT_EnSF_RTC()		SFIE=SFIE|RTCFIE
#define INT_DisSF_RTC()		SFIE=SFIE&(~RTCFIE)
#define INT_EnSF_MCD()		SFIE=SFIE|MCDFIE
#define INT_DisSF_MCD()		SFIE=SFIE&(~MCDFIE)
#define	INT_EnSF_UART0()	SFRPI=0;S0CFG=S0CFG|UTIE;SFRPI=0
#define	INT_DisSF_UART0()	SFRPI=0;S0CFG=S0CFG&(~UTIE);SFRPI=0
#define INT_EnSF_SID()		SFIE=SFIE|SIDFIE
#define INT_DisSF_SID()		SFIE=SFIE&(~SIDFIE)
#define INT_EnSF_OBM0()		SFRPI=2;BM0CR=BM0CR|BM0IE;SFRPI=0
#define INT_DisSF_OBM0()	SFRPI=2;BM0CR=BM0CR&(~BM0IE);SFRPI=0
#define INT_EnDF_OBM1()		SFRPI=6;BM1CR=BM1CR|BM1IE;SFRPI=0
#define INT_DisSF_OBM1()	SFRPI=6;BM1CR=BM1CR&(~BM1IE);SFRPI=0

// 中断优先级
#define IP_INT0		BIT0
#define IP_T0		BIT1
#define IP_INT1		BIT2
#define IP_T1		BIT3
#define IP_UART0	BIT4
#define IP_T2		BIT5
#define IP_INT2		BIT6
#define IP_INT3		BIT7
#define INT_SetIPHighest(x)			IP0H=IP0H|x;IP0L=IP0L|x
#define INT_SetIPHigh(x)			IP0H=IP0H|x;IP0L=IP0L&(~x)
#define INT_SetIPLow(x)				IP0H=IP0H&(~x);IP0L=IP0L|x
#define INT_SetIPLowest(x)			IP0H=IP0H&(~x);IP0L=IP0L&(~x)

#define IPX_SPI		BIT0
#define IPX_ADC		BIT1
#define IPX_PCA		BIT2
#define IPX_SF		BIT3
#define IPX_KB		BIT5
#define IPX_TWI0	BIT6
#define IPX_AC0		BIT7
#define INT_SetIPXHighest(x)		EIP1H=EIP1H|x;EIP1L=EIP1L|x
#define INT_SetIPXHigh(x)			EIP1H=EIP1H|x;EIP1L=EIP1L&(~x)
#define INT_SetIPXLow(x)			EIP1H=EIP1H&(~x);EIP1L=EIP1L|x
#define INT_SetIPXLowest(x)			EIP1H=EIP1H&(~x);EIP1L=EIP1L&(~x)

#define IPX2_TIMER3		BIT0
#define IPX2_AC1		BIT1
#define IPX2_UART2		BIT2
#define IPX2_TIMER4		BIT3
#define IPX2_UART3		BIT4
#define IPX2_TWI1		BIT6
#define IPX2_AC2		BIT7
#define INT_SetIPX2Highest(x)		EIP2H=EIP2H|x;EIP2L=EIP2L|x
#define INT_SetIPX2High(x)			EIP2H=EIP2H|x;EIP2L=EIP2L&(~x)
#define INT_SetIPX2Low(x)			EIP2H=EIP2H&(~x);EIP2L=EIP2L|x
#define INT_SetIPX2Lowest(x)		EIP2H=EIP2H&(~x);EIP2L=EIP2L&(~x)

#define INT_SetDMAIPHighest(x)		SFRPI=8;DMACG0=DMACG0|(PDMAH|PDMAL);SFRPI=0
#define INT_SetDMAIPHigh(x)			SFRPI=8;DMACG0=(DMACG0&(~(PDMAH|PDMAL)))|(PDMAH);SFRPI=0
#define INT_SetDMAIPLow(x)			SFRPI=8;DMACG0=(DMACG0&(~(PDMAH|PDMAL)))|(PDMAL);SFRPI=0
#define INT_SetDMAIPLowest(x)		SFRPI=8;DMACG0=DMACG0&(~(PDMAH|PDMAL));SFRPI=0

// 外部中断 选择中断引脚
#define INT_SetINT0P32()			XICFG=XICFG&(~(INT0IS0|INT0IS1));SFRPI =1;XICFG1=XICFG1&(~INT0IS2);SFRPI=0
#define INT_SetINT0P30()			XICFG=(XICFG&(~INT0IS1))|INT0IS0;SFRPI =1;XICFG1=XICFG1&(~INT0IS2);SFRPI=0
#define INT_SetINT0P34()			XICFG=(XICFG&(~INT0IS0))|INT0IS1;SFRPI =1;XICFG1=XICFG1&(~INT0IS2);SFRPI=0
#define INT_SetINT0P40()			XICFG=XICFG|(INT0IS0|INT0IS1);SFRPI =1;XICFG1=XICFG1&(~INT0IS2);SFRPI=0
#define INT_SetINT0P10()			XICFG=XICFG&(~(INT0IS0|INT0IS1));SFRPI =1;XICFG1=XICFG1|INT0IS2;SFRPI=0
#define INT_SetINT0P14()			XICFG=(XICFG&(~INT0IS1))|INT0IS0;SFRPI =1;XICFG1=XICFG1|INT0IS2;SFRPI=0
#define INT_SetINT0P36()			XICFG=(XICFG&(~INT0IS0))|INT0IS1;SFRPI =1;XICFG1=XICFG1|INT0IS2;SFRPI=0
#define INT_SetINT0P05()			XICFG=XICFG|(INT0IS0|INT0IS1);SFRPI =1;XICFG1=XICFG1|INT0IS2;SFRPI=0

#define INT_SetINT1P33()			XICFG=XICFG&(~(INT1IS0|INT1IS1));SFRPI =1;XICFG1=XICFG1&(~INT1IS2);SFRPI=0
#define INT_SetINT1P31()			XICFG=(XICFG&(~INT1IS1))|INT1IS0;SFRPI =1;XICFG1=XICFG1&(~INT1IS2);SFRPI=0
#define INT_SetINT1P35()			XICFG=(XICFG&(~INT1IS0))|INT1IS1;SFRPI =1;XICFG1=XICFG1&(~INT1IS2);SFRPI=0
#define INT_SetINT1P41()			XICFG=XICFG|(INT1IS0|INT1IS1);SFRPI =1;XICFG1=XICFG1&(~INT1IS2);SFRPI=0
#define INT_SetINT1P45()			XICFG=XICFG&(~(INT1IS0|INT1IS1));SFRPI =1;XICFG1=XICFG1|INT1IS2;SFRPI=0
#define INT_SetINT1P15()			XICFG=(XICFG&(~INT1IS1))|INT1IS0;SFRPI =1;XICFG1=XICFG1|INT1IS2;SFRPI=0
#define INT_SetINT1P37()			XICFG=(XICFG&(~INT1IS0))|INT1IS1;SFRPI =1;XICFG1=XICFG1|INT1IS2;SFRPI=0
#define INT_SetINT1P07()			XICFG=XICFG|(INT1IS0|INT1IS1);SFRPI =1;XICFG1=XICFG1|INT1IS2;SFRPI=0

#define INT_SetINT2P43()			SFRPI=1;XICFG1=XICFG1&(~(INT2IS0|INT2IS1));SFRPI=0
#define INT_SetINT2P20()			SFRPI=1;XICFG1=(XICFG1&(~INT2IS1))|INT2IS0;SFRPI=0
#define INT_SetINT2P11()			SFRPI=1;XICFG1=(XICFG1&(~INT2IS0))|INT2IS1;SFRPI=0
#define INT_SetINT2P25()			SFRPI=1;XICFG1=XICFG1|(INT2IS0|INT2IS1);SFRPI=0
#define INT_SetINT2P44()			SFRPI=1;XICFG1=XICFG1&(~(INT2IS0|INT2IS1));SFRPI=2;XICFG2=XICFG2|INT2IS2;SFRPI=0
#define INT_SetINT2P60()			SFRPI=1;XICFG1=(XICFG1&(~INT2IS1))|INT2IS0;SFRPI=2;XICFG2=XICFG2|INT2IS2;SFRPI=0
#define INT_SetINT2P01()			SFRPI=1;XICFG1=(XICFG1&(~INT2IS0))|INT2IS1;SFRPI=2;XICFG2=XICFG2|INT2IS2;SFRPI=0
#define INT_SetINT2P62()			SFRPI=1;XICFG1=XICFG1|(INT2IS0|INT2IS1);SFRPI=2;XICFG2=XICFG2|INT2IS2;SFRPI=0

#define INT_SetINT3P42()			SFRPI=2;XICFG2=XICFG2&(~(INT3IS0|INT3IS1|INT3IS2));SFRPI=0
#define INT_SetINT3P21()			SFRPI=2;XICFG2=(XICFG2&(~(INT3IS0|INT3IS1|INT3IS2)))|(INT3IS0);SFRPI=0
#define INT_SetINT3P00()			SFRPI=2;XICFG2=(XICFG2&(~(INT3IS0|INT3IS1|INT3IS2)))|(INT3IS1);SFRPI=0
#define INT_SetINT3P16()			SFRPI=2;XICFG2=(XICFG2&(~(INT3IS0|INT3IS1|INT3IS2)))|(INT3IS0|INT3IS1);SFRPI=0
#define INT_SetINT3P02()			SFRPI=2;XICFG2=(XICFG2&(~(INT3IS0|INT3IS1|INT3IS2)))|(INT3IS2);SFRPI=0
#define INT_SetINT3P61()			SFRPI=2;XICFG2=(XICFG2&(~(INT3IS0|INT3IS1|INT3IS2)))|(INT3IS0|INT3IS2);SFRPI=0
#define INT_SetINT3P06()			SFRPI=2;XICFG2=(XICFG2&(~(INT3IS0|INT3IS1|INT3IS2)))|(INT3IS1|INT3IS2);SFRPI=0
#define INT_SetINT3P63()			SFRPI=2;XICFG2=(XICFG2&(~(INT3IS0|INT3IS1|INT3IS2)))|(INT3IS0|INT3IS1|INT3IS2);SFRPI=0

// 外部中断 设置检测电平/边沿
#define INT_SetINT0_DetectLevel()		IT0=0
#define	INT_SetINT0_DetectEdge()		IT0=1

#define INT_SetINT1_DetectLevel()		IT1=0
#define	INT_SetINT1_DetectEdge()		IT1=1

#define INT_SetINT2_DetectLevel()		IT2=0
#define	INT_SetINT2_DetectEdge()		IT2=1

#define INT_SetINT3_DetectLevel()		IT3=0
#define	INT_SetINT3_DetectEdge()		IT3=1

// 外部中断 详细判断是哪一沿/电平
#define INT_SetINT0_DetectLowFalling()	AUXR0=AUXR0&(~INT0H)
#define INT_SetINT0_DetectHighRising()	AUXR0=AUXR0|INT0H

#define INT_SetINT1_DetectLowFalling()	AUXR0=AUXR0&(~INT1H)
#define INT_SetINT1_DetectHighRising()	AUXR0=AUXR0|INT1H

#define INT_SetINT2_DetectLowFalling()	INT2H=0
#define INT_SetINT2_DetectHighRising()	INT2H=1

#define INT_SetINT3_DetectLowFalling()	INT3H=0
#define INT_SetINT3_DetectHighRising()	INT3H=1


// 外部中断 设置滤波
#define INT_SetINT0Filter_Disable()				XICFG=XICFG&(~X0FLT);SFRPI=1;XICFG1=XICFG1&(~X0FLT1);SFRPI=0
#define INT_SetINT0Filter_Sysclk_x3()			XICFG=XICFG|X0FLT;SFRPI=1;XICFG1=XICFG1&(~X0FLT1);SFRPI=0
#define INT_SetINT0Filter_SysclkDiv6_x3()		XICFG=XICFG&(~X0FLT);SFRPI=1;XICFG1=XICFG1|X0FLT1;SFRPI=0
#define INT_SetINT0Filter_S0TOF_x3()			XICFG=XICFG|X0FLT;SFRPI=1;XICFG1=XICFG1|X0FLT1;SFRPI=0

#define INT_SetINT1Filter_Disable()				XICFG=XICFG&(~X1FLT);SFRPI=1;XICFG1=XICFG1&(~X1FLT1);SFRPI=0
#define INT_SetINT1Filter_Sysclk_x3()			XICFG=XICFG|X1FLT;SFRPI=1;XICFG1=XICFG1&(~X1FLT1);SFRPI=0
#define INT_SetINT1Filter_SysclkDiv6_x3()		XICFG=XICFG&(~X1FLT);SFRPI=1;XICFG1=XICFG1|X1FLT1;SFRPI=0
#define INT_SetINT1Filter_S0TOF_x3()			XICFG=XICFG|X1FLT;SFRPI=1;XICFG1=XICFG1|X1FLT1;SFRPI=0

#define INT_SetINT2Filter_Disable()				XICFG=XICFG&(~X2FLT);SFRPI=1;XICFG1=XICFG1&(~X2FLT1);SFRPI=0
#define INT_SetINT2Filter_Sysclk_x3()			XICFG=XICFG|X2FLT;SFRPI=1;XICFG1=XICFG1&(~X2FLT1);SFRPI=0
#define INT_SetINT2Filter_SysclkDiv6_x3()		XICFG=XICFG&(~X2FLT);SFRPI=1;XICFG1=XICFG1|X2FLT1;SFRPI=0
#define INT_SetINT2Filter_S0TOF_x3()			XICFG=XICFG|X2FLT;SFRPI=1;XICFG1=XICFG1|X2FLT1;SFRPI=0

#define INT_SetINT3Filter_Disable()				XICFG=XICFG&(~X3FLT);SFRPI=1;XICFG1=XICFG1&(~X3FLT1);SFRPI=0
#define INT_SetINT3Filter_Sysclk_x3()			XICFG=XICFG|X3FLT;SFRPI=1;XICFG1=XICFG1&(~X3FLT1);SFRPI=0
#define INT_SetINT3Filter_SysclkDiv6_x3()		XICFG=XICFG&(~X3FLT);SFRPI=1;XICFG1=XICFG1|X3FLT1;SFRPI=0
#define INT_SetINT3Filter_S0TOF_x3()			XICFG=XICFG|X3FLT;SFRPI=1;XICFG1=XICFG1|X3FLT1;SFRPI=0

/*
*************************************************************************************
*/ 



/*
*************************************************************************************
*  IAP 
*
*************************************************************************************
*/
//IAP 命令
#define ISP_STANBY		0
#define ISP_READ		1 	  //读
#define ISP_WRITE 	    2 	  //写
#define ISP_ERASE 	    3 	  //擦除
#define ISP_WRITE_P     4     //写P页寄存器
#define ISP_READ_P     	5     //读P页寄存器
#define ISP_AUTO_FLASH_CRC	0x80	// 自动计算FLASH CRC
#define ISP_READ_BYTE_INC	0x81	// FLASH读字节,地址递增
#define ISP_WRITE_BYTE_INC	0x82	// FLASH写字节,地址递增

/*
*************************************************************************************
*/


/*
*************************************************************************************
*  CLOCK 
*
*************************************************************************************
*/
// CLOCK IHRCO 是12MHz/11.0592MHz
#define IHRCO_12MHz					0
#define IHRCO_110592MHz				0x80
#define CLK_SelIHRCO12MHz()					CKCON0=CKCON0&(~AFS)				
#define CLK_SelIHRCO110592MHz()				CKCON0=CKCON0|(AFS)	

// CLOCK 设置CPUCLK 
#define CPUCLK_SYSCLK_DIV_1			0
#define CPUCLK_SYSCLK_DIV_2			0x08
#define CLK_SetCpuclkUseSysclk()			CKCON0=CKCON0&(~CCKS)				
#define CLK_SetCpuclkUseSysclkDiv2()		CKCON0=CKCON0|(CCKS)	

// CLOCK 设置SYSCLK
#define SYSCLK_MCKDO_DIV_1			0
#define SYSCLK_MCKDO_DIV_2			1
#define SYSCLK_MCKDO_DIV_4			2
#define SYSCLK_MCKDO_DIV_8			3
#define SYSCLK_MCKDO_DIV_16			4
#define SYSCLK_MCKDO_DIV_32			5
#define SYSCLK_MCKDO_DIV_64			6
#define SYSCLK_MCKDO_DIV_128		7
#define CLK_SetSysclk(x)					CKCON0=(CKCON0&(~0x07))|x

// CLOCK 开启/关闭CKM
#define DISABLE_CKM					0
#define ENABLE_CKM					0x40
#define CLK_DisCKM()						CKCON0=CKCON0&(~ENCKM)
#define CLK_EnCKM()							CKCON0=CKCON0|(ENCKM)

// CLOCK 选择CKM输入源
#define CKM_OSCIN_DIV_1				0x00	// OSCin=5~7MHz
#define CKM_OSCIN_DIV_2				0x10	// OSCin=10~14MHz	
#define CKM_OSCIN_DIV_4				0x20	// OSCin=20~28MHz
#define CKM_OSCIN_DIV_6				0x30	// OSCin=30~42MHz
#define CLK_SetCKMOSCin(x)					CKCON0=(CKCON0&(~0x30))|x

// CLOCK 设置CKCON0
#define CLK_SetCKCON0(x)					CKCON0=x

// CLOCK 设置CKCON2

#define DISABLE_IHRCO				0x00
#define ENABLE_IHRCO				0x10

#define ENABLE_XTAL					0x20
#define DISABLE_XTAL				0x00

#define	GAIN_FOR_32K				0x00	// for 32.768KHz
#define GAIN_FOR_2M_25M				0x40	// for 2~25MHz
#define	GAIN_FOR_LOW				0x80    // for 32.768KHz

#define MCK_OSCin					0x00
#define MCK_CKMI_X4					0x04
#define MCK_CKMI_X533				0x08
#define MCK_CKMI_X8					0x0C
#define MCK_CKMI_X4_X6				0x04
#define MCK_CKMI_X533_X8			0x08
#define MCK_CKMI_X8_x12				0x0C

#define OSCIn_IHRCO					0x00
#define OSCIn_XTAL					0x01
#define OSCIn_ILRCO					0x02
#define OSCIn_ECKI_P60				0x03

#define CLK_SetCKCON2(x)			ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON2_P;IFMT=ISP_WRITE_P;IFD=x;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0

// CLOCK 设置CKCON3
#define WDTCS_ILRCO					0x00
#define WDTCS_XTAL2_ECKI			0x40
#define WDTCS_SYSCLK_DIV_12			0x80
#define WDTCS_S0TOF					0xC0

#define FWKP_NORMAL_120uS			0x00
#define FWKP_FAST_30uS				0x20

#define MCKDO_MCK_DIV_1				0x00
#define MCKDO_MCK_DIV_2				0x04
#define MCKDO_MCK_DIV_4				0x08
#define MCKDO_MCK_DIV_8				0x0C

#define CLK_SetCKCON3(x)			ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON3_P;IFMT=ISP_WRITE_P;IFD=x;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0

// CLOCK 设置倍频模式x4x533x8/x6x8x12
#define CLK_SetCKM_x4x533x8()		ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON5_P;IFMT=ISP_WRITE_P;IFD=0x00;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0
#define CLK_SetCKM_x6x8x12()		ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON5_P;IFMT=ISP_WRITE_P;IFD=0x01;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0

// CLOCK 设置CPU超高速运行(>25MHz)
#define CLK_SeCpuCLK_HighSpeed()	ISPCR=0x80;IFADRH=0x0;IFADRL=DCON0_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=IFD|(HSE_P|HSE1_P);IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0

// CLOCK 设置IAP时基 
#define CLK_SetIAPTimeBase(x)		CKCON1=x-1

// CLOCK 设置P60输出CLK
#define CLK_P60OC_P60()			AUXR0=AUXR0&(~(P60OC0|P60OC1))
#define CLK_P60OC_MCK()			AUXR0=(AUXR0&(~(P60OC0|P60OC1)))|(P60OC0)
#define CLK_P60OC_MCKDiv2()		AUXR0=(AUXR0&(~(P60OC0|P60OC1)))|(P60OC1)
#define CLK_P60OC_MCKDiv4()		AUXR0=AUXR0|((P60OC0|P60OC1))

// CLOCK 设置P60输出CLK快速驱动
#define Pin_P60EnFastDrive()	AUXR0=AUXR0|P60FD
#define	Pin_P60DisFastDrive()	AUXR0=AUXR0&~P60FD;
/*
*************************************************************************************
*/ 

/*
*************************************************************************************
*  TIMER0 & TIMER1 
*
*************************************************************************************
*/
// 使能或关闭TIMER
#define TM_DisableT0()			TR0=0					
#define TM_EnableT0()			TR0=1
#define TM_DisableT1()			TR1=0					
#define TM_EnableT1()			TR1=1

// 选择TIMER工作模式
#define TM_SetT0Mode_0_8BIT_PWM()				TMOD=(TMOD&(~(T0M0|T0M1)))
#define TM_SetT0Mode_1_16BIT_TIMER()			TMOD=(TMOD&(~(T0M0|T0M1)))|(T0M0)
#define TM_SetT0Mode_2_8BIT_AUTORELOAD()		TMOD=(TMOD&(~(T0M0|T0M1)))|(T0M1)
#define TM_SetT0Mode_3_TWO_8BIT()				TMOD=(TMOD|(T0M0|T0M1))


#define TM_SetT0Gate_Disable()					TMOD=TMOD&(~T0GATE);SFRPI=6;AUXR9=AUXR9&(~T0G1);SFRPI=0
#define TM_SetT0Gate_INT0()						TMOD=TMOD|T0GATE;SFRPI=6;AUXR9=AUXR9&(~T0G1);SFRPI=0
#define TM_SetT0Gate_TF2()						TMOD=TMOD&(~T0GATE);SFRPI=6;AUXR9=AUXR9|(T0G1);SFRPI=0
#define TM_SetT0Gate_KBI()						TMOD=TMOD|T0GATE;SFRPI=6;AUXR9=AUXR9|(T0G1);SFRPI=0

#define TM_SetT1Mode_0_8BIT_PWM()				TMOD=(TMOD&(~(T1M0|T1M1)))          
#define TM_SetT1Mode_1_16BIT_TIMER()			TMOD=(TMOD&(~(T1M0|T1M1)))|(T1M0)    
#define TM_SetT1Mode_2_8BIT_AUTORELOAD()		TMOD=(TMOD&(~(T1M0|T1M1)))|(T1M1)    
#define TM_SetT1Mode_3_Disable()				TMOD=(TMOD|(T1M0|T1M1))             

#define TM_SetT1Gate_Disable()					TMOD=TMOD&(~T1GATE);SFRPI=6;AUXR9=AUXR9&(~T1G1);SFRPI=0
#define TM_SetT1Gate_INT1()						TMOD=TMOD|T1GATE;SFRPI=6;AUXR9=AUXR9&(~T1G1);SFRPI=0
#define TM_SetT1Gate_TF3()						TMOD=TMOD&(~T1GATE);SFRPI=6;AUXR9=AUXR9|(T1G1);SFRPI=0
#define TM_SetT1Gate_TI1()						TMOD=TMOD|T1GATE;SFRPI=6;AUXR9=AUXR9|(T1G1);SFRPI=0

// 选择TIMER时钟源
#define TM_SetT0Clock_SYSCLKDiv12()				AUXR3=(AUXR3&(~T0XL));AUXR2=(AUXR2&(~T0X12));TMOD=(TMOD&(~T0C_T))
#define TM_SetT0Clock_T0Port()					AUXR3=(AUXR3&(~T0XL));AUXR2=(AUXR2&(~T0X12));TMOD=(TMOD|(T0C_T))
#define TM_SetT0Clock_SYSCLK()					AUXR3=(AUXR3&(~T0XL));AUXR2=(AUXR2|(T0X12));TMOD=(TMOD&(~T0C_T))
#define TM_SetT0Clock_ILRCO()					AUXR3=(AUXR3&(~T0XL));AUXR2=(AUXR2|(T0X12));TMOD=(TMOD|(T0C_T))
#define TM_SetT0Clock_SYSCLKDiv48()				AUXR3=(AUXR3|(T0XL));AUXR2=(AUXR2&(~T0X12));TMOD=(TMOD&(~T0C_T))
#define TM_SetT0Clock_WDTPS()					AUXR3=(AUXR3|(T0XL));AUXR2=(AUXR2&(~T0X12));TMOD=(TMOD|(T0C_T))
#define TM_SetT0Clock_SYSCLKDiv192()			AUXR3=(AUXR3|(T0XL));AUXR2=(AUXR2|(T0X12));TMOD=(TMOD&(~T0C_T))
#define TM_SetT0Clock_T1OF()					AUXR3=(AUXR3|(T0XL));AUXR2=(AUXR2|(T0X12));TMOD=(TMOD|(T0C_T))

#define TM_SetT1Clock_SYSCLKDiv12()				AUXR2=(AUXR2&(~T1X12));TMOD=(TMOD&(~T1C_T))
#define TM_SetT1Clock_T1Port()					AUXR2=(AUXR2&(~T1X12));TMOD=(TMOD|(T1C_T))
#define TM_SetT1Clock_SYSCLK()					AUXR2=(AUXR2|(T1X12));TMOD=(TMOD&(~T1C_T))
#define TM_SetT1Clock_SYSCLKDiv48()				AUXR2=(AUXR2|(T1X12));TMOD=(TMOD|(T1C_T))

// 设置TIMER值
#define TM_SetT0HighByte(x)			TH0=x
#define TM_SetT0LowByte(x)			TL0=x

#define TM_SetT1LowByte(x)			TL1=x
#define TM_SetT1HighByte(x)			TH1=x


// 设置TIMER输出引脚
#define TM_SetT0CKO_P34()			AUXR3=AUXR3&(~(T0PS0|T0PS1))
#define TM_SetT0CKO_P44()			AUXR3=(AUXR3&(~(T0PS0|T0PS1)))|(T0PS0)
#define TM_SetT0CKO_P46()			AUXR3=(AUXR3&(~(T0PS0|T0PS1)))|(T0PS1)
#define TM_SetT0CKO_P17()			AUXR3=AUXR3|(T0PS0|T0PS1)

#define TM_SetT1CKO_P35()			SFRPI=1;AUXR4=AUXR4&(~(T1PS0|T1PS1));SFRPI=0
#define TM_SetT1CKO_P45()			SFRPI=1;AUXR4=(AUXR4&(~(T1PS0|T1PS1)))|(T1PS0);SFRPI=0
#define TM_SetT1CKO_P36()			SFRPI=1;AUXR4=(AUXR4&(~(T1PS0|T1PS1)))|(T1PS1);SFRPI=0
#define TM_SetT1CKO_P37()			SFRPI=1;AUXR4=AUXR4|(T1PS0|T1PS1);SFRPI=0

// 设置TIMER CKO输出使能/禁止
#define TM_EnT0CKO()				AUXR2=AUXR2|(T0CKOE)
#define TM_EnT1CKO()				AUXR2=AUXR2|(T1CKOE)

#define TM_DisT0CKO()				AUXR2=AUXR2&(~T0CKOE)
#define TM_DisT1CKO()				AUXR2=AUXR2&(~T1CKOE)

// 设置TIMER CKO多路输出使能/禁止
#define TM_EnT0CKOA_P40()			SFRPI=9;AUXR12=AUXR12|T0COAE;SFRPI=0
#define TM_DisT0CKOA_P40()			SFRPI=9;AUXR12=AUXR12&(~T0COAE);SFRPI=0

#define TM_EnT0CKOB_P43()			SFRPI=9;AUXR12=AUXR12|T0COBE;SFRPI=0
#define TM_DisT0CKOB_P43()			SFRPI=9;AUXR12=AUXR12&(~T0COBE);SFRPI=0



/*
*************************************************************************************
*/ 

/*
*************************************************************************************
*  TIMER2
*
*************************************************************************************
*/
// 使能或关闭TIMER2
#define TM_DisableT2()			TR2=0					
#define TM_EnableT2()			TR2=1

// 使能或关闭TL2
#define TM_DisableTL2()			T2MOD=T2MOD&(~TR2L)					
#define TM_EnableTL2()			T2MOD=T2MOD|(TR2L)	

// 使能或关闭硬件清TR2L
#define TM_DisableTR2LC()			T2MOD=T2MOD&(~TR2LC)					
#define TM_EnableTR2LC()			T2MOD=T2MOD|(TR2LC)	

// 选择TIMER2工作模式
#define TM_SetT2Mode_AutoRLandExI()			CP_RL2=0;T2MOD=T2MOD&(~T2MS0)
#define TM_SetT2Mode_AutoRLwithExI()		CP_RL2=0;T2MOD=T2MOD|(T2MS0)
#define TM_SetT2Mode_Capture()				CP_RL2=1;T2MOD=T2MOD&(~T2MS0)
#define TM_SetT2Mode_CapturewithAutoZero()	CP_RL2=1;T2MOD=T2MOD|(T2MS0)
#define TM_SetT2Mode_8bitPWM()				CP_RL2=0;T2MOD=T2MOD&(~T2MS0);T2MOD=T2MOD|(T2SPL);SFRPI=1;T2MOD1=T2MOD1|T2MS1;SFRPI=0
#define TM_SetT2Mode_DutyCapture()			CP_RL2=1;T2MOD=T2MOD&(~T2MS0);SFRPI=1;T2MOD1=T2MOD1|T2MS1;SFRPI=0

// 设置TIMER2分割模式
#define TM_EnT2Split()					T2MOD=T2MOD|(T2SPL)
#define TM_DisT2Split()					T2MOD=T2MOD&(~T2SPL)


// 设置TIMER2输出
#define TM_EnT2CKO()					T2MOD=T2MOD|(T2OE)
#define TM_DisT2CKO()					T2MOD=T2MOD&(~T2OE)

// 选择TIMER2时钟源
#define TM_SetT2Clock_SYSCLKDiv12()		C_T2=0;T2MOD=T2MOD&(~T2X12);SFRPI=1;T2MOD1=T2MOD1&(~T2CKS);SFRPI=0
#define TM_SetT2Clock_T2Pin()			C_T2=1;T2MOD=T2MOD&(~T2X12);SFRPI=1;T2MOD1=T2MOD1&(~T2CKS);SFRPI=0
#define TM_SetT2Clock_SYSCLK()			C_T2=0;T2MOD=T2MOD|(T2X12);SFRPI=1;T2MOD1=T2MOD1&(~T2CKS);SFRPI=0
#define TM_SetT2Clock_INT0ET()			C_T2=1;T2MOD=T2MOD|(T2X12);SFRPI=1;T2MOD1=T2MOD1&(~T2CKS);SFRPI=0	
#define TM_SetT2Clock_S0TOF()			C_T2=0;T2MOD=T2MOD&(~T2X12);SFRPI=1;T2MOD1=T2MOD1|(T2CKS);SFRPI=0
#define TM_SetT2Clock_T0OF()			C_T2=1;T2MOD=T2MOD&(~T2X12);SFRPI=1;T2MOD1=T2MOD1|(T2CKS);SFRPI=0
#define TM_SetT2Clock_AC0ES()			C_T2=0;T2MOD=T2MOD|(T2X12);SFRPI=1;T2MOD1=T2MOD1|(T2CKS);SFRPI=0
#define TM_SetT2Clock_KBIET()			C_T2=1;T2MOD=T2MOD|(T2X12);SFRPI=1;T2MOD1=T2MOD1|(T2CKS);SFRPI=0

// 选择TIMER2分割模式下TL2时钟源
#define TM_SetTL2Clock_SYSCLKDiv12()	T2MOD=T2MOD&(~TL2X12);SFRPI=1;T2MOD1=T2MOD1&(~TL2CS);SFRPI=0
#define TM_SetTL2Clock_SYSCLK()			T2MOD=T2MOD|(TL2X12);SFRPI=1;T2MOD1=T2MOD1&(~TL2CS);SFRPI=0
#define TM_SetTL2Clock_S0TOF()			T2MOD=T2MOD&(~TL2X12);SFRPI=1;T2MOD1=T2MOD1|(TL2CS);SFRPI=0
#define TM_SetTL2Clock_INT0ET()			T2MOD=T2MOD|(TL2X12);SFRPI=1;T2MOD1=T2MOD1|(TL2CS);SFRPI=0

// 设置使能访问TF2L及其中断使能
#define TM_EnableAccessTF2L()				SFRPI=1;T2MOD1=T2MOD1|TL2IS;SFRPI=0;
#define TM_DisableAccessTF2L()				SFRPI=1;T2MOD1=T2MOD1&(~TL2IS);SFRPI=0;

// 选择TIMER2捕捉模式下的捕捉源
#define TM_SetT2Capture_T2EXPin()		SFRPI=1;T2MOD1=T2MOD1&(0xF8);SFRPI=0
#define TM_SetT2Capture_RXD0()			SFRPI=1;T2MOD1=(T2MOD1&(0xF8))|(CP2S0);SFRPI=0
#define TM_SetT2Capture_P60()			SFRPI=1;T2MOD1=(T2MOD1&(0xF8))|(CP2S1);SFRPI=0
#define TM_SetT2Capture_INT2ET()		SFRPI=1;T2MOD1=(T2MOD1&(0xF8))|(CP2S0|CP2S1);SFRPI=0	
#define TM_SetT2Capture_ILRCO()			SFRPI=1;T2MOD1=(T2MOD1&(0xF8))|CP2S2;SFRPI=0	
#define TM_SetT2Capture_AC0OUT()		SFRPI=1;T2MOD1=(T2MOD1&(0xF8))|(CP2S2|CP2S0);SFRPI=0	
#define TM_SetT2Capture_KBIET()			SFRPI=1;T2MOD1=(T2MOD1&(0xF8))|(CP2S1|CP2S2);SFRPI=0
#define TM_SetT2Capture_TWI0SCL()		SFRPI=1;T2MOD1=T2MOD1|(CP2S0|CP2S1|CP2S2);SFRPI=0

// 设置TIMER2引脚
#define TM_SetT2CKOT2EX_P10P11()		SFRPI=1;AUXR4=AUXR4&(~(T2PS1|T2PS0));SFRPI=0
#define TM_SetT2CKOT2EX_P32P33()		SFRPI=1;AUXR4=(AUXR4&(~(T2PS1|T2PS0)))|(T2PS0);SFRPI=0
#define TM_SetT2CKOT2EX_P67P57()		SFRPI=1;AUXR4=(AUXR4&(~(T2PS1|T2PS0)))|(T2PS1);SFRPI=0
#define TM_SetT2CKOT2EX_P45P44()		SFRPI=1;AUXR4=AUXR4|((T2PS1|T2PS0));SFRPI=0

// 设置TIMER2引脚中断检测边沿
#define TM_EnableT2EX_DetectFalling()					EXEN2=1
#define TM_DisableT2EX_DetectFalling()					EXEN2=0
#define TM_EnableT2EX_DetectRising()					T2MOD=T2MOD|T2EXH
#define TM_DisableT2EX_DetectRising()					T2MOD=T2MOD&(~T2EXH)

// 设置TIMER2 T2CKO输出反向
#define TM_DisableT2CKO_Invert()		T2MOD=T2MOD&(~TR2LC)
#define TM_EnableT2CKO_Invert()			T2MOD=T2MOD|(TR2LC)

// 设置TIMER2值
#define TM_SetT2HighByte(x)			TH2=x
#define TM_SetT2LowByte(x)			TL2=x

#define TM_SetT2RLHighByte(x)		RCAP2H=x
#define TM_SetT2RLLowByte(x)		RCAP2L=x

/*
*************************************************************************************
*/ 

/*
*************************************************************************************
*  TIMER3
*
*************************************************************************************
*/
// 使能或关闭TIMER3
#define TM_DisableT3()			SFRPI=1;TR3=0;SFRPI=0					
#define TM_EnableT3()			SFRPI=1;TR3=1;SFRPI=0

// 使能或关闭TL3
#define TM_DisableTL3()			SFRPI=1;T3MOD=T3MOD&(~TR3L);SFRPI=0				
#define TM_EnableTL3()			SFRPI=1;T3MOD=T3MOD|(TR3L);SFRPI=0	

// 使能或关闭硬件清TR3L
#define TM_DisableTR3LC()			SFRPI=1;T3MOD=T3MOD&(~TR3LC);SFRPI=0					
#define TM_EnableTR3LC()			SFRPI=1;T3MOD=T3MOD|(TR3LC);SFRPI=0	

// 选择TIMER3工作模式
#define TM_SetT3Mode_AutoRLandExI()			SFRPI=1;CP_RL3=0;T3MOD=T3MOD&(~T3MS0);SFRPI=0
#define TM_SetT3Mode_AutoRLwithExI()		SFRPI=1;CP_RL3=0;T3MOD=T3MOD|(T3MS0);SFRPI=0
#define TM_SetT3Mode_Capture()				SFRPI=1;CP_RL3=1;T3MOD=T3MOD&(~T3MS0);SFRPI=0
#define TM_SetT3Mode_CapturewithAutoZero()	SFRPI=1;CP_RL3=1;T3MOD=T3MOD|(T3MS0);SFRPI=0
#define TM_SetT3Mode_8bitPWM()				SFRPI=1;CP_RL3=0;T3MOD=T3MOD&(~T3MS0);T3MOD=T3MOD|(T3SPL);SFRPI=2;T3MOD1=T3MOD1|T3MS1;SFRPI=0
#define TM_SetT3Mode_DutyCapture()			SFRPI=1;CP_RL3=1;T3MOD=T3MOD&(~T3MS0);SFRPI=2;T3MOD1=T3MOD1|T3MS1;SFRPI=0

// 设置TIMER3分割模式
#define TM_EnT3Split()					SFRPI=1;T3MOD=T3MOD|(T3SPL);SFRPI=0
#define TM_DisT3Split()					SFRPI=1;T3MOD=T3MOD&(~T3SPL);SFRPI=0

// 设置TIMER3输出
#define TM_EnT3CKO()					SFRPI=1;T3MOD=T3MOD|(T3OE);SFRPI=0
#define TM_DisT3CKO()					SFRPI=1;T3MOD=T3MOD&(~T3OE);SFRPI=0

// 选择TIMER3时钟源
#define TM_SetT3Clock_SYSCLKDiv12()		SFRPI=1;C_T3=0;T3MOD=T3MOD&(~T3X12);SFRPI=2;T3MOD1=T3MOD1&(~T3CKS);SFRPI=0
#define TM_SetT3Clock_T3Pin()			SFRPI=1;C_T3=1;T3MOD=T3MOD&(~T3X12);SFRPI=2;T3MOD1=T3MOD1&(~T3CKS);SFRPI=0
#define TM_SetT3Clock_SYSCLK()			SFRPI=1;C_T3=0;T3MOD=T3MOD|(T3X12);SFRPI=2;T3MOD1=T3MOD1&(~T3CKS);SFRPI=0
#define TM_SetT3Clock_INT1ET()			SFRPI=1;C_T3=1;T3MOD=T3MOD|(T3X12);SFRPI=2;T3MOD1=T3MOD1&(~T3CKS);SFRPI=0	
#define TM_SetT3Clock_S1TOF()			SFRPI=1;C_T3=0;T3MOD=T3MOD&(~T3X12);SFRPI=2;T3MOD1=T3MOD1|(T3CKS);SFRPI=0
#define TM_SetT3Clock_T0OF()			SFRPI=1;C_T3=1;T3MOD=T3MOD&(~T3X12);SFRPI=2;T3MOD1=T3MOD1|(T3CKS);SFRPI=0
#define TM_SetT3Clock_AC1ES()			SFRPI=1;C_T3=0;T3MOD=T3MOD|(T3X12);SFRPI=2;T3MOD1=T3MOD1|(T3CKS);SFRPI=0
#define TM_SetT3Clock_T1OF()			SFRPI=1;C_T3=1;T3MOD=T3MOD|(T3X12);SFRPI=2;T3MOD1=T3MOD1|(T3CKS);SFRPI=0

// 选择TIMER3分割模式下TL3时钟源
#define TM_SetTL3Clock_SYSCLKDiv12()	SFRPI=1;T3MOD=T3MOD&(~TL3X12);SFRPI=2;T3MOD1=T3MOD1&(~TL3CS);SFRPI=0
#define TM_SetTL3Clock_SYSCLK()			SFRPI=1;T3MOD=T3MOD|(TL3X12);SFRPI=2;T3MOD1=T3MOD1&(~TL3CS);SFRPI=0
#define TM_SetTL3Clock_S1TOF()			SFRPI=1;T3MOD=T3MOD&(~TL3X12);SFRPI=2;T3MOD1=T3MOD1|(TL3CS);SFRPI=0
#define TM_SetTL3Clock_INT1ET()			SFRPI=1;T3MOD=T3MOD|(TL3X12);SFRPI=2;T3MOD1=T3MOD1|(TL3CS);SFRPI=0

// 选择TIMER3捕捉模式下的捕捉源
#define TM_SetT3Capture_T3EXPin()		SFRPI=2;T3MOD1=T3MOD1&(0xF8);SFRPI=0
#define TM_SetT3Capture_INT0ET()		SFRPI=2;T3MOD1=(T3MOD1&(0xF8))|(CP3S0);SFRPI=0
#define TM_SetT3Capture_P60()			SFRPI=2;T3MOD1=(T3MOD1&(0xF8))|(CP3S1);SFRPI=0
#define	TM_SetT3Capture_nSS()			SFRPI=2;T3MOD1=(T3MOD1&(0xF8))|(CP3S0|CP3S1);SFRPI=0
#define TM_SetT3Capture_KBIET()			SFRPI=2;T3MOD1=(T3MOD1&(0xF8))|(CP3S2);SFRPI=0
#define TM_SetT3Capture_AC0OUT()		SFRPI=2;T3MOD1=(T3MOD1&(0xF8))|(CP3S0|CP3S2);SFRPI=0
#define TM_SetT3Capture_AC1OUT()		SFRPI=2;T3MOD1=(T3MOD1&(0xF8))|(CP3S1|CP3S2);SFRPI=0
#define TM_SetT3Capture_ILRCO()			SFRPI=2;T3MOD1=T3MOD1|(CP3S0|CP3S1|CP3S2);SFRPI=0

// 设置TIMER3引脚
#define TM_SetT3CKOT3EX_P46P00()		SFRPI=5;AUXR8=AUXR8&(~(T3PS1|T3PS0));SFRPI=0
#define TM_SetT3CKOT3EX_P33P32()		SFRPI=5;AUXR8=(AUXR8&(~(T3PS1|T3PS0)))|(T3PS0);SFRPI=0
#define TM_SetT3CKOT3EX_P21P20()		SFRPI=5;AUXR8=(AUXR8&(~(T3PS1|T3PS0)))|(T3PS1);SFRPI=0
#define TM_SetT3CKOT3EX_P66P65()		SFRPI=5;AUXR8=AUXR8|((T3PS1|T3PS0));SFRPI=0

// 设置TIMER3引脚中断检测边沿
#define TM_EnableT3EX_DetectFalling()					SFRPI=1;EXEN3=1;SFRPI=0
#define TM_DisableT3EX_DetectFalling()					SFRPI=1;EXEN3=0;SFRPI=0
#define TM_EnableT3EX_DetectRising()					SFRPI=1;T3MOD=T3MOD|T3EXH;SFRPI=0
#define TM_DisableT3EX_DetectRising()					SFRPI=1;T3MOD=T3MOD&(~T3EXH);SFRPI=0

// 设置TIMER3 T3CKO输出反向
#define TM_DisableT3CKO_Invert()		SFRPI=1;T3MOD=T3MOD&(~TR3LC);SFRPI=0
#define TM_EnableT3CKO_Invert()			SFRPI=1;T3MOD=T3MOD|(TR3LC);SFRPI=0

// 设置TIMER3值
#define TM_SetT3HighByte(x)			SFRPI=1;TH3=x;SFRPI=0
#define TM_SetT3LowByte(x)			SFRPI=1;TL3=x;SFRPI=0

#define TM_SetT3RLHighByte(x)		SFRPI=1;RCAP3H=x;SFRPI=0
#define TM_SetT3RLLowByte(x)		SFRPI=1;RCAP3L=x;SFRPI=0

/*
*************************************************************************************
*/ 

/*
*************************************************************************************
*  TIMER4
*
*************************************************************************************
*/
// 使能或关闭TIMER4
#define TM_DisableT4()			SFRPI=2;TR4=0;SFRPI=0					
#define TM_EnableT4()			SFRPI=2;TR4=1;SFRPI=0

// 使能或关闭TL4
#define TM_DisableTL4()			SFRPI=2;T4MOD=T4MOD&(~TR4L);SFRPI=0				
#define TM_EnableTL4()			SFRPI=2;T4MOD=T4MOD|(TR4L);SFRPI=0	

// 使能或关闭硬件清TR4L
#define TM_DisableTR4LC()			SFRPI=2;T4MOD=T4MOD&(~TR4LC);SFRPI=0					
#define TM_EnableTR4LC()			SFRPI=2;T4MOD=T4MOD|(TR4LC);SFRPI=0	

// 选择TIMER4工作模式
#define TM_SetT4Mode_AutoRLandExI()			SFRPI=2;CP_RL4=0;T4MOD=T4MOD&(~T4MS0);SFRPI=0
#define TM_SetT4Mode_AutoRLwithExI()		SFRPI=2;CP_RL4=0;T4MOD=T4MOD|(T4MS0);SFRPI=0
#define TM_SetT4Mode_Capture()				SFRPI=2;CP_RL4=1;T4MOD=T4MOD&(~T4MS0);SFRPI=0
#define TM_SetT4Mode_CapturewithAutoZero()	SFRPI=2;CP_RL4=1;T4MOD=T4MOD|(T4MS0);SFRPI=0
#define TM_SetT4Mode_8bitPWM()				SFRPI=2;CP_RL4=0;T4MOD=T4MOD&(~T4MS0);T4MOD=T4MOD|(T4SPL);SFRPI=3;T4MOD1=T4MOD1|T4MS1;SFRPI=0
#define TM_SetT4Mode_DutyCapture()			SFRPI=2;CP_RL4=1;T4MOD=T4MOD&(~T4MS0);SFRPI=3;T4MOD1=T4MOD1|T4MS1;SFRPI=0

// 设置TIMER4分割模式
#define TM_EnT4Split()					SFRPI=2;T4MOD=T4MOD|(T4SPL);SFRPI=0
#define TM_DisT4Split()					SFRPI=2;T4MOD=T4MOD&(~T4SPL);SFRPI=0

// 设置TIMER4输出
#define TM_EnT4CKO()					SFRPI=2;T4MOD=T4MOD|(T4OE);SFRPI=0
#define TM_DisT4CKO()					SFRPI=2;T4MOD=T4MOD&(~T4OE);SFRPI=0

// 选择TIMER4时钟源
#define TM_SetT4Clock_SYSCLKDiv12()		SFRPI=2;C_T4=0;T4MOD=T4MOD&(~T4X12);SFRPI=3;T4MOD1=T4MOD1&(~T4CKS);SFRPI=0
#define TM_SetT4Clock_T4Pin()			SFRPI=2;C_T4=1;T4MOD=T4MOD&(~T4X12);SFRPI=3;T4MOD1=T4MOD1&(~T4CKS);SFRPI=0
#define TM_SetT4Clock_SYSCLK()			SFRPI=2;C_T4=0;T4MOD=T4MOD|(T4X12);SFRPI=3;T4MOD1=T4MOD1&(~T4CKS);SFRPI=0
#define TM_SetT4Clock_INT2ET()			SFRPI=2;C_T4=1;T4MOD=T4MOD|(T4X12);SFRPI=3;T4MOD1=T4MOD1&(~T4CKS);SFRPI=0	
#define TM_SetT4Clock_T0OF()			SFRPI=2;C_T4=1;T4MOD=T4MOD&(~T4X12);SFRPI=3;T4MOD1=T4MOD1|(T4CKS);SFRPI=0
#define TM_SetT4Clock_AC2ES()			SFRPI=2;C_T4=0;T4MOD=T4MOD|(T4X12);SFRPI=3;T4MOD1=T4MOD1|(T4CKS);SFRPI=0
#define TM_SetT4Clock_AC0ES()			SFRPI=2;C_T4=1;T4MOD=T4MOD|(T4X12);SFRPI=3;T4MOD1=T4MOD1|(T4CKS);SFRPI=0

// 选择TIMER4分割模式下TL4时钟源
#define TM_SetTL4Clock_SYSCLKDiv12()	SFRPI=2;T4MOD=T4MOD&(~TL4X12);SFRPI=3;T4MOD1=T4MOD1&(~TL4CS);SFRPI=0
#define TM_SetTL4Clock_SYSCLK()			SFRPI=2;T4MOD=T4MOD|(TL4X12);SFRPI=3;T4MOD1=T4MOD1&(~TL4CS);SFRPI=0
#define TM_SetTL4Clock_INT2ET()			SFRPI=2;T4MOD=T4MOD(TL4X12);SFRPI=3;T4MOD1=T4MOD1|(TL4CS);SFRPI=0

// 选择TIMER4捕捉模式下的捕捉源
#define TM_SetT4Capture_T4EXPin()		SFRPI=3;T4MOD1=T4MOD1&(0xF8);SFRPI=0
#define TM_SetT4Capture_nSS()			SFRPI=3;T4MOD1=(T4MOD1&(0xF8))|(CP4S0);SFRPI=0
#define TM_SetT4Capture_INT3ET()		SFRPI=3;T4MOD1=(T4MOD1&(0xF8))|(CP4S1);SFRPI=0
#define TM_SetT4Capture_AC0OUT()		SFRPI=3;T4MOD1=(T4MOD1&(0xF8))|(CP4S0|CP4S2);SFRPI=0
#define TM_SetT4Capture_AC2OUT()		SFRPI=3;T4MOD1=(T4MOD1&(0xF8))|(CP4S1|CP4S2);SFRPI=0
#define TM_SetT4Capture_TWI1_SCL()		SFRPI=3;T4MOD1=T4MOD1|(CP4S0|CP4S1|CP4S2);SFRPI=0

// 设置TIMER4引脚
#define TM_SetT4CKOT4EX_P70P71()		SFRPI=0x0A;AUXR13=AUXR13&(~(T4PS1|T4PS0));SFRPI=0
#define TM_SetT4CKOT4EX_P37P36()		SFRPI=0x0A;AUXR13=(AUXR13&(~(T4PS1|T4PS0)))|(T4PS0);SFRPI=0
#define TM_SetT4CKOT4EX_P72P50()		SFRPI=0x0A;AUXR13=(AUXR13&(~(T4PS1|T4PS0)))|(T4PS1);SFRPI=0
#define TM_SetT4CKOT4EX_P36P37()		SFRPI=0x0A;AUXR13=AUXR13|((T4PS1|T4PS0));SFRPI=0

// 设置TIMER4引脚中断检测边沿
#define TM_EnableT4EX_DetectFalling()					SFRPI=2;EXEN4=1;SFRPI=0
#define TM_DisableT4EX_DetectFalling()					SFRPI=2;EXEN4=0;SFRPI=0
#define TM_EnableT4EX_DetectRising()					SFRPI=2;T4MOD=T4MOD|T4EXH;SFRPI=0
#define TM_DisableT4EX_DetectRising()					SFRPI=2;T4MOD=T4MOD&(~T4EXH);SFRPI=0

// 设置TIMER4 T4CKO输出反向
#define TM_DisableT4CKO_Invert()		SFRPI=2;T4MOD=T4MOD&(~TR4LC);SFRPI=0
#define TM_EnableT4CKO_Invert()			SFRPI=2;T4MOD=T4MOD|(TR4LC);SFRPI=0

// 设置TIMER4值
#define TM_SetT4HighByte(x)			SFRPI=2;TH4=x;SFRPI=0
#define TM_SetT4LowByte(x)			SFRPI=2;TL4=x;SFRPI=0

#define TM_SetT4RLHighByte(x)		SFRPI=2;RCAP4H=x;SFRPI=0
#define TM_SetT4RLLowByte(x)		SFRPI=2;RCAP4L=x;SFRPI=0


/*
*************************************************************************************
*/ 

/*
*************************************************************************************
*  TIMER 全局控制
*
*************************************************************************************
*/
#define	TIMER0_GLOBAL_BIT  		0x01
#define	TIMER1_GLOBAL_BIT  		0x02
#define	TIMER2_GLOBAL_BIT  		0x04
#define	TIMER3_GLOBAL_BIT  		0x08
#define	TIMER4_GLOBAL_BIT  		0x10
#define	TIMER2TL_GLOBAL_BIT 	0x20
#define	TIMER3TL_GLOBAL_BIT 	0x40
#define	TIMER4TL_GLOBAL_BIT 	0x80

// 全局使能TIMER
#define TM_GlobalStart(x)				SFRPI=1;TREN0=TREN0|x;SFRPI=0

// 全局触发Timer重载
#define TM_GlobalReload(x)				SFRPI=2;TRLC0=TRLC0|x;SFRPI=0

// 全局停止TIMER
#define TM_GlobalSTOP(x)				SFRPI=3;TSPC0=TSPC0|x;SFRPI=0

/*
*************************************************************************************
*/ 


/*
*************************************************************************************
*  IO PORT 
*
*************************************************************************************
*/
// 设置P0脚模式
#define PORT_SetP0OpenDrainPu(x)			P0M0=P0M0|(x);SFRPI=4;P0M1=P0M1|(x);SFRPI=0
#define PORT_SetP0PushPull(x)				P0M0=P0M0|(x);SFRPI=4;P0M1=P0M1&(~(x));SFRPI=0
#define PORT_SetP0AInputOnly(x)				P0M0=P0M0&(~(x));SFRPI=4;P0M1=P0M1|(x);SFRPI=0
#define PORT_SetP0OpenDrain(x)				P0M0=P0M0&(~(x));SFRPI=4;P0M1=P0M1&(~(x));SFRPI=0
// 设置P0输出能力
#define PORT_P00P01P02P03_HighDrive()		SFRPI=2;PDRVC0=PDRVC0&(~(P0DC0));SFRPI=0
#define PORT_P00P01P02P03_LowDrive()		SFRPI=2;PDRVC0=PDRVC0|((P0DC0));SFRPI=0
#define PORT_P04P05P06P07_HighDrive()		SFRPI=2;PDRVC0=PDRVC0&(~(P0DC1));SFRPI=0
#define PORT_P04P05P06P07_LowDrive()		SFRPI=2;PDRVC0=PDRVC0|((P0DC1));SFRPI=0
#define PORT_P0_EnFastDrive(x)				SFRPI=11;P0FDC=P0FDC|((x));SFRPI=0
#define PORT_P0_DisFastDrive(x)				SFRPI=11;P0FDC=P0FDC&(~(x));SFRPI=0

// 设置P1引脚模式
#define PORT_SetP1OpenDrainPu(x)			P1M0=P1M0|(x);P1M1=P1M1|(x)
#define PORT_SetP1PushPull(x)				P1M0=P1M0|(x);P1M1=P1M1&(~(x))
#define PORT_SetP1AInputOnly(x)				P1M0=P1M0&(~(x));P1M1=P1M1|(x)
#define PORT_SetP1OpenDrain(x)				P1M0=P1M0&(~(x));P1M1=P1M1&(~(x))
// 设置P1口输出能力
#define PORT_P10P11P12P13_HighDrive()		SFRPI=2;PDRVC0=PDRVC0&(~(P1DC0));SFRPI=0
#define PORT_P10P11P12P13_LowDrive()		SFRPI=2;PDRVC0=PDRVC0|((P1DC0));SFRPI=0
#define PORT_P14P15P16P17_HighDrive()		SFRPI=2;PDRVC0=PDRVC0&(~(P1DC1));SFRPI=0
#define PORT_P14P15P16P17_LowDrive()		SFRPI=2;PDRVC0=PDRVC0|((P1DC1));SFRPI=0
#define PORT_P1_EnFastDrive(x)				SFRPI=8;P1FDC=P1FDC|((x));SFRPI=0
#define PORT_P1_DisFastDrive(x)				SFRPI=8;P1FDC=P1FDC&(~(x));SFRPI=0

// 设置P2引脚模式
#define PORT_SetP2OpenDrainPu(x)			P2M0=P2M0|(x); SFRPI=1;P2M1=P2M1|(x) ;SFRPI=0 
#define PORT_SetP2PushPull(x)				P2M0=P2M0|(x); SFRPI=1;P2M1=P2M1&(~(x));SFRPI=0 
#define PORT_SetP2AInputOnly(x)				P2M0=P2M0&(~(x));SFRPI=1;P2M1=P2M1|(x) ;SFRPI=0
#define PORT_SetP2OpenDrain(x)				P2M0=P2M0&(~(x));SFRPI=1;P2M1=P2M1&(~(x));SFRPI=0
// 设置P2口输出能力
#define PORT_P20P21P22P23_HighDrive()		SFRPI=2;PDRVC0=PDRVC0&(~(P2DC0));SFRPI=0
#define PORT_P20P21P22P23_LowDrive()		SFRPI=2;PDRVC0=PDRVC0|((P2DC0));SFRPI=0
#define PORT_P24P25P26P27_HighDrive()		SFRPI=2;PDRVC0=PDRVC0&(~(P2DC1));SFRPI=0
#define PORT_P24P25P26P27_LowDrive()		SFRPI=2;PDRVC0=PDRVC0|((P2DC1));SFRPI=0
#define PORT_P2_EnFastDrive(x)				SFRPI=9;P2FDC=P2FDC|((x));SFRPI=0
#define PORT_P2_DisFastDrive(x)				SFRPI=9;P2FDC=P2FDC&(~(x));SFRPI=0

// 设置P3引脚模式
#define PORT_SetP3QuasiBi(x)				P3M0=P3M0&(~(x));P3M1=P3M1&(~(x))
#define PORT_SetP3PushPull(x)				P3M0=P3M0&(~(x));P3M1=P3M1|(x)
#define PORT_SetP3InputOnly(x)				P3M0=P3M0|(x);P3M1=P3M1&(~(x))
#define PORT_SetP3OpenDrain(x)				P3M0=P3M0|(x);P3M1=P3M1|(x)

// 设置P3口输出能力
#define PORT_P30P31P32P33_HighDrive()		SFRPI=2;PDRVC0=PDRVC0&(~(P3DC0));SFRPI=0
#define PORT_P30P31P32P33_LowDrive()		SFRPI=2;PDRVC0=PDRVC0|((P3DC0));SFRPI=0
#define PORT_P34P35P36P37_HighDrive()		SFRPI=2;PDRVC0=PDRVC0&(~(P3DC1));SFRPI=0
#define PORT_P34P35P36P37_LowDrive()		SFRPI=2;PDRVC0=PDRVC0|((P3DC1));SFRPI=0
#define PORT_P3_EnFastDrive(x)				SFRPI=7;P3FDC=P3FDC|((x));SFRPI=0
#define PORT_P3_DisFastDrive(x)				SFRPI=7;P3FDC=P3FDC&(~(x));SFRPI=0

// 设置P4引脚模式
#define PORT_SetP4OpenDrainPu(x)			P4M0=P4M0|(x); SFRPI=2;P4M1=P4M1|(x) ;SFRPI=0 
#define PORT_SetP4PushPull(x)				P4M0=P4M0|(x); SFRPI=2;P4M1=P4M1&(~(x));SFRPI=0 
#define PORT_SetP4AInputOnly(x)				P4M0=P4M0&(~(x));SFRPI=2;P4M1=P4M1|(x) ;SFRPI=0
#define PORT_SetP4OpenDrain(x)				P4M0=P4M0&(~(x));SFRPI=2;P4M1=P4M1&(~(x));SFRPI=0
// 设置P4口输出能力
#define PORT_P44P45P46_HighDrive()			SFRPI=3;PDRVC1=PDRVC1&(~(P4DC1));SFRPI=0
#define PORT_P44P45P46_LowDrive()			SFRPI=3;PDRVC1=PDRVC1|((P4DC1));SFRPI=0
#define PORT_P40P41P42P43_HighDrive()		SFRPI=3;PDRVC1=PDRVC1&(~(P4DC0));SFRPI=0
#define PORT_P40P41P42P43_LowDrive()		SFRPI=3;PDRVC1=PDRVC1|((P4DC0));SFRPI=0
#define PORT_P4_EnFastDrive(x)				SFRPI=10;P4FDC=P4FDC|((x));SFRPI=0
#define PORT_P4_DisFastDrive(x)				SFRPI=10;P4FDC=P4FDC&(~(x));SFRPI=0

// 设置P5引脚模式
#define PORT_SetP5OpenDrainPu(x)			P5M0=P5M0|(x); SFRPI=5;P5M1=P5M1|(x) ;SFRPI=0 
#define PORT_SetP5PushPull(x)				P5M0=P5M0|(x); SFRPI=5;P5M1=P5M1&(~(x));SFRPI=0 
#define PORT_SetP5AInputOnly(x)				P5M0=P5M0&(~(x));SFRPI=5;P5M1=P5M1|(x) ;SFRPI=0
#define PORT_SetP5OpenDrain(x)				P5M0=P5M0&(~(x));SFRPI=5;P5M1=P5M1&(~(x));SFRPI=0
// 设置P5口输出能力
#define PORT_P54P55P56P57_HighDrive()		SFRPI=3;PDRVC1=PDRVC1&(~(P5DC1));SFRPI=0
#define PORT_P54P55P56P57_LowDrive()		SFRPI=3;PDRVC1=PDRVC1|((P5DC1));SFRPI=0
#define PORT_P50P51P52P53_HighDrive()		SFRPI=3;PDRVC1=PDRVC1&(~(P5DC0));SFRPI=0
#define PORT_P50P51P52P53_LowDrive()		SFRPI=3;PDRVC1=PDRVC1|((P5DC0));SFRPI=0
#define PORT_P5_EnFastDrive(x)				SFRPI=13;P5FDC=P5FDC|((x));SFRPI=0
#define PORT_P5_DisFastDrive(x)				SFRPI=13;P5FDC=P5FDC&(~(x));SFRPI=0
                                                                               
// 设置P6引脚模式
#define PORT_SetP6OpenDrainPu(x)			SFRPI=1;P6M0=P6M0|(x); SFRPI=3;P6M1=P6M1|(x) ;SFRPI=0 
#define PORT_SetP6PushPull(x)				SFRPI=1;P6M0=P6M0|(x); SFRPI=3;P6M1=P6M1&(~(x));SFRPI=0 
#define PORT_SetP6AInputOnly(x)				SFRPI=1;P6M0=P6M0&(~(x));SFRPI=3;P6M1=P6M1|(x) ;SFRPI=0
#define PORT_SetP6OpenDrain(x)				SFRPI=1;P6M0=P6M0&(~(x));SFRPI=3;P6M1=P6M1&(~(x));SFRPI=0
// 设置P6口输出能力
#define PORT_P64P65P66P67_HighDrive()		SFRPI=3;PDRVC1=PDRVC1&(~(P6DC1));SFRPI=0
#define PORT_P64P65P66P67_LowDrive()		SFRPI=3;PDRVC1=PDRVC1|((P6DC1));SFRPI=0
#define PORT_P62P63_HighDrive()				SFRPI=3;PDRVC1=PDRVC1&(~(P6DC0));SFRPI=0
#define PORT_P62P63_LowDrive()				SFRPI=3;PDRVC1=PDRVC1|((P6DC0));SFRPI=0
#define PORT_P6_EnFastDrive(x)				SFRPI=12;P6FDC=P6FDC|((x));SFRPI=0
#define PORT_P6_DisFastDrive(x)				SFRPI=12;P6FDC=P6FDC&(~(x));SFRPI=0

// 设置P7引脚模式
#define PORT_SetP7OpenDrainPu(x)			P7M0=P7M0|(x); SFRPI=6;P7M1=P7M1|(x) ;SFRPI=0 
#define PORT_SetP7PushPull(x)				P7M0=P7M0|(x); SFRPI=6;P7M1=P7M1&(~(x));SFRPI=0 
#define PORT_SetP7AInputOnly(x)				P7M0=P7M0&(~(x));SFRPI=6;P7M1=P7M1|(x) ;SFRPI=0
#define PORT_SetP7OpenDrain(x)				P7M0=P7M0&(~(x));SFRPI=6;P7M1=P7M1&(~(x));SFRPI=0
// 设置P7口输出能力
#define PORT_P70P71P72P73_HighDrive()		SFRPI=3;PDRVC1=PDRVC1&(~(P7DC0));SFRPI=0
#define PORT_P70P71P72P73_LowDrive() 		SFRPI=3;PDRVC1=PDRVC1|((P7DC0));SFRPI=0
#define PORT_P7_EnFastDrive(x)				SFRPI=14;P7FDC=P7FDC|((x));SFRPI=0
#define PORT_P7_DisFastDrive(x)				SFRPI=14;P7FDC=P7FDC&(~(x));SFRPI=0

/*
*************************************************************************************
*/ 

/*
*************************************************************************************
*  TWI0 
*
*************************************************************************************
*/
// 清TWI0
#define TWI0_Clear()				SICON=0
// 开启或关闭TWI0
#define TWI0_Disable()				SICON=SICON&(~ENSI)					
#define TWI0_Enable()				SICON=SICON|(ENSI)

// 选择TWI0 的传送速度  主机最高1MHz, 从机同步任何主机的频率,最高支持400KHz
#define TWI0_CLK_SYSCLK_8	0	// SYSCLK/8
#define TWI0_CLK_SYSCLK_16	1	// SYSCLK/16
#define TWI0_CLK_SYSCLK_32	2	// SYSCLK/32
#define TWI0_CLK_SYSCLK_64	3	// SYSCLK/64
#define TWI0_CLK_SYSCLK_128	4	// SYSCLK/128
#define TWI0_CLK_SYSCLK_256	5	// SYSCLK/256
#define TWI0_CLK_S0TOF_6	6	// S0TOF/6	
#define TWI0_CLK_T0OF_6		7	// T0OF/6	

#define TWI0_SetClock(x)			SICON=(SICON&B01111100)|((x&0x03)|((x<<5)&B10000000))

#define TWI0_SetSlaveAddr(x)		SIADR=x			// 设置从机地址
#define TWI0_SendACK()				SICON |= AA		// 发送ACK
#define TWI0_SendNOACK()			SICON &= ~AA	// 发送 NO ACK
#define TWI0_SendSTART()			SICON |= STA	// 发送起始位
#define TWI0_ClearSTART()			SICON &= ~STA	// 清除起始标志
#define TWI0_SendSTOP()				SICON |= STO	// 发送停止位，停止标志由硬件清除
#define TWI0_ClearSI()				SICON &= ~SI	// 清楚TWI中断标志

// 选择TWI0 所用的IO口 SCL/SDA
#define TWI0_SetSCLSDAP40P41()			AUXR3=(AUXR3&(~(TWIPS1|TWIPS0)))
#define TWI0_SetSCLSDAP60P61()			AUXR3=(AUXR3&(~(TWIPS1|TWIPS0)))|TWIPS0
#define TWI0_SetSCLSDAP31P30()			AUXR3=(AUXR3&(~(TWIPS1|TWIPS0)))|TWIPS1
#define TWI0_SetSCLSDAP25P23()			AUXR3=(AUXR3|(TWIPS1|TWIPS0))

// 开启或关闭TWI0 时钟滤波
#define TWI0_DisClkFilter()			SFRPI=7;AUXR10=AUXR10&(~TWICF);SFRPI=0			
#define TWI0_EnClkFilter()			SFRPI=7;AUXR10=AUXR10|(TWICF);SFRPI=0

// 开启或关闭TWI0 DMA模式下预设ACK
#define TWI0_DisPAA()				SFRPI=7;AUXR10=AUXR10&(~PAA);SFRPI=0			
#define TWI0_EnPAA()				SFRPI=7;AUXR10=AUXR10|(PAA);SFRPI=0

/*
*************************************************************************************
*/ 

/*
*************************************************************************************
*  TWI1 
*
*************************************************************************************
*/
// 清TWI1
#define TWI1_Clear()				SFRPI=1;SI1CON=0;SFRPI=0
// 开启或关闭TWI1
#define TWI1_Disable()				SFRPI=1;SI1CON=SI1CON&(~ENSI1);SFRPI=0					
#define TWI1_Enable()				SFRPI=1;SI1CON=SI1CON|(ENSI1);SFRPI=0 

// 选择TWI1 的传送速度  主机最高1MHz, 从机同步任何主机的频率,最高支持400KHz
#define TWI1_CLK_SYSCLK_8	0	// SYSCLK/8
#define TWI1_CLK_SYSCLK_16	1	// SYSCLK/16
#define TWI1_CLK_SYSCLK_32	2	// SYSCLK/32
#define TWI1_CLK_SYSCLK_64	3	// SYSCLK/64
#define TWI1_CLK_SYSCLK_128	4	// SYSCLK/128
#define TWI1_CLK_SYSCLK_256	5	// SYSCLK/256
#define TWI1_CLK_S0TOF_6	6	// S0TOF/6	
#define TWI1_CLK_T0OF_6		7	// T0OF/6	

#define TWI1_SetClock(x)			SFRPI=1;SI1CON=(SI1CON&B01111100)|((x&0x03)|((x<<5)&B10000000));SFRPI=0

#define TWI1_SetSlaveAddr(x)		SFRPI=1;SI1ADR=x;SFRPI=0			// 设置从机地址
#define TWI1_SendACK()				SFRPI=1;SI1CON |= AA1;SFRPI=0		// 发送ACK
#define TWI1_SendNOACK()			SFRPI=1;SI1CON &= ~AA1;SFRPI=0		// 发送 NO ACK
#define TWI1_SendSTART()			SFRPI=1;SI1CON |= STA1;SFRPI=0		// 发送起始位
#define TWI1_ClearSTART()			SFRPI=1;SI1CON &= ~STA1;SFRPI=0		// 清除起始标志
#define TWI1_SendSTOP()				SFRPI=1;SI1CON |= STO1;SFRPI=0		// 发送停止位，停止标志由硬件清除
#define TWI1_ClearSI()				SFRPI=1;SI1CON &= ~SI1;SFRPI=0		// 清楚TWI中断标志


// 选择TWI1 所用的IO口 SCL/SDA
#define TWI1_SetSCLSDAP42P43()			SFRPI=8;AUXR11=(AUXR11&(~(I2C1PS1|I2C1PS0)));SFRPI=0
#define TWI1_SetSCLSDAP60P61()			SFRPI=8;AUXR11=(AUXR11&(~(I2C1PS1|I2C1PS0)))|I2C1PS0;SFRPI=0
#define TWI1_SetSCLSDAP65P66()			SFRPI=8;AUXR11=(AUXR11&(~(I2C1PS1|I2C1PS0)))|I2C1PS1;SFRPI=0
#define TWI1_SetSCLSDAP63P62()			SFRPI=8;AUXR11=(AUXR11|(I2C1PS1|I2C1PS0));SFRPI=0

// 开启或关闭TWI1 时钟滤波
#define TWI1_DisClkFilter()			SFRPI=8;AUXR11=AUXR11&(~I2C1CF);SFRPI=0			
#define TWI1_EnClkFilter()			SFRPI=8;AUXR11=AUXR11|(I2C1CF);SFRPI=0

// 开启或关闭TWI1 DMA模式下预设ACK
#define TWI1_DisPAA()				SFRPI=8;AUXR11=AUXR11&(~PAA);SFRPI=0			
#define TWI1_EnPAA()				SFRPI=8;AUXR11=AUXR11|(PAA);SFRPI=0	

/*
*************************************************************************************
*/ 

/*
*************************************************************************************
*  UART0 
*
*************************************************************************************
*/
// UART0 设置UART0工作模式
#define UART0_SetModeShiftReg()				S0CFG=S0CFG&(~SM30);SM00_FE=0;SM10=0
#define UART0_SetMode8bitUARTVar()			S0CFG=S0CFG&(~SM30);SM00_FE=0;SM10=1
#define UART0_SetMode9bitUART()				S0CFG=S0CFG&(~SM30);SM00_FE=1;SM10=0
#define UART0_SetMode9bitUARTVar()			S0CFG=S0CFG&(~SM30);SM00_FE=1;SM10=1
#define UART0_SetModeSPIMaster()			S0CFG=S0CFG|(SM30);SM00_FE=0;SM10=0
#define UART0_SetModeLIN()					S0CFG=S0CFG|(SM30);SM00_FE=0;SM10=1;S0CR1=S0CR1|(S0TCK|S0RCK)
#define UART0_SetMode9bitUART_S0BRT8BitTimer()	S0CFG=S0CFG&(~SM30);SM00_FE=1;SM10=0;S0CR1=S0CR1|(S0TCK|S0RCK);S0CFG=S0CFG&(~S0DOR)
#define UART0_SetMode9bitUART_S0BRT16BitTimer()	S0CFG=S0CFG&(~SM30);SM00_FE=1;SM10=0;S0CR1=S0CR1|(S0TCK|S0RCK);S0CFG=S0CFG|(S0DOR)

// UART0 开启/关闭自动地址识别功能
#define UART0_EnAutoAddressRecognition()	SM20=1
#define UART0_DisAutoAddressRecognition()	SM20=0

// UART0 开启/关闭接收数据功能
#define UART0_EnReception()					REN0=1
#define UART0_DisReception()				REN0=0

// UART0 设置数据位序(MSB:高位在前,LSB:低位在前)
#define UART0_SetDataMSB()					S0CFG=S0CFG&(~S0DOR)
#define UART0_SetDataLSB()					S0CFG=S0CFG|(S0DOR)

// UART0 开启/关闭响应发送完成中断TI0
#define UART0_EnTI0toS0Interrupt()			S0CFG=S0CFG&(~BTI)
#define UART0_DisTI0toS0Interrupt()			S0CFG=S0CFG|(BTI)

// UART0 开启/关闭系统标志中断响应发送完成中断TI0
#define UART0_EnTI0toSysFlagInterrupt()		S0CFG=S0CFG|(UTIE)
#define UART0_DisTI0toSysFlagInterrupt()	S0CFG=S0CFG&(~UTIE)

// UART0 设置从机地址
#define UART0_SetSlaveAddr(x)				SADDR=x

// UART0 设置从机地址掩码
#define UART0_SetSlaveAddrMask(x)			SADEN=x

// UART0 设置波特率加倍设置 
#define UART0_SetT1BaudRateX1()				PCON0=PCON0&(~SMOD1);S0CFG=S0CFG&(~SMOD2)
#define UART0_SetT1BaudRateX2()				PCON0=PCON0|(SMOD1);S0CFG=S0CFG&(~SMOD2)
#define UART0_SetT1BaudRateX4()				PCON0=PCON0&(~SMOD1);S0CFG=S0CFG|(SMOD2)
#define UART0_SetT1BaudRateX8()				PCON0=PCON0|(SMOD1);S0CFG=S0CFG|(SMOD2)

#define UART0_SetT2BaudRateX1()				S0CFG=S0CFG&(~SMOD2)
#define UART0_SetT2BaudRateX2()				PCON0=PCON0&(~SMOD1);S0CFG=S0CFG|(SMOD2)
#define UART0_SetT2BaudRateX4()				PCON0=PCON0|(SMOD1);S0CFG=S0CFG|(SMOD2)

#define UART0_SetS0BRGBaudRateX1()			PCON0=PCON0&(~SMOD1);S0CFG=S0CFG&(~SMOD2)
#define UART0_SetS0BRGBaudRateX2()			PCON0=PCON0&(~SMOD1);S0CFG=S0CFG|(SMOD2)

#define UART0_SetS1BRGBaudRateX1()			PCON0=PCON0&(~SMOD1);S0CFG=S0CFG&(~SMOD2)
#define UART0_SetS1BRGBaudRateX2()			PCON0=PCON0|(SMOD1);S0CFG=S0CFG&(~SMOD2)
#define UART0_SetS1BRGBaudRateX4()			PCON0=PCON0&(~SMOD1);S0CFG=S0CFG|(SMOD2)
#define UART0_SetS1BRGBaudRateX8()			PCON0=PCON0|(SMOD1);S0CFG=S0CFG|(SMOD2)



// UART0 设置波特率来源
#define UART0_SetBRGFromT1()				S0CR1=S0CR1&(~(S0TCK|S0RCK));SFRPI=1;T2MOD1=T2MOD1&(~TL2IS);SFRPI=0;RCLK_TF2L=0;TCLK_TL2IE=0;S0CFG=S0CFG&(~URTS)		
#define UART0_SetBRGFromT2()				S0CR1=S0CR1&(~(S0TCK|S0RCK));SFRPI=1;T2MOD1=T2MOD1&(~TL2IS);SFRPI=0;RCLK_TF2L=1;TCLK_TL2IE=1
#define UART0_SetBRGFromS0BRG()				S0CR1=S0CR1|(S0TCK|S0RCK)		
#define UART0_SetBRGFromS1BRG()				S0CR1=S0CR1&(~(S0TCK|S0RCK));SFRPI=1;T2MOD1=T2MOD1&(~TL2IS);SFRPI=0;RCLK_TF2L=0;TCLK_TL2IE=0;S0CFG=S0CFG|(URTS)

// UART0 设置IO口
#define UART0_SetRxTxP30P31()				SFRPI=7;AUXR10=AUXR10&(~S0PS1);SFRPI=0;	AUXR3=AUXR3&(~S0PS0)
#define UART0_SetRxTxP44P45()				SFRPI=7;AUXR10=AUXR10&(~S0PS1);SFRPI=0;	AUXR3=AUXR3|(S0PS0)
#define UART0_SetRxTxP31P30()				SFRPI=7;AUXR10=AUXR10|(S0PS1);SFRPI=0;	AUXR3=AUXR3&(~S0PS0)
#define UART0_SetRxTxP70P71()				SFRPI=7;AUXR10=AUXR10|(S0PS1);SFRPI=0;	AUXR3=AUXR3|(S0PS0)


// UART 设置UART0/UART1 SPI模式下MISO所用的IO口
#define UART_SetS0S1MISOP16P61()			SFRPI=3;AUXR6=AUXR6&(~SnMIPS);SFRPI=0				
#define UART_SetS0S1MISOP33P47()			SFRPI=3;AUXR6=AUXR6|(SnMIPS);SFRPI=0				

// UART0 开启/关闭S0BRG波特率发生器
#define UART0_EnS0BRG()						S0CR1=S0CR1|S0TR
#define UART0_DisS0BRG()					S0CR1=S0CR1&(~S0TR)

// UART0 设置S0BRG时钟源
#define UART0_SetS0BRGSelSYSCLK()			S0CR1=S0CR1|(S0TX12)
#define UART0_SetS0BRGSelSYSCLKdiv12()		S0CR1=S0CR1&(~S0TX12)

// UART0 设置S0BRG Value
#define UART0_SetS0BRGValue(x)				S1BRT=x;S1BRC=x

// UART0 开启/关闭S0CKO 输出
#define UART0_EnS0CKO()						S0CR1=S0CR1|S0CKOE
#define UART0_DisS0CKO()					S0CR1=S0CR1&(~S0CKOE)

// UART0 选择S0CKO 输出引脚
#define UART0_SetS0CKOP33()					SFRPI=3;AUXR6=AUXR6|S0COPS;SFRPI=0
#define UART0_SetS0CKOP47()					SFRPI=3;AUXR6=AUXR6&(~S0COPS);SFRPI=0

// UART0 开启/关闭自动重发
#define UART0_EnAutoRepeatTrans()			S0CR1=S0CR1|ARTE
#define UART0_DisAutoRepeatTrans()			S0CR1=S0CR1&(~ARTE)

// UART0 设置SFR 0xB9地址是访问SADEN还是S0CR1
#define UART0_SetAccess_SADEN()				S0CFG=S0CFG&(~SMOD3)
#define UART0_SetAccess_S0CR1()				S0CFG=S0CFG|(SMOD3)

// UART0 设置SPI MASTER的SPI速度
#define UART0_SetSPIClock_SYSCLKDiv4()		S0CFG=S0CFG|(URM0X3)
#define UART0_SetSPIClock_SYSCLKDiv12()		S0CFG=S0CFG&(~URM0X3)

// UART0 设置SPI MASTER的S0MI（MISO）引脚
#define UART0_SetMISOP16()					SFRPI=3;AUXR6=AUXR6&(~SnMIPS);SFRPI=0
#define UART0_SetMISOP33()					SFRPI=3;AUXR6=AUXR6|SnMIPS;SFRPI=0

// UART0 设置LIN TX/RX
#define UART0_LIN_SetTX()					S0CFG1=S0CFG1|(TXRX0)
#define UART0_LIN_SetRX()					S0CFG1=S0CFG1&(~TXRX0)

// UART0 LIN TX发送同步信号,RX等待接收SYNC
#define UART0_LIN_SendSYNC()				S0CFG1=S0CFG1|(SYNC0)
#define UART0_LIN_WaitSYNC()				S0CFG1=S0CFG1|(SYNC0)

// UART0 LIN TX SYNC 13bi/16Bit
#define UART0_LIN_TxSYNC_16Bit()			S0CFG1=S0CFG1|(S0SB16)
#define UART0_LIN_TxSYNC_13Bit()			S0CFG1=S0CFG1&(~S0SB16)

// UART0 LIN RX 自动波特率调整
#define UART0_LIN_AutoBR()					S0CFG1=S0CFG1|(ATBR0)
#define UART0_LIN_DisAutoBR()				S0CFG1=S0CFG1&(~ATBR0)

// UART0 LIN 清标志位
#define UART0_LIN_ClrSBF0()					S0CFG1=S0CFG1|(SBF0)
#define UART0_LIN_ClrTXER0()				S0CFG1=S0CFG1|(TXER0)
/*
*************************************************************************************
*/ 

/*
*************************************************************************************
*  UART1 
*
*************************************************************************************
*/
// UART1 设置UART1工作模式
#define UART1_SetModeShiftReg()				SFRPI=1;S1CFG=S1CFG&(~SM31);SM01=0;SM11=0;SFRPI=0
#define UART1_SetMode8bitUARTVar()			SFRPI=1;S1CFG=S1CFG&(~SM31);SM01=0;SM11=1;SFRPI=0
#define UART1_SetMode9bitUART()				SFRPI=1;S1CFG=S1CFG&(~SM31);SM01=1;SM11=0;SFRPI=0
#define UART1_SetMode9bitUARTVar()			SFRPI=1;S1CFG=S1CFG&(~SM31);SM01=1;SM11=1;SFRPI=0
#define UART1_SetModeSPIMaster()			SFRPI=1;S1CFG=S1CFG|(SM31);SM01=0;SM11=0;SFRPI=0
#define UART1_SetMode8BitTimer()			SFRPI=1;S1CFG=S1CFG|(S1TME);S1CFG=S1CFG&(~S1DOR);SFRPI=0
#define UART1_SetMode16BitTimer()			SFRPI=1;S1CFG=S1CFG|(S1TME);S1CFG=S1CFG|(S1DOR);SFRPI=0


// UART1 开启/关闭接收数据功能
#define UART1_EnReception()					SFRPI=1;REN1=1;SFRPI=0
#define UART1_DisReception()				SFRPI=1;REN1=0;SFRPI=0

// UART1 设置数据位序(MSB:高位在前,LSB:低位在前)
#define UART1_SetDataMSB()					SFRPI=1;S1CFG=S1CFG&(~S1DOR);SFRPI=0
#define UART1_SetDataLSB()					SFRPI=1;S1CFG=S1CFG|(S1DOR);SFRPI=0

// UART1 设置波特率加倍设置 
#define UART1_SetBaudRateX1()				SFRPI=1;S1CFG=S1CFG&(~S1MOD1);SFRPI=0
#define UART1_SetBaudRateX2()				SFRPI=1;S1CFG=S1CFG|(S1MOD1);SFRPI=0

// UART1 开启/关闭波特率发生器
#define UART1_EnS1BRG()						SFRPI=1;S1CFG=S1CFG|(S1TR);SFRPI=0
#define UART1_DisS1BRG()					SFRPI=1;S1CFG=S1CFG&(~S1TR);SFRPI=0

// UART1 设置S1BRG时钟源
#define UART1_SetS1BRGSelSYSCLK()			SFRPI=1;S1CFG=S1CFG|(S1TX12);SFRPI=0
#define UART1_SetS1BRGSelSYSCLKdiv12()		SFRPI=1;S1CFG=S1CFG&(~S1TX12);SFRPI=0

// UART1 设置S1BRG Value
#define UART1_SetS1BRGValue(x)				SFRPI=1;S1BRT=x;S1BRC=x;SFRPI=0

// UART1 选择UART1 所用的IO口
#define UART1_SetRxTxP12P13()				SFRPI=6;AUXR9=AUXR9&(~(S1PS1|S1PS0));SFRPI=0
#define UART1_SetRxTxP71P70()				SFRPI=6;AUXR9=(AUXR9&(~(S1PS1|S1PS0)))|S1PS0;SFRPI=0
#define UART1_SetRxTxP20P21()				SFRPI=6;AUXR9=(AUXR9&(~(S1PS1|S1PS0)))|S1PS1;SFRPI=0
#define UART1_SetRxTxP34P35()				SFRPI=6;AUXR9=AUXR9|(S1PS1|S1PS0);SFRPI=0

// UART1 设置SPI MASTER的SPI速度
#define UART1_SetSPIClock_SYSCLKDiv4()		SFRPI=1;S1CFG=S1CFG|(S1M0X3);SFRPI=0
#define UART1_SetSPIClock_SYSCLKDiv12()		SFRPI=1;S1CFG=S1CFG&(~S1M0X3);SFRPI=0

// UART1 设置SPI MASTER的S1MI（MISO）引脚
#define UART1_SetMISOP61()					SFRPI=3;AUXR6=AUXR6&(~SnMIPS);SFRPI=0
#define UART1_SetMISOP47()					SFRPI=3;AUXR6=AUXR6|SnMIPS;SFRPI=0

/*
*************************************************************************************
*  UART2 
*
*************************************************************************************
*/
// UART2 设置UART2工作模式
#define UART2_SetModeShiftReg()				SFRPI=3;S2CFG=S2CFG&(~SM32);SM02=0;SM12=0;SFRPI=0
#define UART2_SetMode8bitUARTVar()			SFRPI=3;S2CFG=S2CFG&(~SM32);SM02=0;SM12=1;SFRPI=0
#define UART2_SetMode9bitUART()				SFRPI=3;S2CFG=S2CFG&(~SM32);SM02=1;SM12=0;SFRPI=0
#define UART2_SetMode9bitUARTVar()			SFRPI=3;S2CFG=S2CFG&(~SM32);SM02=1;SM12=1;SFRPI=0
#define UART2_SetModeSPIMaster()			SFRPI=3;S2CFG=S2CFG|(SM32);SM02=0;SM12=0;SFRPI=0
#define UART2_SetMode8BitTimer()			SFRPI=3;S2CFG=S2CFG|(S2TME);S2CFG=S2CFG&(~S2DOR);SFRPI=0
#define UART2_SetMode16BitTimer()			SFRPI=3;S2CFG=S2CFG|(S2TME);S2CFG=S2CFG|(S2DOR);SFRPI=0

// UART2 开启/关闭接收数据功能
#define UART2_EnReception()					SFRPI=3;REN2=1;SFRPI=0
#define UART2_DisReception()				SFRPI=3;REN2=0;SFRPI=0

// UART2 设置数据位序(MSB:高位在前,LSB:低位在前)
#define UART2_SetDataMSB()					SFRPI=3;S2CFG=S2CFG&(~S2DOR);SFRPI=0
#define UART2_SetDataLSB()					SFRPI=3;S2CFG=S2CFG|(S2DOR);SFRPI=0

// UART2 设置波特率加倍设置 
#define UART2_SetBaudRateX1()				SFRPI=3;S2CFG=S2CFG&(~S2MOD1);SFRPI=0
#define UART2_SetBaudRateX2()				SFRPI=3;S2CFG=S2CFG|(S2MOD1);SFRPI=0

// UART2 开启/关闭波特率发生器
#define UART2_EnS2BRG()						SFRPI=3;S2CFG=S2CFG|(S2TR);SFRPI=0
#define UART2_DisS2BRG()					SFRPI=3;S2CFG=S2CFG&(~S2TR);SFRPI=0

// UART2 设置S2BRG时钟源
#define UART2_SetS2BRGSelSYSCLK()			SFRPI=3;S2CFG=S2CFG|(S2TX12);SFRPI=0
#define UART2_SetS2BRGSelSYSCLKdiv12()		SFRPI=3;S2CFG=S2CFG&(~S2TX12);SFRPI=0

// UART2 设置S2BRG Value
#define UART2_SetS2BRGValue(x)				SFRPI=3;S2BRT=x;S2BRC=x;SFRPI=0

// UART2 选择UART2 所用的IO口
#define UART2_SetRxTxP32P33()				SFRPI=10;AUXR13=AUXR13&(~S2PS0);SFRPI=0
#define UART2_SetRxTxP67P57()				SFRPI=10;AUXR13=AUXR13|S2PS0;SFRPI=0

// UART2 设置SPI MASTER的SPI速度
#define UART2_SetSPIClock_SYSCLKDiv4()		SFRPI=3;S2CFG=S2CFG|(S2M0X3);SFRPI=0
#define UART2_SetSPIClock_SYSCLKDiv12()		SFRPI=3;S2CFG=S2CFG&(~S2M0X3);SFRPI=0

// UART2 设置SPI MASTER的S2MI（MISO）引脚
#define UART2_SetMISOP70()					SFRPI=3;AUXR6=AUXR6&(~SnMIPS);SFRPI=0
#define UART2_SetMISOP66()					SFRPI=3;AUXR6=AUXR6|SnMIPS;SFRPI=0

/*
*************************************************************************************
*  UART3 
*
*************************************************************************************
*/
// UART3 设置UART3工作模式
#define UART3_SetModeShiftReg()				SFRPI=4;S3CFG=S3CFG&(~SM33);SM03=0;SM13=0;SFRPI=0
#define UART3_SetMode8bitUARTVar()			SFRPI=4;S3CFG=S3CFG&(~SM33);SM03=0;SM13=1;SFRPI=0
#define UART3_SetMode9bitUART()				SFRPI=4;S3CFG=S3CFG&(~SM33);SM03=1;SM13=0;SFRPI=0
#define UART3_SetMode9bitUARTVar()			SFRPI=4;S3CFG=S3CFG&(~SM33);SM03=1;SM13=1;SFRPI=0
#define UART3_SetModeSPIMaster()			SFRPI=4;S3CFG=S3CFG|(SM33);SM03=0;SM13=0;SFRPI=0
#define UART3_SetMode8BitTimer()			SFRPI=4;S3CFG=S3CFG|(S3TME);S3CFG=S3CFG&(~S3DOR);SFRPI=0
#define UART3_SetMode16BitTimer()			SFRPI=4;S3CFG=S3CFG|(S3TME);S3CFG=S3CFG|(S3DOR);SFRPI=0

// UART3 开启/关闭接收数据功能
#define UART3_EnReception()					SFRPI=4;REN3=1;SFRPI=0
#define UART3_DisReception()				SFRPI=4;REN3=0;SFRPI=0

// UART3 设置数据位序(MSB:高位在前,LSB:低位在前)
#define UART3_SetDataMSB()					SFRPI=4;S3CFG=S3CFG&(~S3DOR);SFRPI=0
#define UART3_SetDataLSB()					SFRPI=4;S3CFG=S3CFG|(S3DOR);SFRPI=0

// UART3 设置波特率加倍设置 
#define UART3_SetBaudRateX1()				SFRPI=4;S3CFG=S3CFG&(~S3MOD1);SFRPI=0
#define UART3_SetBaudRateX2()				SFRPI=4;S3CFG=S3CFG|(S3MOD1);SFRPI=0

// UART3 开启/关闭波特率发生器
#define UART3_EnS3BRG()						SFRPI=4;S3CFG=S3CFG|(S3TR);SFRPI=0
#define UART3_DisS3BRG()					SFRPI=4;S3CFG=S3CFG&(~S3TR);SFRPI=0

// UART3 设置S3BRG时钟源
#define UART3_SetS3BRGSelSYSCLK()			SFRPI=4;S3CFG=S3CFG|(S3TX12);SFRPI=0
#define UART3_SetS3BRGSelSYSCLKdiv12()		SFRPI=4;S3CFG=S3CFG&(~S3TX12);SFRPI=0

// UART3 设置S3BRG Value
#define UART3_SetS3BRGValue(x)				SFRPI=4;S3BRT=x;S3BRC=x;SFRPI=0

// UART3 选择UART3 所用的IO口
#define UART3_SetRxTxP36P37()				SFRPI=10;AUXR13=AUXR13&(~S3PS0);SFRPI=0
#define UART3_SetRxTxP65P66()				SFRPI=10;AUXR13=AUXR13|S3PS0;SFRPI=0

// UART3 设置SPI MASTER的SPI速度
#define UART3_SetSPIClock_SYSCLKDiv4()		SFRPI=4;S3CFG=S3CFG|(S3M0X3);SFRPI=0
#define UART3_SetSPIClock_SYSCLKDiv12()		SFRPI=4;S3CFG=S3CFG&(~S3M0X3);SFRPI=0

// UART3 设置SPI MASTER的S3MI（MISO）引脚
#define UART3_SetMISOP71()					SFRPI=3;AUXR6=AUXR6&(~SnMIPS);SFRPI=0
#define UART3_SetMISOP65()					SFRPI=3;AUXR6=AUXR6|SnMIPS;SFRPI=0

/*
*************************************************************************************
*/ 

/*
*************************************************************************************
*  ADC
*
*************************************************************************************
*/
#define ADC_AIN0 0x01
#define ADC_AIN1 0x02
#define ADC_AIN2 0x04
#define ADC_AIN3 0x08
#define ADC_AIN4 0x10
#define ADC_AIN5 0x20
#define ADC_AIN6 0x40
#define ADC_AIN7 0x80
#define ADC_AIN8 0x01
#define ADC_AIN9 0x02
#define ADC_AIN10 0x04
#define ADC_AIN11 0x08
#define ADC_AIN12 0x10
#define ADC_AIN13 0x20
#define ADC_AIN14 0x40
#define ADC_AIN15 0x80

// 开启或关闭ADC
#define ADC_Disable()						ADCON0=ADCON0&(~ADCEN)					
#define ADC_Enable()						ADCON0=ADCON0|(ADCEN)

// 开启或关闭ADC
#define ADC_SoftStart()						ADCON0=ADCON0|(ADCS)					

// 设置ADC的转换时钟源
#define ADC_SetClock_SYSCLK()				ADCFG0=ADCFG0&(~(ADCKS0|ADCKS1|ADCKS2))
#define ADC_SetClock_SYSCLKDiv2()			ADCFG0=(ADCFG0&(~(ADCKS0|ADCKS1|ADCKS2)))|(ADCKS0)
#define ADC_SetClock_SYSCLKDiv4()			ADCFG0=(ADCFG0&(~(ADCKS0|ADCKS1|ADCKS2)))|(ADCKS1)
#define ADC_SetClock_SYSCLKDiv8()			ADCFG0=(ADCFG0&(~(ADCKS0|ADCKS1|ADCKS2)))|(ADCKS0|ADCKS1)
#define ADC_SetClock_SYSCLKDiv16()			ADCFG0=(ADCFG0&(~(ADCKS0|ADCKS1|ADCKS2)))|(ADCKS2)
#define ADC_SetClock_SYSCLKDiv32()			ADCFG0=(ADCFG0&(~(ADCKS0|ADCKS1|ADCKS2)))|(ADCKS0|ADCKS2)
#define ADC_SetClock_S0TOFDiv2()			ADCFG0=(ADCFG0&(~(ADCKS0|ADCKS1|ADCKS2)))|(ADCKS1|ADCKS2)
#define ADC_SetClock_T2OFDiv2()				ADCFG0=ADCFG0|(ADCKS0|ADCKS1|ADCKS2)

// 设置ADC的转换通道
#define ADC_SetChannel_AIN0()				ADCFG0=ADCFG0&(~ACHS);ADCON0=(ADCON0&(~(CHS3|CHS2|CHS1|CHS0)))
#define ADC_SetChannel_AIN1()				ADCFG0=ADCFG0&(~ACHS);ADCON0=(ADCON0&(~(CHS3|CHS2|CHS1|CHS0)))|(CHS0)
#define ADC_SetChannel_AIN2()				ADCFG0=ADCFG0&(~ACHS);ADCON0=(ADCON0&(~(CHS3|CHS2|CHS1|CHS0)))|(CHS1)
#define ADC_SetChannel_AIN3()				ADCFG0=ADCFG0&(~ACHS);ADCON0=(ADCON0&(~(CHS3|CHS2|CHS1|CHS0)))|(CHS1|CHS0)
#define ADC_SetChannel_AIN4()				ADCFG0=ADCFG0&(~ACHS);ADCON0=(ADCON0&(~(CHS3|CHS2|CHS1|CHS0)))|(CHS2)
#define ADC_SetChannel_AIN5()				ADCFG0=ADCFG0&(~ACHS);ADCON0=(ADCON0&(~(CHS3|CHS2|CHS1|CHS0)))|(CHS2|CHS0)
#define ADC_SetChannel_AIN6()				ADCFG0=ADCFG0&(~ACHS);ADCON0=(ADCON0&(~(CHS3|CHS2|CHS1|CHS0)))|(CHS2|CHS1)
#define ADC_SetChannel_AIN7()				ADCFG0=ADCFG0&(~ACHS);ADCON0=(ADCON0&(~(CHS3|CHS2|CHS1|CHS0)))|(CHS2|CHS1|CHS0)
#define ADC_SetChannel_AIN8()				ADCFG0=ADCFG0&(~ACHS);ADCON0=(ADCON0&(~(CHS3|CHS2|CHS1|CHS0)))|(CHS3)
#define ADC_SetChannel_AIN9()				ADCFG0=ADCFG0&(~ACHS);ADCON0=(ADCON0&(~(CHS3|CHS2|CHS1|CHS0)))|(CHS3|CHS0)
#define ADC_SetChannel_AIN10()				ADCFG0=ADCFG0&(~ACHS);ADCON0=(ADCON0&(~(CHS3|CHS2|CHS1|CHS0)))|(CHS3|CHS1)
#define ADC_SetChannel_AIN11()				ADCFG0=ADCFG0&(~ACHS);ADCON0=(ADCON0&(~(CHS3|CHS2|CHS1|CHS0)))|(CHS3|CHS1|CHS0)
#define ADC_SetChannel_AIN12()				ADCFG0=ADCFG0&(~ACHS);ADCON0=(ADCON0&(~(CHS3|CHS2|CHS1|CHS0)))|(CHS3|CHS2)
#define ADC_SetChannel_AIN13()				ADCFG0=ADCFG0&(~ACHS);ADCON0=(ADCON0&(~(CHS3|CHS2|CHS1|CHS0)))|(CHS3|CHS2|CHS0)
#define ADC_SetChannel_AIN14()				ADCFG0=ADCFG0&(~ACHS);ADCON0=(ADCON0&(~(CHS3|CHS2|CHS1|CHS0)))|(CHS3|CHS2|CHS1)
#define ADC_SetChannel_AIN15()				ADCFG0=ADCFG0&(~ACHS);ADCON0=(ADCON0&(~(CHS3|CHS2|CHS1|CHS0)))|(CHS3|CHS2|CHS1|CHS0)
#define ADC_SetChannel_IVR14()				ADCFG0=ADCFG0|(ACHS);ADCON0=(ADCON0&(~(CHS3|CHS2|CHS1|CHS0)))
#define ADC_SetChannel_AVSS()				ADCFG0=ADCFG0|(ACHS);ADCON0=(ADCON0&(~(CHS3|CHS2|CHS1|CHS0)))|(CHS0)

// 设置ADC的转换模式
#define ADC_SetMode_SetADCS()				SFRPI=4;ADCFG4=(ADCFG4&(~(ADTM3|ADTM2)));SFRPI=0;ADCFG0=ADCFG0&(~(ADTM0|ADTM1))
#define ADC_SetMode_T0Overflow()			SFRPI=4;ADCFG4=(ADCFG4&(~(ADTM3|ADTM2)));SFRPI=0;ADCFG0=(ADCFG0&(~(ADTM0|ADTM1)))|(ADTM0)
#define ADC_SetMode_FreeRunning()			SFRPI=4;ADCFG4=(ADCFG4&(~(ADTM3|ADTM2)));SFRPI=0;ADCFG0=(ADCFG0&(~(ADTM0|ADTM1)))|(ADTM1)
#define ADC_SetMode_S0BRGOverflow()			SFRPI=4;ADCFG4=(ADCFG4&(~(ADTM3|ADTM2)));SFRPI=0;ADCFG0=ADCFG0|(ADTM0|ADTM1)
#define ADC_SetMode_KBIET()					SFRPI=4;ADCFG4=(ADCFG4&(~(ADTM3|ADTM2)))|(ADTM2);SFRPI=0;ADCFG0=ADCFG0&(~(ADTM0|ADTM1))
#define ADC_SetMode_INT1ET()				SFRPI=4;ADCFG4=(ADCFG4&(~(ADTM3|ADTM2)))|(ADTM2);SFRPI=0;ADCFG0=(ADCFG0&(~(ADTM0|ADTM1)))|(ADTM0)
#define ADC_SetMode_INT2ET()				SFRPI=4;ADCFG4=(ADCFG4&(~(ADTM3|ADTM2)))|(ADTM2);SFRPI=0;ADCFG0=(ADCFG0&(~(ADTM0|ADTM1)))|(ADTM1)
#define ADC_SetMode_INT3ET()				SFRPI=4;ADCFG4=(ADCFG4&(~(ADTM3|ADTM2)))|(ADTM2);SFRPI=0;ADCFG0=(ADCFG0&(~(ADTM0|ADTM1)))|(ADTM1|ADTM0)
#define ADC_SetMode_T2EXES()				SFRPI=4;ADCFG4=(ADCFG4&(~(ADTM3|ADTM2)))|(ADTM3);SFRPI=0;ADCFG0=ADCFG0&(~(ADTM0|ADTM1))
#define ADC_SetMode_AC0ES()					SFRPI=4;ADCFG4=(ADCFG4&(~(ADTM3|ADTM2)))|(ADTM3);SFRPI=0;ADCFG0=(ADCFG0&(~(ADTM0|ADTM1)))|(ADTM0)
#define ADC_SetMode_T3EXES()				SFRPI=4;ADCFG4=(ADCFG4&(~(ADTM3|ADTM2)))|(ADTM3);SFRPI=0;ADCFG0=(ADCFG0&(~(ADTM0|ADTM1)))|(ADTM1)
#define ADC_SetMode_AC1ES()					SFRPI=4;ADCFG4=(ADCFG4&(~(ADTM3|ADTM2)))|(ADTM3);SFRPI=0;ADCFG0=(ADCFG0&(~(ADTM0|ADTM1)))|(ADTM1|ADTM0)
#define ADC_SetMode_PCA0Overflow()			SFRPI=4;ADCFG4=(ADCFG4&(~(ADTM3|ADTM2)))|(ADTM3|ADTM2);SFRPI=0;ADCFG0=(ADCFG0&(~(ADTM0|ADTM1)))
#define ADC_SetMode_C0CMP6()				SFRPI=4;ADCFG4=(ADCFG4&(~(ADTM3|ADTM2)))|(ADTM3|ADTM2);SFRPI=0;ADCFG0=(ADCFG0&(~(ADTM0|ADTM1)))|(ADTM0)
#define ADC_SetMode_C0CMP6orC0CMP7()		SFRPI=4;ADCFG4=(ADCFG4&(~(ADTM3|ADTM2)))|(ADTM3|ADTM2);SFRPI=0;ADCFG0=(ADCFG0&(~(ADTM0|ADTM1)))|(ADTM1)
#define ADC_SetMode_BM0FES()				SFRPI=4;ADCFG4=(ADCFG4|(ADTM3|ADTM2));SFRPI=0;ADCFG0=(ADCFG0|(ADTM1|ADTM0))

// 设置ADC值左对齐(ADCDH[7:0]ADCDL[7:4])/右对齐(ADCDH[3:0]ADCDL[7:0])
#define ADC_SetLeftJustified()				ADCFG0=ADCFG0&(~ADRJ)					
#define ADC_SetRightJustified()				ADCFG0=ADCFG0|(ADRJ)					

//	设置DMA的ADC值数据包大小
#define ADC_SetDMADataSize_1Byte()			SFRPI=4;ADCFG4=ADCFG4|DBSD;SFRPI=0
#define ADC_SetDMADataSize_2Byte()			SFRPI=4;ADCFG4=ADCFG4&(~DBSD);SFRPI=0

// 设置DMA的ADC值数据先后ADCDL先/ADCDH先
#define ADC_SetDMADataADCDLFirst()		SFRPI=4;ADCFG4=ADCFG4&(~(DBSD|DOSD));SFRPI=0
#define ADC_SetDMADataADCDHFirst()		SFRPI=4;ADCFG4=(ADCFG4&(~(DBSD)))|DOSD;SFRPI=0

// 设置ADC的采样保持时间
#define ADC_SetSampleHoldTime(x)		SFRPI=2;ADCFG2=x;SFRPI=0	// SHT=x

// 设置ADC的转换偏移值
#define ADC_SetOffsetAdd(x)				SFRPI=1;ADCFG1=(ADCFG1&(~(0x1F)))|(x);SFRPI=0	// ADC值+x (x=0~15)
#define ADC_SetOffsetSub(x)				SFRPI=1;ADCFG1=(ADCFG1&(~(0x1F)))|((~x));SFRPI=0	// ADC值-x (x=1~16)

// 设置ADC在范围内还是在范围外置位 ADCWI
#define ADC_SetWindow_WithIn()			SFRPI=0x04;ADCFG4=ADCFG4&(~ADWM0);SFRPI=0
#define ADC_SetWindow_OutSide()			SFRPI=0x04;ADCFG4=ADCFG4|(ADWM0);SFRPI=0

// 设置ADC的窗口值
#define ADC_SetWindow_High(x)			SFRPI=0x0B;ADCFG11=(((x&0x0F)<<4)|0x0F)	;SFRPI=0x0C;ADCFG12=((x>>4)&0xFF);SFRPI=0
#define ADC_SetWindow_Low(x)			SFRPI=0x0D;ADCFG13=(((x&0x0F)<<4)&0xF0)	;SFRPI=0x0E;ADCFG14=((x>>4)&0xFF);SFRPI=0

// 设置ADC 的数据位数
#define ADC_SetADCData_12Bit()			SFRPI=0x03;ADCFG3=ADCFG3&(~(ARES1|ARES0));SFRPI=0
#define ADC_SetADCData_10Bit()			SFRPI=0x03;ADCFG3=(ADCFG3&(~(ARES1|ARES0)))|(ARES0);SFRPI=0
#define ADC_SetADCData_8Bit()			SFRPI=0x03;ADCFG3=(ADCFG3&(~(ARES1|ARES0)))|(ARES1);SFRPI=0

// 设置ADC 通道自动扫描
#define ADC_SetChannelAutoScan(x)		SFRPI=0x05;ADCFG5=(LOBYTE(x));SFRPI=0x06;ADCFG6=(HIBYTE(x));SFRPI=0

// 设置ADC完成标志位ADCI是否产生中断
#define ADC_EnInterrupt_ADCI()			SFRPI=1;ADCFG1=ADCFG1&(~IGADCI);SFRPI=0
#define ADC_DisInterrupt_ADCI()			SFRPI=1;ADCFG1=ADCFG1|(IGADCI);SFRPI=0

// 设置ADC窗口标志位ADCWI是否产生中断
#define ADC_DisInterrupt_ADCWI()		SFRPI=1;ADCFG1=ADCFG1&(~EADCWI);SFRPI=0
#define ADC_EnInterrupt_ADCWI()			SFRPI=1;ADCFG1=ADCFG1|(EADCWI);SFRPI=0

// 设置ADC采样完成标志位SFPF是否产生中断
#define ADC_DisInterrupt_SMPF()			SFRPI=1;ADCFG1=ADCFG1&(~SMPFIE);SFRPI=0
#define ADC_ENInterrupt_SMPF()			SFRPI=1;ADCFG1=ADCFG1|(SMPFIE);SFRPI=0

#define ADC_ClrInterruptFlag()			ADCON0=ADCON0&(~ADCI)

/*
*************************************************************************************
*/ 

/*
*************************************************************************************
*  IVREF
*
*************************************************************************************
*/
#define IVREF_Enable()						ISPCR=0x80;IFADRH=0x0;IFADRL=PCON3_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0xEF))|(IVREN_P);IFADRH=0x0;IFADRL=CKCON3_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0		
#define IVREF_Disable()						ISPCR=0x80;IFADRH=0x0;IFADRL=PCON3_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0xEF));IFADRH=0x0;IFADRL=CKCON3_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0		

/*
*************************************************************************************
*/ 

/*
*************************************************************************************
*  SPI 
*
*************************************************************************************
*/
// 开启或关闭SPI
#define SPI_Disable()						SPCON=SPCON&(~SPEN)					
#define SPI_Enable()						SPCON=SPCON|(SPEN)

// 开启或关闭QPI
#define SPI_DisQPI()						SPSTAT=SPSTAT&(~QPIEN)					
#define SPI_EnQPI()							SPSTAT=SPSTAT|(QPIEN)

// 开启或关闭菊花链
#define SPI_DisDaisyChain()					SFRPI=4;AUXR7=AUXR7&(~SPI0M0);SFRPI=0					
#define SPI_EnDaisyChain()					SFRPI=4;AUXR7=AUXR7|SPI0M0;SFRPI=0	

// 选择MASTER/SLAVE
#define SPI_SelectMASTERByMSTRbit()			SPCON=SPCON|(SSIG|MSTR)					
#define SPI_SelectSLAVEByMSTRbit()			SPCON=(SPCON|(SSIG))&(~MSTR)						
#define SPI_SelectMASTERSLAVEBynSSpin()		SPCON=SPCON&(~SSIG)

// 选择SPICLK 初始电平 0:SPICLK  初始为低电平 1: SPICLK  初始为低电平
#define SPI_SetCPOL_0()						SPCON=SPCON&(~CPOL)
#define SPI_SetCPOL_1()						SPCON=SPCON|(CPOL)

// 选择数据传送顺序	0:高位在先 1: 低位在先
#define SPI_SetDataMSB()					SPCON=SPCON&(~DORD)
#define SPI_SetDataLSB()					SPCON=SPCON|(DORD)

// 选择SPI 的传送速度
#define SPI_CLK_SYSCLK_4	0	// SYSCLK/4
#define SPI_CLK_SYSCLK_8	1	// SYSCLK/8
#define SPI_CLK_SYSCLK_16	2	// SYSCLK/16
#define SPI_CLK_SYSCLK_32	3	// SYSCLK/32
#define SPI_CLK_SYSCLK_64	4	// SYSCLK/64
#define SPI_CLK_SYSCLK_2	5	// SYSCLK/2
#define SPI_CLK_S0TOF_6		6	// S0TOF/6	
#define SPI_CLK_T0OF_6		7	// T0OF/6	

#define SPI_SetClock(x)						SPCON=(SPCON&B11111100)|(x&0x03);SPSTAT=(x>>2)

// 选择数据采样模式	0:前沿采样,后沿改变数据 1: 前沿改变数据,后沿采样
#define SPI_SetCPHA_0()						SPCON=SPCON&(~CPHA)
#define SPI_SetCPHA_1()						SPCON=SPCON|(CPHA)

// 选择SPI 所用的IO口 nSS/MOSI/MISO/SPICLK/SIO2/SIO3
#define SPI_SetUseP14P15P16P17P53P54()			SFRPI=7;AUXR10=AUXR10&(~(SPIPS0|SPIPS1));SFRPI=0
#define SPI_SetUseP43P42P41P40P36P37()			SFRPI=7;AUXR10=(AUXR10&(~(SPIPS0|SPIPS1)))|SPIPS0;SFRPI=0
#define SPI_SetUseP40P41P42P43P65P57()			SFRPI=7;AUXR10=(AUXR10&(~(SPIPS0|SPIPS1)))|SPIPS1;SFRPI=0
#define SPI_SetUseP43P41P42P40P65P57()			SFRPI=7;AUXR10=AUXR10|(SPIPS0|SPIPS1);SFRPI=0

// SPI 清/检测SPI完成标志
#define SPI_ClearCompleteFlag()		SPSTAT=SPSTAT|SPIF
#define SPI_ChkCompleteFlag()		(SPSTAT&SPIF)

// SPI自动清除SPIF使能
#define SPI_DisAutoSPIFClear()		SFRPI=7;AUXR10=AUXR10&(~SPFACE);SFRPI=0	
#define SPI_EnAutoSPIFClear()		SFRPI=7;AUXR10=AUXR10|SPFACE;SFRPI=0	

// SPI 发送/接收数据
#define SPI_SendData(x)		SPDAT=x
#define SPI_GetData()		SPDAT

/*
*************************************************************************************
*/ 

/*
*************************************************************************************
*  PCA
*
*************************************************************************************
*/
// PCA 开启/关闭计数器
#define	PCA_EnPCACounter()					CR=1
#define	PCA_DisPCACounter()					CR=0

// PCA 设置时钟源, 选择CKMIX16或MCKDO时,必须满足>=SYSCLKx2 & >=CPUCLKx3
#define PCA_SetCLOCK_SYSCLKdiv12()			CMOD=CMOD&(~(CPS2|CPS1|CPS0))
#define PCA_SetCLOCK_SYSCLKdiv2()			CMOD=(CMOD&(~(CPS2|CPS1|CPS0)))|(CPS0)
#define PCA_SetCLOCK_TO()					CMOD=(CMOD&(~(CPS2|CPS1|CPS0)))|(CPS1)
#define PCA_SetCLOCK_ECI()					CMOD=(CMOD&(~(CPS2|CPS1|CPS0)))|(CPS1|CPS0)
#define PCA_SetCLOCK_CKMIX16()				CMOD=(CMOD&(~(CPS2|CPS1|CPS0)))|(CPS2)
#define PCA_SetCLOCK_SYSCLK()				CMOD=(CMOD&(~(CPS2|CPS1|CPS0)))|(CPS2|CPS0)
#define PCA_SetCLOCK_S0BRT()				CMOD=(CMOD&(~(CPS2|CPS1|CPS0)))|(CPS2|CPS1)
#define PCA_SetCLOCK_MCKDO()				CMOD=CMOD|(CPS2|CPS1|CPS0)

// PCA 设置PCA FIFO Data的时钟
#define PCA_SetFDCK_T0OF()					SFRPI=6;AUXR9=AUXR9&(C0FDC1|C0FDC0);SFRPI=0			
#define PCA_SetFDCK_T1OF()					SFRPI=6;AUXR9=(AUXR9&(C0FDC1|C0FDC0))|(C0FDC0);SFRPI=0			
#define PCA_SetFDCK_T3OF()					SFRPI=6;AUXR9=(AUXR9&(C0FDC1|C0FDC0))|(C0FDC1);SFRPI=0			
#define PCA_SetFDCK_S0TOF()					SFRPI=6;AUXR9=AUXR9|(C0FDC1|C0FDC0));SFRPI=0			

// PCA 设置计数器值及其重载值
#define PCA_SetCounter(x)					CH=HIBYTE(x);CL=LOBYTE(x)
#define PCA_SetCounterReload(x)				CHRL=HIBYTE(x);CLRL=LOBYTE(x)


// PCA 设置模式
#define PCA_CH0_SetMode_16BitCaptureRising()	CCAPM0=CAPP0		
#define PCA_CH0_SetMode_16BitCaptureFalling()	CCAPM0=CAPN0		
#define PCA_CH0_SetMode_16BitCaptureDaul()		CCAPM0=CAPP0|CAPN0		
#define PCA_CH0_SetMode_16BitSoftwareTimer()	CCAPM0=ECOM0|MAT0		
#define PCA_CH0_SetMode_16BitHighSpeedOutput()	CCAPM0=ECOM0|MAT0|TOG0		
#define PCA_CH0_SetMode_PWM()					CCAPM0=ECOM0|PWM0	
#define PCA_CH0_SetMode_COPM()					CCAPM0=ECOM0|PWM0|TOG0	
#define PCA_CH0_SetMode_FIFOData()				CCAPM0=ECOM0|CAPN0|PWM0	

#define PCA_CH1_SetMode_16BitCaptureRising()	CCAPM1=CAPP1		
#define PCA_CH1_SetMode_16BitCaptureFalling()	CCAPM1=CAPN1		
#define PCA_CH1_SetMode_16BitCaptureDaul()		CCAPM1=CAPP1|CAPN1		
#define PCA_CH1_SetMode_16BitSoftwareTimer()	CCAPM1=ECOM1|MAT1		
#define PCA_CH1_SetMode_16BitHighSpeedOutput()	CCAPM1=ECOM1|MAT1|TOG1		
#define PCA_CH1_SetMode_PWM()					CCAPM1=ECOM1|PWM1	
#define PCA_CH1_SetMode_COPM()					CCAPM1=ECOM1|PWM1|TOG1	
#define PCA_CH1_SetMode_FIFOData()				CCAPM1=ECOM1|CAPN1|PWM1	

#define PCA_CH2_SetMode_16BitCaptureRising()	CCAPM2=CAPP2		
#define PCA_CH2_SetMode_16BitCaptureFalling()	CCAPM2=CAPN2		
#define PCA_CH2_SetMode_16BitCaptureDaul()		CCAPM2=CAPP2|CAPN2		
#define PCA_CH2_SetMode_16BitSoftwareTimer()	CCAPM2=ECOM2|MAT2		
#define PCA_CH2_SetMode_16BitHighSpeedOutput()	CCAPM2=ECOM2|MAT2|TOG2		
#define PCA_CH2_SetMode_PWM()					CCAPM2=ECOM2|PWM2	
#define PCA_CH2_SetMode_COPM()					CCAPM2=ECOM2|PWM2|TOG2	
#define PCA_CH2_SetMode_FIFOData()				CCAPM2=ECOM2|CAPN2|PWM2	

#define PCA_CH3_SetMode_16BitCaptureRising()	CCAPM3=CAPP3		
#define PCA_CH3_SetMode_16BitCaptureFalling()	CCAPM3=CAPN3		
#define PCA_CH3_SetMode_16BitCaptureDaul()		CCAPM3=CAPP3|CAPN3		
#define PCA_CH3_SetMode_16BitSoftwareTimer()	CCAPM3=ECOM3|MAT3		
#define PCA_CH3_SetMode_16BitHighSpeedOutput()	CCAPM3=ECOM3|MAT3|TOG3		
#define PCA_CH3_SetMode_PWM()					CCAPM3=ECOM3|PWM3	
#define PCA_CH3_SetMode_COPM()					CCAPM3=ECOM3|PWM3|TOG3	
#define PCA_CH3_SetMode_FIFOData()				CCAPM3=ECOM3|CAPN3|PWM3	

#define PCA_CH4_SetMode_16BitCaptureRising()	CCAPM4=CAPP4		
#define PCA_CH4_SetMode_16BitCaptureFalling()	CCAPM4=CAPN4		
#define PCA_CH4_SetMode_16BitCaptureDaul()		CCAPM4=CAPP4|CAPN4		
#define PCA_CH4_SetMode_16BitSoftwareTimer()	CCAPM4=ECOM4|MAT4		
#define PCA_CH4_SetMode_16BitHighSpeedOutput()	CCAPM4=ECOM4|MAT4|TOG4		
#define PCA_CH4_SetMode_PWM()					CCAPM4=ECOM4|PWM4	
#define PCA_CH4_SetMode_COPM()					CCAPM4=ECOM4|PWM4|TOG4	
#define PCA_CH4_SetMode_FIFOData()				CCAPM4=ECOM4|CAPN4|PWM4	

#define PCA_CH5_SetMode_16BitCaptureRising()	CCAPM5=CAPP5		
#define PCA_CH5_SetMode_16BitCaptureFalling()	CCAPM5=CAPN5		
#define PCA_CH5_SetMode_16BitCaptureDaul()		CCAPM5=CAPP5|CAPN5		
#define PCA_CH5_SetMode_16BitSoftwareTimer()	CCAPM5=ECOM5|MAT5		
#define PCA_CH5_SetMode_16BitHighSpeedOutput()	CCAPM5=ECOM5|MAT5|TOG5		
#define PCA_CH5_SetMode_PWM()					CCAPM5=ECOM5|PWM5	
#define PCA_CH5_SetMode_COPM()					CCAPM5=ECOM5|PWM5|TOG5	
#define PCA_CH5_SetMode_FIFOData()				CCAPM5=ECOM5|CAPN5|PWM5	

#define PCA_CH6_SetMode_16BitSoftwareTimer()	SFRPI=1;CCAPM6=ECOM6|MAT6;SFRPI=0		
#define PCA_CH6_SetMode_16BitHighSpeedOutput()	SFRPI=1;CCAPM6=ECOM6|MAT6|TOG6;SFRPI=0		
#define PCA_CH6_SetMode_PWM()					SFRPI=1;CCAPM6=ECOM6|PWM6;SFRPI=0	
#define PCA_CH6_SetMode_COPM()					SFRPI=1;CCAPM6=ECOM6|PWM6|TOG6;SFRPI=0	
#define PCA_CH6_SetMode_FIFOData()				SFRPI=1;CCAPM6=ECOM6|CAPN6|PWM6;SFRPI=0	

#define PCA_CH7_SetMode_16BitSoftwareTimer()	SFRPI=1;CCAPM7=ECOM7|MAT7;SFRPI=0		
#define PCA_CH7_SetMode_16BitHighSpeedOutput()	SFRPI=1;CCAPM7=ECOM7|MAT7|TOG7;SFRPI=0		
#define PCA_CH7_SetMode_PWM()					SFRPI=1;CCAPM7=ECOM7|PWM7;SFRPI=0	
#define PCA_CH7_SetMode_COPM()					SFRPI=1;CCAPM7=ECOM7|PWM7|TOG7;SFRPI=0	
#define PCA_CH7_SetMode_FIFOData()				SFRPI=1;CCAPM7=ECOM7|CAPN7|PWM7;SFRPI=0	

#define PCA_CH0_SetPWM_8Bit()					PCAPWM0=PCAPWM0&(~(P0RS0|P0RS1))
#define PCA_CH0_SetPWM_10Bit()					PCAPWM0=(PCAPWM0&(~(P0RS0|P0RS1)))|(P0RS0)
#define PCA_CH0_SetPWM_12Bit()					PCAPWM0=(PCAPWM0&(~(P0RS0|P0RS1)))|(P0RS1)
#define PCA_CH0_SetPWM_16Bit()					PCAPWM0=PCAPWM0|((P0RS0|P0RS1))

#define PCA_CH1_SetPWM_8Bit()					PCAPWM1=PCAPWM1&(~(P1RS0|P1RS1))
#define PCA_CH1_SetPWM_10Bit()					PCAPWM1=(PCAPWM1&(~(P1RS0|P1RS1)))|(P1RS0)
#define PCA_CH1_SetPWM_12Bit()					PCAPWM1=(PCAPWM1&(~(P1RS0|P1RS1)))|(P1RS1)
#define PCA_CH1_SetPWM_16Bit()					PCAPWM1=PCAPWM1|((P1RS0|P1RS1))

#define PCA_CH2_SetPWM_8Bit()					PCAPWM2=PCAPWM0&(~(P2RS0|P2RS1))
#define PCA_CH2_SetPWM_10Bit()					PCAPWM2=(PCAPWM0&(~(P2RS0|P2RS1)))|(P2RS0)
#define PCA_CH2_SetPWM_12Bit()					PCAPWM2=(PCAPWM0&(~(P2RS0|P2RS1)))|(P2RS1)
#define PCA_CH2_SetPWM_16Bit()					PCAPWM2=PCAPWM0|((P2RS0|P2RS1))

#define PCA_CH3_SetPWM_8Bit()					PCAPWM3=PCAPWM0&(~(P3RS0|P3RS1))
#define PCA_CH3_SetPWM_10Bit()					PCAPWM3=(PCAPWM0&(~(P3RS0|P3RS1)))|(P3RS0)
#define PCA_CH3_SetPWM_12Bit()					PCAPWM3=(PCAPWM0&(~(P3RS0|P3RS1)))|(P3RS1)
#define PCA_CH3_SetPWM_16Bit()					PCAPWM3=PCAPWM0|((P3RS0|P3RS1))

#define PCA_CH4_SetPWM_8Bit()					PCAPWM4=PCAPWM0&(~(P4RS0|P4RS1))
#define PCA_CH4_SetPWM_10Bit()					PCAPWM4=(PCAPWM0&(~(P4RS0|P4RS1)))|(P4RS0)
#define PCA_CH4_SetPWM_12Bit()					PCAPWM4=(PCAPWM0&(~(P4RS0|P4RS1)))|(P4RS1)
#define PCA_CH4_SetPWM_16Bit()					PCAPWM4=PCAPWM0|((P4RS0|P4RS1))

#define PCA_CH5_SetPWM_8Bit()					PCAPWM5=PCAPWM0&(~(P5RS0|P5RS1))
#define PCA_CH5_SetPWM_10Bit()					PCAPWM5=(PCAPWM0&(~(P5RS0|P5RS1)))|(P5RS0)
#define PCA_CH5_SetPWM_12Bit()					PCAPWM5=(PCAPWM0&(~(P5RS0|P5RS1)))|(P5RS1)
#define PCA_CH5_SetPWM_16Bit()					PCAPWM5=PCAPWM0|((P5RS0|P5RS1))

#define PCA_CH6_SetPWM_8Bit()					SFRPI=1;PCAPWM6=PCAPWM0&(~(P6RS0|P6RS1));SFRPI=0	
#define PCA_CH6_SetPWM_10Bit()					SFRPI=1;PCAPWM6=(PCAPWM0&(~(P6RS0|P6RS1)))|(P6RS0);SFRPI=0	
#define PCA_CH6_SetPWM_12Bit()					SFRPI=1;PCAPWM6=(PCAPWM0&(~(P6RS0|P6RS1)))|(P6RS1);SFRPI=0	
#define PCA_CH6_SetPWM_16Bit()					SFRPI=1;PCAPWM6=PCAPWM0|((P6RS0|P6RS1));SFRPI=0	

#define PCA_CH7_SetPWM_8Bit()					SFRPI=1;PCAPWM7=PCAPWM0&(~(P7RS0|P7RS1));SFRPI=0	
#define PCA_CH7_SetPWM_10Bit()					SFRPI=1;PCAPWM7=(PCAPWM0&(~(P7RS0|P7RS1)))|(P7RS0);SFRPI=0	
#define PCA_CH7_SetPWM_12Bit()					SFRPI=1;PCAPWM7=(PCAPWM0&(~(P7RS0|P7RS1)))|(P7RS1);SFRPI=0	
#define PCA_CH7_SetPWM_16Bit()					SFRPI=1;PCAPWM7=PCAPWM0|((P7RS0|P7RS1));SFRPI=0	

// PCA 设置各通道值
#define PCA_CH0_SetValue(x,y)					if((x==0)&&(y==0)){PCAPWM0=PCAPWM0|0x03;}else{CCAP0L=y;CCAP0H=x;PCAPWM0=PCAPWM0&(~0x03);}		// 0 0,1
#define PCA_CH1_SetValue(x,y)					if((x==0)&&(y==0)){PCAPWM1=PCAPWM1|0x03;}else{CCAP1L=y;CCAP1H=x;PCAPWM1=PCAPWM1&(~0x03);}		// 1 0,1
#define PCA_CH2_SetValue(x,y)					if((x==0)&&(y==0)){PCAPWM2=PCAPWM2|0x03;}else{CCAP2L=y;CCAP2H=x;PCAPWM2=PCAPWM2&(~0x03);}		// 2 2,3
#define PCA_CH3_SetValue(x,y)					if((x==0)&&(y==0)){PCAPWM3=PCAPWM3|0x03;}else{CCAP3L=y;CCAP3H=x;PCAPWM3=PCAPWM3&(~0x03);}		// 3 2,3
#define PCA_CH4_SetValue(x,y)					if((x==0)&&(y==0)){PCAPWM4=PCAPWM4|0x03;}else{CCAP4L=y;CCAP4H=x;PCAPWM4=PCAPWM4&(~0x03);}		// 4 4,5
#define PCA_CH5_SetValue(x,y)					if((x==0)&&(y==0)){PCAPWM5=PCAPWM5|0x03;}else{CCAP5L=y;CCAP5H=x;PCAPWM5=PCAPWM5&(~0x03);}		// 5 4,5
#define PCA_CH6_SetValue(x,y)					SFRPI=1;if((x==0)&&(y==0)){PCAPWM6=PCAPWM6|0x03;}else{CCAP6L=y;CCAP6H=x;PCAPWM6=PCAPWM6&(~0x03);};SFRPI=0		// 6 
#define PCA_CH7_SetValue(x,y)					SFRPI=1;if((x==0)&&(y==0)){PCAPWM7=PCAPWM7|0x03;}else{CCAP7L=y;CCAP7H=x;PCAPWM7=PCAPWM7&(~0x03);};SFRPI=0		// 7 

#define PCA_CH0_SetValue_L(x)					CCAP0L=x                        // 0
#define PCA_CH1_SetValue_L(x)					CCAP1L=x                        // 1
#define PCA_CH2_SetValue_L(x)					CCAP2L=x                        // 2
#define PCA_CH3_SetValue_L(x)					CCAP3L=x                        // 3
#define PCA_CH4_SetValue_L(x)					CCAP4L=x                        // 4
#define PCA_CH5_SetValue_L(x)					CCAP5L=x                        // 5
#define PCA_CH6_SetValue_L(x)					SFRPI=1;CCAP6L=x;SFRPI=0		// 6 
#define PCA_CH7_SetValue_L(x)					SFRPI=1;CCAP7L=x;SFRPI=0		// 7 

#define PCA_CH0_SetValue_H(x)					CCAP0H=x						// 0
#define PCA_CH1_SetValue_H(x)					CCAP1H=x                        // 1
#define PCA_CH2_SetValue_H(x)					CCAP2H=x                        // 2
#define PCA_CH3_SetValue_H(x)					CCAP3H=x                        // 3
#define PCA_CH4_SetValue_H(x)					CCAP4H=x                        // 4
#define PCA_CH5_SetValue_H(x)					CCAP5H=x                        // 5
#define PCA_CH6_SetValue_H(x)					SFRPI=1;CCAP6H=x;SFRPI=0		// 6 
#define PCA_CH7_SetValue_H(x)					SFRPI=1;CCAP7H=x;SFRPI=0		// 7 

#define PCA_CH0_SetValue_9thBit_L_0()			PCAPWM0=PCAPWM0&(~0x01)        	// 0
#define PCA_CH1_SetValue_9thBit_L_0()			PCAPWM1=PCAPWM1&(~0x01)         // 1
#define PCA_CH2_SetValue_9thBit_L_0()			PCAPWM2=PCAPWM2&(~0x01)         // 2
#define PCA_CH3_SetValue_9thBit_L_0()			PCAPWM3=PCAPWM3&(~0x01)         // 3
#define PCA_CH4_SetValue_9thBit_L_0()			PCAPWM4=PCAPWM4&(~0x01)         // 4
#define PCA_CH5_SetValue_9thBit_L_0()			PCAPWM5=PCAPWM5&(~0x01)         // 5
#define PCA_CH6_SetValue_9thBit_L_0()			SFRPI=1;PCAPWM6=PCAPWM6&(~0x01);SFRPI=0		// 6 
#define PCA_CH7_SetValue_9thBit_L_0()			SFRPI=1;PCAPWM7=PCAPWM7&(~0x01);SFRPI=0		// 7 
#define PCA_CH0_SetValue_9thBit_L_1()			PCAPWM0=PCAPWM0|(0x01)        	// 0             
#define PCA_CH1_SetValue_9thBit_L_1()			PCAPWM1=PCAPWM1|(0x01)         // 1             
#define PCA_CH2_SetValue_9thBit_L_1()			PCAPWM2=PCAPWM2|(0x01)         // 2             
#define PCA_CH3_SetValue_9thBit_L_1()			PCAPWM3=PCAPWM3|(0x01)         // 3             
#define PCA_CH4_SetValue_9thBit_L_1()			PCAPWM4=PCAPWM4|(0x01)         // 4             
#define PCA_CH5_SetValue_9thBit_L_1()			PCAPWM5=PCAPWM5|(0x01)         // 5             
#define PCA_CH6_SetValue_9thBit_L_1()			SFRPI=1;PCAPWM6=PCAPWM6|(0x01);SFRPI=0		// 6 
#define PCA_CH7_SetValue_9thBit_L_1()			SFRPI=1;PCAPWM7=PCAPWM7|(0x01);SFRPI=0		// 7 

#define PCA_CH0_SetValue_9thBit_H_0()			PCAPWM0=PCAPWM0&(~0x02)        	// 0             
#define PCA_CH1_SetValue_9thBit_H_0()			PCAPWM1=PCAPWM1&(~0x02)         // 1             
#define PCA_CH2_SetValue_9thBit_H_0()			PCAPWM2=PCAPWM2&(~0x02)         // 2             
#define PCA_CH3_SetValue_9thBit_H_0()			PCAPWM3=PCAPWM3&(~0x02)         // 3             
#define PCA_CH4_SetValue_9thBit_H_0()			PCAPWM4=PCAPWM4&(~0x02)         // 4             
#define PCA_CH5_SetValue_9thBit_H_0()			PCAPWM5=PCAPWM5&(~0x02)         // 5             
#define PCA_CH6_SetValue_9thBit_H_0()			SFRPI=1;PCAPWM6=PCAPWM6&(~0x02);SFRPI=0		// 6 
#define PCA_CH7_SetValue_9thBit_H_0()			SFRPI=1;PCAPWM7=PCAPWM7&(~0x02);SFRPI=0		// 7 
#define PCA_CH0_SetValue_9thBit_H_1()			PCAPWM0=PCAPWM0|(0x02)        	// 0             
#define PCA_CH1_SetValue_9thBit_H_1()			PCAPWM1=PCAPWM1|(0x02)         // 1              
#define PCA_CH2_SetValue_9thBit_H_1()			PCAPWM2=PCAPWM2|(0x02)         // 2              
#define PCA_CH3_SetValue_9thBit_H_1()			PCAPWM3=PCAPWM3|(0x02)         // 3              
#define PCA_CH4_SetValue_9thBit_H_1()			PCAPWM4=PCAPWM4|(0x02)         // 4              
#define PCA_CH5_SetValue_9thBit_H_1()			PCAPWM5=PCAPWM5|(0x02)         // 5              
#define PCA_CH6_SetValue_9thBit_H_1()			SFRPI=1;PCAPWM6=PCAPWM6|(0x02);SFRPI=0		// 6 
#define PCA_CH7_SetValue_9thBit_H_1()			SFRPI=1;PCAPWM7=PCAPWM7|(0x02);SFRPI=0		// 7 

// PCA 设置PWM输出反向
#define PCA_CH0_SetPWM_EnInvert()				PCAPWM0=PCAPWM0|P0INV
#define PCA_CH1_SetPWM_EnInvert()				PCAPWM1=PCAPWM1|P1INV
#define PCA_CH2_SetPWM_EnInvert()				PCAPWM2=PCAPWM2|P2INV
#define PCA_CH3_SetPWM_EnInvert()				PCAPWM3=PCAPWM3|P3INV
#define PCA_CH4_SetPWM_EnInvert()				PCAPWM4=PCAPWM4|P4INV
#define PCA_CH5_SetPWM_EnInvert()				PCAPWM5=PCAPWM5|P5INV
#define PCA_CH6_SetPWM_EnInvert()				SFRPI=1;PCAPWM6=PCAPWM6|P6INV;SFRPI=0	
#define PCA_CH7_SetPWM_EnInvert()				SFRPI=1;PCAPWM7=PCAPWM7|P7INV;SFRPI=0	

#define PCA_CH0_SetPWM_DisInvert()				PCAPWM0=PCAPWM0&(~(P0INV))
#define PCA_CH1_SetPWM_DisInvert()				PCAPWM1=PCAPWM1&(~(P1INV))
#define PCA_CH2_SetPWM_DisInvert()				PCAPWM2=PCAPWM2&(~(P2INV))
#define PCA_CH3_SetPWM_DisInvert()				PCAPWM3=PCAPWM3&(~(P3INV))
#define PCA_CH4_SetPWM_DisInvert()				PCAPWM4=PCAPWM4&(~(P4INV))
#define PCA_CH5_SetPWM_DisInvert()				PCAPWM5=PCAPWM5&(~(P5INV))
#define PCA_CH6_SetPWM_DisInvert()				SFRPI=1;PCAPWM6=PCAPWM6&(~(P6INV));SFRPI=0	
#define PCA_CH7_SetPWM_DisInvert()				SFRPI=1;PCAPWM7=PCAPWM7&(~(P7INV));SFRPI=0	

// PCA 使能/禁止PWM输出
#define PCA_SetPWM0_EnOutput()					PAOE=PAOE|(POE0)
#define PCA_SetPWM1_EnOutput()					PAOE=PAOE|(POE1)
#define PCA_SetPWM2_EnOutput()					PAOE=PAOE|(POE2)
#define PCA_SetPWM3_EnOutput()					PAOE=PAOE|(POE3)
#define PCA_SetPWM0_2nd_EnOutput()				PAOE=PAOE|(POE0A)
#define PCA_SetPWM0_3rd_EnOutput()				PAOE=PAOE|(POE0B)
#define PCA_SetPWM2_2nd_EnOutput()				PAOE=PAOE|(POE2A)
#define PCA_SetPWM2_3rd_EnOutput()				PAOE=PAOE|(POE2B)
#define PCA_SetPWM4_EnOutput()					SFRPI=4;AUXR7=AUXR7|(POE4);SFRPI=0
#define PCA_SetPWM5_EnOutput()					SFRPI=4;AUXR7=AUXR7|(POE5);SFRPI=0
#define PCA_SetPWM6_EnOutput()					SFRPI=5;AUXR8=AUXR8|(POE6);SFRPI=0
#define PCA_SetPWM7_EnOutput()					SFRPI=5;AUXR8=AUXR8|(POE7);SFRPI=0

#define PCA_SetPWM0_DisOutput()					PAOE=PAOE&(~POE0)
#define PCA_SetPWM1_DisOutput()					PAOE=PAOE&(~POE1)
#define PCA_SetPWM2_DisOutput()					PAOE=PAOE&(~POE2)
#define PCA_SetPWM3_DisOutput()					PAOE=PAOE&(~POE3)
#define PCA_SetPWM0_2nd_DisOutput()				PAOE=PAOE&(~POE0A)
#define PCA_SetPWM0_3rd_DisOutput()				PAOE=PAOE&(~POE0B)
#define PCA_SetPWM2_2nd_DisOutput()				PAOE=PAOE&(~POE2A)
#define PCA_SetPWM2_3rd_DisOutput()				PAOE=PAOE&(~POE2B)
#define PCA_SetPWM4_DisOutput()					SFRPI=4;AUXR7=AUXR7&(~POE4);SFRPI=0
#define PCA_SetPWM5_DisOutput()					SFRPI=4;AUXR7=AUXR7&(~POE5);SFRPI=0
#define PCA_SetPWM6_DisOutput()					SFRPI=5;AUXR8=AUXR8&(~POE6);SFRPI=0
#define PCA_SetPWM7_DisOutput()					SFRPI=5;AUXR8=AUXR8&(~POE7);SFRPI=0

// PCA 设置POEn功能生效方法
#define PCA_SetPOEn_WrCpu()						SFRPI=8;AUXR11=AUXR11&(~POEM0);SFRPI=0
#define PCA_SetPOEn_PWMCycle()					SFRPI=8;AUXR11=AUXR11|(POEM0);SFRPI=0

// PCA 设置PWM 缓冲模式
#define PCA_CH01_SetPWM_EnBufferMode()				CMOD=CMOD|BME0
#define PCA_CH23_SetPWM_EnBufferMode()				CMOD=CMOD|BME2
#define PCA_CH45_SetPWM_EnBufferMode()				CMOD=CMOD|BME4
#define PCA_CH01_SetPWM_DisBufferMode()				CMOD=CMOD&(~(BME0))
#define PCA_CH23_SetPWM_DisBufferMode()				CMOD=CMOD&(~(BME2))
#define PCA_CH45_SetPWM_DisBufferMode()				CMOD=CMOD&(~(BME4))

// PCA 设置PWM 死区, 必须在缓冲模式下才有效
#define PCA_CH01_SetPWM_EnDeadTime()				CCAPM0=CCAPM0|DTE0
#define PCA_CH23_SetPWM_EnDeadTime()				CCAPM2=CCAPM2|DTE2
#define PCA_CH45_SetPWM_EnDeadTime()				CCAPM4=CCAPM4|DTE4
#define PCA_CH01_SetPWM_DisDeadTime()				CCAPM0=CCAPM0&(~(DTE0))
#define PCA_CH23_SetPWM_DisDeadTime()				CCAPM2=CCAPM2&(~(DTE2))
#define PCA_CH45_SetPWM_DisDeadTime()				CCAPM4=CCAPM4&(~(DTE4))

// PCA 设置死区预分频
#define PCA_SetDeadTimePreScaler_Sysclk()		SFRPI=1;PDTCRA=(PDTCRA&(~(DTPS1|DTPS0)));SFRPI=0
#define PCA_SetDeadTimePreScaler_SysclkDiv2()	SFRPI=1;PDTCRA=(PDTCRA&(~(DTPS1|DTPS0)))|(DTPS0);SFRPI=0
#define PCA_SetDeadTimePreScaler_SysclkDiv4()	SFRPI=1;PDTCRA=(PDTCRA&(~(DTPS1|DTPS0)))|(DTPS1);SFRPI=0
#define PCA_SetDeadTimePreScaler_SysclkDiv8()	SFRPI=1;PDTCRA=(PDTCRA|((DTPS1|DTPS0)));SFRPI=0

// PCA 设置死区时间周期 PreScaler*(x) 0:DT无效, 1~63:PreScaler*(1~63)
#define PCA_SetDeadTimePeriod(x)				SFRPI=1;PDTCRA=(PDTCRA&((DTPS1|DTPS0)))|x;SFRPI=0

// PCA 设置中断

#define PCA_CF_EnInterrupt()				CMOD=CMOD|ECF
#define PCA_CF_DisInterrupt()				CMOD=CMOD&(~(ECF))

#define PCA_CH0_EnInterrupt()				CCAPM0=CCAPM0|ECCF0
#define PCA_CH1_EnInterrupt()				CCAPM1=CCAPM1|ECCF1
#define PCA_CH2_EnInterrupt()				CCAPM2=CCAPM2|ECCF2
#define PCA_CH3_EnInterrupt()				CCAPM3=CCAPM3|ECCF3
#define PCA_CH4_EnInterrupt()				CCAPM4=CCAPM4|ECCF4
#define PCA_CH5_EnInterrupt()				CCAPM5=CCAPM5|ECCF5
#define PCA_CH6_EnInterrupt()				SFRPI=1;CCAPM6=CCAPM6|ECCF6;SFRPI=0	
#define PCA_CH7_EnInterrupt()				SFRPI=1;CCAPM7=CCAPM7|ECCF7;SFRPI=0	

#define PCA_CH0_DisInterrupt()				CCAPM0=CCAPM0&(~(ECCF0))
#define PCA_CH1_DisInterrupt()				CCAPM1=CCAPM1&(~(ECCF1))
#define PCA_CH2_DisInterrupt()				CCAPM2=CCAPM2&(~(ECCF2))
#define PCA_CH3_DisInterrupt()				CCAPM3=CCAPM3&(~(ECCF3))
#define PCA_CH4_DisInterrupt()				CCAPM4=CCAPM4&(~(ECCF4))
#define PCA_CH5_DisInterrupt()				CCAPM5=CCAPM5&(~(ECCF5))
#define PCA_CH6_DisInterrupt()				SFRPI=1;CCAPM6=CCAPM6&(~(ECCF6));SFRPI=0	
#define PCA_CH7_DisInterrupt()				SFRPI=1;CCAPM7=CCAPM7&(~(ECCF7));SFRPI=0	

// PCA 设置PWM对齐方式
#define PCA_SetPWM_EdgeAligned()			PWMCR=PWMCR&(~PCAE)
#define PCA_SetPWM_CenterAligned()			PWMCR=PWMCR|(PCAE)

// PCA 设置PWM在中心对齐时是否支持可变解析度
#define PCA_SetCenterAligned_DisVar()			SFRPI=8;AUXR11=AUXR11&(~C0M0);SFRPI=0
#define PCA_SetCenterAligned_EnVar()			SFRPI=8;AUXR11=AUXR11|(C0M0);SFRPI=0

// PCA 设置PWM在中心对齐时CF在顶端/底端置位
#define PCA_SetCenterAligned_CFonTop()			SFRPI=8;AUXR11=AUXR11&(~C0OFS);SFRPI=0
#define PCA_SetCenterAligned_CFonBottom()		SFRPI=8;AUXR11=AUXR11|(C0OFS);SFRPI=0

// PCA 引脚设置
#define PCA_SetPCA0ClockOut_P47()			SFRPI=2;AUXR5=AUXR5&(~C0COPS);SFRPI=0
#define PCA_SetPCA0ClockOut_P72()			SFRPI=2;AUXR5=AUXR5|(C0COPS);SFRPI=0

#define PCA_SetPCA0ECIPort_P21()			SFRPI=2;AUXR5=AUXR5&(~ECIPS0);SFRPI=0
#define PCA_SetPCA0ECIPort_P46()			SFRPI=2;AUXR5=AUXR5|(ECIPS0);SFRPI=0

#define PCA_SetCEX0CEX2CEX4_P22P24P26()		SFRPI=2;AUXR5=AUXR5&(~C0PS0);SFRPI=0
#define PCA_SetCEX0CEX2CEX4_P40P41P32()		SFRPI=2;AUXR5=AUXR5|(C0PS0);SFRPI=0

#define PCA_SetCEX1CEX3CEX5_P23P25P27()		SFRPI=2;AUXR5=AUXR5&(~C0PS1);SFRPI=0
#define PCA_SetCEX1CEX3CEX5_P33P34P35()		SFRPI=2;AUXR5=AUXR5|(C0PS1);SFRPI=0

#define PCA_SetPWM0APWM0B_P36P37()			SFRPI=2;AUXR5=AUXR5&(~C0PPS0);SFRPI=0
#define PCA_SetPWM0APWM0B_P60P61()			SFRPI=2;AUXR5=AUXR5|(C0PPS0);SFRPI=0

#define PCA_SetPWM2APWM2B_P40P41()			SFRPI=2;AUXR5=AUXR5&(~C0PPS1);SFRPI=0
#define PCA_SetPWM2APWM2B_P34P35()			SFRPI=2;AUXR5=AUXR5|(C0PPS1);SFRPI=0

#define PCA_SetCEX2InputPort_CEX2()			SFRPI=2;AUXR5=AUXR5&(~C0IC2S0);SFRPI=0
#define PCA_SetCEX2InputPort_T3EXI()		SFRPI=2;AUXR5=AUXR5|(C0IC2S0);SFRPI=0

#define PCA_SetCEX4InputPort_CEX4()			SFRPI=2;AUXR5=AUXR5&(~C0IC4S0);SFRPI=0
#define PCA_SetCEX4InputPort_T2EXI()		SFRPI=2;AUXR5=AUXR5|(C0IC4S0);SFRPI=0

#define PCA_SetPWM6PWM7_P62P63()			SFRPI=5;AUXR8=AUXR8&(~(C0PPS2));SFRPI=0
#define PCA_SetPWM6PWM7_P60P61()			SFRPI=5;AUXR8=AUXR8|((C0PPS2));SFRPI=0

/*
*************************************************************************************
*/ 


/*
*************************************************************************************
*  WDT 
*
*************************************************************************************
*/
#define WDT_SetClock_32K_DIV_1_16ms()		WDTCR=(WDTCR&(~0x07))
#define WDT_SetClock_32K_DIV_2_32ms()		WDTCR=(WDTCR&(~0x07))|0x01	
#define WDT_SetClock_32K_DIV_4_64ms()		WDTCR=(WDTCR&(~0x07))|0x02
#define WDT_SetClock_32K_DIV_8_128ms()		WDTCR=(WDTCR&(~0x07))|0x03
#define WDT_SetClock_32K_DIV_16_256ms()		WDTCR=(WDTCR&(~0x07))|0x04
#define WDT_SetClock_32K_DIV_32_512ms()		WDTCR=(WDTCR&(~0x07))|0x05
#define WDT_SetClock_32K_DIV_64_1024ms()	WDTCR=(WDTCR&(~0x07))|0x06
#define WDT_SetClock_32K_DIV_128_2048ms()	WDTCR=(WDTCR|(0x07))

// WDT 设置时钟来源
#define WDT_SetClock_ILRCO()			ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON3_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=IFD&(0x3F);IFADRH=0x0;IFADRL=CKCON3_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0
#define WDT_SetClock_XTAL2()			ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON3_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0x3F))|WDTCS0_P;IFADRH=0x0;IFADRL=CKCON3_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0
#define WDT_SetClock_SYSCLKDiv12()		ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON3_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0x3F))|WDTCS1_P;IFADRH=0x0;IFADRL=CKCON3_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0
#define WDT_SetClock_S0TOF()			ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON3_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0x3F))|(WDTCS1_P|WDTCS0_P);IFADRH=0x0;IFADRL=CKCON3_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0

#define WDT_EnReset()		WDTCR|=WREN
#define WDT_Enable_OnPD()	WDTCR|=NSW
#define WDT_Enable_OnIDLE()	WDTCR|=WIDL
#define WDT_Clear()			WDTCR|=CLRW
#define WDT_Enable()		WDTCR|=ENW

#define CLRWDT()	WDTCR|=CLRW
#define ENWDT()		WDTCR|=ENW
/*
*************************************************************************************
*/ 

/*
*************************************************************************************
*  RTC  
*
*************************************************************************************
*/
// RTC 使能或禁止
#define RTC_Enable()			RTCCR=RTCCR|RTCE
#define RTC_Disable()			RTCCR=RTCCR&(~RTCE)

// RTC 使能或禁止RTCO输出到P4.5 
#define RTC_EnRTCO_OutP45()			RTCCR=RTCCR|RTCO
#define RTC_DisRTCO_OutP45()		RTCCR=RTCCR&(~RTCO)

// RTC 设置重载值 x=0~63 
#define RTC_SetReload(x)		RTCCR=(RTCCR&(0xC0))|(x)

// RTC 设置计数值 x=0~63
#define RTC_SetCounter(x)		RTCTM=(RTCTM&(0xC0))|(x)

// RTC 设置时钟分频值 x=0~15 2^15~2^0
#define RTC_SetClock_Div_1()		RTCTM=(RTCTM&(0x3F))|(BIT7|BIT6);ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0xFC))|(BIT1|BIT0);IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0
#define RTC_SetClock_Div_2()		RTCTM=(RTCTM&(0x3F))|(BIT7|BIT6);ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0xFC))|(BIT1);IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0
#define RTC_SetClock_Div_4()		RTCTM=(RTCTM&(0x3F))|(BIT7|BIT6);ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0xFC))|(BIT0);IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0
#define RTC_SetClock_Div_8()		RTCTM=(RTCTM&(0x3F))|(BIT7|BIT6);ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0xFC));IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0
#define RTC_SetClock_Div_16()		RTCTM=(RTCTM&(0x3F))|(BIT7);ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0xFC))|(BIT1|BIT0);IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0
#define RTC_SetClock_Div_32()		RTCTM=(RTCTM&(0x3F))|(BIT7);ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0xFC))|(BIT1);IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0
#define RTC_SetClock_Div_64()		RTCTM=(RTCTM&(0x3F))|(BIT7);ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0xFC))|(BIT0);IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0
#define RTC_SetClock_Div_128()		RTCTM=(RTCTM&(0x3F))|(BIT7);ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0xFC));IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0
#define RTC_SetClock_Div_256()		RTCTM=(RTCTM&(0x3F))|(BIT6);ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0xFC))|(BIT1|BIT0);IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0
#define RTC_SetClock_Div_512()		RTCTM=(RTCTM&(0x3F))|(BIT6);ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0xFC))|(BIT1);IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0
#define RTC_SetClock_Div_1024()		RTCTM=(RTCTM&(0x3F))|(BIT6);ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0xFC))|(BIT0);IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0
#define RTC_SetClock_Div_2048()		RTCTM=(RTCTM&(0x3F))|(BIT6);ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0xFC));IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0
#define RTC_SetClock_Div_4096()		RTCTM=(RTCTM&(0x3F));ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0xFC))|(BIT1|BIT0);IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0
#define RTC_SetClock_Div_8192()		RTCTM=(RTCTM&(0x3F));ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0xFC))|(BIT1);IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0
#define RTC_SetClock_Div_16384()	RTCTM=(RTCTM&(0x3F));ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0xFC))|(BIT0);IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0
#define RTC_SetClock_Div_32768()	RTCTM=(RTCTM&(0x3F));ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0xFC));IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0

// RTC 设置时钟来源
#define RTC_SetClock_XTAL2_ECKI_P60()		ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0x1F));IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0
#define RTC_SetClock_ILRCO()				ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0x1F))|(RCSS0_P);IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0
#define RTC_SetClock_WDTPS()				ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0x1F))|(RCSS1_P);IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0
#define RTC_SetClock_WDTOF()				ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0x1F))|(RCSS1_P|RCSS0_P);IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0
#define RTC_SetClock_SYSCLK()				ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0x1F))|(RCSS2_P);IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0
#define RTC_SetClock_SYSCLKDiv12()			ISPCR=0x80;IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_READ_P;SCMD=0x46;SCMD=0xB9;_nop_();IFD=(IFD&(0x1F))|(RCSS2_P|RCSS0_P);IFADRH=0x0;IFADRL=CKCON4_P;IFMT=ISP_WRITE_P;SCMD=0x46;SCMD=0xB9;_nop_();IFMT=0;ISPCR=0x0

// RTC 清标志位RTCF
#define RTC_ClearRTCF()			PCON1=PCON1|RTCF

// RTC 使能/禁止RTC中断
#define RTC_EnInterrupt()			SFIE=SFIE|RTCFIE
#define RTC_DisInterrupt()			SFIE=SFIE&(~RTCFIE)

/*
*************************************************************************************
*/ 

/*
*************************************************************************************
*  SID  I2C START/STOP 侦测
*
*************************************************************************************
*/

/*
*************************************************************************************
*/ 

/*
*************************************************************************************
*  BEEPER  
*
*************************************************************************************
*/
// BEEPER 设置IO口
#define BEEP_SetP44_IOPort()						AUXR3=AUXR3&(~(BPOC0|BPOC1))
#define BEEP_SetP44_OutILRCODiv32_1KHz()			AUXR3=(AUXR3&(~(BPOC0|BPOC1)))|(BPOC0)
#define BEEP_SetP44_OutILRCODiv16_2KHz()			AUXR3=(AUXR3&(~(BPOC0|BPOC1)))|(BPOC1)
#define BEEP_SetP44_OutILRCODiv8_4KHz()				AUXR3=AUXR3|((BPOC0|BPOC1))

/*
*************************************************************************************
*/ 


/*
*************************************************************************************
*  KBI  
*
*************************************************************************************
*/
// KBI 设置IO口
#define KBI_SetKBI0123P10P11P12P13()		SFRPI=3;AUXR6=AUXR6&(~(KBILPS0|KBILPS1));SFRPI=0
#define KBI_SetKBI0123P00P01P02P03()		SFRPI=3;AUXR6=(AUXR6&(~(KBILPS0|KBILPS1)))|KBILPS0;SFRPI=0
#define KBI_SetKBI0123P20P21P22P23()		SFRPI=3;AUXR6=(AUXR6&(~(KBILPS0|KBILPS1)))|KBILPS1;SFRPI=0
#define KBI_SetKBI0123P40P41P42P43()		SFRPI=3;AUXR6=AUXR6|(KBILPS1|KBILPS0);SFRPI=0

#define KBI_SetKBI4567P14P15P16P17()		SFRPI=3;AUXR6=AUXR6&(~(KBIHPS0|KBIHPS1));SFRPI=0
#define KBI_SetKBI4567P04P05P06P07()		SFRPI=3;AUXR6=(AUXR6&(~(KBIHPS0|KBIHPS1)))|KBIHPS0;SFRPI=0
#define KBI_SetKBI4567P24P25P26P27()		SFRPI=3;AUXR6=(AUXR6&(~(KBIHPS0|KBIHPS1)))|KBIHPS1;SFRPI=0
#define KBI_SetKBI4567P60P61P62P63()		SFRPI=3;AUXR6=AUXR6|(KBIHPS1|KBIHPS0);SFRPI=0

// KBI 设置比较值
#define KBI0_1	0x01
#define KBI1_1	0x02
#define KBI2_1	0x04
#define KBI3_1	0x08
#define KBI4_1	0x10
#define KBI5_1	0x20
#define KBI6_1	0x40
#define KBI7_1	0x80
#define KBI0_0	0x00
#define KBI1_0	0x00
#define KBI2_0	0x00
#define KBI3_0	0x00
#define KBI4_0	0x00
#define KBI5_0	0x00
#define KBI6_0	0x00
#define KBI7_0	0x00
#define KBI_SetKBPattern(x)					KBPATN=x

// KBI 选择使能KBI通道
#define KBI0	0x01
#define KBI1	0x02
#define KBI2	0x04
#define KBI3	0x08
#define KBI4	0x10
#define KBI5	0x20
#define KBI6	0x40
#define KBI7	0x80
#define KBI_SetKBMask(x)					KBMASK=x

// KBI 清中断标志位
#define KBI_ClrITFlag()						KBCON=KBCON&(~KBIF)

// KBI 选择侦测边沿/电平
#define KBI_SetDetectLevel()				KBCON=KBCON&(~KBES)
#define KBI_SetDetectEdge()					KBCON=KBCON|(KBES)

// KBI 选择等于比较值还不等于比较值
#define KBI_SetPatternNotEqual()			KBCON=KBCON&(~PATN_SEL)
#define KBI_SetPatternEqual()				KBCON=KBCON|(PATN_SEL)

// KBI 设置滤波
#define KBI_SetFilter_Disable()				KBCON=KBCON&(~(KBCS0|KBCS1))
#define KBI_SetFilter_Sysclk_x3()			KBCON=(KBCON&(~(KBCS0|KBCS1)))|(KBCS0)
#define KBI_SetFilter_SysclkDiv6_x3()		KBCON=(KBCON&(~(KBCS0|KBCS1)))|(KBCS1)
#define KBI_SetFilter_S0TOF_x3()			KBCON=KBCON|((KBCS0|KBCS1))
/*
*************************************************************************************
*/ 

/*
*************************************************************************************
*  DMA
*
*************************************************************************************
*/
// DMA 使能/禁止
#define DMA_Enable()			DMACR0=DMACR0|DMAE0
#define DMA_Disable()			DMACR0=DMACR0&(~DMAE0)

// DMA 启动/挂起
#define DMA_Start()				DMACR0=DMACR0|DMAS0
#define DMA_Suspend()			DMACR0=DMACR0&(~DMAS0)

// DMA 使能/禁止DCF0中断
#define DMA_EnDCF0Int()			DMACR0=DMACR0|DIE0
#define DMA_DisDCF0Int()		DMACR0=DMACR0&(~DIE0)

// DMA 使能/禁止copy数据到CRC16
#define DMA_EnCopy2CRC16()		SFRPI=8;DMACG0=DMACG0|CRCW0;SFRPI=0
#define DMA_DisCopy2CRC16()		SFRPI=8;DMACG0=DMACG0&(~CRCW0);SFRPI=0

// DMA 设置触发源
#define DMA_SetTrig_Software()		SFRPI=8;DMACG0=DMACG0&(~(EXTS00|EXTS10));SFRPI=0
#define DMA_SetTrig_INT2ET()		SFRPI=8;DMACG0=(DMACG0&(~(EXTS00|EXTS10)))|(EXTS00);SFRPI=0
#define DMA_SetTrig_KBIET()			SFRPI=8;DMACG0=DMACG0|(EXTS00|EXTS10);SFRPI=0

// DMA 使能/禁止DMA 循环模式
#define DMA_EnLoop()				SFRPI=8;DMACG0=DMACG0|LOOP0;SFRPI=0
#define DMA_DisLoop()				SFRPI=8;DMACG0=DMACG0&(~LOOP0);SFRPI=0

// DMA 设置来源和目标
#define DMA_SetSource_S0RX()		SFRPI=9;DMADS0=(DMADS0&(0x0F))|(DSS00);SFRPI=0
#define DMA_SetSource_S1RX()		SFRPI=9;DMADS0=(DMADS0&(0x0F))|(DSS10);SFRPI=0
#define DMA_SetSource_S2RX()		SFRPI=9;DMADS0=(DMADS0&(0x0F))|(DSS10|DDS00);SFRPI=0
#define DMA_SetSource_S3RX()		SFRPI=9;DMADS0=(DMADS0&(0x0F))|(DSS20);SFRPI=0
#define DMA_SetSource_TWI0RX()		SFRPI=9;DMADS0=(DMADS0&(0x0F))|(DSS20|DSS00);SFRPI=0
#define DMA_SetSource_TWI1RX()		SFRPI=9;DMADS0=(DMADS0&(0x0F))|(DSS20|DSS10);SFRPI=0
#define DMA_SetSource_SPI0RX()		SFRPI=9;DMADS0=(DMADS0&(0x0F))|(DSS20|DSS10|DSS00);SFRPI=0
#define DMA_SetSource_ADC0()		SFRPI=9;DMADS0=(DMADS0&(0x0F))|(DSS30|DSS00);SFRPI=0
#define DMA_SetSource_EMB()			SFRPI=9;DMADS0=(DMADS0&(0x0F))|(DSS30|DDS10|DSS00);SFRPI=0
#define DMA_SetSource_XRAM()		SFRPI=9;DMADS0=DMADS0|(0xF0);SFRPI=0
#define DMA_SetDestination_S0TX()		SFRPI=9;DMADS0=(DMADS0&(0xF0))|(DDS00);SFRPI=0
#define DMA_SetDestination_S1TX()		SFRPI=9;DMADS0=(DMADS0&(0xF0))|(DDS10);SFRPI=0
#define DMA_SetDestination_S2TX()		SFRPI=9;DMADS0=(DMADS0&(0xF0))|(DDS10|DDS00);SFRPI=0
#define DMA_SetDestination_S3TX()		SFRPI=9;DMADS0=(DMADS0&(0xF0))|(DDS20);SFRPI=0
#define DMA_SetDestination_TWI0TX()		SFRPI=9;DMADS0=(DMADS0&(0xF0))|(DDS20|DDS00);SFRPI=0
#define DMA_SetDestination_TWI1TX()		SFRPI=9;DMADS0=(DMADS0&(0xF0))|(DDS20|DDS10);SFRPI=0
#define DMA_SetDestination_SPI0TX()		SFRPI=9;DMADS0=(DMADS0&(0xF0))|(DDS20|DDS10|DDS00);SFRPI=0
#define DMA_SetDestination_EMB()		SFRPI=9;DMADS0=(DMADS0&(0xF0))|(DDS30|DDS10|DDS00);SFRPI=0
#define DMA_SetDestination_CRC()		SFRPI=9;DMADS0=(DMADS0&(0xF0))|(DDS30|DDS20|DDS00);SFRPI=0
#define DMA_SetDestination_XRAM()		SFRPI=9;DMADS0=DMADS0|(0x0F);SFRPI=0

// DMA 设置DMA XRAM起始地址
#define DMA_SetXRAMStartaddr(x)		SFRPI=4;TL6=(x)&0xFF;TH6=(x)>>8;SFRPI=0
// DMA 设置DMA XRAM起始地址重载值
#define DMA_SetXRAMStartaddrRL(x)	SFRPI=4;TLR6=(x)&0xFF;THR6=(x)>>8;SFRPI=0

// DMA 设置DMA 计数
#define DMA_SetCount(x)				SFRPI=3;TL5=(x)&0xFF;TH5=(x)>>8;SFRPI=0
// DMA 设置DMA 计数重载值
#define DMA_SetCountRL(x)			SFRPI=3;TLR5=(x)&0xFF;THR5=(x)>>8;SFRPI=0

// DMA 清/检测DMA完成标志
#define DMA_ClearCompleteFlag()		DMACR0=DMACR0&(~DCF0)
#define DMA_ChkCompleteFlag()		(DMACR0&DCF0)

/*
*************************************************************************************
*/ 

/*
*************************************************************************************
*  GPL 通用逻辑
*
*************************************************************************************
*/
// GPL 位序更改
#define GPL_BOREV(x)						BOREV=x;x=BOREV

// GPL CRC操作

#define GPL_CRC_CRC0DA_DataIn()				AUXR1=(CRCDS0|CRCDS1)

#define GPL_CRC_WriteCRCSeedL(x)			AUXR1=AUXR1&(~(CRCDS0|CRCDS1));CRC0DA=x
#define GPL_CRC_WriteCRCSeedH(x)			AUXR1=(AUXR1&(~(CRCDS0|CRCDS1)))|(CRCDS0);CRC0DA=x
#define GPL_CRC_WriteCRCData(x)				AUXR1=(AUXR1&(~(CRCDS0|CRCDS1)))|(CRCDS0|CRCDS1);CRC0DA=x
#define GPL_CRC_ReadCRCResultL(x)			AUXR1=AUXR1&(~(CRCDS0|CRCDS1));x=CRC0DA
#define GPL_CRC_ReadCRCResultH(x)			AUXR1=(AUXR1&(~(CRCDS0|CRCDS1)))|(CRCDS0);x=CRC0DA

/*
*************************************************************************************
*/ 

/*
*************************************************************************************
*  POWER 电源控制
*
*************************************************************************************
*/
#define POW_BOD1_Enable_OnPD		BIT7
#define POW_BOD1_Disable_OnPD		0x00

#define POW_BOD1_Set20V			0x00
#define POW_BOD1_Set24V			B00010000
#define POW_BOD1_Set37V			B00100000
#define POW_BOD1_Set42V			B00110000

#define POW_BOD1_Enable			BIT2
#define POW_BOD1_Disable		0x00

#define POW_BOD1_EnReset		BIT3
#define POW_BOD1_DisReset		0x00

#define POW_BOD0_EnReset		BIT1
#define POW_BOD0_DisReset		0x00

#define POW_SetMode_PD()			PCON0|=PD;

#define POW_ResetToAP()			ISPCR=SRST
#define POW_ResetToISP()		ISPCR=SWBS|SRST

/*
*************************************************************************************
*/ 

/*
*************************************************************************************
*  OBM0 
*
*************************************************************************************
*/

// OBM0 设置低位通道
#define OBM0_SetChannelSourceL_VSS()				SFRPI=4;BM0LCR=BM0LCR&(~(BM0LS0|BM0LS1|BM0LS2|BM0LS3));SFRPI=0
#define OBM0_SetChannelSourceL_KBI2()				SFRPI=4;BM0LCR=(BM0LCR&(~(BM0LS0|BM0LS1|BM0LS2|BM0LS3)))|(BM0LS0);SFRPI=0
#define OBM0_SetChannelSourceL_KBI4()				SFRPI=4;BM0LCR=(BM0LCR&(~(BM0LS0|BM0LS1|BM0LS2|BM0LS3)))|(BM0LS1);SFRPI=0
#define OBM0_SetChannelSourceL_T0CKO()				SFRPI=4;BM0LCR=(BM0LCR&(~(BM0LS0|BM0LS1|BM0LS2|BM0LS3)))|(BM0LS0|BM0LS1);SFRPI=0
#define OBM0_SetChannelSourceL_INT0ET()				SFRPI=4;BM0LCR=(BM0LCR&(~(BM0LS0|BM0LS1|BM0LS2|BM0LS3)))|(BM0LS2);SFRPI=0
#define OBM0_SetChannelSourceL_PWM0O()				SFRPI=4;BM0LCR=(BM0LCR&(~(BM0LS0|BM0LS1|BM0LS2|BM0LS3)))|(BM0LS2|BM0LS0);SFRPI=0
#define OBM0_SetChannelSourceL_PWM1O()				SFRPI=4;BM0LCR=(BM0LCR&(~(BM0LS0|BM0LS1|BM0LS2|BM0LS3)))|(BM0LS2|BM0LS1);SFRPI=0
#define OBM0_SetChannelSourceL_PWM4O()				SFRPI=4;BM0LCR=(BM0LCR&(~(BM0LS0|BM0LS1|BM0LS2|BM0LS3)))|(BM0LS2|BM0LS1|BM0LS0);SFRPI=0
#define OBM0_SetChannelSourceL_T2CKO()				SFRPI=4;BM0LCR=(BM0LCR&(~(BM0LS0|BM0LS1|BM0LS2|BM0LS3)))|(BM0LS3);SFRPI=0
#define OBM0_SetChannelSourceL_T2EXI()				SFRPI=4;BM0LCR=(BM0LCR&(~(BM0LS0|BM0LS1|BM0LS2|BM0LS3)))|(BM0LS3|BM0LS0);SFRPI=0
#define OBM0_SetChannelSourceL_INT2ET()				SFRPI=4;BM0LCR=(BM0LCR&(~(BM0LS0|BM0LS1|BM0LS2|BM0LS3)))|(BM0LS3|BM0LS1);SFRPI=0
#define OBM0_SetChannelSourceL_RTCKO()				SFRPI=4;BM0LCR=(BM0LCR&(~(BM0LS0|BM0LS1|BM0LS2|BM0LS3)))|(BM0LS3|BM0LS1|BM0LS0);SFRPI=0
#define OBM0_SetChannelSourceL_PWM6O()				SFRPI=4;BM0LCR=(BM0LCR&(~(BM0LS0|BM0LS1|BM0LS2|BM0LS3)))|(BM0LS3|BM0LS2);SFRPI=0

// OBM0 设置低位通道反相
#define OBM0_SetChannelL_EnInvert()					SFRPI=4;BM0LCR=BM0LCR|BM0LCP;SFRPI=0
#define OBM0_SetChannelL_DisInvert()				SFRPI=4;BM0LCR=BM0LCR&(~BM0LCP);SFRPI=0

// OBM0 设置高位通道
#define OBM0_SetChannelSourceH_VSS()				SFRPI=5;BM0HCR=BM0HCR&(~(BM0HS0|BM0HS1|BM0HS2|BM0HS3));SFRPI=0
#define OBM0_SetChannelSourceH_KBI3()				SFRPI=5;BM0HCR=(BM0HCR&(~(BM0HS0|BM0HS1|BM0HS2|BM0HS3)))|(BM0HS0);SFRPI=0
#define OBM0_SetChannelSourceH_KBI5()				SFRPI=5;BM0HCR=(BM0HCR&(~(BM0HS0|BM0HS1|BM0HS2|BM0HS3)))|(BM0HS1);SFRPI=0
#define OBM0_SetChannelSourceH_S0CKO()				SFRPI=5;BM0HCR=(BM0HCR&(~(BM0HS0|BM0HS1|BM0HS2|BM0HS3)))|(BM0HS0|BM0HS1);SFRPI=0
#define OBM0_SetChannelSourceH_INT1ET()				SFRPI=5;BM0HCR=(BM0HCR&(~(BM0HS0|BM0HS1|BM0HS2|BM0HS3)))|(BM0HS2);SFRPI=0
#define OBM0_SetChannelSourceH_PWM2O()				SFRPI=5;BM0HCR=(BM0HCR&(~(BM0HS0|BM0HS1|BM0HS2|BM0HS3)))|(BM0HS2|BM0HS0);SFRPI=0
#define OBM0_SetChannelSourceH_PWM3O()				SFRPI=5;BM0HCR=(BM0HCR&(~(BM0HS0|BM0HS1|BM0HS2|BM0HS3)))|(BM0HS2|BM0HS1);SFRPI=0
#define OBM0_SetChannelSourceH_PWM5O()				SFRPI=5;BM0HCR=(BM0HCR&(~(BM0HS0|BM0HS1|BM0HS2|BM0HS3)))|(BM0HS2|BM0HS1|BM0HS0);SFRPI=0
#define OBM0_SetChannelSourceH_T3CKO()				SFRPI=5;BM0HCR=(BM0HCR&(~(BM0HS0|BM0HS1|BM0HS2|BM0HS3)))|(BM0HS3);SFRPI=0
#define OBM0_SetChannelSourceH_T3EXI()				SFRPI=5;BM0HCR=(BM0HCR&(~(BM0HS0|BM0HS1|BM0HS2|BM0HS3)))|(BM0HS3|BM0HS0);SFRPI=0
#define OBM0_SetChannelSourceH_INT3ET()				SFRPI=5;BM0HCR=(BM0HCR&(~(BM0HS0|BM0HS1|BM0HS2|BM0HS3)))|(BM0HS3|BM0HS1);SFRPI=0
#define OBM0_SetChannelSourceH_PWM7O()				SFRPI=5;BM0HCR=(BM0HCR&(~(BM0HS0|BM0HS1|BM0HS2|BM0HS3)))|(BM0HS3|BM0HS1|BM0HS0);SFRPI=0
#define OBM0_SetChannelSourceH_T4CKO()				SFRPI=5;BM0HCR=(BM0HCR&(~(BM0HS0|BM0HS1|BM0HS2|BM0HS3)))|(BM0HS3|BM0HS2);SFRPI=0
#define OBM0_SetChannelSourceH_T4EXI()				SFRPI=5;BM0HCR=(BM0HCR&(~(BM0HS0|BM0HS1|BM0HS2|BM0HS3)))|(BM0HS3|BM0HS2|BM0HS0);SFRPI=0

// OBM0 设置高位通道反相
#define OBM0_SetChannelH_EnInvert()					SFRPI=5;BM0HCR=BM0HCR|BM0HCP;SFRPI=0
#define OBM0_SetChannelH_DisInvert()				SFRPI=5;BM0HCR=BM0HCR&(~BM0HCP);SFRPI=0

// OBM0 设置滤波
#define OBM0_SetFilter_Disable()					SFRPI=2;BM0CR=(BM0CR&(~(M0FLT0|M0FLT1)));SFRPI=0
#define OBM0_SetFilter_Sysclk_x3()					SFRPI=2;BM0CR=(BM0CR&(~(M0FLT0|M0FLT1)))|(M0FLT0);SFRPI=0
#define OBM0_SetFilter_SysclkDiv6_x3()				SFRPI=2;BM0CR=(BM0CR&(~(M0FLT0|M0FLT1)))|(M0FLT1);SFRPI=0
#define OBM0_SetFilter_T3OF_x3()					SFRPI=2;BM0CR=(BM0CR|(M0FLT0|M0FLT1));SFRPI=0

// OBM0 设置模式
#define OBM0_SetMode_BME0E0AndBM0E1()				SFRPI=2;BM0CR=(BM0CR&(~(BM0M0|BM0M1)));SFRPI=0
#define OBM0_SetMode_BME0E0OrBM0E1_Clear()			SFRPI=2;BM0CR=(BM0CR&(~(BM0M0|BM0M1)))|(BM0M0);SFRPI=0
#define OBM0_SetMode_BME0E0OrBM0E1_Set()			SFRPI=2;BM0CR=(BM0CR&(~(BM0M0|BM0M1)))|(BM0M1);SFRPI=0
#define OBM0_SetMode_BME0E0OrBM0E1_Toggle()			SFRPI=2;BM0CR=BM0CR |(BM0M0|BM0M1);SFRPI=0

// OBM0 设置输出使能
#define OBM0_EnOutput_INT0ET()						SFRPI=3;BM0SE=(BM0SE&(~(BM0E00|BM0E01|BM0E02|BM0E03)))|(BM0E00);SFRPI=0
#define OBM0_EnOutput_KBIET()						SFRPI=3;BM0SE=(BM0SE&(~(BM0E00|BM0E01|BM0E02|BM0E03)))|(BM0E01);SFRPI=0
#define OBM0_EnOutput_PWM2O()						SFRPI=3;BM0SE=(BM0SE&(~(BM0E00|BM0E01|BM0E02|BM0E03)))|(BM0E01|BM0E00);SFRPI=0
#define OBM0_EnOutput_T0CKO()						SFRPI=3;BM0SE=(BM0SE&(~(BM0E00|BM0E01|BM0E02|BM0E03)))|(BM0E02);SFRPI=0
#define OBM0_EnOutput_S0DO()						SFRPI=3;BM0SE=(BM0SE&(~(BM0E00|BM0E01|BM0E02|BM0E03)))|(BM0E02|BM0E00);SFRPI=0
#define OBM0_EnOutput_AC0OUT()						SFRPI=3;BM0SE=(BM0SE&(~(BM0E00|BM0E01|BM0E02|BM0E03)))|(BM0E02|BM0E01);SFRPI=0
#define OBM0_EnOutput_PWM5O()						SFRPI=3;BM0SE=(BM0SE&(~(BM0E00|BM0E01|BM0E02|BM0E03)))|(BM0E02|BM0E01|BM0E00);SFRPI=0
#define OBM0_EnOutput_INT2ET()						SFRPI=3;BM0SE=(BM0SE&(~(BM0E00|BM0E01|BM0E02|BM0E03)))|(BM0E03);SFRPI=0
#define OBM0_EnOutput_RTCKO()						SFRPI=3;BM0SE=(BM0SE&(~(BM0E00|BM0E01|BM0E02|BM0E03)))|(BM0E03|BM0E00);SFRPI=0
#define OBM0_EnOutput_T3CKO()						SFRPI=3;BM0SE=(BM0SE&(~(BM0E00|BM0E01|BM0E02|BM0E03)))|(BM0E03|BM0E01);SFRPI=0
#define OBM0_EnOutput_T3EXI()						SFRPI=3;BM0SE=(BM0SE&(~(BM0E00|BM0E01|BM0E02|BM0E03)))|(BM0E03|BM0E01|BM0E00);SFRPI=0
#define OBM0_EnOutput_AC2OUT()						SFRPI=3;BM0SE=(BM0SE&(~(BM0E00|BM0E01|BM0E02|BM0E03)))|(BM0E03|BM0E02|BM0E00);SFRPI=0
#define OBM0_EnOutput1_INT1ET()						SFRPI=3;BM0SE=(BM0SE&(~(BM0E10|BM0E11|BM0E12|BM0E13)))|(BM0E10);SFRPI=0
#define OBM0_EnOutput1_KBI1ET()						SFRPI=3;BM0SE=(BM0SE&(~(BM0E10|BM0E11|BM0E12|BM0E13)))|(BM0E11);SFRPI=0
#define OBM0_EnOutput1_PWM3O()						SFRPI=3;BM0SE=(BM0SE&(~(BM0E10|BM0E11|BM0E12|BM0E13)))|(BM0E11|BM0E10);SFRPI=0
#define OBM0_EnOutput1_T1CKO()						SFRPI=3;BM0SE=(BM0SE&(~(BM0E10|BM0E11|BM0E12|BM0E13)))|(BM0E12);SFRPI=0
#define OBM0_EnOutput1_T2CKO()						SFRPI=3;BM0SE=(BM0SE&(~(BM0E10|BM0E11|BM0E12|BM0E13)))|(BM0E12|BM0E10);SFRPI=0
#define OBM0_EnOutput1_T2EXI()						SFRPI=3;BM0SE=(BM0SE&(~(BM0E10|BM0E11|BM0E12|BM0E13)))|(BM0E12|BM0E11);SFRPI=0
#define OBM0_EnOutput1_PWM4O()						SFRPI=3;BM0SE=(BM0SE&(~(BM0E10|BM0E11|BM0E12|BM0E13)))|(BM0E12|BM0E11|BM0E10);SFRPI=0
#define OBM0_EnOutput1_BOD1ET()						SFRPI=3;BM0SE=(BM0SE&(~(BM0E10|BM0E11|BM0E12|BM0E13)))|(BM0E13);SFRPI=0
#define OBM0_EnOutput1_INT3ET()						SFRPI=3;BM0SE=(BM0SE&(~(BM0E10|BM0E11|BM0E12|BM0E13)))|(BM0E13|BM0E10);SFRPI=0
#define OBM0_EnOutput1_S1DO()						SFRPI=3;BM0SE=(BM0SE&(~(BM0E10|BM0E11|BM0E12|BM0E13)))|(BM0E13|BM0E11);SFRPI=0
#define OBM0_EnOutput1_AC1OUT()						SFRPI=3;BM0SE=(BM0SE&(~(BM0E10|BM0E11|BM0E12|BM0E13)))|(BM0E13|BM0E11|BM0E10);SFRPI=0
#define OBM0_EnOutput1_PWM6O()						SFRPI=3;BM0SE=(BM0SE&(~(BM0E10|BM0E11|BM0E12|BM0E13)))|(BM0E13|BM0E12);SFRPI=0
#define OBM0_EnOutput1_T4CKO()						SFRPI=3;BM0SE=(BM0SE&(~(BM0E10|BM0E11|BM0E12|BM0E13)))|(BM0E13|BM0E12|BM0E10);SFRPI=0
#define OBM0_EnOutput1_T4EXI()						SFRPI=3;BM0SE=(BM0SE&(~(BM0E10|BM0E11|BM0E12|BM0E13)))|(BM0E13|BM0E12|BM0E11);SFRPI=0

// OBM0 设置BM0OUT输出引脚
#define OBM0_SetBM0OUTP42()							SFRPI=4;AUXR7=(AUXR7&(~(BM0OE0|BM0OE1)))|(BM0OE0);SFRPI=0
#define OBM0_SetBM0OUTP30()							SFRPI=4;AUXR7=(AUXR7&(~(BM0OE0|BM0OE1)))|(BM0OE1);SFRPI=0
#define OBM0_SetBM0OUTP50()							SFRPI=4;AUXR7=(AUXR7&(~(BM0OE0|BM0OE1)))|(BM0OE0|BM0OE1);SFRPI=0

/*
*************************************************************************************
*/ 

/*
*************************************************************************************
*  OBM1 
*
*************************************************************************************
*/
// OBM1 设置低位通道
#define OBM1_SetChannelSourceL_VSS()				SFRPI=8;BM1LCR=BM1LCR&(~(BM1LS0|BM1LS1|BM1LS2|BM1LS3));SFRPI=0
#define OBM1_SetChannelSourceL_KBI2()				SFRPI=8;BM1LCR=(BM1LCR&(~(BM1LS0|BM1LS1|BM1LS2|BM1LS3)))|(BM1LS0);SFRPI=0
#define OBM1_SetChannelSourceL_KBI4()				SFRPI=8;BM1LCR=(BM1LCR&(~(BM1LS0|BM1LS1|BM1LS2|BM1LS3)))|(BM1LS1);SFRPI=0
#define OBM1_SetChannelSourceL_T0CKO()				SFRPI=8;BM1LCR=(BM1LCR&(~(BM1LS0|BM1LS1|BM1LS2|BM1LS3)))|(BM1LS0|BM1LS1);SFRPI=0
#define OBM1_SetChannelSourceL_INT0ET()				SFRPI=8;BM1LCR=(BM1LCR&(~(BM1LS0|BM1LS1|BM1LS2|BM1LS3)))|(BM1LS2);SFRPI=0
#define OBM1_SetChannelSourceL_PWM0O()				SFRPI=8;BM1LCR=(BM1LCR&(~(BM1LS0|BM1LS1|BM1LS2|BM1LS3)))|(BM1LS2|BM1LS0);SFRPI=0
#define OBM1_SetChannelSourceL_PWM1O()				SFRPI=8;BM1LCR=(BM1LCR&(~(BM1LS0|BM1LS1|BM1LS2|BM1LS3)))|(BM1LS2|BM1LS1);SFRPI=0
#define OBM1_SetChannelSourceL_PWM4O()				SFRPI=8;BM1LCR=(BM1LCR&(~(BM1LS0|BM1LS1|BM1LS2|BM1LS3)))|(BM1LS2|BM1LS1|BM1LS0);SFRPI=0
#define OBM1_SetChannelSourceL_T2CKO()				SFRPI=8;BM1LCR=(BM1LCR&(~(BM1LS0|BM1LS1|BM1LS2|BM1LS3)))|(BM1LS3);SFRPI=0
#define OBM1_SetChannelSourceL_T2EXI()				SFRPI=8;BM1LCR=(BM1LCR&(~(BM1LS0|BM1LS1|BM1LS2|BM1LS3)))|(BM1LS3|BM1LS0);SFRPI=0
#define OBM1_SetChannelSourceL_INT2ET()				SFRPI=8;BM1LCR=(BM1LCR&(~(BM1LS0|BM1LS1|BM1LS2|BM1LS3)))|(BM1LS3|BM1LS1);SFRPI=0
#define OBM1_SetChannelSourceL_RTCKO()				SFRPI=8;BM1LCR=(BM1LCR&(~(BM1LS0|BM1LS1|BM1LS2|BM1LS3)))|(BM1LS3|BM1LS1|BM1LS0);SFRPI=0
#define OBM1_SetChannelSourceL_PWM6O()				SFRPI=8;BM1LCR=(BM1LCR&(~(BM1LS0|BM1LS1|BM1LS2|BM1LS3)))|(BM1LS3|BM1LS2);SFRPI=0

// OBM1 设置低位通道反相
#define OBM1_SetChannelL_EnInvert()					SFRPI=8;BM1LCR=BM1LCR|BM1LCP;SFRPI=0
#define OBM1_SetChannelL_DisInvert()				SFRPI=8;BM1LCR=BM1LCR&(~BM1LCP);SFRPI=0

// OBM1 设置高位通道
#define OBM1_SetChannelSourceH_VSS()				SFRPI=9;BM1HCR=BM1HCR&(~(BM1HS0|BM1HS1|BM1HS2|BM1HS3));SFRPI=0
#define OBM1_SetChannelSourceH_KBI3()				SFRPI=9;BM1HCR=(BM1HCR&(~(BM1HS0|BM1HS1|BM1HS2|BM1HS3)))|(BM1HS0);SFRPI=0
#define OBM1_SetChannelSourceH_KBI5()				SFRPI=9;BM1HCR=(BM1HCR&(~(BM1HS0|BM1HS1|BM1HS2|BM1HS3)))|(BM1HS1);SFRPI=0
#define OBM1_SetChannelSourceH_S0CKO()				SFRPI=9;BM1HCR=(BM1HCR&(~(BM1HS0|BM1HS1|BM1HS2|BM1HS3)))|(BM1HS0|BM1HS1);SFRPI=0
#define OBM1_SetChannelSourceH_INT1ET()				SFRPI=9;BM1HCR=(BM1HCR&(~(BM1HS0|BM1HS1|BM1HS2|BM1HS3)))|(BM1HS2);SFRPI=0
#define OBM1_SetChannelSourceH_PWM2O()				SFRPI=9;BM1HCR=(BM1HCR&(~(BM1HS0|BM1HS1|BM1HS2|BM1HS3)))|(BM1HS2|BM1HS0);SFRPI=0
#define OBM1_SetChannelSourceH_PWM3O()				SFRPI=9;BM1HCR=(BM1HCR&(~(BM1HS0|BM1HS1|BM1HS2|BM1HS3)))|(BM1HS2|BM1HS1);SFRPI=0
#define OBM1_SetChannelSourceH_PWM5O()				SFRPI=9;BM1HCR=(BM1HCR&(~(BM1HS0|BM1HS1|BM1HS2|BM1HS3)))|(BM1HS2|BM1HS1|BM1HS0);SFRPI=0
#define OBM1_SetChannelSourceH_T3CKO()				SFRPI=9;BM1HCR=(BM1HCR&(~(BM1HS0|BM1HS1|BM1HS2|BM1HS3)))|(BM1HS3);SFRPI=0
#define OBM1_SetChannelSourceH_T3EXI()				SFRPI=9;BM1HCR=(BM1HCR&(~(BM1HS0|BM1HS1|BM1HS2|BM1HS3)))|(BM1HS3|BM1HS0);SFRPI=0
#define OBM1_SetChannelSourceH_INT3ET()				SFRPI=9;BM1HCR=(BM1HCR&(~(BM1HS0|BM1HS1|BM1HS2|BM1HS3)))|(BM1HS3|BM1HS1);SFRPI=0
#define OBM1_SetChannelSourceH_PWM7O()				SFRPI=9;BM1HCR=(BM1HCR&(~(BM1HS0|BM1HS1|BM1HS2|BM1HS3)))|(BM1HS3|BM1HS1|BM1HS0);SFRPI=0
#define OBM1_SetChannelSourceH_T4CKO()				SFRPI=9;BM1HCR=(BM1HCR&(~(BM1HS0|BM1HS1|BM1HS2|BM1HS3)))|(BM1HS3|BM1HS2);SFRPI=0
#define OBM1_SetChannelSourceH_T4EXI()				SFRPI=9;BM1HCR=(BM1HCR&(~(BM1HS0|BM1HS1|BM1HS2|BM1HS3)))|(BM1HS3|BM1HS2|BM1HS0);SFRPI=0

// OBM1 设置高位通道反相
#define OBM1_SetChannelH_EnInvert()					SFRPI=9;BM1HCR=BM1HCR|BM1HCP;SFRPI=0
#define OBM1_SetChannelH_DisInvert()				SFRPI=9;BM1HCR=BM1HCR&(~BM1HCP);SFRPI=0

// OBM1 设置滤波
#define OBM1_SetFilter_Disable()					SFRPI=6;BM1CR=(BM1CR&(~(M1FLT0|M1FLT1)));SFRPI=0
#define OBM1_SetFilter_Sysclk_x3()					SFRPI=6;BM1CR=(BM1CR&(~(M1FLT0|M1FLT1)))|(M1FLT0);SFRPI=0
#define OBM1_SetFilter_SysclkDiv6_x3()				SFRPI=6;BM1CR=(BM1CR&(~(M1FLT0|M1FLT1)))|(M1FLT1);SFRPI=0
#define OBM1_SetFilter_T3OF_x3()					SFRPI=6;BM1CR=(BM1CR|(M1FLT0|M1FLT1));SFRPI=0

// OBM1 设置模式
#define OBM1_SetMode_BME1E0AndBM1E1()				SFRPI=6;BM1CR=(BM1CR&(~(BM1M0|BM1M1)));SFRPI=0
#define OBM1_SetMode_BME1E0OrBM1E1_Clear()			SFRPI=6;BM1CR=(BM1CR&(~(BM1M0|BM1M1)))|(BM1M0);SFRPI=0
#define OBM1_SetMode_BME1E0OrBM1E1_Set()			SFRPI=6;BM1CR=(BM1CR&(~(BM1M0|BM1M1)))|(BM1M1);SFRPI=0
#define OBM1_SetMode_BME1E0OrBM1E1_Toggle()			SFRPI=6;BM1CR=BM1CR|(BM1M0|BM1M1);SFRPI=0

// OBM1 设置输出使能
#define OBM1_EnOutput_INT0ET()						SFRPI=7;BM1SE=(BM1SE&(~(BM1E00|BM1E01|BM1E02|BM1E03)))|(BM1E00);SFRPI=0
#define OBM1_EnOutput_KBIET()						SFRPI=7;BM1SE=(BM1SE&(~(BM1E00|BM1E01|BM1E02|BM1E03)))|(BM1E01);SFRPI=0
#define OBM1_EnOutput_PWM2O()						SFRPI=7;BM1SE=(BM1SE&(~(BM1E00|BM1E01|BM1E02|BM1E03)))|(BM1E01|BM1E00);SFRPI=0
#define OBM1_EnOutput_T0CKO()						SFRPI=7;BM1SE=(BM1SE&(~(BM1E00|BM1E01|BM1E02|BM1E03)))|(BM1E02);SFRPI=0
#define OBM1_EnOutput_S0DO()						SFRPI=7;BM1SE=(BM1SE&(~(BM1E00|BM1E01|BM1E02|BM1E03)))|(BM1E02|BM1E00);SFRPI=0
#define OBM1_EnOutput_AC0OUT()						SFRPI=7;BM1SE=(BM1SE&(~(BM1E00|BM1E01|BM1E02|BM1E03)))|(BM1E02|BM1E01);SFRPI=0
#define OBM1_EnOutput_PWM5O()						SFRPI=7;BM1SE=(BM1SE&(~(BM1E00|BM1E01|BM1E02|BM1E03)))|(BM1E02|BM1E01|BM1E00);SFRPI=0
#define OBM1_EnOutput_INT2ET()						SFRPI=7;BM1SE=(BM1SE&(~(BM1E00|BM1E01|BM1E02|BM1E03)))|(BM1E03);SFRPI=0
#define OBM1_EnOutput_RTCKO()						SFRPI=7;BM1SE=(BM1SE&(~(BM1E00|BM1E01|BM1E02|BM1E03)))|(BM1E03|BM1E00);SFRPI=0
#define OBM1_EnOutput_T3CKO()						SFRPI=7;BM1SE=(BM1SE&(~(BM1E00|BM1E01|BM1E02|BM1E03)))|(BM1E03|BM1E01);SFRPI=0
#define OBM1_EnOutput_T3EXI()						SFRPI=7;BM1SE=(BM1SE&(~(BM1E00|BM1E01|BM1E02|BM1E03)))|(BM1E03|BM1E01|BM1E00);SFRPI=0
#define OBM1_EnOutput_AC2OUT()						SFRPI=7;BM1SE=(BM1SE&(~(BM1E00|BM1E01|BM1E02|BM1E03)))|(BM1E03|BM1E02|BM1E00);SFRPI=0
#define OBM1_EnOutput1_INT1ET()						SFRPI=7;BM1SE=(BM1SE&(~(BM1E10|BM1E11|BM1E12|BM1E13)))|(BM1E10);SFRPI=0
#define OBM1_EnOutput1_KBI1ET()						SFRPI=7;BM1SE=(BM1SE&(~(BM1E10|BM1E11|BM1E12|BM1E13)))|(BM1E11);SFRPI=0
#define OBM1_EnOutput1_PWM3O()						SFRPI=7;BM1SE=(BM1SE&(~(BM1E10|BM1E11|BM1E12|BM1E13)))|(BM1E11|BM1E10);SFRPI=0
#define OBM1_EnOutput1_T1CKO()						SFRPI=7;BM1SE=(BM1SE&(~(BM1E10|BM1E11|BM1E12|BM1E13)))|(BM1E12);SFRPI=0
#define OBM1_EnOutput1_T2CKO()						SFRPI=7;BM1SE=(BM1SE&(~(BM1E10|BM1E11|BM1E12|BM1E13)))|(BM1E12|BM1E10);SFRPI=0
#define OBM1_EnOutput1_T2EXI()						SFRPI=7;BM1SE=(BM1SE&(~(BM1E10|BM1E11|BM1E12|BM1E13)))|(BM1E12|BM1E11);SFRPI=0
#define OBM1_EnOutput1_PWM4O()						SFRPI=7;BM1SE=(BM1SE&(~(BM1E10|BM1E11|BM1E12|BM1E13)))|(BM1E12|BM1E11|BM1E10);SFRPI=0
#define OBM1_EnOutput1_BOD1ET()						SFRPI=7;BM1SE=(BM1SE&(~(BM1E10|BM1E11|BM1E12|BM1E13)))|(BM1E13);SFRPI=0
#define OBM1_EnOutput1_INT3ET()						SFRPI=7;BM1SE=(BM1SE&(~(BM1E10|BM1E11|BM1E12|BM1E13)))|(BM1E13|BM1E10);SFRPI=0
#define OBM1_EnOutput1_S1DO()						SFRPI=7;BM1SE=(BM1SE&(~(BM1E10|BM1E11|BM1E12|BM1E13)))|(BM1E13|BM1E11);SFRPI=0
#define OBM1_EnOutput1_AC1OUT()						SFRPI=7;BM1SE=(BM1SE&(~(BM1E10|BM1E11|BM1E12|BM1E13)))|(BM1E13|BM1E11|BM1E10);SFRPI=0
#define OBM1_EnOutput1_PWM6O()						SFRPI=7;BM1SE=(BM1SE&(~(BM1E10|BM1E11|BM1E12|BM1E13)))|(BM1E13|BM1E12);SFRPI=0
#define OBM1_EnOutput1_T4CKO()						SFRPI=7;BM1SE=(BM1SE&(~(BM1E10|BM1E11|BM1E12|BM1E13)))|(BM1E13|BM1E12|BM1E10);SFRPI=0
#define OBM1_EnOutput1_T4EXI()						SFRPI=7;BM1SE=(BM1SE&(~(BM1E10|BM1E11|BM1E12|BM1E13)))|(BM1E13|BM1E12|BM1E11);SFRPI=0

// OBM1 设置BM1OUT输出引脚
#define OBM1_SetBM1OUTP43()							SFRPI=4;AUXR7=(AUXR7&(~(BM1OE0|BM1OE1)))|(BM1OE0);SFRPI=0
#define OBM1_SetBM1OUTP31()							SFRPI=4;AUXR7=(AUXR7&(~(BM1OE0|BM1OE1)))|(BM1OE1);SFRPI=0
#define OBM1_SetBM1OUTP72()							SFRPI=4;AUXR7=AUXR7|(BM1OE0|BM1OE1);SFRPI=0

/*
*************************************************************************************
*/ 

/*
*************************************************************************************
*  AC0 
*
*************************************************************************************
*/
// AC0 设置低功耗模式
#define AC0_DisLowPower()							AC0CON=AC0CON&(~(AC0LP))
#define AC0_EnLowPower()							AC0CON=AC0CON|AC0LP

// AC0 设置掉电模式继续运行
#define AC0_DisPDRunning()							AC0CON=AC0CON&(~(AC0PDX))
#define AC0_EnPDRunning()							AC0CON=AC0CON|AC0PDX

// AC0 设置使能
#define AC0_Disable()								AC0CON=AC0CON&(~(AC0EN))
#define AC0_Enable()								AC0CON=AC0CON|AC0EN

// AC0 设置输出反相
#define AC0_SetOutput_DisInvert()					AC0CON=AC0CON&(~(AC0INV))
#define AC0_SetOutput_EnInvert()					AC0CON=AC0CON|AC0INV

// AC0 设置中断模式
#define AC0_SetInterruptMode_Falling()				AC0CON=AC0CON&(~(AC0M0|AC0M1))|(AC0M0)
#define AC0_SetInterruptMode_Rising()				AC0CON=AC0CON&(~(AC0M0|AC0M1))|(AC0M1)
#define AC0_SetInterruptMode_Togging()				AC0CON=AC0CON|(AC0M0|AC0M1)

// AC0 设置负极参考电压输入
#define AC0_SetNegaReferenceVoltage_IntVref()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRL)
#define AC0_SetNegaReferenceVoltage_1_24_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS0|NVRL)
#define AC0_SetNegaReferenceVoltage_2_24_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS1|NVRL)
#define AC0_SetNegaReferenceVoltage_3_24_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS0|NVRS1|NVRL)
#define AC0_SetNegaReferenceVoltage_4_24_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS2|NVRL)
#define AC0_SetNegaReferenceVoltage_5_24_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS2|NVRS0|NVRL)
#define AC0_SetNegaReferenceVoltage_6_24_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS2|NVRS1|NVRL)
#define AC0_SetNegaReferenceVoltage_7_24_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS2|NVRS1|NVRS0|NVRL)
#define AC0_SetNegaReferenceVoltage_8_24_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS3|NVRL)
#define AC0_SetNegaReferenceVoltage_9_24_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS3|NVRS0|NVRL)
#define AC0_SetNegaReferenceVoltage_10_24_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS3|NVRS1|NVRL)
#define AC0_SetNegaReferenceVoltage_11_24_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS3|NVRS1|NVRS0|NVRL)
#define AC0_SetNegaReferenceVoltage_12_24_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS3|NVRS2|NVRL)
#define AC0_SetNegaReferenceVoltage_13_24_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS3|NVRS2|NVRS0|NVRL)
#define AC0_SetNegaReferenceVoltage_14_24_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS3|NVRS2|NVRS1|NVRL)
#define AC0_SetNegaReferenceVoltage_15_24_VDD()		AC0MOD=AC0MOD|(NVRS3|NVRS2|NVRS1|NVRS0|NVRL)
#define AC0_SetNegaReferenceVoltage_AC0NI()			AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))
#define AC0_SetNegaReferenceVoltage_9_32_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS0)
#define AC0_SetNegaReferenceVoltage_10_32_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS1)
#define AC0_SetNegaReferenceVoltage_11_32_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS0|NVRS1)
#define AC0_SetNegaReferenceVoltage_12_32_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS2)
#define AC0_SetNegaReferenceVoltage_13_32_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS2|NVRS0)
#define AC0_SetNegaReferenceVoltage_14_32_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS2|NVRS1)
#define AC0_SetNegaReferenceVoltage_15_32_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS2|NVRS1|NVRS0)
#define AC0_SetNegaReferenceVoltage_16_32_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS3)
#define AC0_SetNegaReferenceVoltage_17_32_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS3|NVRS0)
#define AC0_SetNegaReferenceVoltage_18_32_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS3|NVRS1)
#define AC0_SetNegaReferenceVoltage_19_32_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS3|NVRS1|NVRS0)
#define AC0_SetNegaReferenceVoltage_20_32_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS3|NVRS2)
#define AC0_SetNegaReferenceVoltage_21_32_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS3|NVRS2|NVRS0)
#define AC0_SetNegaReferenceVoltage_22_32_VDD()		AC0MOD=AC0MOD&(~(NVRS0|NVRS1|NVRS2|NVRS3|NVRL))|(NVRS3|NVRS2|NVRS1)
#define AC0_SetNegaReferenceVoltage_23_32_VDD()		AC0MOD=AC0MOD&(~(NVRL))|(NVRS3|NVRS2|NVRS1|NVRS0)

// AC0 设置输出滤波
#define AC0_SetFilter_Disable()						AC0MOD=AC0MOD&(~(AC0FLT|AC0FLT1))
#define AC0_SetFilter_Sysclk_x3()					AC0MOD=AC0MOD&(~(AC0FLT|AC0FLT1))|(AC0FLT)
#define AC0_SetFilter_SysclkDiv6_x3()				AC0MOD=AC0MOD&(~(AC0FLT|AC0FLT1))|(AC0FLT1)
#define AC0_SetFilter_T3OF_x3()						AC0MOD=AC0MOD|(AC0FLT|AC0FLT1)

// AC0 设置正极输入引脚
#define AC0_SetPosInputP02()						AC0MOD=AC0MOD&(~(AC0PIS0|AC0PIS1))
#define AC0_SetPosInputP03()						AC0MOD=AC0MOD&(~(AC0PIS0|AC0PIS1))|(AC0PIS0)
#define AC0_SetPosInputP04()						AC0MOD=AC0MOD&(~(AC0PIS0|AC0PIS1))|(AC0PIS1)
#define AC0_SetPosInputP05()						AC0MOD=AC0MOD|(AC0PIS0|AC0PIS1)

// AC0 设置迟滞控制
#define AC0_DisHysteresis()							SFRPI=7;AUXR10=AUXR10&(~(AC0HC0));SFRPI=0
#define AC0_EnHysteresis()							SFRPI=7;AUXR10=AUXR10|AC0HC0;SFRPI=0

// AC0 设置AC0OUTPUT输出
#define AC0_DisableAC0OUTPUTP00()					SFRPI=1;AUXR4=AUXR4&(~(AC0OE));SFRPI=0
#define AC0_EnableAC0OUTPUTP00()					SFRPI=1;AUXR4=AUXR4|AC0OE;SFRPI=0

/*
*************************************************************************************
*/ 

/*
*************************************************************************************
*  AC1 
*
*************************************************************************************
*/
// AC1 设置低功耗模式
#define AC1_DisLowPower()							SFRPI=1;AC1CON=AC1CON&(~(AC1LP));SFRPI=0
#define AC1_EnLowPower()							SFRPI=1;AC1CON=AC1CON|AC1LP;SFRPI=0

// AC1 设置掉电模式继续运行
#define AC1_DisPDRunning()							SFRPI=1;AC1CON=AC1CON&(~(AC1PDX));SFRPI=0
#define AC1_EnPDRunning()							SFRPI=1;AC1CON=AC1CON|AC1PDX;SFRPI=0

// AC1 设置使能
#define AC1_Disable()								SFRPI=1;AC1CON=AC1CON&(~(AC1EN));SFRPI=0
#define AC1_Enable()								SFRPI=1;AC1CON=AC1CON|AC1EN;SFRPI=0

// AC1 设置输出反相
#define AC1_SetOutput_DisInvert()					SFRPI=1;AC1CON=AC1CON&(~(AC1INV));SFRPI=0
#define AC1_SetOutput_EnInvert()					SFRPI=1;AC1CON=AC1CON|AC1INV;SFRPI=0

// AC1 设置中断模式
#define AC1_SetInterruptMode_Falling()				SFRPI=1;AC1CON=AC1CON&(~(AC1M0|AC1M1))|(AC1M0);SFRPI=0
#define AC1_SetInterruptMode_Rising()				SFRPI=1;AC1CON=AC1CON&(~(AC1M0|AC1M1))|(AC1M1);SFRPI=0
#define AC1_SetInterruptMode_Togging()				SFRPI=1;AC1CON=AC1CON|(AC1M0|AC1M1);SFRPI=0

// AC1 设置负极参考电压输入
#define AC1_SetNegaInput_AC1NI()					SFRPI=1;AC1MOD=AC1MOD&(~(AC1NIS|NVRL1));SFRPI=0
#define AC1_SetNegaReferenceVoltage_IntVref()		SFRPI=1;AC1MOD=AC1MOD&(~(AC1NIS|NVRL1))|(NVRL1);SFRPI=0
#define AC1_SetNegaInput_ACNI0()					SFRPI=1;AC1MOD=AC1MOD&(~(AC1NIS|NVRL1))|(AC1NIS);SFRPI=0

// AC1 设置输出滤波
#define AC1_SetFilter_Disable()						SFRPI=1;AC1MOD=AC1MOD&(~(AC1FLT|AC1FLT1));SFRPI=0
#define AC1_SetFilter_Sysclk_x3()					SFRPI=1;AC1MOD=AC1MOD&(~(AC1FLT|AC1FLT1))|(AC1FLT);SFRPI=0
#define AC1_SetFilter_SysclkDiv6_x3()				SFRPI=1;AC1MOD=AC1MOD&(~(AC1FLT|AC1FLT1))|(AC1FLT1);SFRPI=0
#define AC1_SetFilter_T3OF_x3()						SFRPI=1;AC1MOD=AC1MOD|(AC1FLT|AC1FLT1);SFRPI=0

// AC1 设置迟滞控制
#define AC1_DisHysteresis()							SFRPI=7;AUXR10=AUXR10&(~(AC1HC0));SFRPI=0
#define AC1_EnHysteresis()							SFRPI=7;AUXR10=AUXR10|AC1HC0;SFRPI=0

// AC1 设置AC1OUTPUT输出
#define AC1_DisableAC1OUTPUTP72()					SFRPI=1;AUXR4=AUXR4&(~(AC1OE));SFRPI=0
#define AC1_EnableAC1OUTPUTP72()					SFRPI=1;AUXR4=AUXR4|AC1OE;SFRPI=0

/*
*************************************************************************************
*/ 

/*
*************************************************************************************
*  AC2 
*
*************************************************************************************
*/
// AC2 设置低功耗模式
#define AC2_DisLowPower()							SFRPI=2;AC2CON=AC2CON&(~(AC2LP));SFRPI=0
#define AC2_EnLowPower()							SFRPI=2;AC2CON=AC2CON|AC2LP;SFRPI=0

// AC2 设置掉电模式继续运行
#define AC2_DisPDRunning()							SFRPI=2;AC2CON=AC2CON&(~(AC2PDX));SFRPI=0
#define AC2_EnPDRunning()							SFRPI=2;AC2CON=AC2CON|AC2PDX;SFRPI=0

// AC2 设置使能
#define AC2_Disable()								SFRPI=2;AC2CON=AC2CON&(~(AC2EN));SFRPI=0
#define AC2_Enable()								SFRPI=2;AC2CON=AC2CON|AC2EN;SFRPI=0

// AC2 设置输出反相
#define AC2_SetOutput_DisInvert()					SFRPI=2;AC2CON=AC2CON&(~(AC2INV));SFRPI=0
#define AC2_SetOutput_EnInvert()					SFRPI=2;AC2CON=AC2CON|AC2INV;SFRPI=0

// AC2 设置中断模式
#define AC2_SetInterruptMode_Falling()				SFRPI=2;AC2CON=AC2CON&(~(AC2M0|AC2M1))|(AC2M0);SFRPI=0
#define AC2_SetInterruptMode_Rising()				SFRPI=2;AC2CON=AC2CON&(~(AC2M0|AC2M1))|(AC2M1);SFRPI=0
#define AC2_SetInterruptMode_Togging()				SFRPI=2;AC2CON=AC2CON|(AC2M0|AC2M1);SFRPI=0

// AC2 设置负极参考电压输入
#define AC2_SetNegaInput_AC2NI()					SFRPI=2;AC2MOD=AC2MOD&(~(AC2NIS|NVRL2));SFRPI=0
#define AC2_SetNegaReferenceVoltage_IntVref()		SFRPI=2;AC2MOD=AC2MOD&(~(AC2NIS|NVRL2))|(NVRL2);SFRPI=0
#define AC2_SetNegaInput_ACNI0()					SFRPI=2;AC2MOD=AC2MOD&(~(AC2NIS|NVRL2))|(AC2NIS);SFRPI=0

// AC2 设置输出滤波
#define AC2_SetFilter_Disable()						SFRPI=2;AC2MOD=AC2MOD&(~(AC2FLT|AC2FLT1));SFRPI=0
#define AC2_SetFilter_Sysclk_x3()					SFRPI=2;AC2MOD=AC2MOD&(~(AC2FLT|AC2FLT1))|(AC2FLT);SFRPI=0
#define AC2_SetFilter_SysclkDiv6_x3()				SFRPI=2;AC2MOD=AC2MOD&(~(AC2FLT|AC2FLT1))|(AC2FLT1);SFRPI=0
#define AC2_SetFilter_T3OF_x3()						SFRPI=2;AC2MOD=AC2MOD|(AC2FLT|AC2FLT1);SFRPI=0

// AC2 设置迟滞控制
#define AC2_DisHysteresis()							SFRPI=11;AUXR14=AUXR14&(~(AC2HC0));SFRPI=0
#define AC2_EnHysteresis()							SFRPI=11;AUXR14=AUXR14|AC2HC0;SFRPI=0

// AC2 设置AC2OUTPUT输出
#define AC2_DisableAC1OUTPUTP64()					SFRPI=11;AUXR4=AUXR4&(~(AC2OE));SFRPI=0
#define AC2_EnableAC1OUTPUTP64()					SFRPI=11;AUXR4=AUXR4|AC2OE;SFRPI=0

/*
*************************************************************************************
*/ 

/*
*************************************************************************************
*  EMB 
*
*************************************************************************************
*/
// EMB 设置数据线的Setup/Hold电平时间
#define EMB_SetDataBusSetupHoldTime_1Clk()					SFRPI=3;EMBTC=EMBTC&(~(RWSH));SFRPI=0
#define EMB_SetDataBusSetupHoldTime_2Clk()					SFRPI=3;EMBTC=EMBTC|RWSH;SFRPI=0

// EMB 设置数据线的Read/Write电平时间
#define EMB_SetReadWriteCommandTime_1Clk()					SFRPI=3;EMBTC=EMBTC&(~(RWS0|RWS1|RWS2));SFRPI=0
#define EMB_SetReadWriteCommandTime_2Clk()					SFRPI=3;EMBTC=EMBTC&(~(RWS0|RWS1|RWS2))|(RWS0);SFRPI=0
#define EMB_SetReadWriteCommandTime_3Clk()					SFRPI=3;EMBTC=EMBTC&(~(RWS0|RWS1|RWS2))|(RWS1);SFRPI=0
#define EMB_SetReadWriteCommandTime_4Clk()					SFRPI=3;EMBTC=EMBTC&(~(RWS0|RWS1|RWS2))|(RWS0|RWS1);SFRPI=0
#define EMB_SetReadWriteCommandTime_5Clk()					SFRPI=3;EMBTC=EMBTC&(~(RWS0|RWS1|RWS2))|(RWS2);SFRPI=0
#define EMB_SetReadWriteCommandTime_6Clk()					SFRPI=3;EMBTC=EMBTC&(~(RWS0|RWS1|RWS2))|(RWS0|RWS2);SFRPI=0
#define EMB_SetReadWriteCommandTime_7Clk()					SFRPI=3;EMBTC=EMBTC&(~(RWS0|RWS1|RWS2))|(RWS1|RWS2);SFRPI=0
#define EMB_SetReadWriteCommandTime_8Clk()					SFRPI=3;EMBTC=EMBTC|(RWS0|RWS1|RWS2);SFRPI=0

// EMB 设置EMB引脚
#define EMB_SetADPinP00_P07()								SFRPI=11;AUXR14=AUXR14&(~(ADLPS0|ADLPS1));SFRPI=0
#define EMB_SetADPinP20_P27()								SFRPI=11;AUXR14=AUXR14&(~(ADLPS0|ADLPS1))|(ADLPS0);SFRPI=0
#define EMB_SetADPinP72_P70P57P67_P64()						SFRPI=11;AUXR14=AUXR14&(~(ADLPS0|ADLPS1))|(ADLPS1);SFRPI=0
#define EMB_SetWRRDPinP36P37()								SFRPI=11;AUXR14=AUXR14&(~RWPS0);SFRPI=0
#define EMB_SetWRRDPinP37P36()								SFRPI=11;AUXR14=AUXR14|RWPS0;SFRPI=0

#endif



