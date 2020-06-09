/*********************************************************************
    ��Ŀ����:MG82F6D64-DEMO
    ��    ��:YZY
			���԰�ʹ�� MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=12MHz, SysCLK=12MHz
	����������
			RTCʱ��ΪXTAL 32.768KHz
			RTC Clock Ԥ��Ƶ 32768
			RTC ����ֵ (64-1)=63
			RTC���Ƶ��Ϊ 32768/32768/1 = 1Hz
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
��������:   u8 IAP_ReadPPage(u8 PsfrAddr)
��������:��ȡPҳ���⹦��������
�������:   
		 u8 PsfrAddr: Pҳ��ַ
���ز���:     
		 u8: Pҳ����
*************************************************************************************/
u8 IAP_ReadPPage(u8 PsfrAddr)
{
	bit bEA=EA;
	EA = 0; 					//���ж�
	IFADRH = 0; 				//IFADRH����Ϊ0
	IFADRL= PsfrAddr;			//��Pҳ��ַ;
	IFMT = ISP_READ_P;
	ISPCR = 0x80;				//���õȴ�ʱ��, ����ISP/IAP����
	SCMD = 0x46;
	SCMD = 0xB9;
	nop();
	IFMT=0;
	ISPCR = 0;					//��ISP/IAP����Ĵ�������ֹ�����
	EA = bEA;					//�ָ��ж�
	return IFD;
}


/***********************************************************************************
��������:void IAP_WritePPage(u8 PsfrAddr,u8 PsfrData)
��������:дPҳ���⹦����
�������:   
		 u8 PsfrAddr: Pҳ��ַ,u8 PsfrData:  Pҳ����
���ز���:     
*************************************************************************************/
void IAP_WritePPage(u8 PsfrAddr,u8 PsfrData)
{
	bit bEA=EA;
	EA = 0; 					//���ж�
	IFADRH = 0; 				//IFADRH����Ϊ0
	IFADRL= PsfrAddr;			//��Pҳ��ַ;
	IFD= PsfrData;				//��Pҳ����
	IFMT = ISP_WRITE_P;
	ISPCR = 0x80;				//���õȴ�ʱ��, ����ISP/IAP����
	SCMD = 0x46;
	SCMD = 0xB9;
	nop();
	IFMT=0;
	ISPCR = 0;					//��ISP/IAP����Ĵ�������ֹ�����
	EA = bEA;					//�ָ��ж�
}


/***********************************************************************************
��������:   void InitRTC(void)
��������:RTC ��ʼ������	
		RTCʱ��ΪILRCO 32KHz
		RTC Clock Ԥ��Ƶ 32
		RTC ����ֵ (64-10)=54
		RTC���Ƶ��Ϊ 32K/32/10=100Hz
�������:   
���ز���:     
*************************************************************************************/
void InitRTC(void)
{
	u8 x;
	x=IAP_ReadPPage(CKCON2_P);
	x=(x&0x3F)|(ENABLE_XTAL|GAIN_FOR_32K);
	IAP_WriteByte(CKCON2_P,x);
	while(CKCON1 & XTOR == 0x00);		// �ȴ�XTAL׼������
	
	RTC_SetClock_XTAL2_ECKI_P60();		// ѡ��RTCʱ��Ϊ XTAL 32.768KHz
	RTC_SetClock_Div_32768();			// ѡ��RTC Clock Ԥ��Ƶ 32768
	RTC_SetReload(64-1);				// RTC ����ֵ (64-1)=63
	RTC_SetCounter(64-1);	
	RTC_ClearRTCF();					// ��RTCF
	RTC_Enable();						// ʹ��RTC
	
}

/***********************************************************************************
��������:   void InitInterrupt(void)
��������:InitInterrupt��ʼ������
�������:   
���ز���:     
*************************************************************************************/
void InitInterrupt(void)
{
	INT_EnSF_RTC();			// ʹ��RTC�ж�,������ʹ��ϵͳ��־�ж�
	INT_EnSF();				// ʹ��ϵͳ��־�ж�

	
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
	InitRTC();			
	InitInterrupt();		// �жϳ�ʼ��
	
	INT_EnAll();			// ʹ��ȫ���ж�

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

