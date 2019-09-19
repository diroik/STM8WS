//=============================================================================
#include <Classes.h>
//=============================================================================
//*****************************************************************************
//*** Class TObject
//*****************************************************************************
//=============================================================================
// Запись в память констант  
//=======================================
void TObject::Save_ParamsInInfo(Data_Params  *Params)
{
  WriteBufToPage((Byte *)&PARAMS, (Byte *)Params, sizeof(*Params)-1 ); 
}
//=============================================================================
//  
//=======================================
void TObject::WriteBufToPage(Byte* PageAddr, Byte *Buf, Word BufLen)
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
//=============================================================================
//  
//=======================================
void TObject::ClrFlashSegment(Byte* SegAddr)
{  
 
}
//=============================================================================
//  
//=======================================
void TObject::WriteByteToFlash(Byte data, Byte* Addr)
{
  FLASH_DUKR = 0xAE;
  FLASH_DUKR = 0x56; 
  
  *Addr = data;
}
//=============================================================================
//  
//=======================================
#pragma inline
inline __noreturn void  TObject::Soft_Reset(void)//inline
{
    __disable_interrupt();
    while(1);
}
//*****************************************************************************
//***TBuffer
//****************************************************************************
//=============================================================================
//
//=======================================
void   TBuffer::ClrBUF(Byte *BUF, Word Len)
{
  for (Word i=0;i<Len;i++)
  {
    CLEAR_WDT;
    BUF[i] = 0;
  }
}
//=============================================================================
//
//=======================================
Word TBuffer::HexToWord(Byte *ptr)
{
  Word number = 0;
    
  sscanf((char*)ptr, "%X", &number);
  return number;
}
//=============================================================================
//
//=======================================
sWord TBuffer::HexToByte(Byte *ptr)
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
//=============================================================================
//
//=======================================
void TBuffer::ByteToHex(Byte *HEX, Byte BYTE)
{
 Byte ch = (BYTE >> 4) & 0x0F;
 Byte k  = 0;
  
   while(k < 2)
   {
     if(ch > 9)
     {  *HEX++ = ch + 'A' - 10;}  
     else 
     {  *HEX++ = ch + '0';}
  
     ch = BYTE & 0x0F;
     k++;
   }
   //HEX = (Byte*)NULL;
}
//=============================================================================
//
//=======================================
void TBuffer::WordToHex(Byte *HEX, Word WORD)
{
  ByteToHex(HEX, (Byte)( WORD >> 8) );
  ByteToHex(&HEX[2], (Byte)( WORD) );
}
//=============================================================================
//
//=======================================
int  TBuffer::find_first_of(Byte *BUFFER, Word Len, Byte simb)
{
  int returnVal = -1;

  for(Word i=0; i < Len; i++)
  {
    if(BUFFER[i] == simb)
    {
      returnVal = i;
      break;
    }
  }

return returnVal;
}

//=============================================================================  
//*****************************************************************************
//*** TTimer
//*****************************************************************************
//=============================================================================
// Constructor
//=======================================
TTimer::TTimer(Byte Timers):TimerCNT(Timers)
{
  //HOLD_WDT;
    Timer = new sDWord[TimerCNT];
    for(Word i = 0; i < TimerCNT; i++)
    { 
      CLEAR_WDT;
      SetTimer(i,0);
    }
} 
//=============================================================================
// Destructor
//======================================= 
TTimer::~TTimer()
{
    delete[] Timer;
}
//=============================================================================
// 
//=======================================  
#pragma inline
inline bool TTimer::SetTimer(Byte TimNam, DWord Value)
{ 
  bool state = false;
  if (TimNam<TimerCNT){
    Timer[TimNam] = Value;
    state = true;
  }

return state;
}
//=============================================================================
// 
//=======================================  
#pragma inline
inline void TTimer::DecTimer(Byte TimNam)
{
  if (TimNam >= TimerCNT)//debug
  {  return;}
  if (Timer[TimNam] > 0)
  {  Timer[TimNam]--;}
}
//=============================================================================
// 
//=======================================  
#pragma inline
inline void TTimer::DecTimer(Byte TimNam, Word cnt)
{
  if (TimNam >= TimerCNT)
  {  return;}
  
  if (Timer[TimNam] > 0){  
    Timer[TimNam]-=cnt;
  }
  
  if( Timer[TimNam] < 0 ){
    Timer[TimNam] = 0;
  }
  /*
  if (Timer[TimNam] > cnt){  
    Timer[TimNam]-=cnt;
  }else{
    Timer[TimNam] = 0;
  }*/
}
//=============================================================================
// 
//=======================================
#pragma inline
inline sDWord TTimer::GetTimValue(Byte TimNam)
{

    sDWord Val = Timer[TimNam];
    if (TimNam >= TimerCNT)
      Val = -1;

return Val;
}
//=============================================================================




//*****************************************************************************
//*****************************************************************************