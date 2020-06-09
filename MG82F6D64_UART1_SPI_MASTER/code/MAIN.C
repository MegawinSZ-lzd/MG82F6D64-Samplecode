/*********************************************************************
    ��Ŀ����:MG82F6D64-DEMO
    ��    ��:YZY
			���԰�ʹ�� MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=12MHz, SysCLK=12MHz
			����������
				����1��������ģʽ4 (��SPI����ģʽ)
				SPICLK <-->TXD	P11
				MOSI	<-->RXD  P10
				MISO	<-->S0MI P61
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

#define SPI_NSS		P33

sfr UARTSPICLK		= 0xBB;

// ѡ��SPICLK ��ʼ��ƽ
#define SPICLK_LOW	0	// SPICLK  ��ʼΪ�͵�ƽ
#define SPICLK_HIGH	1	// SPICLK  ��ʼΪ�͵�ƽ
#define SPI_CPOL  SPICLK_HIGH	

// ѡ�����ݴ���˳��
#define LSB	0	// ��λ����
#define MSB	1	// ��λ����
#define SPI_DORD	MSB

// ѡ��SPI �Ĵ����ٶ�
#define SPI_CLK_SYSCLK_4	0	// SYSCLK/4
#define SPI_CLK_SYSCLK_12	1	// SYSCLK/12
#define SPI_CLOCK	SPI_CLK_SYSCLK_4

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
��������:   void InitPort(void)
��������:   IO����
�������:   
���ز���:     
*************************************************************************************/
void InitPort(void)
{
	PORT_SetP5PushPull(BIT5|BIT6|BIT7);
	PORT_SetP3PushPull(BIT3);				// ����P33,P34,P35Ϊ�����������NSS
	PORT_SetP1PushPull(BIT2|BIT3);			// ����P12��P13Ϊ���죬������SPIʹ��
	PORT_SetP6OpenDrainPu(BIT1);			// ����P61,��MISO
}

/***********************************************************************************
��������:   u8 Uart1SPITransceiver(u8 ToSend)
��������:   ����SPI�������ͺ���
�������:   u8 ToSend
���ز���:     
*************************************************************************************/
u8 Uart1SPITransceiver(u8 ToSend)
{
	u8 x;
	SFR_SetPage(1);
	S1BUF=ToSend;
	while(TI1==0);
	TI1=0;
	x=S1BUF;
	SFR_SetPage(0);
	return x;
}

/***********************************************************************************
��������:   void InitUart1_SPI_Master(void)
��������:   Uart1��ʼ�� SPI Master
�������:   
���ز���:     
*************************************************************************************/
void InitUart1_SPI_Master(void)
{
	P13=1;
	UART1_SetModeSPIMaster();			// ���ô���SPI����ģʽ
	UART1_SetMISOP61();					// ���ô���SPI��MISO����ΪP61
	UART1_SetRxTxP12P13();				// ���ô���SPI��RX��MOSI��,TX(SPI_CLK)����ΪP12,P13
#if (SPI_CLOCK == SPI_CLK_SYSCLK_4)
	UART1_SetSPIClock_SYSCLKDiv4();		// ����SPIʱ��Ϊϵͳʱ�ӵ�4��Ƶ
#else
	UART1_SetSPIClock_SYSCLKDiv12();	// ����SPIʱ��Ϊϵͳʱ�ӵ�12��Ƶ
#endif
#if (SPI_DORD==MSB)
	UART1_SetDataMSB();
#else
	UART1_SetDataLSB();
#endif

#if (SPI_CPOL==SPICLK_LOW)
	UARTSPICLK = 0x23;					// SPICLK  ��ʼΪ�͵�ƽ

#endif

	
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
	InitUart1_SPI_Master();
}

void main()
{
	InitSystem();
	LED_G_1=0;LED_R=0;
	DelayXms(1000);
	LED_G_1=1;LED_R=1;	
	while(1)
	{
		DelayXms(100);
		LED_G_0 = !LED_G_0;	
        SPI_NSS=0;
		Uart1SPITransceiver(0x05);	
        SPI_NSS=1;
	}

}

