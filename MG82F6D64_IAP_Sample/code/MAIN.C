/*********************************************************************
    项目名称:MG82F6D64-DEMO
    作    者:YZY
			测试板使用 MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=12MHz, SysCLK=12MHz
	功能描述：
			IAP 测试
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

#define LED_G_0		P55
#define LED_R		P56
#define LED_G_1		P57

#define ISP_ADDRESS    0xFE00	 //ISP起始地址 
#define IAP_ADDRESS    0xF600	 //IAP起始地址  
idata u8 TrapFlag[3];

/*************************************************
函数名称: char putchar (char c)   
功能描述: 用于printf 的串口输出程序
输入参数:     char c
输出参数:     
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
函数名称:   	void CheckTrapFlag(void)
功能描述:   	检查是否按正确的流程进入IAP操作，防止误操作
				如果TrapFlag != "Win",则表示是非法进入,单片机软复位
输入参数:   	
返回参数:     
*************************************************************************************/
void CheckTrapFlag(void)
{ 
	if ((TrapFlag[0]!='W')||(TrapFlag[1]!='i')||(TrapFlag[2]!='n'))
	{
		//软件复位
		POW_ResetToAP();
	}
}

/***********************************************************************************
函数名称:   u8 IAP_ReadPPage(u8 PsfrAddr)
功能描述:读取P页特殊功能器内容
输入参数:   
		 u8 PsfrAddr: P页地址
返回参数:     
		 u8: P页内容
*************************************************************************************/
u8 IAP_ReadPPage(u8 PsfrAddr)
{
	bit bEA=EA;
	EA = 0; 					//关中断
	IFADRH = 0; 				//IFADRH必须为0
	IFADRL= PsfrAddr;			//送P页地址;
	IFMT = ISP_READ_P;
	ISPCR = 0x80;				//设置等待时间, 允许ISP/IAP操作
	CheckTrapFlag();
	SCMD = 0x46;
	CheckTrapFlag();
	SCMD = 0xB9;
	nop();
	IFMT=0;
	ISPCR = 0;					//清ISP/IAP特殊寄存器，防止误操作
	EA = bEA;					//恢复中断
	return IFD;
}


/***********************************************************************************
函数名称:void IAP_WritePPage(u8 PsfrAddr,u8 PsfrData)
功能描述:写P页特殊功能器
输入参数:   
		 u8 PsfrAddr: P页地址,u8 PsfrData:  P页内容
返回参数:     
*************************************************************************************/
void IAP_WritePPage(u8 PsfrAddr,u8 PsfrData)
{
	bit bEA=EA;
	EA = 0; 					//关中断
	IFADRH = 0; 				//IFADRH必须为0
	IFADRL= PsfrAddr;			//送P页地址;
	IFD= PsfrData;				//送P页内容
	IFMT = ISP_WRITE_P;
	ISPCR = 0x80;				//设置等待时间, 允许ISP/IAP操作
	CheckTrapFlag();
	SCMD = 0x46;
	CheckTrapFlag();
	SCMD = 0xB9;
	nop();
	IFMT=0;
	ISPCR = 0;					//清ISP/IAP特殊寄存器，防止误操作
	EA = bEA;					//恢复中断
}


/***********************************************************************************
函数名称:u8 IAP_ReadByte(u16 ByteAddr)
功能描述:读指定地址的IAP内容
输入参数:   
		 u16 ByteAddr: IAP地址
返回参数:     
*************************************************************************************/
u8 IAP_ReadByte(u16 ByteAddr)
{
		bit bEA=EA;
		IFADRH = ByteAddr>>8;		//送地址高字节
		IFADRL= ByteAddr;			//送地址低字节;
		EA = 0;						//关中断
		IFMT = ISP_READ;
		ISPCR = 0x80; 				//允许ISP/IAP操作
		CheckTrapFlag();
		SCMD = 0x46;
		CheckTrapFlag();
		SCMD = 0xB9;
		IFMT=0;
		ISPCR = 0;					//清ISP/IAP特殊寄存器，防止误操作
		EA = bEA; 					//恢复中断
		return IFD;
}
// 可以使用MOVC的方式读取IAP的数据
#define	IAP_ReadByteByMOVC(x)	CBYTE[x]

/***********************************************************************************
函数名称:void IAP_ErasePage(u8 ByteAddr)
功能描述:页面擦除, 擦除指定地址所在的页面。
		擦除一个页面大约需要30ms
输入参数:   
		 u8 ByteAddr: 指定的页面地址高8位
*************************************************************************************/
void IAP_ErasePage(u8 ByteAddr)
{
		bit bEA=EA;
		IFADRH = ByteAddr;		//送地址高字节
		IFADRL= 0x00;			//送地址低字节;
		EA = 0;					//关中断
		IFMT = ISP_ERASE;
		ISPCR = 0x80; 			//允许ISP/IAP操作
		CheckTrapFlag();
		SCMD = 0x46;
		CheckTrapFlag();
		SCMD = 0xB9;
		nop();
		IFMT=0;
		ISPCR = 0;					//清ISP/IAP特殊寄存器，防止误操作
		EA = bEA; 					//恢复中断
}

/***********************************************************************************
函数名称:void IAP_WriteByte(u16 ByteAddr,u8 ByteData)
功能描述:写字节到指定地址, 该地址必须为空(0xFF),否则要先执行页面擦除
		 写入一个字节数据大约需要80us
输入参数:   
		 u16 ByteAddr: 指定的IAP地址, u8 ByteData: 待写的数据
返回参数:     
*************************************************************************************/

void IAP_WriteByte(u16 ByteAddr,u8 ByteData)
{
		bit bEA=EA;
		IFD = ByteData;				//送所要写的数据	
		IFADRH = ByteAddr>>8;		//送地址高字节
		IFADRL= ByteAddr;			//送地址低字节;
		EA = 0;						//关中断
		IFMT = ISP_WRITE;
		ISPCR = 0x80;               //允许ISP/IAP操作
		CheckTrapFlag();
		SCMD = 0x46;
		CheckTrapFlag();
		SCMD = 0xB9;
		nop();
		IFMT=0;
		ISPCR = 0;					//清ISP/IAP特殊寄存器，防止误操作
		EA = bEA; 					//恢复中断
}

/***********************************************************************************
函数名称:   void InitUart0(void)
功能描述:   Uart0初始化
		 使用S0BRG作为波特率来源
输入参数:   
返回参数:     
*************************************************************************************/
void InitUart0_S0BRG(void)
{
	UART0_SetAccess_S0CR1();			// 设置SFR 0xB9地址是访问S0CR1
	UART0_SetMode8bitUARTVar();			// 8位 可变波特率
	UART0_EnReception();				// 使能接收
	UART0_SetBRGFromS0BRG();			// 设置波特率来源为 S0BRG
	UART0_SetS0BRGBaudRateX2();			// 设置2x波特率
	UART0_SetS0BRGSelSYSCLK();			// 设置S0BRG时钟输入为Sysclk
	UART0_SetRxTxP30P31();
	// Sets B.R. value
	UART0_SetS0BRGValue(S0BRG_BRGRL_9600_2X_12000000_1T);

	UART0_EnS0BRG();					// 开启S0BRG时钟
}

/***********************************************************************************
函数名称:   void InitPort(void)
功能描述:   IO配置
输入参数:   
返回参数:     
*************************************************************************************/
void InitPort(void)
{
	PORT_SetP5PushPull(BIT5|BIT6|BIT7);					// 设置P55,P56,P57为推挽输出(LED)
	PORT_SetP1OpenDrainPu(BIT0);					    // 设置P10 为开漏输出带上拉模式
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

	// 设置IAP时基 OSCin=12MHz
	CLK_SetIAPTimeBase(12);
}

/***********************************************************************************
函数名称:   void InitSystem(void)
功能描述:   系统初始化
输入参数:   
返回参数:     
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
	//读IAP初始起始地址
	i=IAP_ReadPPage(IAPLB_P);
    printf("\nRead IAPLB:%02bX",i);

	// 设置IAP起始地址为ISP地址,即无IAP空间
	IAP_WritePPage(IAPLB_P,ISP_ADDRESS/256);
	i=IAP_ReadPPage(IAPLB_P);
    printf("\nNOW IAPLB:%02bX  ",i);
	TrapFlag[2]=0xFF;

	printf("\nOLD IAP %04X:\n",IAP_ADDRESS);
	i=0;
	do{
		LED_G_1=0;
		IAPData=IAP_ReadByteByMOVC(IAP_ADDRESS+i);	// 用MOVC的方式读取数据
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
				// 设置IAP起始地址为欲操作的IAP地址
				printf("\nWrite IAPLB:%02bX  ",IAP_ADDRESS/256);
				IAP_WritePPage(IAPLB_P,IAP_ADDRESS/256);
				i=IAP_ReadPPage(IAPLB_P);
				printf("\nRead IAPLB:%02bX  ",i);
				LED_R=0;
				// 擦除页面
				IAP_ErasePage(IAP_ADDRESS/256);
				LED_R=1;
				TrapFlag[2]=0xFF;

				// 写入数据
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
				
				// 设置IAP起始地址为ISP地址,即无IAP空间
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
					IAPData=IAP_ReadByteByMOVC(IAP_ADDRESS+i);	// 用MOVC的方式读取数据
					LED_G_1=1;
					printf("%02bX ",IAPData);
					if((i&0x0F)==0x0F)
					{
						printf("\n");
					}
					// 判断读出的数据是否和写入的数据一致
					if(IAPData != (u8)(rand+i))
					{
						bOK=FALSE;
					}
					i++;
				}while(i!=0);
				LED_G_1=1;LED_R=1;
				if(bOK)
				{	// 数据相同
					LED_G_1=0;
				}
				else
				{	// 数据不同
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


