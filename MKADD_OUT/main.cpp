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
#include "Classes.cpp"
#include "Class_Tamplate.cpp"
#include "Usart.cpp"
#include "interrupts.cpp"
//******************************************************************************
//** 
//******************************************************************************
void MainLogicManagmentProcess(void);
void SetReleValue(Byte val);
float GetSimpleTempValue(DWord* ADCMEM);
sWord HexToByte(Byte *ptr);
void ByteToHex(Byte *HEX, Byte BYTE);
Byte CheckSum(Byte *BUF, Word Len);
bool ParsingAnswer( Byte *BUF, Word Len);
//****************************************
//**************************************
//** 
//******************************************************************************
void main(void)
{
    SYSTEM_INIT(); 
    DATA.ReleValues = PtrToConstants->DATA[RELE_STAT];
    SetReleValue(DATA.ReleValues);
  __enable_interrupt(); 
  
    Start_ADC(0);
    Usart.open(COM1, PtrToConstants->DATA[BOUD_RATE], PtrToConstants->DATA[TIME_OUT]);
    
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
  if(ADC_Ready)
  {
    ADC_Ready = false;
    for(Byte i=0;i<MAX_ADC_INPUTS;i++)
    {
      float value = GetSimpleTempValue(&AKB[i].AKB_OUT);
      if(value >= 199.0)
        value = 199.0;
      else if(value <= -99.0)
        value = -99.0;  
      
      float kvalue = (value == 199.0 || value == -99.0) ?  value :  value*(PARAMS.DATA[KOEF1+i]/1000.0);

      DATA.TermoDatchiks[i]             = (sWord)( kvalue*10);
      DATA.TermoSimpleDatchiks[i]       = (sWord)( value*10 );
    }
  }
  
  if(Input_Ready)
  {
    Input_Ready = false;
    for(Byte i=0;i<MAX_DISCRETE_INPUTS; i++)
    {
      bool inv = PtrToConstants->DATA[INPUT_INV] & (1 << i);
      bool state = inv ^ (bool)( *DISCRETE[i].REG & DISCRETE[i].BIT );
      DISCRETE[i].cnt += state ? 1:-1;
      
      if(DISCRETE[i].cnt >= FILTER_CNT){
        //on
        DISCRETE[i].cnt = FILTER_CNT;
        DATA.InputValues |= (1 << i);
      }
      else if(DISCRETE[i].cnt <= 0){
        //off
        DISCRETE[i].cnt = 0;
        DATA.InputValues &= ~(1 << i);
      }
    }
    //PC_ODR ^= BIT4;
    DATA.ReleValues = PtrToConstants->DATA[RELE_STAT];
    SetReleValue(DATA.ReleValues);
  }
  
  
  DataLen = Usart.RecvTo( Buffer, MAX_BUFF_SIZE, ETX );
  if(DataLen > 0)
  {
    if(ParsingAnswer(Buffer, DataLen))
    {
      sWord s = TBuffer::find_first_of(Buffer, DataLen, STX);
      if(s >=0)
      {
        Bshort reg;
        Bshort data;
        Bshort cnt;
        Byte bf[] = {0,0,0,0};
        Byte addr = HexToByte(&Buffer[s+1]);
        if(PtrToConstants->DATA[DEV_ADDR] == addr)
        {
          Byte fun = HexToByte(&Buffer[s+3]);
          reg.Data_b[0] = HexToByte(&Buffer[s+5]);
          reg.Data_b[1] = HexToByte(&Buffer[s+7]);
          
          bool failFlag = true;
          DataLen = 0;
          Buffer[DataLen++] = STX;
          ByteToHex(bf, PtrToConstants->DATA[DEV_ADDR]);
          Buffer[DataLen++] = bf[0];
          Buffer[DataLen++] = bf[1];
          ByteToHex(bf, fun);
          Buffer[DataLen++] = bf[0];
          Buffer[DataLen++] = bf[1];
          
          Word END_HOLDIG_REGISTERS_ADDR  = ( START_HOLDIG_REGISTERS_ADDR+(sizeof(DATA)/2) );
          Word END_DISCRETE_INPUTS_ADDR   = ( START_DISCRETE_INPUTS_ADDR+(sizeof(DATA)*8));
          switch(fun)
          {
          case 2:
              if(reg.Data_s >= START_DISCRETE_INPUTS_ADDR && reg.Data_s < END_DISCRETE_INPUTS_ADDR)
              {
                  cnt.Data_b[0] = HexToByte(&Buffer[s+9]);
                  cnt.Data_b[1] = HexToByte(&Buffer[s+11]);
                  Word l = reg.Data_s+cnt.Data_s > END_DISCRETE_INPUTS_ADDR ? (END_DISCRETE_INPUTS_ADDR-reg.Data_s):cnt.Data_s;
                  Word ln = l/16;
                  if(l%16 > 0)
                    ln++;
                  ByteToHex(bf, sizeof(data)*ln);
                  Buffer[DataLen++] = bf[0];
                  Buffer[DataLen++] = bf[1];
              
                  Word *data_ptr = (Word*)&DATA.InputValues;
                  Word start = (reg.Data_s-START_DISCRETE_INPUTS_ADDR)/16;
                  for(int i=start; i<(start+ln); i++)
                  {
                    data.Data_s = data_ptr[i];
                    ByteToHex(bf, data.Data_b[1]);
                    Buffer[DataLen++] = bf[0];
                    Buffer[DataLen++] = bf[1];
                    ByteToHex(bf, data.Data_b[0]);
                    Buffer[DataLen++] = bf[0];
                    Buffer[DataLen++] = bf[1];
                  }
                  failFlag = false;
            }
            break;
            
          case 3:
              if(  (reg.Data_s >= START_HOLDIG_REGISTERS_ADDR && reg.Data_s < END_HOLDIG_REGISTERS_ADDR)  ||
                 reg.Data_s >= (Word)PtrToConstants && reg.Data_s < ((Word)PtrToConstants + (sizeof(Data_Params)/2 - 1) ) )
              {//проверка на адекват диапазонов
                
                 Word START_ADDR =  (reg.Data_s >= START_HOLDIG_REGISTERS_ADDR && reg.Data_s < END_HOLDIG_REGISTERS_ADDR) ? 
                                      START_HOLDIG_REGISTERS_ADDR : (Word)PtrToConstants;
                 Word END_ADDR   =  (reg.Data_s >= START_HOLDIG_REGISTERS_ADDR && reg.Data_s < END_HOLDIG_REGISTERS_ADDR) ? 
                                      END_HOLDIG_REGISTERS_ADDR : ((Word)PtrToConstants + (sizeof(Data_Params)/2 - 1) );
                  
                  cnt.Data_b[0] = HexToByte(&Buffer[s+9]);
                  cnt.Data_b[1] = HexToByte(&Buffer[s+11]);
                  Word l = reg.Data_s+cnt.Data_s > END_ADDR ? (END_ADDR-reg.Data_s):cnt.Data_s;
                  ByteToHex(bf, sizeof(data)*l);
                  Buffer[DataLen++] = bf[0];
                  Buffer[DataLen++] = bf[1];
              
                  Word *data_ptr = (reg.Data_s >= START_HOLDIG_REGISTERS_ADDR && reg.Data_s < END_HOLDIG_REGISTERS_ADDR) ?
                    (Word*)&DATA : (Word*)PtrToConstants;
                  int start = (reg.Data_s-START_ADDR);
                  for(int i=start; i < (start+l) ; i++)
                  {
                    data.Data_s = data_ptr[i];
                    ByteToHex(bf, data.Data_b[0]);
                    Buffer[DataLen++] = bf[0];
                    Buffer[DataLen++] = bf[1];
                    ByteToHex(bf, data.Data_b[1]);
                    Buffer[DataLen++] = bf[0];
                    Buffer[DataLen++] = bf[1];
                  }
                  failFlag = false;
            }
            break;
              
          case 6://PtrToConstants addr = 0x4000. SET_RELE: fun=6 reg=0x4004 (16388) =>SetReleValue(PtrToConstants->DATA[RELE_STAT]) 
            if( reg.Data_s >= (Word)PtrToConstants && reg.Data_s < ((Word)PtrToConstants + (sizeof(Data_Params)/2 - 1) ) )
            {
              data.Data_b[0] = HexToByte(&Buffer[s+9]);
              data.Data_b[1] = HexToByte(&Buffer[s+11]); 
              
              Data_Params Params = *PtrToConstants;
              int ind = reg.Data_s -(Word)PtrToConstants;
              Word *param_ptr = (Word*)&Params;
              param_ptr[ind] = data.Data_s;
              TObject::Save_ParamsInInfo(&Params);//save

              ByteToHex(bf, reg.Data_b[0]);
              Buffer[DataLen++] = bf[0];
              Buffer[DataLen++] = bf[1];
              ByteToHex(bf, reg.Data_b[1]);
              Buffer[DataLen++] = bf[0];
              Buffer[DataLen++] = bf[1];
              
              Params = *PtrToConstants;
              data.Data_s = param_ptr[ind];
              ByteToHex(bf, data.Data_b[0]);
              Buffer[DataLen++] = bf[0];
              Buffer[DataLen++] = bf[1];
              
              ByteToHex(bf, data.Data_b[1]);
              Buffer[DataLen++] = bf[0];
              Buffer[DataLen++] = bf[1];
              
              failFlag = false;
            }
            else if(reg.Data_s == REBOOT_REGISTERS_ADDR)
            {
              Soft_Reset();
            }
            break;  
          };
     
          if(failFlag)
          {
            data.Data_s = 0x7FFF;
            ByteToHex(bf, sizeof(data));
            Buffer[DataLen++] = bf[0];
            Buffer[DataLen++] = bf[1];
            
            ByteToHex(bf, data.Data_b[0]);
            Buffer[DataLen++] = bf[0];
            Buffer[DataLen++] = bf[1];
            
            ByteToHex(bf, data.Data_b[1]);
            Buffer[DataLen++] = bf[0];
            Buffer[DataLen++] = bf[1];
          }
          ByteToHex(bf, CheckSum(&Buffer[1], DataLen-1));
          Buffer[DataLen++] = bf[0];
          Buffer[DataLen++] = bf[1];

          Buffer[DataLen++] = 0x0D;
          Buffer[DataLen++] = 0x0A;
        
          //PC_ODR ^= BIT4;
          Usart.Send(Buffer, DataLen);
          
          RebootCounter = 0;
        }
      }
    }
  }


}
//******************************************************************************
//******************************************************************************
//******************************************************************************
//*****************************************************************************
//==============================================================================
// 
//=======================================
void SetReleValue(Byte Data)
{  
  for(Byte i=0; i < 4;i++)
  {
    if(Data & (1 << i))
      PC_ODR |= (BIT7 >> i);
    else
      PC_ODR &= ~(BIT7 >> i);
  }
  
  //PC_ODR = BIT4+BIT5+BIT6+BIT7;
}
//=======================================
float GetSimpleTempValue(DWord* ADCMEM)
{
  float U = (5.0* (*ADCMEM))/(1023.0*1024.0);

  
  if(U > 4.99)
    U = 4.99;
  else if(U < 0.01)
    U = 0.01;

  
      float R = U/((5.0-U)/RESISTOR);
      float d = (R-1000.0)/7.9;
      if(d > 300)
        return 199.0;
      return 25.0+d;
}
//=======================================
sWord HexToByte(Byte *ptr)
{
   Byte ch  = *ptr++;
   Byte k   = 0;
   Byte result[2] = {0,0};


  while(k < 2)
  {
    if( (ch >= 'A') && (ch <= 'F') )
    {  result[k] = (ch - 'A') + 10; }
    else if( (ch >= '0') && (ch <= '9') )
    {  result[k] = ch - '0';}
    else
    {  return -1;}
    ch = *ptr;
    k++;
  }
  return (result[0] <<= 4) + result[1];
}
//=======================================
Byte CheckSum(Byte *BUF, Word Len)
{
  int ires = 0;
  for (int i = 0; i < Len; i+=2){
    ires += HexToByte(&BUF[i]);
  }
  return (0x100-ires);
}
//=======================================
void ByteToHex(Byte *HEX, Byte BYTE)
{
 Byte ch = (BYTE >> 4) & 0x0F;
 Byte k  = 0;
   while(k < 2)
   {
     if(ch > 9)
      *HEX++ = ch + 'A' - 10;
     else
       *HEX++ = ch + '0';
     ch = BYTE & 0x0F;
     k++;
   }
   HEX = NULL;
}
//=======================================
/*bool ParsingAnswer( Byte *BUF, Word Len)
{
  if(Len > 10)
  {
    if(BUF[Len-2]==0x0D && BUF[Len-1]==0x0A)
    {
      int s = TBuffer::find_first_of(BUF, Len, STX);
      if(s >=0 && (Len-s) > 10)
      {
        Byte lrc = HexToByte(&BUF[Len-4]);
        Byte calcLrc = CheckSum(&BUF[s+1], Len-s-5);
	if(lrc == calcLrc)
          return true;
      }
    }
    return false;
  }
  else 
    return false;
}
*/
bool ParsingAnswer( Byte *BUF, Word Len)
{
  if(Len > 10)
  {
    if(BUF[Len-2]==0x0D && BUF[Len-1]==0x0A)
    {
      int s = TBuffer::find_first_of(BUF, Len, STX);
      if(s >=0 && (Len-s) > 10)
      {
        Byte addr = HexToByte(&BUF[s+1]);
        Byte needAddr = PtrToConstants->DATA[DEV_ADDR];
        if(addr != needAddr)
          return false;
        
        Byte lrc = HexToByte(&BUF[Len-4]);
        Byte calcLrc = CheckSum(&BUF[s+1], Len-s-5);
	if(lrc == calcLrc)
          return true;
      }
    }
    return false;
  }
  else 
    return false;
}

//==============================================================================
//==============================================================================
//==============================================================================