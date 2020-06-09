/*********************************************************************
    ��Ŀ����:MG82F6D64-DEMO
    ��    ��:YZY
			���԰�ʹ�� MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=32KHz, SysCLK=32KHz
	����������
			����ʱ��Ϊ ILRCO 32KHz, P60���MCK/4
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

#define LED_G_0		P55
#define LED_R		P56
#define LED_G_1		P57

/*************************************************
��������:     void DelayXms(u16 xMs)
����:   		��ʱ���򣬵�λΪms
���ú���:        
�������:     u16 xMs -> *1ms  (1~65535)
�������:     
����:        		
����:
*************************************************/
void DelayXms(u16 xMs)
{
	while(xMs!=0)
	{
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();

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
	PORT_SetP5PushPull(BIT5|BIT6|BIT7);					// ����P55,P56,P57Ϊ�������(LED)
	PORT_SetP6PushPull(BIT0);					    // ����P60 Ϊ�������ģʽ

}

/***********************************************************************************
��������:   void InitClock_ILRCO(void)
��������:   ��ʼ��Clock����
�������:   
���ز���:     
*************************************************************************************/
void InitClock_ILRCO(void)
{
	// P60 ��� MCK/4
	CLK_P60OC_MCKDiv4();			//32KHz/4=8KHz

	// IHRCO Enable, MCK=OSCin, OSCin=ILRCO
	CLK_SetCKCON2(ENABLE_IHRCO|MCK_OSCin|OSCIn_ILRCO);
	nop();
	// IHRCO Disable, MCK=OSCin, OSCin=ILRCO
	CLK_SetCKCON2(DISABLE_IHRCO|MCK_OSCin|OSCIn_ILRCO);

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
	InitClock_ILRCO();	
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
