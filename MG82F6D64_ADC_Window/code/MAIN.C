/*********************************************************************
    ��Ŀ����:MG82F6D64-DEMO
    ��    ��:YZY
			���԰�ʹ�� MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=12MHz, SysCLK=12MHz
	����������
			ADC ���ڵ�ѹ��⣬�����ɼ�ADCֵ�����uart��
			��ADCֵ���ڴ�����ֵ�ڣ�����ڴ��������Ϣ��
			�ɼ����ţ�P10	   ���ڣ�P30 P31

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

#define LED_G_0		P55
#define LED_R		P56
#define LED_G_1		P57

bit bES0;
bit bADCFinish =0;
WordTypeDef wAdcValue;

#define TEST_ADC_BUF_SIZE	16
xdata u16 wAdcBuf[TEST_ADC_BUF_SIZE];
u8 AdcBufCnt;

/***********************************************************************************
��������:   void INT_ADC(void)
��������:ADC �жϷ������
		 
�������:   
���ز���:     
*************************************************************************************/
void INT_ADC(void) interrupt INT_VECTOR_ADC
{
	
	if((ADCON0 & ADCWI) != 0)	//�����ж�
	{
		ADCON0 &= ~ADCWI;//��������ж�
		LED_R = ~LED_R;
		wAdcValue.B.BHigh = ADCDH;
   	 	wAdcValue.B.BLow = ADCDL;
		wAdcValue.W &= 0xFFF;
		wAdcBuf[AdcBufCnt]=wAdcValue.W;
		AdcBufCnt++;
		if(AdcBufCnt>=TEST_ADC_BUF_SIZE)
		{
			bADCFinish=TRUE;
			INT_DisADC();		// Buf����,��ֹADC�ж�
		}
		
	}
	
}

/*************************************************
��������: char putchar (char c)   
��������: ����printf �Ĵ����������
�������:     char c
�������:     
*************************************************/
char putchar (char c)   
{      
	bit bES;
	bES=ES0;
    ES0=0;        
    S0BUF = c;        
    while(TI0==0);        
    TI0=0;        
    ES0=bES;        
    return 0;
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
��������:   void InitUart0(void)
��������:   Uart0��ʼ��
		 ʹ��S0BRG��Ϊ��������Դ
�������:   
���ز���:     
*************************************************************************************/
void InitUart0_S0BRG(void)
{
	
	UART0_SetAccess_S0CR1();			// ����SFR 0xB9��ַ�Ƿ���S0CR1
	UART0_SetMode8bitUARTVar();			// 8λ �ɱ䲨����
	UART0_EnReception();				// ʹ�ܽ���
	UART0_SetBRGFromS0BRG();			// ���ò�������ԴΪ S0BRG
	UART0_SetS0BRGBaudRateX2();			// ����2x������
	UART0_SetS0BRGSelSYSCLK();			// ����S0BRGʱ������ΪSysclk
	UART0_SetRxTxP30P31();

	// ����S0BRG Value
	UART0_SetS0BRGValue(S0BRG_BRGRL_9600_2X_12000000_1T);
	
	UART0_EnS0BRG();					// ����S0BRGʱ��
}

/***********************************************************************************
��������:   void InitPort(void)
��������:   IO����
�������:   
���ز���:     
*************************************************************************************/
void InitPort(void)
{
	PORT_SetP5PushPull(BIT5|BIT6|BIT7);		// ���� P55,P56,P57��Ϊ�������(LED)
	PORT_SetP1AInputOnly(BIT0);				// ����P10(AIN0)Ϊ��ģ������
}

/***********************************************************************************
��������:   void InitADC(void)
��������:   ADC��ʼ��
�������:   
���ز���:     
*************************************************************************************/
void InitADC(void)
{
	ADC_Enable();						// ʹ��ADC
	ADC_SetClock_SYSCLK();			    // ADCת��ʱ��Ϊ SYSCLK
	ADC_SetMode_FreeRunning();			// ADCȫ������
	ADC_SetADCData_12Bit();				// ADC����λ: 12bit			
	ADC_SetRightJustified();			// ADCת�������Ҷ���

	ADC_SetWindow_WithIn();				// �����ڴ�������λADCWI
	ADC_SetWindow_High(0x700);			// ����ADC������ֵ�ߵ�
	ADC_SetWindow_Low(0x000);			// ����ADC������ֵ�͵�

	ADC_DisInterrupt_ADCI();			// ��ֹADC��ɱ�־λADCI�Ƿ�����ж�
	ADC_EnInterrupt_ADCWI();			// ʹ��ADC���ڱ�־λADCWI�����ж�
	ADC_DisInterrupt_SMPF();			// ��ֹADC������ɱ�־λSFPF�����ж�
	
}

/***********************************************************************************
��������:   void InitInterrupt(void)
��������:   �жϳ�ʼ��
�������:   
���ز���:     
*************************************************************************************/
void InitInterrupt(void)
{
	INT_EnADC();						// ʹ��ADC�ж�
	INT_EnAll();						//  ʹ��ȫ���ж�
	
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
	InitADC();
	InitUart0_S0BRG();
	InitInterrupt();
}

void main()
{
	u8 i;
	
	InitSystem();
	AdcBufCnt=0;
	bADCFinish=FALSE;

	printf("\nStart!");

	LED_G_0=0;LED_R=0;LED_G_1=0;
	DelayXms(1000);
	LED_G_0=1;LED_R=1;LED_G_1=1;

  	while(1)
  	{
		DelayXms(100);
		LED_G_0 = !LED_G_0;

		if(bADCFinish)
		{
			LED_R=1;
			printf("\nADC Win:");
			for(i=0;i<TEST_ADC_BUF_SIZE;i++)
			{
				printf("%04X ",wAdcBuf[i]);
			}
			bADCFinish=FALSE;
			INT_EnADC();	// ����������,����ʹ��ADC�ж�
		}
			
    }
}

