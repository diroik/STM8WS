#ifndef CLASSES_H
#define CLASSES_H
//=============================================================================
#include <init.h>
#include <type_define.h>
#include <Classes_def.h>
#include <Class_Tamplate.h>
#include <iostream>
#include <sstream>
//=================================================

  
class TBuffer;
//=============================================================================
// TObject declaration
//====================================
class TObject
{
public:
       static void   Save_ParamsInInfo(Data_Params  *Params); 
       //static void   Soft_Reset(void);
       
private: 
       static void  WriteByteToFlash(Byte data, Byte* Addr);
       static void  WriteBufToPage(Byte* PageAddr, Byte *Buf, Word BufLen); 
       static void  ClrFlashSegment(Byte* SegAddr);         
};
//=============================================================================
// TBuffer declaration
//====================================
class TBuffer
{
public:         
        static void   ClrBUF(Byte *BUF, Word Len);
        static sWord  HexToByte(Byte *ptr);
        static Word   HexToWord(Byte *ptr);
 
        static void   ByteToHex(Byte *HEX, Byte BYTE);
        static void   WordToHex(Byte *HEX, Word WORD);
        static int    find_first_of(Byte *BUFFER, Word Len, Byte simb);

        
        
private:
};
//=============================================================================
// TTimer declaration
//==================================== 
class TTimer
{ 
public:
  TTimer(Byte TimNum);
  ~TTimer();

  bool  inline  SetTimer(Byte TimNamber, DWord Value);
  void  inline  DecTimer(Byte TimNamber);
  void  inline  DecTimer(Byte TimNamber, Word cnt);
  sDWord inline GetTimValue(Byte TimNamber); 
  
  
private:
  sDWord* Timer;
  Byte    TimerCNT;
};
//=============================================================================
#endif
