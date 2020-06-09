/*********************************************************************
    项目名称:MG82F6D64-DEMO
			测试板使用 MG82F6D64 LQFP64_V10 EV Board (TH210A)
			CpuCLK=12MHz, SysCLK=12MHz
	功能描述：
			串口0，LIN Master
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

#define LED_G_0			P55
#define LED_R			P56
#define LED_G_1			P57

#define LIN_TXD			P31
#define LIN_RXD			P30

#define LIN_TIME_MAX				10

#define LIN_SYNC_BYTE				0x55

#define LIN_AUTO_BAUDRATE_FLAG		0		// 0->禁止自动校准波特率             1->使能自动校准波特率			

#define LIN_PID_MASTER_TX_MSG0		0x10		
#define LIN_PID_MASTER_TX_MSG1		0x11	

#define LIN_PID_MASTER_RX_MSG0		0x21
#define LIN_PID_MASTER_RX_MSG1		0x31




// 定义LIN状态
typedef enum
{
  LIN_IDLE_BREAK=0,
  LIN_SYNC,
  LIN_PID,
  LIN_MASTER_RX_DATA,
  LIN_MASTER_RX_CHECKSUM,
  LIN_MASTER_RX_DONE,
  LIN_MASTER_TX_DATA,
  LIN_MASTER_TX_CHECKSUM,
  LIN_MASTER_TX_DONE,
  LIN_ERR
}LIN_State;

// 定义LIN模式
typedef enum
{
  	LIN_MODE_MASTER_TX=0,
	LIN_MODE_MASTER_RX,
	LIN_MODE_SLAVE_TX,
	LIN_MODE_SLAVE_RX
}LIN_Mode;

// 定义帧结构体
typedef struct
{
	LIN_Mode Mode;
  	LIN_State State;
  	u8 PID ;
  	u8 Data[8];
  	u8 DataLength;
  	u8 DataInx;
  	u8 Check;
} LIN_Frame ;

#define BIT(A,B)      ((A>>B)&0x01)   // A 为变量 

idata LIN_Frame LINFrame;


u8 LedTime;
u8 LINOverTime;

void LIN_Master_StartFrame(void);
u8 LIN_CalcParity(u8 id);
u8 LIN_CalcChecksum(u8 id,u8 *lindata,u8 length);

/***********************************************************************************
函数名称:   void INT_UART0(void)
功能描述:UART0 中断服务程序
		 
输入参数:   
返回参数:     
*************************************************************************************/
void INT_UART0(void) interrupt INT_VECTOR_UART0
{
	_push_(SFRPI);
	SFR_SetPage(0);
	if(RI0==1)
	{
		if(LINFrame.State==LIN_MASTER_RX_DATA)
		{ // Rx data
			
			LINFrame.Data[LINFrame.DataInx] = S0BUF;
			LINFrame.DataInx++;
			if(LINFrame.DataInx>=LINFrame.DataLength)
			{
				LINFrame.State++;
			}

		}
		else if(LINFrame.State==LIN_MASTER_RX_CHECKSUM)
		{ // Rx checksum
			LINFrame.Check=S0BUF;
			LINFrame.State++;
			
		}
		else if(LINFrame.State==LIN_MASTER_RX_DONE)
		{ // Rx done
		
		}
		else
		{
			LINFrame.State=LIN_ERR;
		}
		RI0 = 0;
	}
	else
	{
		TI0 = 0;
		
		if(LINFrame.State==LIN_IDLE_BREAK)
		{
			if((S0CFG1&SBF0)!=0)
			{ // Send break end
				UART0_LIN_ClrSBF0();
				S0BUF=LIN_SYNC_BYTE;		// Send SYNC
				LINFrame.State++;
			}
		}
		/**/
		else if(LINFrame.State==LIN_SYNC)
		{// Send SYNC end
			LINFrame.State++;
			S0BUF=LINFrame.PID;				// Send PID
		}
		/**/
		else if(LINFrame.State==LIN_PID)
		{// Send PID end 
			if(LINFrame.Mode == LIN_MODE_MASTER_RX)
			{
				LINFrame.DataInx=0;
				LINFrame.State= LIN_MASTER_RX_DATA;
				UART0_LIN_SetRX();			// 设置为 LIN RX模式
				
			}
			else if(LINFrame.Mode == LIN_MODE_MASTER_TX)
			{// Start send DATA
				S0BUF = LINFrame.Data[0];
				LINFrame.DataInx=1;
				LINFrame.State= LIN_MASTER_TX_DATA;
			}
			else
			{ 
				LINFrame.State=LIN_ERR;
			}
		}
		else if(LINFrame.State==LIN_MASTER_TX_DATA)
		{
			
			S0BUF=LINFrame.Data[LINFrame.DataInx];
			LINFrame.DataInx++;
			if(LINFrame.DataInx>=LINFrame.DataLength)
			{
				LINFrame.State++;
			}

		}
		else if(LINFrame.State==LIN_MASTER_TX_CHECKSUM)
		{
			S0BUF=LINFrame.Check;
			LINFrame.State++;
		}
		else if(LINFrame.State==LIN_MASTER_TX_DONE)
		{
		}
		else
		{
			LINFrame.State=LIN_ERR;
		}
	}
	_pop_(SFRPI);
}

/***********************************************************************************
函数名称:   void INT_T0(void)
功能描述:T0 中断服务程序
		 间隔1ms
输入参数:   
返回参数:     
*************************************************************************************/
void INT_T0(void) interrupt INT_VECTOR_T0
{
	TH0=TIMER_12T_1ms_TH;
	TL0=TIMER_12T_1ms_TL;

	if(LedTime!=0) LedTime--;
	if(LINOverTime!=0) LINOverTime--;
}


/*************************************************
函数名称:     void DelayXus(u16 xUs)
功能描述:   	延时程序，单位为us
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
函数名称:   void InitUart0_LIN(void)
功能描述:   Uart0初始化 为 LIN
输入参数:   
返回参数:     
*************************************************************************************/
void InitUart0_LIN(void)
{
	
	UART0_SetAccess_S0CR1();			// 设置SFR 0xB9地址是访问S0CR1
	UART0_SetModeLIN();					// 设置串口LIN 模式				
	UART0_EnReception();				// 开启接收功能
	UART0_SetBRGFromS0BRG();			// 设置波特率来源为 S0BRG
	UART0_SetS0BRGBaudRateX2();			// 设置2x波特率
	UART0_SetS0BRGSelSYSCLK();			// 设置S0BRG时钟输入为Sysclk

	// 设置S0BRG Value
	UART0_SetS0BRGValue(S0BRG_BRGRL_19200_2X_12000000_1T);
	
	UART0_EnS0BRG();					// 开启S0BRG时钟
	
	UART0_LIN_TxSYNC_16Bit();			// 设置 TX break 16bit
	
}

/***********************************************************************************
函数名称:   void InitTimer0(void)
功能描述:Timer0初始化设置
		 定义T0为16位定时器,时钟为Sysclk/192 6us~393216us	
输入参数:   
返回参数:     
*************************************************************************************/
void InitTimer0(void)
{
	TM_SetT0Mode_1_16BIT_TIMER();			// 设置T0模式为16位模式
	TM_SetT0Clock_SYSCLKDiv12();					// 设置T0时钟源为 SYSCLK/12
	TM_SetT0Gate_Disable();

	TM_SetT0LowByte(TIMER_12T_1ms_TL);		// 设置T0低8位
	TM_SetT0HighByte(TIMER_12T_1ms_TH);		// 设置T0高8位

	TM_EnableT0();							// 使能T0
}


/***********************************************************************************
函数名称:   void InitInterrupt(void)
功能描述:   中断初始化
输入参数:   
返回参数:     
*************************************************************************************/
void InitInterrupt(void)
{
//	INT_EnUART0();				// 使能T0中断
	INT_EnTIMER0();				// 使能T0中断
}	

/***********************************************************************************
函数名称:   void InitPort(void)
功能描述:   IO配置
输入参数:   
返回参数:     
*************************************************************************************/
void InitPort(void)
{
	PORT_SetP5PushPull(BIT2|BIT3|BIT4|BIT5|BIT6|BIT7);					// 设置P52,P53,P54,P55,P56,P57为推挽输出
	PORT_SetP3QuasiBi(BIT0|BIT1|BIT3|BIT4|BIT5);						// 设置 P30,P31,P33,P34,P35 作为准双向口
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
	InitTimer0();
	InitUart0_LIN();
	InitInterrupt();
}


/*
*************************************************************************************
*  Function
*
*************************************************************************************

/***********************************************************************************
函数名称:   void LIN_Master_StartFrame(void)
功能描述:   LIN Master 开始一个帧的通信
输入参数:   
返回参数:     
*************************************************************************************/
void LIN_Master_StartFrame(void)
{
	UART0_LIN_SetTX();
	UART0_LIN_ClrTXER0();
	UART0_LIN_ClrSBF0();
	TI0=0;
	UART0_LIN_SendSYNC();
	S0BUF=0x55;
}

/***********************************************************************************
函数名称:	uint8 LIN_CalcParity(uint8 id)
功能描述: LIN奇偶校验计算
输入参数:	
返回参数:	  
*************************************************************************************/
u8 LIN_CalcParity(u8 id)
{
	u8 parity, p0,p1;
	parity=id; 
	p0=(BIT(parity,0)^BIT(parity,1)^BIT(parity,2)^BIT(parity,4))<<6;     //偶校验位
	p1=(!(BIT(parity,1)^BIT(parity,3)^BIT(parity,4)^BIT(parity,5)))<<7;  //奇校验位
	parity|=(p0|p1);
	return parity;
}


/***********************************************************************************
函数名称:	u8 LIN_CalcChecksum(uint8 id,uint8 *data,uint8 length)
功能描述: LIN校验和计算
输入参数:	
返回参数:	  
*************************************************************************************/
u8 LIN_CalcChecksum(u8 id,u8 *lindata,u8 length)
{
	u8 i;
	u16 check_sum = 0;
	//0x3C 0x3D 使用标准型校验
	if(id != 0x3c && id != 0x7d)  //使用增强型校验
	{
	 	check_sum  = id ;
	} 
	else //使用标准校验
	{
	 	check_sum = 0 ;
	}
	for (i = 0; i < length; i++) 
	{    
	 	 check_sum += *(lindata++);
	  
	  	if (check_sum > 0xFF)      //进位
	  	{
	  		check_sum -= 0xFF;
	  	}
	}
	return (~check_sum);  //取反
}




/*
*************************************************************************************
*/ 

/***********************************************************************************
函数名称:	void TestMasterTxMsgToSlave(u8 uPID)
功能描述: 测试发送数据到从机
输入参数:	u8 uPID: PID ->0x00~0x3F
返回参数:	  
*************************************************************************************/
void TestMasterTxMsgToSlave(u8 uPID)
{
	u8 i;

	// Master Tx Test
	LINFrame.Mode=LIN_MODE_MASTER_TX;
	LINFrame.PID=LIN_CalcParity(uPID);
	LINFrame.DataInx=0;
	LINFrame.DataLength=8;						// 测试数据长度为8 Byte
	for(i=0;i<8;i++)
	{
		LINFrame.Data[i]=i+0x30+uPID;			//  设置测试数据
	}
	LINFrame.Check=LIN_CalcChecksum(LINFrame.PID, &LINFrame.Data[0],LINFrame.DataLength);
	LINFrame.State=LIN_IDLE_BREAK;
	LIN_Master_StartFrame();
	TI0=0;RI0=0;
	INT_EnUART0();		// 使能 UART0中断
	
	LINOverTime=20;
	while (LINOverTime!=0)
	{
		if((LINFrame.State==LIN_ERR)||(LINFrame.State==LIN_MASTER_TX_DONE))
		{
		  break;
		}
	}
	INT_DisUART0(); 	// 禁止 UART0中断
	if(LINFrame.State==LIN_MASTER_TX_DONE)
	{// 发送成功
		LED_R=0;
	}
	DelayXms(2);
	LED_R=1;
}

/***********************************************************************************
函数名称:	void TestMasterRxMsgFromSlave(u8 uPID)
功能描述: 测试读取从机的数据
输入参数:	u8 uPID: PID ->0x00~0x3F
返回参数:	  
*************************************************************************************/
void TestMasterRxMsgFromSlave(u8 uPID)
{
	u8 i;
	// Master Rx Test
	LINFrame.Mode=LIN_MODE_MASTER_RX;
	LINFrame.PID=LIN_CalcParity(uPID);
	LINFrame.DataInx=0;
	LINFrame.DataLength=8;
	for(i=0;i<8;i++)
	{
		LINFrame.Data[i]=0x00;
	}
	LINFrame.State=LIN_IDLE_BREAK;
	LIN_Master_StartFrame();
	TI0=0;RI0=0;
	INT_EnUART0();		// 使能 UART0中断
	
	LINOverTime=20;
	while (LINOverTime!=0)
	{
		if((LINFrame.State==LIN_ERR)||(LINFrame.State==LIN_MASTER_RX_DONE))
		{
		  break;
		}
	}
	INT_DisUART0(); 	// 禁止 UART0中断
	if(LINFrame.State==LIN_MASTER_RX_DONE)
	{// 接收成功
		// 校验数据
		if(LINFrame.Check == LIN_CalcChecksum(LINFrame.PID, &LINFrame.Data[0], LINFrame.DataLength))
		{
			LED_R=0;
		}
	}
	DelayXms(2);
	LED_R=1;
}

void main()
{
	u8 uPID;
	
	InitSystem();
	
	LED_G_0=0;LED_R=0;LED_G_1=0;
	DelayXms(1000);
	LED_G_0=1;LED_R=1;LED_G_1=1;	

	INT_EnAll();		// 使能全局中断

	uPID=0x00;
	
	while(1)
    {
    	DelayXms(50);
		if(uPID<0x20)
		{
			LED_G_0=!LED_G_0;
			LED_G_1=1;
			TestMasterTxMsgToSlave(uPID);
		}
		else
		{
			LED_G_1=!LED_G_1;
    		LED_G_0=1;
			TestMasterRxMsgFromSlave(uPID);
		}
		uPID++;
		if(uPID>=0x40)
		{
			uPID=0x00;
		}
		
    }

}


