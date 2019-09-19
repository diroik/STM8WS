//=============================================================================
#include <Classes.h>
//=================================================
using namespace constants;
using namespace SramVariables;
using namespace Classes_Def;
//=============================================================================
namespace BaseClasses
{  
//=============================================================================
//*****************************************************************************
//*** Class TObject
//*****************************************************************************
//=============================================================================
// Запись в память констант  
//=======================================
bool TObject::Save_ParamsInInfo(Data_Params  *Params)
{
//Перезапись всей страницы с временным(защитным) хранилищем в другой странице 
 Word         Len;
 Byte        *Tmp                  = (Byte *)Params;  //взяли указатель 
  
  Len = sizeof(*Params); 
  for(Word i=0; i<3; i++)
  {
    WriteBufToPage((Byte const*)&TMP_PARAMS, Tmp, Len-1 );  //записали во временную(защитную) память
    WriteBufToPage((Byte const*)&PARAMS    , Tmp, Len-1 );  //записали куда надо
  
    if(VerifyMemory((Byte const*)&PARAMS,Tmp, Len))
    {//удалили временную запись 
      ClrFlashSegment( (Byte const*)&TMP_PARAMS );
      return true; 
    }
    else
    { continue;}
  }
  return false;
}
//=============================================================================
//  
//=======================================
bool TObject::VerifyMemory(Byte const* DestAddr, Byte* ScrAddr,Word Len)
{
  for(Word i=0; i<Len; i++)
  {
    CLEAR_WDT;
    if(DestAddr[i]!=ScrAddr[i])
    { return false;}
  }
  return true; 
}
//=============================================================================
//  
//=======================================
bool TObject::RestoreMemParams(void)
{
 Word         Len;
 Data_Params  TmpParam            = *PtrToConstants;    //сохранили текущие значения параметров в оперативке
 Byte        *Tmp                 = (Byte *)&TmpParam;  //взяли указатель 
  
  Len = sizeof(TmpParam);
  
  for(Word i = 0;i<3;i++)
  {
    CLEAR_WDT;
    WriteBufToPage((Byte const*)&PARAMS    , Tmp, Len-1 );  //записали куда надо
    if(VerifyMemory((Byte const*)&PARAMS,Tmp, Len))
    {//удалили временную запись 
      ClrFlashSegment( (Byte const*)&TMP_PARAMS );
      return true; 
    }
    else
    { continue;}
  }
  return false;  
}
//=============================================================================
//  
//=======================================
void TObject::RestoreDefault(void)
{
  Word         Len;
  Data_Params  TmpParam;
  Byte        *Tmp             = (Byte *)&TmpParam;  //взяли указатель 
 
  CLEAR_WDT;
  
  Len = sizeof(TmpParam);
  WriteBufToPage((Byte const*)&PARAMS    , Tmp, Len-1 );  //записали куда надо
}
//=============================================================================
//  
//=======================================
void TObject::WriteBufToPage(Byte const* PageAddr, Byte *Buf, Word BufLen)
{
  ClrFlashSegment( PageAddr );
   for (Word i=0; i < BufLen; i++)
   {
      CLEAR_WDT;
      WriteByteToFlash(Buf[i],PageAddr++);
   }
   Byte tmp = 0xAA;
   //WriteByteToFlash(tmp,--PageAddr);
   WriteByteToFlash(tmp,PageAddr);
}
//=============================================================================
//  
//=======================================
void TObject::SimpleWriteBufToPage(Byte const* PageAddr, Byte *Buf, Word BufLen)
{
  ClrFlashSegment( PageAddr );
   for (Word i=0; i < BufLen; i++)
   {
      CLEAR_WDT;
      //if(i == 511)
      //{ CLEAR_WDT;}
      WriteByteToFlash(Buf[i],PageAddr++);
   }
}
//=============================================================================
//  
//=======================================
void  TObject::WriteHexBufToPage(Byte const* PageAddr, Byte *HexBuf, Word BufLen)
{
 Byte TmpMass[3] = {0,0,0};
 Byte TmpByte;
 DWord TmpWord = (DWord)PageAddr;
 
 if( TmpWord == 0xFE00)
 {
   Word const *TmpWordPtr = (Word const*)0xFFFE;
   TmpWord     = *TmpWordPtr;
   ClrFlashSegment( PageAddr );
   WriteWordToFlash(TmpWord, TmpWordPtr);
   BufLen -=2;
 }
 else
 {
    ClrFlashSegment( PageAddr );  
 }
  for (Word i=0; i < (BufLen/2); i++)
  {
    TmpMass[0]  = HexBuf[i*2]; 
    TmpMass[1]  = HexBuf[i*2+1];
    TmpByte     = TBuffer::HexToWord(TmpMass);
    WriteByteToFlash(TmpByte,PageAddr++);
    CLEAR_WDT;      
   }
}
//=============================================================================
//  
//=======================================
bool TObject::ClrFlashSegment(Byte const* SegAddr)
{  
  bool flag     = false;
  Word old_wdt  = WDTCTL;                     
  WDTCTL        = WDTPW + WDTHOLD;  
  
  Word old_SR   = __get_SR_register();
  if(old_SR & GIE)
  {flag = true;}
  __disable_interrupt(); 

 while(FCTL3&(1<<BUSY));
 if( FCTL3&LOCKA )
 {  FCTL3 = FWKEY | LOCKA;}// Очистка LOCK + LOCKA
 else
 {  FCTL3 = FWKEY;}       // Очистка LOCK
 FCTL1 = FWKEY | ERASE;         // Разрешение записи	

 *((Byte*)SegAddr)  = 0x00;    //Фиктивная запись, стирание S1
 FCTL3 = FWKEY | LOCKA | LOCK;          // Установка LOCK
 
 WDTCTL = WDTPW + ( old_wdt & 0x00ff );
  if(flag)
  {__enable_interrupt();}
  return true;  
}
//=============================================================================
//  
//=======================================
bool TObject::WriteByteToFlash(Byte data, Byte const* Addr)
{
  bool flag     = false;
  Word old_wdt  = WDTCTL;                     
  WDTCTL        = WDTPW + WDTHOLD;  
  
  Word old_SR   = __get_SR_register();
  if(old_SR & GIE)
  {flag = true;}
  __disable_interrupt(); 
 
 while(FCTL3&(1<<BUSY));                
 if( FCTL3&LOCKA )
 {  FCTL3 = FWKEY | LOCKA;}// Очистка LOCK + LOCKA
 else
 {  FCTL3 = FWKEY;}       // Очистка LOCK
 FCTL1 = FWKEY | WRT;           // Разрешение записи	
 *((Byte*)Addr)  = data;
 FCTL1 = FWKEY;                 // Выполнено. Очистка WRT
 FCTL3 = FWKEY | LOCKA | LOCK;          // Установка LOCK
 
 WDTCTL = WDTPW + ( old_wdt & 0x00ff );
  if(flag)
  {__enable_interrupt();}
  return true;
}
//=============================================================================
//  
//=======================================
bool TObject::WriteWordToFlash( Word data, Word const* Addr )
{
  bool flag     = false;
  Word old_wdt  = WDTCTL;                     
  WDTCTL        = WDTPW + WDTHOLD;  
  
  Word old_SR   = __get_SR_register();
  if(old_SR & GIE)
  {flag = true;}
  __disable_interrupt(); 

 while(FCTL3&(1<<BUSY)); 
 if( FCTL3&LOCKA )
 {  FCTL3 = FWKEY | LOCKA;}// Очистка LOCK + LOCKA
 else
 {  FCTL3 = FWKEY;}       // Очистка LOCK
 FCTL1 = FWKEY | WRT;           // Разрешение записи	
 *((Word*)Addr)  = data;
 FCTL1 = FWKEY;                 // Выполнено. Очистка WRT
 FCTL3 = FWKEY | LOCKA | LOCK;          // Установка LOCK
 
  WDTCTL = WDTPW + ( old_wdt & 0x00ff );
  if(flag)
  {__enable_interrupt();}   
    return true;
}
//=============================================================================
//  
//=======================================
bool TObject::WriteBlockToFlash( Word* data, Word const* Addr )
{
  Word old_wdt = WDTCTL;                     
  WDTCTL = WDTPW + WDTHOLD;             
  __disable_interrupt(); 
  Byte Len = 32;
  
 while(FCTL3&(1<<BUSY));
 if( FCTL3&LOCKA )
 {  FCTL3 = FWKEY | LOCKA;}// Очистка LOCK + LOCKA
 else
 {  FCTL3 = FWKEY;}       // Очистка LOCK
 FCTL1 = FWKEY | BLKWRT | WRT;           // Разрешение записи	
 while(Len--)
 {
    *((Word*)Addr++)  = *data++;
    *((Word*)Addr++)  = *data++;
     while(FCTL3&(1<<WAIT));
 }
 FCTL1 = FWKEY;                 // Выполнено. Очистка WRT
 FCTL3 = FWKEY | LOCKA | LOCK;          // Установка LOCK
 
  WDTCTL = WDTPW + ( old_wdt & 0x00ff );
  __enable_interrupt();   
    return true;
}
//=============================================================================
//  
//=======================================
#pragma inline
inline __noreturn void  TObject::Soft_Reset(void)//inline
{
    __disable_interrupt();
  //SYSCTL = 0;

  //TA1CTL  = 0;
  //TA1CCTL0 = 0;
  //TA1CCTL1 = 0;
  //TA1CCTL2 = 0;

  /*
  ADC12CTL0 = 0;
  ADC12CTL1 = 0;
  ADC12CTL2 = 0;
  ADC12IE   = 0;
  ADC12IFG  = 0;
  ADC12MCTL0  = 0;
  ADC12MCTL1  = 0;
  ADC12MCTL2  = 0;
  ADC12MCTL3  = 0;
  ADC12MCTL4  = 0;
  ADC12MCTL5  = 0;
  ADC12MCTL6  = 0;
  ADC12MCTL7  = 0;
  ADC12MCTL8  = 0;
  ADC12MCTL9  = 0;
  ADC12MCTL10  = 0;
  ADC12MCTL11  = 0;
  ADC12MCTL12  = 0;
  ADC12MCTL13  = 0;
  ADC12MCTL14  = 0;
  ADC12MCTL15  = 0;
*/
  //SFRIE1 &= ~OFIE;

  //WDTCTL = (WDTPW | WDTSSEL__SMCLK | WDTCNTCL_L | WDTIS__64); 
  while(1);
}
//=============================================================================
//  
//=======================================
#pragma inline
inline __noreturn void   TObject::DirectJump(Word Addr)
{
    //SYSCTL &= ~SYSRIVECT;
  //(*(void(**)())Addr)();  
  __disable_interrupt();
    HOLD_WDT;
  __set_R4_register(Addr);
  __asm("BR R4");
}
//*****************************************************************************
//*** Class TDispatcher
//*****************************************************************************
TDispatcher::TDispatcher()
{
  
}
//=============================================================================
//  
//=======================================
TDispatcher::~TDispatcher()
{
    PocessList.clear();
}
//=============================================================================
//  
//=======================================     
void TDispatcher::CreateProcess(PFV Process)
{
    PocessList.push_back(Process);
}  
//=============================================================================
//  
//=======================================
#pragma inline
inline void TDispatcher::Scheduler()
{
  for(Byte i = 0; i < PocessList.size(); i++)
  { 
    CLEAR_WDT;
    PocessList[i](NULL); 
  }
  //__low_power_mode_0(); 
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
//*****************************************************************************
//*** TTimer
//*****************************************************************************
//=============================================================================
// Constructor
//=======================================
TTimer::TTimer(Byte Timers):TimerCNT(Timers)
{
  //HOLD_WDT;
    Timer = new DWord[TimerCNT];
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
  bool flag     = false;
  Word old_SR   = __get_SR_register();
  if(old_SR & GIE)
  {flag = true;}
  __disable_interrupt(); 
bool state;
  if (TimNam<TimerCNT)
  {
    Timer[TimNam] = Value;
    state = true;
  }
  else
  {  state = false;}
  if(flag)
  {__enable_interrupt(); }
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
inline sDWord TTimer::GetTimValue(Byte TimNam)
{
  bool flag     = false;
  Word old_SR   = __get_SR_register();
  if(old_SR & GIE)
  {flag = true;}
  __disable_interrupt(); 
sDWord Val;
    if (TimNam>=TimerCNT)
    {  Val = -1;}
    else
    {  Val = Timer[TimNam];}
    if(flag)
    {__enable_interrupt();} 
return Val;
}
//=============================================================================
// 
//=======================================
#pragma inline
inline void TTimer::wait_1ms(Word cnt)
{
    Word i;

        HOLD_WDT;
	for (i = 0; i < cnt; i++)
        { 
          __delay_cycles(1170);
        }
        CLEAR_WDT;
}
//=============================================================================
// 
//=======================================
#pragma inline
inline void TTimer::wait_1us(Word cnt)
{
        // HOLD_WDT;
	for (Word i = 0; i < cnt; i++)
        { 
          CLEAR_WDT;
          __delay_cycles(8);
          //__delay_cycles(8);//Было 8 но после добавления CLEAR_WDT; 500 мс превратилось в 610...
          //__delay_cycles(9);
        }
        CLEAR_WDT;
}
//=============================================================================  
//*****************************************************************************
}//namespace
//*****************************************************************************
//*****************************************************************************