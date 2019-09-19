#ifndef CLASSES_H
#define CLASSES_H
//=============================================================================
#include <init.h>
#include <type_define.h>
#include <Classes_def.h>
#include <stdio.h>
//=================================================
namespace BaseClasses
{  
  class TBuffer;
//=============================================================================
// TObject declaration
//====================================
class TObject
{
public:

       static bool   Save_ParamsInInfo(Data_Params  *Params);
       static bool   VerifyMemory(Byte const* DestAddr, Byte* ScrAddr,Word Len); 

       static bool   RestoreMemParams(void); 
       static void   RestoreDefault(void);  
       
       static bool   WriteByteToFlash(Byte data, Byte const* Addr);
       static bool   WriteWordToFlash( Word data, Word const* Addr );
       static bool   WriteBlockToFlash( Word* data, Word const* Addr );      
       
       static void   Soft_Reset(void);
       static void   DirectJump(Word Addr);
       
       static const  Data_Params *PtrToConstants;
       
       static void   WriteHexBufToPage(Byte const* PageAddr,              Byte *HexBuf, Word BufLen);
       static void  SimpleWriteBufToPage(Byte const *PageAddr, Byte *Buf, Word BufLen);              
       
private: 
       static void  WriteBufToPage(Byte const* PageAddr, Byte *Buf, Word BufLen); 
       static bool  ClrFlashSegment(Byte const* SegAddr);         
};
Data_Params const *TObject::PtrToConstants;
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
        
private:
};
//=============================================================================
//=============================================================================
//=============================================================================
}//namespace
#endif
