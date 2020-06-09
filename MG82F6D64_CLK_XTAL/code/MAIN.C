/*********************************************************************
    ��Ŀ����:MG82F6D64-DEMO
    ��    ��:LZD
			���԰�ʹ�� MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=24MHz, SysCLK=24MHz
	����������
			ʹ���ⲿ����
	ע�����

    ����ʱ��:
    �޸���־:
    
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
����ϵͳʱ��SysClk (MAX.50MHz)
��ѡ: 
	11059200,12000000,
	22118400,24000000,
	29491200,32000000,
	33170000,36000000,
	44236800,48000000
*************************************************/
#define MCU_SYSCLK		24000000
/*************************************************/
/*************************************************
����CPUʱ�� CpuClk (MAX.36MHz)
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

/*************************************************
��������:     void DelayXus(u16 xUs)
��������:   	��ʱ���򣬵�λΪus
���ú���:        
�������:     u8 Us -> *1us  (1~255)
�������:     
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
��������:     void DelayXms(u16 xMs)
��������:     ��ʱ���򣬵�λΪms
�������:     u16 xMs -> *1ms  (1~65535)
�������:     
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
��������:   void InitClock_Xtal(void)
��������:   ʱ�ӳ�ʼ��,ʹ���ⲿ����	
�������:   
���ز���:     
*************************************************************************************/
void InitClock_Xtal(void)
{
	// Enable XTAL,Enable IHRCO, MCK=OSCin, OSCin=IHRCO
	CLK_SetCKCON2(GAIN_FOR_2M_25M|ENABLE_XTAL|ENABLE_IHRCO|MCK_OSCin|OSCIn_IHRCO);
	// Wait for XTAL stanby
	while((CKCON1&XTOR)==0);
	// Enable XTAL,Enable IHRCO, MCK=OSCin, OSCin=XTAL
	CLK_SetCKCON2(GAIN_FOR_2M_25M|ENABLE_XTAL|ENABLE_IHRCO|MCK_OSCin|OSCIn_XTAL);
	DelayXms(2);
	// Enable XTAL,Disable IHRCO, MCK=OSCin, OSCin=XTAL
	CLK_SetCKCON2(GAIN_FOR_2M_25M|ENABLE_XTAL|DISABLE_IHRCO|MCK_OSCin|OSCIn_XTAL);

}


/***********************************************************************************
��������:   void InitPort(void)
��������:   IO����
�������:   
���ز���:     
*************************************************************************************/
void InitPort(void)
{
	PORT_SetP5PushPull(BIT5|BIT6|BIT7);					// ����P55,P56,P57Ϊ�������(LED)
	PORT_SetP6PushPull(BIT0);							// ���� P60 Ϊ�������������ʱ�����
}




/***********************************************************************************
��������:   void InitSystem(void)
��������:   ϵͳ��ʼ��
�������:   
���ز���:     
*************************************************************************************/
void InitSystem(void)
{

	InitPort();
	InitClock_Xtal();

}



void main()
{
	
    InitSystem();

	LED_G_0=0;LED_G_1=0;LED_R=0;
	DelayXms(1000);
	LED_G_0=1;LED_G_1=1;LED_R=1;

					
    while(1)
    {
		LED_G_0=0;
		DelayXms(200);
		LED_G_0=1;
		LED_G_1=0;
		DelayXms(200);
		LED_G_1=1;
		LED_R=0;
		DelayXms(200);
		LED_R=1;
    }
}
