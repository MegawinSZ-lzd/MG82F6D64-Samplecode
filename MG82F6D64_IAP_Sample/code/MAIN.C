/*********************************************************************
    ��Ŀ����:MG82F6D64-DEMO
    ��    ��:YZY
			���԰�ʹ�� MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=12MHz, SysCLK=12MHz
	����������
			IAP ����
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

#define ISP_ADDRESS    0xFE00	 //ISP��ʼ��ַ 
#define IAP_ADDRESS    0xF600	 //IAP��ʼ��ַ  
idata u8 TrapFlag[3];

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
��������:   	void CheckTrapFlag(void)
��������:   	����Ƿ���ȷ�����̽���IAP��������ֹ�����
				���TrapFlag != "Win",���ʾ�ǷǷ�����,��Ƭ������λ
�������:   	
���ز���:     
*************************************************************************************/
void CheckTrapFlag(void)
{ 
	if ((TrapFlag[0]!='W')||(TrapFlag[1]!='i')||(TrapFlag[2]!='n'))
	{
		//������λ
		POW_ResetToAP();
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
	CheckTrapFlag();
	SCMD = 0x46;
	CheckTrapFlag();
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
	CheckTrapFlag();
	SCMD = 0x46;
	CheckTrapFlag();
	SCMD = 0xB9;
	nop();
	IFMT=0;
	ISPCR = 0;					//��ISP/IAP����Ĵ�������ֹ�����
	EA = bEA;					//�ָ��ж�
}


/***********************************************************************************
��������:u8 IAP_ReadByte(u16 ByteAddr)
��������:��ָ����ַ��IAP����
�������:   
		 u16 ByteAddr: IAP��ַ
���ز���:     
*************************************************************************************/
u8 IAP_ReadByte(u16 ByteAddr)
{
		bit bEA=EA;
		IFADRH = ByteAddr>>8;		//�͵�ַ���ֽ�
		IFADRL= ByteAddr;			//�͵�ַ���ֽ�;
		EA = 0;						//���ж�
		IFMT = ISP_READ;
		ISPCR = 0x80; 				//����ISP/IAP����
		CheckTrapFlag();
		SCMD = 0x46;
		CheckTrapFlag();
		SCMD = 0xB9;
		IFMT=0;
		ISPCR = 0;					//��ISP/IAP����Ĵ�������ֹ�����
		EA = bEA; 					//�ָ��ж�
		return IFD;
}
// ����ʹ��MOVC�ķ�ʽ��ȡIAP������
#define	IAP_ReadByteByMOVC(x)	CBYTE[x]

/***********************************************************************************
��������:void IAP_ErasePage(u8 ByteAddr)
��������:ҳ�����, ����ָ����ַ���ڵ�ҳ�档
		����һ��ҳ���Լ��Ҫ30ms
�������:   
		 u8 ByteAddr: ָ����ҳ���ַ��8λ
*************************************************************************************/
void IAP_ErasePage(u8 ByteAddr)
{
		bit bEA=EA;
		IFADRH = ByteAddr;		//�͵�ַ���ֽ�
		IFADRL= 0x00;			//�͵�ַ���ֽ�;
		EA = 0;					//���ж�
		IFMT = ISP_ERASE;
		ISPCR = 0x80; 			//����ISP/IAP����
		CheckTrapFlag();
		SCMD = 0x46;
		CheckTrapFlag();
		SCMD = 0xB9;
		nop();
		IFMT=0;
		ISPCR = 0;					//��ISP/IAP����Ĵ�������ֹ�����
		EA = bEA; 					//�ָ��ж�
}

/***********************************************************************************
��������:void IAP_WriteByte(u16 ByteAddr,u8 ByteData)
��������:д�ֽڵ�ָ����ַ, �õ�ַ����Ϊ��(0xFF),����Ҫ��ִ��ҳ�����
		 д��һ���ֽ����ݴ�Լ��Ҫ80us
�������:   
		 u16 ByteAddr: ָ����IAP��ַ, u8 ByteData: ��д������
���ز���:     
*************************************************************************************/

void IAP_WriteByte(u16 ByteAddr,u8 ByteData)
{
		bit bEA=EA;
		IFD = ByteData;				//����Ҫд������	
		IFADRH = ByteAddr>>8;		//�͵�ַ���ֽ�
		IFADRL= ByteAddr;			//�͵�ַ���ֽ�;
		EA = 0;						//���ж�
		IFMT = ISP_WRITE;
		ISPCR = 0x80;               //����ISP/IAP����
		CheckTrapFlag();
		SCMD = 0x46;
		CheckTrapFlag();
		SCMD = 0xB9;
		nop();
		IFMT=0;
		ISPCR = 0;					//��ISP/IAP����Ĵ�������ֹ�����
		EA = bEA; 					//�ָ��ж�
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
	// Sets B.R. value
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
	PORT_SetP5PushPull(BIT5|BIT6|BIT7);					// ����P55,P56,P57Ϊ�������(LED)
	PORT_SetP1OpenDrainPu(BIT0);					    // ����P10 Ϊ��©���������ģʽ
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

	// ����IAPʱ�� OSCin=12MHz
	CLK_SetIAPTimeBase(12);
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
	InitUart0_S0BRG();	
	TrapFlag[1]='i';
}




void main()
{
	u8 IAPData;
	u8 i;
	u8 rand;
	bit bOK=TRUE;

	TrapFlag[0]='W';
	
    InitSystem();
    
	LED_G_0=0;LED_G_1=0;LED_R=0;
	DelayXms(1000);
	printf("\nStart IAP DEMO!");
	LED_G_0=1;LED_G_1=1;LED_R=1;

	TrapFlag[2]='n';
	//��IAP��ʼ��ʼ��ַ
	i=IAP_ReadPPage(IAPLB_P);
    printf("\nRead IAPLB:%02bX",i);

	// ����IAP��ʼ��ַΪISP��ַ,����IAP�ռ�
	IAP_WritePPage(IAPLB_P,ISP_ADDRESS/256);
	i=IAP_ReadPPage(IAPLB_P);
    printf("\nNOW IAPLB:%02bX  ",i);
	TrapFlag[2]=0xFF;

	printf("\nOLD IAP %04X:\n",IAP_ADDRESS);
	i=0;
	do{
		LED_G_1=0;
		IAPData=IAP_ReadByteByMOVC(IAP_ADDRESS+i);	// ��MOVC�ķ�ʽ��ȡ����
		LED_G_1=1;
		printf("%02bX ",IAPData);
		if((i&0x0F)==0x0F)
		{
			printf("\n");
		}
		i++;
	}while(i!=0);
    

    while(1)
    {
    	rand++;
		LED_G_0=!LED_G_0;
	   	DelayXms(100);
		if(P10==0)
		{
			DelayXms(50);
			if(P10==0)
			{
				TrapFlag[2]='n';
				// ����IAP��ʼ��ַΪ��������IAP��ַ
				printf("\nWrite IAPLB:%02bX  ",IAP_ADDRESS/256);
				IAP_WritePPage(IAPLB_P,IAP_ADDRESS/256);
				i=IAP_ReadPPage(IAPLB_P);
				printf("\nRead IAPLB:%02bX  ",i);
				LED_R=0;
				// ����ҳ��
				IAP_ErasePage(IAP_ADDRESS/256);
				LED_R=1;
				TrapFlag[2]=0xFF;

				// д������
				printf("\nWRITE IAP %04X:\n",IAP_ADDRESS);
				TrapFlag[2]='n';
				i=0;
				do{
					IAPData=rand+i;
					LED_G_1=0;
					IAP_WriteByte(IAP_ADDRESS+i,IAPData);
					LED_G_1=1;
					printf("%02bX ",IAPData);
					if((i&0x0F)==0x0F)
					{
						printf("\n");
					}
					i++;
				}while(i!=0);
				
				// ����IAP��ʼ��ַΪISP��ַ,����IAP�ռ�
				printf("\nWrite IAPLB:%02bX  ",ISP_ADDRESS/256);
				IAP_WritePPage(IAPLB_P,ISP_ADDRESS/256);
				i=IAP_ReadPPage(IAPLB_P);
				printf("\nRead IAPLB:%02bX  ",i);
				TrapFlag[2]=0xFF;
				printf("\nNOW IAP %04X:\n",IAP_ADDRESS);
				i=0;
				bOK=TRUE;
				do{
					LED_G_1=0;
					IAPData=IAP_ReadByteByMOVC(IAP_ADDRESS+i);	// ��MOVC�ķ�ʽ��ȡ����
					LED_G_1=1;
					printf("%02bX ",IAPData);
					if((i&0x0F)==0x0F)
					{
						printf("\n");
					}
					// �ж϶����������Ƿ��д�������һ��
					if(IAPData != (u8)(rand+i))
					{
						bOK=FALSE;
					}
					i++;
				}while(i!=0);
				LED_G_1=1;LED_R=1;
				if(bOK)
				{	// ������ͬ
					LED_G_1=0;
				}
				else
				{	// ���ݲ�ͬ
					LED_R=0;
				}
				while(P10==0)
				{
					DelayXms(50);
				}
			}
		}
    }

}

