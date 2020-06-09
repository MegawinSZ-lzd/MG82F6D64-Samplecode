#ifndef __REG_MG82F6D64_H
#define __REG_MG82F6D64_H

#define GF_7         0x80
#define GF_6         0x40
#define GF_5         0x20
#define GF_4         0x10
#define GF_3         0x80
#define GF_2         0x04
#define GF_1         0x02
#define GF_0         0x01


sfr P0             = 0x80;
// (bits in P0)
sbit P07           = P0^7;
sbit P06           = P0^6;
sbit P05           = P0^5;
sbit P04           = P0^4;
sbit P03           = P0^3;
sbit P02           = P0^2;
sbit P01           = P0^1;
sbit P00           = P0^0;

sfr SP             = 0x81;
sfr DPL            = 0x82;
sfr DPH            = 0x83;

sfr SPSTAT         = 0x84;
// (bits in SPSTAT)
#define SPIF         0x80
#define WCOL         0x40
#define THRF         0x20
#define SPIBSY       0x10
#define MODF         0x08
#define QPIEN        0x02
#define SPR2_QDOE    0x01


sfr SPCON          = 0x85;
// (bits in SPCON)
#define SSIG         0x80
#define SPEN         0x40
#define DORD         0x20
#define MSTR         0x10
#define CPOL         0x08
#define CPHA         0x04
#define SPR1         0x02
#define SPR0         0x01

sfr SPDAT          = 0x86;

sfr PCON0          = 0x87;
// (bits in PCON0)
#define SMOD1        0x80
#define SMOD0        0x40
#define POF0         0x10
#define PD           0x02
#define IDL          0x01


sfr TCON           = 0x88;
// (bits in TCON)
sbit TF1           = TCON^7;
sbit TR1           = TCON^6;
sbit TF0           = TCON^5;
sbit TR0           = TCON^4;
sbit IE1           = TCON^3;
sbit IT1           = TCON^2;
sbit IE0           = TCON^1;
sbit IT0           = TCON^0;


sfr TMOD           = 0x89;
// (bits in TMOD)
#define T1GATE       0x80
#define T1C_T        0x40
#define T1M1         0x20
#define T1M0         0x10
#define T0GATE       0x08
#define T0C_T        0x04
#define T0M1         0x02
#define T0M0         0x01

sfr TL0            = 0x8A;
sfr TL1            = 0x8B;
sfr TH0            = 0x8C;
sfr TH1            = 0x8D;

sfr SFIE           = 0x8E;
// (bits in SFIE)
#define SIDFIE       0x80
#define MCDRE        0x40
#define MCDFIE       0x20
#define RTCFIE       0x10
#define SPWIE        0x08
#define BOF1IE       0x04
#define BOF0IE       0x02
#define WDTFIE       0x01

sfr XRPS           = 0x8F;

sfr P1             = 0x90;
// (bits in P1)
sbit P17           = P1^7;
sbit P16           = P1^6;
sbit P15           = P1^5;
sbit P14           = P1^4;
sbit P13           = P1^3;
sbit P12           = P1^2;
sbit P11           = P1^1;
sbit P10           = P1^0;


sfr P1M0           = 0x91;
// (bits in P1M0)
#define P1M07        0x80
#define P1M06        0x40
#define P1M05        0x20
#define P1M04        0x10
#define P1M03        0x08
#define P1M02        0x04
#define P1M01        0x02
#define P1M00        0x01


sfr P1M1           = 0x92;
// (bits in P1M1)
#define P1M17        0x80
#define P1M16        0x40
#define P1M15        0x20
#define P1M14        0x10
#define P1M13        0x08
#define P1M12        0x04
#define P1M11        0x02
#define P1M10        0x01


sfr P2M1           = 0x92;
// (bits in P2M1)
#define P2M17        0x80
#define P2M16        0x40
#define P2M15        0x20
#define P2M14        0x10
#define P2M13        0x08
#define P2M12        0x04
#define P2M11        0x02
#define P2M10        0x01


sfr P4M1           = 0x92;
// (bits in P4M1)
#define P4M17        0x80
#define P4M16        0x40
#define P4M15        0x20
#define P4M14        0x10
#define P4M13        0x08
#define P4M12        0x04
#define P4M11        0x02
#define P4M10        0x01


sfr P6M1           = 0x92;
// (bits in P6M1)
#define P6M17        0x80
#define P6M16        0x40
#define P6M15        0x20
#define P6M14        0x10
#define P6M13        0x08
#define P6M12        0x04
#define P6M11        0x02
#define P6M10        0x01


sfr P0M1           = 0x92;
// (bits in P0M1)
#define P0M17        0x80
#define P0M16        0x40
#define P0M15        0x20
#define P0M14        0x10
#define P0M13        0x08
#define P0M12        0x04
#define P0M11        0x02
#define P0M10        0x01


sfr P5M1           = 0x92;
// (bits in P5M1)
#define P5M17        0x80
#define P5M16        0x40
#define P5M15        0x20
#define P5M14        0x10
#define P5M13        0x08
#define P5M12        0x04
#define P5M11        0x02
#define P5M10        0x01


sfr P7M1           = 0x92;
// (bits in P7M1)
#define P7M13        0x08
#define P7M12        0x04
#define P7M11        0x02
#define P7M10        0x01

sfr P3FDC          = 0x92;
sfr P1FDC          = 0x92;
sfr P2FDC          = 0x92;
sfr P4FDC          = 0x92;
sfr P0FDC          = 0x92;
sfr P6FDC          = 0x92;
sfr P5FDC          = 0x92;
sfr P7FDC          = 0x92;

sfr P0M0           = 0x93;
// (bits in P0M0)
#define P0M07        0x80
#define P0M06        0x40
#define P0M05        0x20
#define P0M04        0x10
#define P0M03        0x08
#define P0M02        0x04
#define P0M01        0x02
#define P0M00        0x01


sfr T2MOD1         = 0x93;
// (bits in T2MOD1)
#define TL2CS        0x80
#define TF2IG        0x40
#define TL2IS        0x20
#define T2CKS        0x10
#define T2MS1        0x08
#define CP2S2        0x04
#define CP2S1        0x02
#define CP2S0        0x01


sfr T3MOD1         = 0x93;
// (bits in T3MOD1)
#define TL3CS        0x80
#define TF3IG        0x40
#define T3CKS        0x10
#define T3MS1        0x08
#define CP3S2        0x04
#define CP3S1        0x02
#define CP3S0        0x01


sfr T4MOD1         = 0x93;
// (bits in T4MOD1)
#define TL4CS        0x80
#define TF4IG        0x40
#define T4CKS        0x10
#define T4MS1        0x08
#define CP4S2        0x04
#define CP4S1        0x02
#define CP4S0        0x01


sfr DMACR0         = 0x94;
// (bits in DMACR0)
#define DMAE0        0x08
#define DMAS0        0x04
#define DIE0         0x02
#define DCF0         0x01


sfr DMACG0         = 0x94;
// (bits in DMACG0)
#define PDMAH        0x80
#define PDMAL        0x40
#define CRCW0        0x20
#define EXTS10       0x08
#define EXTS00       0x04
#define FAEN0        0x02
#define LOOP0        0x01


sfr DMADS0         = 0x94;
// (bits in DMADS0)
#define DSS30        0x80
#define DSS20        0x40
#define DSS10        0x20
#define DSS00        0x10
#define DDS30        0x08
#define DDS20        0x04
#define DDS10        0x02
#define DDS00        0x01


sfr P2M0           = 0x95;
// (bits in P2M0)
#define P2M07        0x80
#define P2M06        0x40
#define P2M05        0x20
#define P2M04        0x10
#define P2M03        0x08
#define P2M02        0x04
#define P2M01        0x02
#define P2M00        0x01


sfr TREN0          = 0x95;
// (bits in TREN0)
#define TR4LE        0x80
#define TR3LE        0x40
#define TR2LE        0x20
#define TR4E         0x10
#define TR3E         0x08
#define TR2E         0x04
#define TR1E         0x02
#define TR0E         0x01


sfr TRLC0          = 0x95;
// (bits in TRLC0)
#define TL4RLC       0x80
#define TL3RLC       0x40
#define TL2RLC       0x20
#define T4RLC        0x10
#define T3RLC        0x08
#define T2RLC        0x04
#define T1RLC        0x02
#define T0RLC        0x01


sfr TSPC0          = 0x95;
// (bits in TSPC0)
#define TL4SC        0x80
#define TL3SC        0x40
#define TL2SC        0x20
#define T4SC         0x10
#define T3SC         0x08
#define T2SC         0x04
#define T1SC         0x02
#define T0SC         0x01

sfr BOREV          = 0x96;

sfr PCON1          = 0x97;
// (bits in PCON1)
#define SWRF         0x80
#define EXRF         0x40
#define MCDF         0x20
#define RTCF         0x10
#define SPWF         0x08
#define BOF1         0x04
#define BOF0         0x02
#define WDTF         0x01


sfr S0CON          = 0x98;
// (bits in S0CON)
sbit SM00_FE       = S0CON^7;
sbit SM10          = S0CON^6;
sbit SM20          = S0CON^5;
sbit REN0          = S0CON^4;
sbit TB80          = S0CON^3;
sbit RB80          = S0CON^2;
sbit TI0           = S0CON^1;
sbit RI0           = S0CON^0;


sfr S1CON          = 0x98;
// (bits in S1CON)
sbit SM01          = S1CON^7;
sbit SM11          = S1CON^6;
sbit SM21          = S1CON^5;
sbit REN1          = S1CON^4;
sbit TB81          = S1CON^3;
sbit RB81          = S1CON^2;
sbit TI1           = S1CON^1;
sbit RI1           = S1CON^0;


sfr S2CON          = 0x98;
// (bits in S2CON)
sbit SM02          = S2CON^7;
sbit SM12          = S2CON^6;
sbit SM22          = S2CON^5;
sbit REN2          = S2CON^4;
sbit TB82          = S2CON^3;
sbit RB82          = S2CON^2;
sbit TI2           = S2CON^1;
sbit RI2           = S2CON^0;


sfr S3CON          = 0x98;
// (bits in S3CON)
sbit SM03          = S3CON^7;
sbit SM13          = S3CON^6;
sbit SM23          = S3CON^5;
sbit REN3          = S3CON^4;
sbit TB83          = S3CON^3;
sbit RB83          = S3CON^2;
sbit TI3           = S3CON^1;
sbit RI3           = S3CON^0;

sfr S0BUF          = 0x99;
sfr S1BUF          = 0x99;
sfr S2BUF          = 0x99;
sfr S3BUF          = 0x99;
sfr S0BRT          = 0x9A;
sfr S1BRT          = 0x9A;
sfr S2BRT          = 0x9A;
sfr S3BRT          = 0x9A;
sfr S0BRC          = 0x9B;
sfr S1BRC          = 0x9B;
sfr S2BRC          = 0x9B;
sfr S3BRC          = 0x9B;

sfr S0CFG          = 0x9C;
// (bits in S0CFG)
#define URTS         0x80
#define SMOD2        0x40
#define URM0X3       0x20
#define SM30         0x10
#define S0DOR        0x08
#define BTI          0x04
#define UTIE         0x02
#define SMOD3        0x01


sfr S1CFG          = 0x9C;
// (bits in S1CFG)
#define SM31         0x80
#define S1M0X3       0x40
#define S1DOR        0x20
#define S1TR         0x10
#define S1MOD1       0x08
#define S1TX12       0x04
#define S1CKOE       0x02
#define S1TME        0x01


sfr S2CFG          = 0x9C;
// (bits in S2CFG)
#define SM32         0x80
#define S2M0X3       0x40
#define S2DOR        0x20
#define S2TR         0x10
#define S2MOD1       0x08
#define S2TX12       0x04
#define S2CKOE       0x02
#define S2TME        0x01


sfr S3CFG          = 0x9C;
// (bits in S3CFG)
#define SM33         0x80
#define S3M0X3       0x40
#define S3DOR        0x20
#define S3TR         0x10
#define S3MOD1       0x08
#define S3TX12       0x04
#define S3CKOE       0x02
#define S3TME        0x01


sfr S0CFG1         = 0x9D;
// (bits in S0CFG1)
#define SBF0         0x80
#define TXER0        0x40
#define S0SB16       0x20
#define ATBR0        0x10
#define TXRX0        0x08
#define SYNC0        0x04


sfr AC0CON         = 0x9E;
// (bits in AC0CON)
#define AC0LP        0x80
#define AC0PDX       0x40
#define AC0OUT       0x20
#define AC0F         0x10
#define AC0EN        0x08
#define AC0INV       0x04
#define AC0M1        0x02
#define AC0M0        0x01


sfr AC1CON         = 0x9E;
// (bits in AC1CON)
#define AC1LP        0x80
#define AC1PDX       0x40
#define AC1OUT       0x20
#define AC1F         0x10
#define AC1EN        0x08
#define AC1INV       0x04
#define AC1M1        0x02
#define AC1M0        0x01


sfr AC2CON         = 0x9E;
// (bits in AC2CON)
#define AC2LP        0x80
#define AC2PDX       0x40
#define AC2OUT       0x20
#define AC2F         0x10
#define AC2EN        0x08
#define AC2INV       0x04
#define AC2M1        0x02
#define AC2M0        0x01


sfr AC0MOD         = 0x9F;
// (bits in AC0MOD)
#define NVRS3        0x80
#define NVRS2        0x40
#define NVRS1        0x20
#define NVRS0        0x10
#define NVRL         0x08
#define AC0FLT       0x04
#define AC0PIS1      0x02
#define AC0PIS0      0x01


sfr AC1MOD         = 0x9F;
// (bits in AC1MOD)
#define AC1NIS       0x10
#define NVRL1        0x08
#define AC1FLT       0x04


sfr AC2MOD         = 0x9F;
// (bits in AC2MOD)
#define AC2NIS       0x10
#define NVRL2        0x08
#define AC2FLT       0x04


sfr P2             = 0xA0;
// (bits in P2)
sbit P27           = P2^7;
sbit P26           = P2^6;
sbit P25           = P2^5;
sbit P24           = P2^4;
sbit P23           = P2^3;
sbit P22           = P2^2;
sbit P21           = P2^1;
sbit P20           = P2^0;


sfr AUXR0          = 0xA1;
// (bits in AUXR0)
#define P60OC1       0x80
#define P60OC0       0x40
#define P60FD        0x20
#define PBKF         0x10
#define BM1F         0x08
#define BM0F         0x04
#define INT1H        0x02
#define INT0H        0x01


sfr AUXR1          = 0xA2;
// (bits in AUXR1)
#define CRCDS1       0x20
#define CRCDS0       0x10
#define DPS          0x01


sfr AUXR2          = 0xA3;
// (bits in AUXR2)
#define STAF         0x80
#define STOF         0x40
#define T1X12        0x08
#define T0X12        0x04
#define T1CKOE       0x02
#define T0CKOE       0x01


sfr AUXR3          = 0xA4;
// (bits in AUXR3)
#define T0PS1        0x80
#define T0PS0        0x40
#define BPOC1        0x20
#define BPOC0        0x10
#define S0PS0        0x08
#define TWIPS1       0x04
#define TWIPS0       0x02
#define T0XL         0x01


sfr AUXR4          = 0xA4;
// (bits in AUXR4)
#define T2PS1        0x80
#define T2PS0        0x40
#define T1PS1        0x20
#define T1PS0        0x10
#define AC1OE        0x08
#define AC1FLT1      0x04
#define AC0OE        0x02
#define AC0FLT1      0x01


sfr AUXR5          = 0xA4;
// (bits in AUXR5)
#define C0IC4S0      0x80
#define C0IC2S0      0x40
#define C0PPS1       0x20
#define C0PPS0       0x10
#define C0PS1        0x08
#define C0PS0        0x04
#define ECIPS0       0x02
#define C0COPS       0x01


sfr AUXR6          = 0xA4;
// (bits in AUXR6)
#define KBIHPS1      0x80
#define KBIHPS0      0x40
#define KBILPS1      0x20
#define KBILPS0      0x10
#define T3FCS        0x08
#define T2FCS        0x04
#define SnMIPS       0x02
#define S0COPS       0x01


sfr AUXR7          = 0xA4;
// (bits in AUXR7)
#define POE5         0x80
#define POE4         0x40
#define C0CKOE       0x20
#define SPI0M0       0x10
#define BM1OE1       0x08
#define BM1OE0       0x04
#define BM0OE1       0x02
#define BM0OE0       0x01


sfr AUXR8          = 0xA4;
// (bits in AUXR8)
#define POE7         0x80
#define POE6         0x40
#define C0PPS2       0x20
#define S1COPS       0x04
#define T3PS1        0x02
#define T3PS0        0x01


sfr AUXR9          = 0xA4;
// (bits in AUXR9)
#define SIDPS1       0x80
#define SIDPS0       0x40
#define T1G1         0x20
#define T0G1         0x10
#define C0FDC1       0x08
#define C0FDC0       0x04
#define S1PS1        0x02
#define S1PS0        0x01


sfr AUXR10         = 0xA4;
// (bits in AUXR10)
#define AC1HC0       0x80
#define AC0HC0       0x40
#define SPIPS1       0x20
#define SPIPS0       0x10
#define S0PS1        0x08
#define SPFACE       0x04
#define TWICF        0x02
#define PAA          0x01


sfr AUXR11         = 0xA4;
// (bits in AUXR11)
#define I2C1CF       0x80
#define PAA1         0x40
#define I2C1PS1      0x20
#define I2C1PS0      0x10
#define POEM0		 0x04
#define C0M0         0x02
#define C0OFS        0x01


sfr AUXR12         = 0xA4;
// (bits in AUXR12)
#define T0COBE       0x20
#define T0COAE       0x10


sfr AUXR13         = 0xA4;
// (bits in AUXR13)
#define S3PS0        0x40
#define S2PS0        0x10
#define T4FCS        0x04
#define T4PS1        0x02
#define T4PS0        0x01


sfr AUXR14         = 0xA4;
// (bits in AUXR14)
#define ADLPS1       0x80
#define ADLPS0       0x40
#define RWPS0        0x08
#define AC2HC0       0x04
#define AC2OE        0x02
#define AC2FLT1      0x01


sfr AUXR15         = 0xA4;
// (bits in AUXR15)
#define MP_EN        0x80
#define MP_DS        0x40
#define TOG_EN       0x20
#define MP_IE        0x10


sfr EIE2           = 0xA5;
// (bits in EIE2)
#define EAC2         0x80
#define ETWI1        0x40
#define ES3          0x10
#define ET4          0x08
#define ES2          0x04
#define EAC1         0x02
#define ET3          0x01


sfr EIP2L          = 0xA6;
// (bits in EIP2L)
#define PAC2L        0x80
#define PTWI1L       0x40
#define PS3L         0x10
#define PT4L         0x08
#define PS2L         0x04
#define PAC1L        0x02
#define PT3L         0x01


sfr EIP2H          = 0xA7;
// (bits in EIP2H)
#define PAC2H        0x80
#define PTWI1H       0x40
#define PS3H         0x10
#define PT4H         0x08
#define PS2H         0x04
#define PAC1H        0x02
#define PT3H         0x01


sfr IE             = 0xA8;
// (bits in IE)
sbit EA            = IE^7;
sbit EDMA          = IE^6;
sbit ET2           = IE^5;
sbit ES0           = IE^4;
sbit ET1           = IE^3;
sbit EX1           = IE^2;
sbit ET0           = IE^1;
sbit EX0           = IE^0;

sfr SADDR          = 0xA9;

sfr SFRPI          = 0xAC;
// (bits in SFRPI)
#define IDX3         0x08
#define IDX2         0x04
#define IDX1         0x02
#define IDX0         0x01


sfr EIE1           = 0xAD;
// (bits in EIE1)
#define EAC0         0x80
#define ETWI0        0x40
#define EKB          0x20
#define ES1          0x10
#define ESF          0x08
#define EPCA         0x04
#define EADC         0x02
#define ESPI         0x01


sfr EIP1L          = 0xAE;
// (bits in EIP1L)
#define PAC0L        0x80
#define PTWI0L       0x40
#define PKBL         0x20
#define PS1L         0x10
#define PSFL         0x08
#define PPCAL        0x04
#define PADCL        0x02
#define PSPIL        0x01


sfr EIP1H          = 0xAF;
// (bits in EIP1H)
#define PAC0H        0x80
#define PTWI0H       0x40
#define PKBH         0x20
#define PS1H         0x10
#define PSFH         0x08
#define PPCAH        0x04
#define PADCH        0x02
#define PSPIH        0x01


sfr P3             = 0xB0;
// (bits in P3)
sbit P37           = P3^7;
sbit P36           = P3^6;
sbit P35           = P3^5;
sbit P34           = P3^4;
sbit P33           = P3^3;
sbit P32           = P3^2;
sbit P31           = P3^1;
sbit P30           = P3^0;


sfr P3M0           = 0xB1;
// (bits in P3M0)
#define P3M07        0x80
#define P3M06        0x40
#define P3M05        0x20
#define P3M04        0x10
#define P3M03        0x08
#define P3M02        0x04
#define P3M01        0x02
#define P3M00        0x01


sfr P3M1           = 0xB2;
// (bits in P3M1)
#define P3M17        0x80
#define P3M16        0x40
#define P3M15        0x20
#define P3M14        0x10
#define P3M13        0x08
#define P3M12        0x04
#define P3M11        0x02
#define P3M10        0x01


sfr P4M0           = 0xB3;
// (bits in P4M0)
#define P4M07        0x80
#define P4M06        0x40
#define P4M05        0x20
#define P4M04        0x10
#define P4M03        0x08
#define P4M02        0x04
#define P4M01        0x02
#define P4M00        0x01


sfr PDRVC0         = 0xB4;
// (bits in PDRVC0)
#define P3DC1        0x80
#define P3DC0        0x40
#define P2DC1        0x20
#define P2DC0        0x10
#define P1DC1        0x08
#define P1DC0        0x04
#define P0DC1        0x02
#define P0DC0        0x01


sfr PDRVC1         = 0xB4;
// (bits in PDRVC1)
#define P7DC0        0x40
#define P6DC1        0x20
#define P6DC0        0x10
#define P5DC1        0x08
#define P5DC0        0x04
#define P4DC1        0x02
#define P4DC0        0x01


sfr P5M0           = 0xB5;
// (bits in P5M0)
#define P5M07        0x80
#define P5M06        0x40
#define P5M05        0x20
#define P5M04        0x10
#define P5M03        0x08
#define P5M02        0x04
#define P5M01        0x02
#define P5M00        0x01


sfr P6M0           = 0xB5;
// (bits in P6M0)
#define P6M07        0x80
#define P6M06        0x40
#define P6M05        0x20
#define P6M04        0x10
#define P6M03        0x08
#define P6M02        0x04
#define P6M01        0x02
#define P6M00        0x01


sfr P7M0           = 0xB5;
// (bits in P7M0)
#define P7M03        0x08
#define P7M02        0x04
#define P7M01        0x02
#define P7M00        0x01


sfr EMBTC          = 0xB5;
// (bits in EMBTC)
#define RWSH         0x08
#define RWS2         0x04
#define RWS1         0x02
#define RWS0         0x01


sfr RTCTM          = 0xB6;
// (bits in RTCTM)
#define RTCCS1       0x80
#define RTCCS0       0x40
#define RTCCT5       0x20
#define RTCCT4       0x10
#define RTCCT3       0x08
#define RTCCT2       0x04
#define RTCCT1       0x02
#define RTCCT0       0x01


sfr IP0H           = 0xB7;
// (bits in IP0H)
#define PX3H         0x80
#define PX2H         0x40
#define PT2H         0x20
#define PSH          0x10
#define PT1H         0x08
#define PX1H         0x04
#define PT0H         0x02
#define PX0H         0x01


sfr IP0L           = 0xB8;
// (bits in IP0L)
sbit PX3L          = IP0L^7;
sbit PX2L          = IP0L^6;
sbit PT2L          = IP0L^5;
sbit PSL           = IP0L^4;
sbit PT1L          = IP0L^3;
sbit PX1L          = IP0L^2;
sbit PT0L          = IP0L^1;
sbit PX0L          = IP0L^0;

sfr SADEN          = 0xB9;

sfr S0CR1          = 0xB9;
// (bits in S0CR1)
#define S0TR         0x80
#define S0TX12       0x40
#define S0TCK        0x20
#define S0RCK        0x10
#define S0CKOE       0x08
#define ARTE         0x04


sfr PWMCR          = 0xBC;
// (bits in PWMCR)
#define PCAE         0x80
#define EXDT         0x40
#define PBKM         0x20
#define PBKE11       0x10
#define PBKE10       0x08
#define PBKE02       0x04
#define PBKE01       0x02
#define PBKE00       0x01


sfr PDTCRA         = 0xBC;
// (bits in PDTCRA)
#define DTPS1        0x80
#define DTPS0        0x40
#define DT5          0x20
#define DT4          0x10
#define DT3          0x08
#define DT2          0x04
#define DT1          0x02
#define DT0          0x01


sfr BM0CR          = 0xBC;
// (bits in BM0CR)
#define BM0OUT       0x80
#define BM0SW        0x40
#define BM0IE        0x20
#define BM0POL       0x10
#define M0FLT1       0x08
#define M0FLT0       0x04
#define BM0M1        0x02
#define BM0M0        0x01


sfr BM0SE          = 0xBC;
// (bits in BM0SE)
#define BM0E13       0x80
#define BM0E12       0x40
#define BM0E11       0x20
#define BM0E10       0x10
#define BM0E03       0x08
#define BM0E02       0x04
#define BM0E01       0x02
#define BM0E00       0x01


sfr BM0LCR         = 0xBC;
// (bits in BM0LCR)
#define BM0LCP       0x80
#define BM0LS3       0x08
#define BM0LS2       0x04
#define BM0LS1       0x02
#define BM0LS0       0x01


sfr BM0HCR         = 0xBC;
// (bits in BM0HCR)
#define BM0HCP       0x80
#define BM0HS3       0x08
#define BM0HS2       0x04
#define BM0HS1       0x02
#define BM0HS0       0x01


sfr BM1CR          = 0xBC;
// (bits in BM1CR)
#define BM1OUT       0x80
#define BM1SW        0x40
#define BM1IE        0x20
#define BM1POL       0x10
#define M1FLT1       0x08
#define M1FLT0       0x04
#define BM1M1        0x02
#define BM1M0        0x01


sfr BM1SE          = 0xBC;
// (bits in BM1SE)
#define BM1E13       0x80
#define BM1E12       0x40
#define BM1E11       0x20
#define BM1E10       0x10
#define BM1E03       0x08
#define BM1E02       0x04
#define BM1E01       0x02
#define BM1E00       0x01


sfr BM1LCR         = 0xBC;
// (bits in BM1LCR)
#define BM1LCP       0x80
#define BM1LS3       0x08
#define BM1LS2       0x04
#define BM1LS1       0x02
#define BM1LS0       0x01


sfr BM1HCR         = 0xBC;
// (bits in BM1HCR)
#define BM1HCP       0x80
#define BM1HS3       0x08
#define BM1HS2       0x04
#define BM1HS1       0x02
#define BM1HS0       0x01

sfr CRC0DA         = 0xBD;

sfr RTCCR          = 0xBE;
// (bits in RTCCR)
#define RTCE         0x80
#define RTCO         0x40
#define RTCRL5       0x20
#define RTCRL4       0x10
#define RTCRL3       0x08
#define RTCRL2       0x04
#define RTCRL1       0x02
#define RTCRL0       0x01


sfr CKCON1         = 0xBF;
// (bits in CKCON1)
#define XTOR         0x80
#define MCKSTA1      0x20
#define MCKSTA0      0x10
#define OSCSTA3      0x08
#define OSCSTA2      0x04
#define OSCSTA1      0x02
#define OSCSTA0      0x01


sfr XICON          = 0xC0;
// (bits in XICON)
sbit INT3H         = XICON^7;
sbit EX3           = XICON^6;
sbit IE3           = XICON^5;
sbit IT3           = XICON^4;
sbit INT2H         = XICON^3;
sbit EX2           = XICON^2;
sbit IE2           = XICON^1;
sbit IT2           = XICON^0;


sfr XICFG          = 0xC1;
// (bits in XICFG)
#define INT1IS1      0x80
#define INT1IS0      0x40
#define INT0IS1      0x20
#define INT0IS0      0x10
#define X3FLT        0x08
#define X2FLT        0x04
#define X1FLT        0x02
#define X0FLT        0x01


sfr XICFG1         = 0xC1;
// (bits in XICFG1)
#define INT1IS2      0x80
#define INT0IS2      0x40
#define INT2IS1      0x20
#define INT2IS0      0x10
#define X3FLT1       0x08
#define X2FLT1       0x04
#define X1FLT1       0x02
#define X0FLT1       0x01


sfr XICFG2         = 0xC1;
// (bits in XICFG2)
#define INT3IS2      0x08
#define INT3IS1      0x04
#define INT3IS0      0x02
#define INT2IS2      0x01


sfr ADCFG0         = 0xC3;
// (bits in ADCFG0)
#define ADCKS2       0x80
#define ADCKS1       0x40
#define ADCKS0       0x20
#define ADRJ         0x10
#define ACHS         0x08
#define SMPF         0x04
#define ADTM1        0x02
#define ADTM0        0x01


sfr ADCFG1         = 0xC3;
// (bits in ADCFG1)
#define IGADCI       0x80
#define EADCWI       0x40
#define SMPFIE       0x20
#define SIGN         0x10
#define AOS3         0x08
#define AOS2         0x04
#define AOS1         0x02
#define AOS0         0x01


sfr ADCFG2         = 0xC3;
// (bits in ADCFG2)
#define SHT7         0x80
#define SHT6         0x40
#define SHT5         0x20
#define SHT4         0x10
#define SHT3         0x08
#define SHT2         0x04
#define SHT1         0x02
#define SHT0         0x01


sfr ADCFG3         = 0xC3;
// (bits in ADCFG3)
#define ADPS1        0x80
#define ADPS0        0x40
#define ARES1        0x08
#define ARES0        0x04
#define ADES0        0x02


sfr ADCFG4         = 0xC3;
// (bits in ADCFG4)
#define ADWM0        0x40
#define ADTM3        0x20
#define ADTM2        0x10
#define DBSD         0x02
#define DOSD         0x01


sfr ADCFG5         = 0xC3;
// (bits in ADCFG5)
#define ASCE7        0x80
#define ASCE6        0x40
#define ASCE5        0x20
#define ASCE4        0x10
#define ASCE3        0x08
#define ASCE2        0x04
#define ASCE1        0x02
#define ASCE0        0x01


sfr ADCFG6         = 0xC3;
// (bits in ADCFG6)
#define ASCE15       0x80
#define ASCE14       0x40
#define ASCE13       0x20
#define ASCE12       0x10
#define ASCE11       0x08
#define ASCE10       0x04
#define ASCE9        0x02
#define ASCE8        0x01


sfr ADCFG11        = 0xC3;
// (bits in ADCFG11)
#define WHB3         0x80
#define WHB2         0x40
#define WHB1         0x20
#define WHB0         0x10


sfr ADCFG12        = 0xC3;
// (bits in ADCFG12)
#define WHB11        0x80
#define WHB10        0x40
#define WHB9         0x20
#define WHB8         0x10
#define WHB7         0x08
#define WHB6         0x04
#define WHB5         0x02
#define WHB4         0x01


sfr ADCFG13        = 0xC3;
// (bits in ADCFG13)
#define WLB3         0x80
#define WLB2         0x40
#define WLB1         0x20
#define WLB0         0x10


sfr ADCFG14        = 0xC3;
// (bits in ADCFG14)
#define WLB11        0x80
#define WLB10        0x40
#define WLB9         0x20
#define WLB8         0x10
#define WLB7         0x08
#define WLB6         0x04
#define WLB5         0x02
#define WLB4         0x01


sfr ADCON0         = 0xC4;
// (bits in ADCON0)
#define ADCEN        0x80
#define ADCWI        0x40
#define CHS3         0x20
#define ADCI         0x10
#define ADCS         0x08
#define CHS2         0x04
#define CHS1         0x02
#define CHS0         0x01


sfr ADCDL          = 0xC5;
// (bits in ADCDL)
#define ADCV3        0x80
#define ADCV2        0x40
#define ADCV1        0x20
#define ADCV0        0x10


sfr ADCDH          = 0xC6;
// (bits in ADCDH)
#define ADCV11       0x80
#define ADCV10       0x40
#define ADCV9        0x20
#define ADCV8        0x10
#define ADCV7        0x08
#define ADCV6        0x04
#define ADCV5        0x02
#define ADCV4        0x01


sfr CKCON0         = 0xC7;
// (bits in CKCON0)
#define AFS          0x80
#define ENCKM        0x40
#define CKMIS1       0x20
#define CKMIS0       0x10
#define CCKS         0x08
#define SCKS2        0x04
#define SCKS1        0x02
#define SCKS0        0x01


sfr T2CON          = 0xC8;
// (bits in T2CON)
sbit TF2           = T2CON^7;
sbit EXF2          = T2CON^6;
sbit RCLK_TF2L     = T2CON^5;
sbit TCLK_TL2IE    = T2CON^4;
sbit EXEN2         = T2CON^3;
sbit TR2           = T2CON^2;
sbit C_T2          = T2CON^1;
sbit CP_RL2        = T2CON^0;


sfr T3CON          = 0xC8;
// (bits in T3CON)
sbit TF3           = T3CON^7;
sbit EXF3          = T3CON^6;
sbit TF3L          = T3CON^5;
sbit TL3IE         = T3CON^4;
sbit EXEN3         = T3CON^3;
sbit TR3           = T3CON^2;
sbit C_T3          = T3CON^1;
sbit CP_RL3        = T3CON^0;


sfr T4CON          = 0xC8;
// (bits in T4CON)
sbit TF4           = T4CON^7;
sbit EXF4          = T4CON^6;
sbit TF4L          = T4CON^5;
sbit TL4IE         = T4CON^4;
sbit EXEN4         = T4CON^3;
sbit TR4           = T4CON^2;
sbit C_T4          = T4CON^1;
sbit CP_RL4        = T4CON^0;


sfr T5CON          = 0xC8;
// (bits in T5CON)
sbit TF5           = T5CON^7;
sbit T5CKS1        = T5CON^5;
sbit T5CKS0        = T5CON^4;
sbit T5IE          = T5CON^3;
sbit TR5           = T5CON^2;
sbit T5GAT1        = T5CON^1;
sbit T5GAT0        = T5CON^0;


sfr T6CON          = 0xC8;
// (bits in T6CON)
sbit TF6           = T6CON^7;
sbit T6CKS1        = T6CON^5;
sbit T6CKS0        = T6CON^4;
sbit T6IE          = T6CON^3;
sbit TR6           = T6CON^2;
sbit T6GAT1        = T6CON^1;
sbit T6GAT0        = T6CON^0;


sfr T2MOD          = 0xC9;
// (bits in T2MOD)
#define T2SPL        0x80
#define TL2X12       0x40
#define T2EXH        0x20
#define T2X12        0x10
#define TR2L         0x08
#define TR2LC        0x04
#define T2OE         0x02
#define T2MS0        0x01


sfr T3MOD          = 0xC9;
// (bits in T3MOD)
#define T3SPL        0x80
#define TL3X12       0x40
#define T3EXH        0x20
#define T3X12        0x10
#define TR3L         0x08
#define TR3LC        0x04
#define T3OE         0x02
#define T3MS0        0x01


sfr T4MOD          = 0xC9;
// (bits in T4MOD)
#define T4SPL        0x80
#define TL4X12       0x40
#define T4EXH        0x20
#define T4X12        0x10
#define TR4L         0x08
#define TR4LC        0x04
#define T4OE         0x02
#define T4MS0        0x01

sfr RCAP2L         = 0xCA;
sfr RCAP3L         = 0xCA;
sfr RCAP4L         = 0xCA;
sfr TLR5           = 0xCA;
sfr TLR6           = 0xCA;
sfr RCAP2H         = 0xCB;
sfr RCAP3H         = 0xCB;
sfr RCAP4H         = 0xCB;
sfr THR5           = 0xCB;
sfr THR6           = 0xCB;
sfr TL2            = 0xCC;
sfr TL3            = 0xCC;
sfr TL4            = 0xCC;
sfr TL5            = 0xCC;
sfr TL6            = 0xCC;
sfr TH2            = 0xCD;
sfr TH3            = 0xCD;
sfr TH4            = 0xCD;
sfr TH5            = 0xCD;
sfr TH6            = 0xCD;
sfr CLRL           = 0xCE;
sfr CHRL           = 0xCF;

sfr PSW            = 0xD0;
// (bits in PSW)
sbit CY            = PSW^7;
sbit AC            = PSW^6;
sbit F0            = PSW^5;
sbit RS1           = PSW^4;
sbit RS0           = PSW^3;
sbit OV            = PSW^2;
sbit F1            = PSW^1;
sbit P             = PSW^0;


sfr SIADR          = 0xD1;
// (bits in SIADR)
#define GC           0x01


sfr SI1ADR         = 0xD1;
// (bits in SI1ADR)
#define GC1          0x01


sfr SIA2           = 0xD1;
// (bits in SIA2)
#define A2E          0x01


sfr SI1A2          = 0xD1;
// (bits in SI1A2)
#define A2E1         0x01

sfr SIDAT          = 0xD2;
sfr SI1DAT         = 0xD2;
sfr SIA2M          = 0xD2;
sfr SI1A2M         = 0xD2;
sfr SISTA          = 0xD3;
sfr SI1STA         = 0xD3;

sfr SICON          = 0xD4;
// (bits in SICON)
#define CR2          0x80
#define ENSI         0x40
#define STA          0x20
#define STO          0x10
#define SI           0x08
#define AA           0x04
#define CR1          0x02
#define CR0          0x01


sfr SI1CON         = 0xD4;
// (bits in SI1CON)
#define CR21         0x80
#define ENSI1        0x40
#define STA1         0x20
#define STO1         0x10
#define SI1          0x08
#define AA1          0x04
#define CR11         0x02
#define CR01         0x01

sfr KBPATN         = 0xD5;

sfr KBCON          = 0xD6;
// (bits in KBCON)
#define KBCS1        0x80
#define KBCS0        0x40
#define KBES         0x20
#define PATN_SEL     0x02
#define KBIF         0x01

sfr KBMASK         = 0xD7;

sfr CCON           = 0xD8;
// (bits in CCON)
sbit CF            = CCON^7;
sbit CR            = CCON^6;
sbit CCF5          = CCON^5;
sbit CCF4          = CCON^4;
sbit CCF3          = CCON^3;
sbit CCF2          = CCON^2;
sbit CCF1          = CCON^1;
sbit CCF0          = CCON^0;


sfr CMOD           = 0xD9;
// (bits in CMOD)
#define CIDL         0x80
#define BME4         0x40
#define BME2         0x20
#define BME0         0x10
#define CPS2         0x08
#define CPS1         0x04
#define CPS0         0x02
#define ECF          0x01


sfr CCAPM0         = 0xDA;
// (bits in CCAPM0)
#define DTE0         0x80
#define ECOM0        0x40
#define CAPP0        0x20
#define CAPN0        0x10
#define MAT0         0x08
#define TOG0         0x04
#define PWM0         0x02
#define ECCF0        0x01


sfr CCAPM6         = 0xDA;
// (bits in CCAPM6)
#define BME6         0x80
#define ECOM6        0x40
#define CAPN6        0x10
#define MAT6         0x08
#define TOG6         0x04
#define PWM6         0x02
#define ECCF6        0x01


sfr CCAPM1         = 0xDB;
// (bits in CCAPM1)
#define ECOM1        0x40
#define CAPP1        0x20
#define CAPN1        0x10
#define MAT1         0x08
#define TOG1         0x04
#define PWM1         0x02
#define ECCF1        0x01


sfr CCAPM7         = 0xDB;
// (bits in CCAPM7)
#define ECOM7        0x40
#define CAPN7        0x10
#define MAT7         0x08
#define TOG7         0x04
#define PWM7         0x02
#define ECCF7        0x01


sfr CCAPM2         = 0xDC;
// (bits in CCAPM2)
#define DTE2         0x80
#define ECOM2        0x40
#define CAPP2        0x20
#define CAPN2        0x10
#define MAT2         0x08
#define TOG2         0x04
#define PWM2         0x02
#define ECCF2        0x01


sfr CCAPM3         = 0xDD;
// (bits in CCAPM3)
#define ECOM3        0x40
#define CAPP3        0x20
#define CAPN3        0x10
#define MAT3         0x08
#define TOG3         0x04
#define PWM3         0x02
#define ECCF3        0x01


sfr CCAPM4         = 0xDE;
// (bits in CCAPM4)
#define DTE4         0x80
#define ECOM4        0x40
#define CAPP4        0x20
#define CAPN4        0x10
#define MAT4         0x08
#define TOG4         0x04
#define PWM4         0x02
#define ECCF4        0x01


sfr CCAPM5         = 0xDF;
// (bits in CCAPM5)
#define ECOM5        0x40
#define CAPP5        0x20
#define CAPN5        0x10
#define MAT5         0x08
#define TOG5         0x04
#define PWM5         0x02
#define ECCF5        0x01


sfr ACC            = 0xE0;
// (bits in ACC)
sbit ACC7          = ACC^7;
sbit ACC6          = ACC^6;
sbit ACC5          = ACC^5;
sbit ACC4          = ACC^4;
sbit ACC3          = ACC^3;
sbit ACC2          = ACC^2;
sbit ACC1          = ACC^1;
sbit ACC0          = ACC^0;


sfr WDTCR          = 0xE1;
// (bits in WDTCR)
#define WREN         0x80
#define NSW          0x40
#define ENW          0x20
#define CLRW         0x10
#define WIDL         0x08
#define PS2          0x04
#define PS1          0x02
#define PS0          0x01

sfr IFD            = 0xE2;
sfr IFADRH         = 0xE3;
sfr IFADRL         = 0xE4;

sfr IFMT           = 0xE5;
// (bits in IFMT)
#define MS7          0x80
#define MS3          0x08
#define MS2          0x04
#define MS1          0x02
#define MS0          0x01

sfr SCMD           = 0xE6;

sfr ISPCR          = 0xE7;
// (bits in ISPCR)
#define ISPEN        0x80
#define SWBS         0x40
#define SRST         0x20
#define CFAIL        0x10


sfr P4             = 0xE8;
// (bits in P4)
sbit P47           = P4^7;
sbit P46           = P4^6;
sbit P45           = P4^5;
sbit P44           = P4^4;
sbit P43           = P4^3;
sbit P42           = P4^2;
sbit P41           = P4^1;
sbit P40           = P4^0;

sfr CL             = 0xE9;
sfr CCAP0L         = 0xEA;
sfr CCAP6L         = 0xEA;
sfr CCAP1L         = 0xEB;
sfr CCAP7L         = 0xEB;
sfr CCAP2L         = 0xEC;
sfr CCAP3L         = 0xED;
sfr CCAP4L         = 0xEE;
sfr CCAP5L         = 0xEF;

sfr B              = 0xF0;
// (bits in B)
sbit B7            = B^7;
sbit B6            = B^6;
sbit B5            = B^5;
sbit B4            = B^4;
sbit B3            = B^3;
sbit B2            = B^2;
sbit B1            = B^1;
sbit B0            = B^0;


sfr PAOE           = 0xF1;
// (bits in PAOE)
#define POE3         0x80
#define POE2B        0x40
#define POE2A        0x20
#define POE2         0x10
#define POE1         0x08
#define POE0B        0x04
#define POE0A        0x02
#define POE0         0x01


sfr PCAPWM0        = 0xF2;
// (bits in PCAPWM0)
#define P0RS1        0x80
#define P0RS0        0x40
#define P0INV        0x04
#define ECAP0H       0x02
#define ECAP0L       0x01


sfr PCAPWM6        = 0xF2;
// (bits in PCAPWM6)
#define P6RS1        0x80
#define P6RS0        0x40
#define CCF6         0x08
#define P6INV        0x04
#define ECAP6H       0x02
#define ECAP6L       0x01


sfr PCAPWM1        = 0xF3;
// (bits in PCAPWM1)
#define P1RS1        0x80
#define P1RS0        0x40
#define P1INV        0x04
#define ECAP1H       0x02
#define ECAP1L       0x01


sfr PCAPWM7        = 0xF3;
// (bits in PCAPWM7)
#define P7RS1        0x80
#define P7RS0        0x40
#define CCF7         0x08
#define P7INV        0x04
#define ECAP7H       0x02
#define ECAP7L       0x01


sfr PCAPWM2        = 0xF4;
// (bits in PCAPWM2)
#define P2RS1        0x80
#define P2RS0        0x40
#define P2INV        0x04
#define ECAP2H       0x02
#define ECAP2L       0x01


sfr PCAPWM3        = 0xF5;
// (bits in PCAPWM3)
#define P3RS1        0x80
#define P3RS0        0x40
#define P3INV        0x04
#define ECAP3H       0x02
#define ECAP3L       0x01


sfr PCAPWM4        = 0xF6;
// (bits in PCAPWM4)
#define P4RS1        0x80
#define P4RS0        0x40
#define P4INV        0x04
#define ECAP4H       0x02
#define ECAP4L       0x01


sfr PCAPWM5        = 0xF7;
// (bits in PCAPWM5)
#define P5RS1        0x80
#define P5RS0        0x40
#define P5INV        0x04
#define ECAP5H       0x02
#define ECAP5L       0x01


sfr P5             = 0xF8;
// (bits in P5)
sbit P57           = P5^7;
sbit P56           = P5^6;
sbit P55           = P5^5;
sbit P54           = P5^4;
sbit P53           = P5^3;
sbit P52           = P5^2;
sbit P51           = P5^1;
sbit P50           = P5^0;


sfr P6             = 0xF8;
// (bits in P6)
sbit P67           = P6^7;
sbit P66           = P6^6;
sbit P65           = P6^5;
sbit P64           = P6^4;
sbit P63           = P6^3;
sbit P62           = P6^2;
sbit P61           = P6^1;
sbit P60           = P6^0;


sfr P7             = 0xF8;
// (bits in P7)
sbit P73           = P7^3;
sbit P72           = P7^2;
sbit P71           = P7^1;
sbit P70           = P7^0;

sfr CH             = 0xF9;
sfr CCAP0H         = 0xFA;
sfr CCAP6H         = 0xFA;
sfr CCAP1H         = 0xFB;
sfr CCAP7H         = 0xFB;
sfr CCAP2H         = 0xFC;
sfr CCAP3H         = 0xFD;
sfr CCAP4H         = 0xFE;
sfr CCAP5H         = 0xFF;

//===== Page P =====//

#define IAPLB_P      0x03
// (bits in IAPLB)
#define IAPLB6_P     0x80
#define IAPLB5_P     0x40
#define IAPLB4_P     0x20
#define IAPLB3_P     0x10
#define IAPLB2_P     0x08
#define IAPLB1_P     0x04
#define IAPLB0_P     0x02


#define CKCON2_P     0x40
// (bits in CKCON2)
#define XTGS1_P      0x80
#define XTGS0_P      0x40
#define XTALE_P      0x20
#define IHRCOE_P     0x10
#define MCKS1_P      0x08
#define MCKS0_P      0x04
#define OSCS1_P      0x02
#define OSCS0_P      0x01


#define CKCON3_P     0x41
// (bits in CKCON3)
#define WDTCS1_P     0x80
#define WDTCS0_P     0x40
#define FWKP_P       0x20
#define WDTFS_P      0x10
#define MCKD1_P      0x08
#define MCKD0_P      0x04
#define MCDS1_P      0x02
#define MCDS0_P      0x01


#define CKCON4_P     0x42
// (bits in CKCON4)
#define RCSS2_P      0x80
#define RCSS1_P      0x40
#define RCSS0_P      0x20
#define RPSC2_P      0x10
#define RPSC1_P      0x08
#define RPSC0_P      0x04
#define RTCCS3_P     0x02
#define RTCCS2_P     0x01


#define CKCON5_P     0x43
// (bits in CKCON5)
#define CKMS0_P      0x01


#define PCON2_P      0x44
// (bits in PCON2)
#define AWBOD1_P     0x80
#define BO1S1_P      0x20
#define BO1S0_P      0x10
#define BO1RE_P      0x08
#define EBOD1_P      0x04
#define BO0RE_P      0x02


#define PCON3_P      0x45
// (bits in PCON3)
#define IVREN_P      0x80
#define PSM0_P       0x20


#define SPCON0_P     0x48
// (bits in SPCON0)
#define P6CTL_P      0x40
#define P4CTL_P      0x20
#define WRCTL_P      0x10
#define CKCTL0_P     0x04
#define PWCTL1_P     0x02
#define PWCTL0_P     0x01


#define DCON0_P      0x4C
// (bits in DCON0)
#define HSE_P        0x80
#define IAPO_P       0x40
#define HSE1_P       0x20
#define IORCTL_P     0x04
#define RSTIO_P      0x02
#define OCDE_P       0x01


#define SPHB_P       0x53
// (bits in SPHB)
#define SPHB3_P      0x08
#define SPHB2_P      0x04
#define SPHB1_P      0x02
#define SPHB0_P      0x01


#define RTCCR_P      0x54
// (bits in RTCCR)
#define RTCE_P       0x80
#define RTCO_P       0x40
#define RTCRL5_P     0x20
#define RTCRL4_P     0x10
#define RTCRL3_P     0x08
#define RTCRL2_P     0x04
#define RTCRL1_P     0x02
#define RTCRL0_P     0x01


#define RTCTM_P      0x55
// (bits in RTCTM)
#define RTCCS1_P     0x80
#define RTCCS0_P     0x40
#define RTCCT5_P     0x20
#define RTCCT4_P     0x10
#define RTCCT3_P     0x08
#define RTCCT2_P     0x04
#define RTCCT1_P     0x02
#define RTCCT0_P     0x01


#define PCON0_P      0x87
// (bits in PCON0)
#define SMOD1_P      0x80
#define SMOD0_P      0x40
#define POF0_P       0x10
#define PD_P         0x02
#define IDL_P        0x01


#define PCON1_P      0x97
// (bits in PCON1)
#define SWRF_P       0x80
#define EXRF_P       0x40
#define MCDF_P       0x20
#define RTCF_P       0x10
#define SPWF_P       0x08
#define BOF1_P       0x04
#define BOF0_P       0x02
#define WDTF_P       0x01


#define CKCON1_P     0xBF
// (bits in CKCON1)
#define XTOR_P       0x80
#define MCKSTA1_P    0x20
#define MCKSTA0_P    0x10
#define OSCSTA3_P    0x08
#define OSCSTA2_P    0x04
#define OSCSTA1_P    0x02
#define OSCSTA0_P    0x01


#define CKCON0_P     0xC7
// (bits in CKCON0)
#define AFS_P        0x80
#define ENCKM_P      0x40
#define CKMIS1_P     0x20
#define CKMIS0_P     0x10
#define CCKS_P       0x08
#define SCKS2_P      0x04
#define SCKS1_P      0x02
#define SCKS0_P      0x01


#define WDTCR_P      0xE1
// (bits in WDTCR)
#define WREN_P       0x80
#define NSW_P        0x40
#define ENW_P        0x20
#define CLRW_P       0x10
#define WIDL_P       0x08
#define PS2_P        0x04
#define PS1_P        0x02
#define PS0_P        0x01


#define P4_P         0xE8
// (bits in P4)
#define P47_P        0x80
#define P46_P        0x40
#define P45_P        0x20
#define P44_P        0x10
#define P43_P        0x08
#define P42_P        0x04
#define P41_P        0x02
#define P40_P        0x01


#define P6_P         0xF8
// (bits in P6)
#define P67_P        0x80
#define P66_P        0x40
#define P65_P        0x20
#define P64_P        0x10
#define P63_P        0x08
#define P62_P        0x04
#define P61_P        0x02
#define P60_P        0x01

#endif
