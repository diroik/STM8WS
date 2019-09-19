//******************************************************************************
//**
//**
//******************************************************************************
#ifndef TIPE_DEF_H
  #define TIPE_DEF_H
//******************************************************************************
  #include <device.h>
  #include <Classes_def.h>
//******************************************************************************
typedef unsigned char  Byte;
typedef unsigned int   Word;
typedef unsigned long  DWord;
typedef unsigned short Short;
typedef long long Word64;

//******************************************************************************
typedef signed char sByte;
typedef signed int  sWord;
typedef signed long sDWord;
//******************************************************************************
typedef union
{
    DWord Data_l;
    Byte Data_b[4];
} Blong;
//******************************************************************************
typedef union
{
    Word Data_s;
    Byte Data_b[2];
} Bshort;
//******************************************************************************
typedef union
{
    float Data_f;
    Byte Data_b[4];
} Bfloat;
//******************************************************************************
typedef unsigned int  HANDLE;
//******************************************************************************
//******************************************************************************
//Oeiu n?aaioie
//******************************************************************************
//#pragma pack(1)
typedef struct Time 
{ 
  int Year, Month, DayOfWeek, Day, Hour, Minute, Second; 
}TRtcTime;
//#pragma pack()
//*****************************************************************************
typedef enum  
{
    BOOT_FLG  = 0,
    DEV_ADDR,//1
    TIME_OUT,//2
    BOUD_RATE,//3
    RELE_STAT,//4
    INPUT_INV,//5
    SERIAL_LOW,//6
    SERIAL_HI,  //7 
    KOEF1,
    KOEF2,
    KOEF3,
    KOEF4
}PARAMETERS;
//******************************************************************************
//#pragma pack(1)
struct Data_Params
{
  
        Word DATA[20];
        
        Byte CRC;
        Byte KeyByte;      
};
//#pragma pack()
//******************************************************************************
//#pragma pack(1)
struct Data_Values
{
  Word    TermoDatchiks[MAX_ADC_INPUTS];
  //Word    InputValues;
  //Word    ReleValues; 
  Word    TermoSimpleDatchiks[MAX_ADC_INPUTS];  
};
//#pragma pack()

//******************************************************************************
//#pragma pack(1)
struct T_ADC_AKB
{  
  //Word*  ADCMEM;   
  DWord  AKB;
  DWord  AKB_OUT;
};
//#pragma pack()
//******************************************************************************
//#pragma pack(1)
struct T_DISCRETE
{  
  int                           cnt;
  unsigned char                 BIT;
  unsigned char volatile       *REG;
};
//#pragma pack()

//******************************************************************************
typedef struct commtim
{
  commtim()
  {
    ReadIntervalTimeout         = 0;
   // ReadTotalTimeout            = 0;
   // ReadTotalTimeoutMultiplier  = 5;
  }
    DWord ReadIntervalTimeout;
   // DWord ReadTotalTimeout;
   // Word ReadTotalTimeoutMultiplier;
}COMMTIMEOUTS;
//*****************************************************************************
typedef enum  
{
    COM0  = 1,
    COM1,
    COM2,
    COM3
}Port;
//*****************************************************************************



//=============================================================================
#endif // TIPE_DEF_H
//******************************************************************************
//******************************************************************************
