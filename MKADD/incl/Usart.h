#ifndef USART_H
#define USART_H

#include <Classes.h>

//=============================================================================

void uart_init(unsigned long baud_rate, unsigned long f_master);

void uart_tx_byte(unsigned char data);

//void uart_tx_data(unsigned char * data, unsigned char len);

//unsigned char uart_rx_byte();

bool uart_rx_byte(unsigned char *data);

void uart_rx_data(unsigned char * data, unsigned char len);




//=============================================================================
// TComPort declaration  Aaciaue eioa?oaeniue eeann
//==================================== 
class TComPort:public TTimer
{
public:
        TComPort();
        TComPort(Word IntBufSize);        
        
       ~TComPort();
      
	HANDLE          open(Port port, DWord baudRate, DWord ReadIntervalTimeout);

        //sWord           Recv(Byte *Buf, Word Len, Word MaxLen);
        sWord           RecvTo(Byte *Buf, Word MaxLen, Byte StopSymbol);
        sWord           Send(Byte *Buf, Word Len);  

        void            PushByteToDATA_Rx(Byte Data);
        Byte            DATA_Tx;
        //bool            RxFlag;
        //Byte            DATA_Rx;
           
protected:
        void            InitPort(DWord BAUDRATE);
        bool            SendByte(Byte byte);        
        bool            RecvByte(Byte *byte); 

        
        bool             DataReceiving;                           
        COMMTIMEOUTS     time_outs;
        Word             ByteIndex; 
        
        TFifo<Byte>      *DATA_Rx;
        
private:

};


//============================================================================
#endif