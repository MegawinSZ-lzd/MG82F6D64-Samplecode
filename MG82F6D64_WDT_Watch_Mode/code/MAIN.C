/*********************************************************************
    ��Ŀ����:MG82F6D64-DEMO
    ��    ��:LZD
			���԰�ʹ�� MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=12MHz, SysCLK=12MHz
		  ����������
			  ����WDT,  ���������ģʽ,Լ2s���� 
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
	44236800,48000000
*************************************************/
#define MCU_SYSCLK		12000000
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

/***********************************************************************************
��������:   void INT_SF(void)
��������:SF(ϵͳ��־λ)�жϷ������
		 RTC,WDTF,BOD0F,BOD1F
�������:   
���ز���:     
*************************************************************************************/
void INT_SF(void) interrupt INT_VECTOR_SF
{
	if((PCON1&WDTF)!=0)
	{
		PCON1=PCON1|WDTF;
		LED_R=!LED_R;
	}
	else if((PCON1&BOF0)!=0)
	{
		PCON1=PCON1|BOF0;
	}
	else if((PCON1&BOF1)!=0)
	{
		PCON1=PCON1|BOF1;
	}
	else if((PCON1&RTCF)!=0)
	{
		PCON1=PCON1|RTCF;
	}
}


/*************************************************
��������:     void DelayXus(u16 xUs)
��������:   	��ʱ���򣬵�λΪus
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
		//CLRWDT();
		DelayXus(200);
		DelayXus(200);
		DelayXus(200);
		DelayXus(200);
		DelayXus(200);
		xMs--;
		
	}
}

/***********************************************************************************
��������:   void InitPort(void)
��������:   IO����
�������:   
���ز���:     
*************************************************************************************/
void InitPort(void)
{
	PORT_SetP5OpenDrainPu(BIT5|BIT6|BIT7);		// ���� P55,P56,P57 ��Ϊ��©������
}


/***********************************************************************************
��������:   void InitInterrupt(void)
��������:InitInterrupt��ʼ������
�������:   
���ز���:     
*************************************************************************************/
void InitInterrupt(void)
{
	INT_EnSF_WDT();			// ʹ��WDT�ж�,������ʹ��ϵͳ��־�ж�
	INT_EnSF();				// ʹ��ϵͳ��־�ж�

	
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
	InitInterrupt();

	INT_EnAll();
}



void main()
{
	u8 i;

	WDT_Clear();		// �忴�Ź�
	
    InitSystem();
    
	WDT_SetClock_32K_DIV_8_128ms(); // ����WDT���ʱ��Ϊ128ms
	WDT_Enable_OnPD();	// �ڵ���ģʽ��ʹ��WDT
	WDT_Enable();		// ʹ�ܿ��Ź�
	
	LED_G_0=0;LED_G_1=0;LED_R=0;
	for(i=0;i<10;i++)
	{
		DelayXms(100);
		WDT_Clear();		// �忴�Ź�
	}
	LED_G_0=1;LED_G_1=1;LED_R=1;

	while(1)
	{ //  δ�忴�Ź�, �������ж�
		for(i=0;i<20;i++)
		{
			LED_G_0=!LED_G_0;
			DelayXms(100);
		}
		WDT_Clear();		// �忴�Ź�
		WDT_SetClock_32K_DIV_128_2048ms();	// ����WDT���ʱ��Ϊ2s
		POW_SetMode_PD();	// �������ģʽ
		_nop_();
		WDT_Clear();		// �忴�Ź�
		WDT_SetClock_32K_DIV_8_128ms();	// ����WDT���ʱ��Ϊ128ms
	}
}
