/*********************************************************************
    ��Ŀ����:MG82F6D64-DEMO
    ��    ��:YZY
			���԰�ʹ�� MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=32MHz, SysCLK=32MHz
	����������
			����P10,P11,P16����INT1,INT0,INT2,�½��ش���
			20s���жϴ���,�������ģʽ
			INT1(P10),INT0(P15),INT2(P11),INT3(P16)�͵�ƽ����

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
����ϵͳʱ��SysClk
��ѡ: 
	11059200,12000000,
	22118400,24000000,
	29491200,32000000,
	44236800,48000000
*************************************************/
#define MCU_SYSCLK		32000000
/*************************************************/
/*************************************************
����CPUʱ�� CpuClk (������36MHz)
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
��������:   void INT_INT0(void)	
��������:INT0�жϷ������
�������:   
���ز���:     
*************************************************************************************/
void INT_INT0(void)		interrupt INT_VECTOR_INT0
{
	WakeUpSorce=0;
	INTCnt=0;
	LED_G_0=1;LED_G_1=1;LED_R=1;
}

/***********************************************************************************
��������:   void INT_INT1(void)	
��������:INT1�жϷ������
�������:   
���ز���:     
*************************************************************************************/
void INT_INT1(void)		interrupt INT_VECTOR_INT1
{
	WakeUpSorce=1;
	INTCnt=0;
	LED_G_0=1;LED_G_1=1;LED_R=1;
}

/***********************************************************************************
��������:   void INT_INT2(void)	
��������:INT2�жϷ������
�������:   
���ز���:     
*************************************************************************************/
void INT_INT2(void)		interrupt INT_VECTOR_INT2
{
	WakeUpSorce=2;
	INTCnt=0;
	LED_G_0=1;LED_G_1=1;LED_R=1;
}

/***********************************************************************************
��������:   void INT_INT3(void)	
��������:INT3�жϷ������
�������:   
���ز���:     
*************************************************************************************/
void INT_INT3(void)		interrupt INT_VECTOR_INT3
{
	WakeUpSorce=3;
	INTCnt=0;
	LED_G_0=1;LED_G_1=1;LED_R=1;
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
��������:   void InitPort(void)
��������:   IO����
�������:   
���ز���:     
*************************************************************************************/
void InitPort(void)
{
	PORT_SetP5PushPull(BIT5|BIT6|BIT7);					// ����P55,P56,P57Ϊ�������(LED)
	PORT_SetP3QuasiBi(BIT0|BIT1|BIT3|BIT4|BIT5);		// ����P30,P31,P33,P34,P35Ϊ׼˫���
	PORT_SetP1OpenDrainPu(BIT0|BIT1|BIT5|BIT6);			// ����P10,P11,P15,P16Ϊ��©�������������INTʹ��
}
/***********************************************************************************
��������:   void InitClock(void)
��������:   ʱ�ӳ�ʼ��	
�������:   
���ز���:     
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

	// P60 ��� MCK/4
	//CLK_P60OC_MCKDiv4();
}

/***********************************************************************************
��������:   void InitInterrupt(void)
��������:   �жϳ�ʼ��
�������:   
���ز���:     
*************************************************************************************/
void InitInterrupt(void)
{
	INT_EnINT0();							//	ʹ��INT0�ж�
	INT_EnINT1();							//	ʹ��INT1�ж�
	INT_EnINT2();							//	ʹ��INT2�ж�
	INT_EnINT3();							//	ʹ��INT3�ж�
	INT_EnAll();				// �������ж�
}	

/***********************************************************************************
��������:   void InitINT0(void)
��������:   INT0��ʼ��
�������:   
���ز���:     
*************************************************************************************/
void InitINT0(void)
{
	INT_SetINT0P10();						//����INT0����P10
	INT_SetINT0_DetectEdge();				//����INT0�ⲿ�жϼ�ⷽʽΪ���ؼ��, ���������ģʽ��,�Զ�תΪ��ƽ���
	INT_SetINT0_DetectLowFalling();			//����INT0�ⲿ�жϼ�ⷽʽΪ�͵�ƽ���½��ش���
	INT_SetINT0Filter_SysclkDiv6_x3();		//����INT0�˲�Ϊ��sysclk/6��*3

}

/***********************************************************************************
��������:   void InitINT1(void)
��������:   INT1��ʼ��
�������:   
���ز���:     
*************************************************************************************/
void InitINT1(void)
{
	INT_SetINT1P15();						//����INT1����P10
	INT_SetINT1_DetectEdge();				//����INT1�ⲿ�жϼ�ⷽʽΪ���ؼ��, ���������ģʽ��,�Զ�תΪ��ƽ���
	INT_SetINT1_DetectLowFalling();			//����INT1�ⲿ�жϼ�ⷽʽΪ�͵�ƽ���½��ش���
	INT_SetINT1Filter_SysclkDiv6_x3();		//����INT1�˲�Ϊ��sysclk/6��*3

}

/***********************************************************************************
��������:   void InitINT2(void)
��������:   INT2��ʼ��
�������:   
���ز���:     
*************************************************************************************/
void InitINT2(void)
{
	INT_SetINT2P11();						//����INT2����P11
	INT_SetINT2_DetectEdge();				//����INT2�ⲿ�жϼ�ⷽʽΪ���ؼ��, ���������ģʽ��,�Զ�תΪ��ƽ���
	INT_SetINT2_DetectLowFalling();			//����INT2�ⲿ�жϼ�ⷽʽΪ�͵�ƽ���½��ش���
	INT_SetINT2Filter_SysclkDiv6_x3();		//����INT2�˲�Ϊ��sysclk/6��*3

}

/***********************************************************************************
��������:   void InitINT3(void)
��������:   INT3��ʼ��
�������:   
���ز���:     
*************************************************************************************/
void InitINT3(void)
{
	INT_SetINT3P16();							//����INT3����P16
	INT_SetINT3_DetectEdge();				//����INT3�ⲿ�жϼ�ⷽʽΪ���ؼ��, ���������ģʽ��,�Զ�תΪ��ƽ���
	INT_SetINT3_DetectLowFalling();			//����INT3�ⲿ�жϼ�ⷽʽΪ�͵�ƽ���½��ش���
	INT_SetINT3Filter_SysclkDiv6_x3();		//����INT3�˲�Ϊ��sysclk/6��*3

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

			BakCKCON2=IAP_ReadPPage(CKCON2_P);							// ����CKCON2 (ʹ����PLL CKM��Ƶ�������)
			IAP_WritePPage(CKCON2_P,BakCKCON2&(~(MCKS0_P|MCKS1_P)));	// MCK=OSCin (ʹ����PLL CKM��Ƶ�������)
			
			POW_SetMode_PD();											// ʹMCU�������ģʽ
			
			_nop_();
			DelayXus(100);												// ��ʱ����CKM�ȶ� (ʹ����PLL CKM��Ƶ�������)
			IAP_WritePPage(CKCON2_P,BakCKCON2); 						// �ָ�CKCON2,ѡ��ʹ�ñ�Ƶ (ʹ����PLL CKM��Ƶ�������)

			INT_DisAll();							// ��ֹȫ���ж�	
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
			INT_EnAll();							// ʹ��ȫ���ж�	
			INTCnt=0;
		}

	}

}

