/**********************************************/
/*              TM 2017                       */
/*                                            */
/*               fable2000@rambler.ru         */
/*                droik@techmonitor.ru        */
/**********************************************/
#include <device.h>
#include <params_def.h>
#include <delay.h>
#include <intrinsics.h>
#include <type_define.h>
#include <flash_map.h>
#include <sram_map.h>
#include "Init.cpp"
#include "interrupts.cpp"
//******************************************************************************
//**  Прототипы функций
//******************************************************************************
void MainLogicManagmentProcess(void);
float GetVoltValue(DWord* ADCMEM);
float GetCurrentValue(DWord* ADCMEM);
void SetLedsValue(Byte val);
void ClrFlashSegment(Byte* SegAddr);
void WriteByteToFlash(Byte data, Byte* Addr);
void WriteBufToPage(Byte* PageAddr, Byte *Buf, Word BufLen);
void Save_ParamsInInfo(Data_Params  *Params);
//******************************************************************************
//** Начало программы
//******************************************************************************
void main(void)
{
  SYSTEM_INIT(); 
  SetLedsValue(0xFF);
  
  __enable_interrupt(); 
  Start_ADC(0); 
  for(;;)
  {
    CLEAR_WDT;
    MainLogicManagmentProcess();
  } 
}
//******************************************************************************
//**  
//******************************************************************************
void MainLogicManagmentProcess()
{
  static float U = 0.0;
  static float I1 = 0.0;
  static float I2 = 0.0;
  
  if(ADC_Ready)
  {
    ADC_Ready = false;
    U  = GetVoltValue(&AKB[0].AKB_OUT);
    I1 = GetCurrentValue(&AKB[1].AKB_OUT)*1000;
    I2 = GetCurrentValue(&AKB[2].AKB_OUT)*1000;
    
    _ledValues[0] = 0;
    _ledValues[1] = 0;
    
    bool Unorm          = U >= 40.0 && U <= 60.0;
    bool KZ1            = !(PA_IDR&BIT1);
    bool KZ2            = !(PA_IDR&BIT2);
    
    //U
    _ledValues[0] = !(PD_IDR&BIT4) || !Unorm ? _ledValues[0]& ~BIT0 : _ledValues[0] | BIT0;
    _ledValues[1] = !(PA_IDR&BIT3) || !Unorm ? _ledValues[1]& ~BIT0 : _ledValues[1] | BIT0;
    
    //KZ
    _ledValues[0] = KZ1 ? _ledValues[0] | BIT1 : _ledValues[0]& ~BIT1;
    _ledValues[1] = KZ2 ? _ledValues[1] | BIT1 : _ledValues[1]& ~BIT1;

    //Brc && Lamp
    if(!KZ1){//L1
      if(I1 < PtrToConstants->PorogValue1*0.2){//Brc
        _ledValues[0] |=  BIT2;
      }
      else{
        if(I1 < PtrToConstants->PorogValue1){//Lamp
          _ledValues[0] |=  BIT3;
        }
      }
    }
    if(!KZ2){//L2
      if(I2 < PtrToConstants->PorogValue2*0.2){//Brc
        _ledValues[1] |=  BIT2;
      }
      else{
        if(I2 < PtrToConstants->PorogValue2){//Lamp
          _ledValues[1] |=  BIT3;
        }
      }
    }
    
    //
    SetLedsValue(_ledValues[0] | _ledValues[1] << 4);
    //PC_ODR ^= BIT6;
  }
  
  if(_butonValue)
  {
      _butonValue = 0;

        Data_Params Params = *PtrToConstants;
        Params.PorogValue1 = (Word)(0.8*I1);
        Params.PorogValue2 = (Word)(0.8*I2);
        Save_ParamsInInfo(&Params);
      
      //blink leds
      SetLedsValue(0xFF);
      _delay_ms(200);
      SetLedsValue(0x0);
      
      _delay_ms(200);
      
      SetLedsValue(0xFF);
      _delay_ms(200);
      SetLedsValue(0x0);
      
      _delay_ms(200);
      
      SetLedsValue(0xFF);
      _delay_ms(200);
      
      
      //blink leds
      Soft_Reset();
  }
  
  
}
//******************************************************************************
//******************************************************************************
//******************************************************************************
//*****************************************************************************
//=======================================
float GetVoltValue(DWord* ADCMEM)
{
  float U = (3.3* (*ADCMEM))/(1023.0*1024.0);
  return U*U_DIVIDER;
}

//==============================================================================
// 
//=======================================
float GetCurrentValue(DWord* ADCMEM)
{  
  float U = (3.3* (*ADCMEM))/(1023.0*1024.0);
  return U*I_DIVIDER/I_SENS;
}
//==============================================================================
// 
//=======================================
void SetLedsValue(Byte Data)
{  
  //0 - NORM1
  //1 - KZ1
  //2 - BRC1
  //3 - LAMP1
  //
  //4 - NORM2
  //5 - KZ2
  //6 - BRC2
  //7 - LAMP2
  
  //strobe init
  PC_ODR &=  ~BIT4;
 
  Word cnt = sizeof(Data)*8;
  for(Byte i=0; i < cnt; i++){
    //data
    PC_ODR = ((1 << i)& Data) ? PC_ODR | BIT3 : PC_ODR & ~BIT3;
    __no_operation();
    
    //clk
    PC_ODR |=  BIT5;
    __no_operation();
    PC_ODR &= ~BIT5;
  };
  PC_ODR &= ~BIT3;//data off
  
  //strobe clk
    PC_ODR |=  BIT4;
    __no_operation();
    PC_ODR &=  ~BIT4;
}
//==============================================================================
//==============================================================================
//==============================================================================
// 
//=======================================
void Save_ParamsInInfo(Data_Params  *Params)
{
   WriteBufToPage((Byte *)&PARAMS, (Byte *)Params, sizeof(*Params)-1 ); 
}
//=============================================================================
//  
//=======================================
void ClrFlashSegment(Byte* SegAddr)
{  

}
//=============================================================================
//  
//=======================================
void WriteByteToFlash(Byte data, Byte* Addr)
{
  FLASH_DUKR = 0xAE;
  FLASH_DUKR = 0x56; 
  
  *Addr = data;
}
//=============================================================================
//  
//=======================================
void WriteBufToPage(Byte* PageAddr, Byte *Buf, Word BufLen)
{
  __disable_interrupt(); 

     ClrFlashSegment( PageAddr );
     
     for (Word i=0; i < BufLen; i++){
       CLEAR_WDT;
       WriteByteToFlash(Buf[i],PageAddr++);
     }
     CLEAR_WDT;
     WriteByteToFlash(0xAA, PageAddr);
   
  __enable_interrupt();
}


//==============================================================================
//==============================================================================