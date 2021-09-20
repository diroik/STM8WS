//=============================================================================
#include <Usart.h>
//=================================================
//******************************************************************************
// Инициализация UART1 STM8S003
//******************************************************************************
void uart_init(unsigned long baud_rate, unsigned long f_master)
{
  
  /*
  //Значение регистра BRR
  unsigned long brr;
   
  //Настраиваем TX на выход, а RX на вход
  PD_DDR_bit.DDR5 = 1;  //TX
  PD_DDR_bit.DDR6 = 0;  //RX
   
  //RX - плавающий вход
  PD_CR1_bit.C16 = 0;
  //Отключает внешние прерывания для RX
  PD_CR2_bit.C26 = 0;
   
  //Настройка скорости передачи
  brr = f_master/baud_rate;
     
  UART1_BRR2 = brr & 0x000F;
  UART1_BRR2 |= brr >> 12;
  UART1_BRR1 = (brr >> 4) & 0x00FF;
   
  //Четность отключена
  UART1_CR1_PIEN = 0;
  //Контроль четности отключен
  UART1_CR1_PCEN = 0;
  //8-битный режим
  UART1_CR1_M = 0;
  //Включить UART
  UART1_CR1_UART0 = 0;
   

  //Запретить прерывание по завершению передачи
  UART1_CR2_TCIEN = 0;
  
  //Запретить прерывание по опустошению передающ. регистра
  UART1_CR2_TIEN = 0;//!!
  
  //Разрешить прерывание по заполнению приемного регистра
  UART1_CR2_RIEN = 1; //!!!
  
  //Запретить прерывание по освобождению линии
  UART1_CR2_ILIEN = 0;
  
  //Передатчик включить
  UART1_CR2_TEN = 1;
  //Приемник включить
  UART1_CR2_REN = 1;
  
  
  //Не посылать break-символ
  //UART1_CR2_SBK = 0;
   
  //Один стоп-бит
  UART1_CR3_STOP = 0;*/
  
    unsigned char tmp = UART1_SR;
    tmp = UART1_DR;
    //  Reset the UART registers to the reset values.
    UART1_CR1 = 0;
    UART1_CR2 = 0;
    UART1_CR4 = 0;
    UART1_CR3 = 0;
    UART1_CR5 = 0;
    UART1_GTR = 0;
    UART1_PSCR = 0;

    UART1_CR1_M    = 0;         //   8 Data bits.
    UART1_CR1_PCEN = 0;         //   set no parity.
    UART1_CR1_PS   = 0;         //   if parity = set parity type
    UART1_CR3      = 0;        //   1 stop bits     

      //Настройка скорости передачи
    unsigned long brr = f_master/baud_rate;
    UART1_BRR2 = brr & 0x000F;
    UART1_BRR2 |= brr >> 12;
    UART1_BRR1 = (brr >> 4) & 0x00FF;
 
    //  Disable the transmitter and receiver.
    UART1_CR2_TEN   = 0;        //  Disable transmit.
    UART1_CR2_REN   = 0;        //  Disable receive.
    //  Set the clock polarity, clock phase and last bit clock pulse.
    UART1_CR3_CPOL  = 0;
    UART1_CR3_CPHA  = 0;
    UART1_CR3_LBCL  = 0;
    //  Set the Receive Interrupt RM0016 p358,362
    UART1_CR2_TIEN  = 0;     // Transmitter interrupt enable
    UART1_CR2_TCIEN = 0;     // Transmission complete interrupt enable
    UART1_CR2_RIEN  = 1;     //  Receiver interrupt enable
    UART1_CR2_ILIEN = 0;     //  IDLE Line interrupt enable

    PD_DDR_bit.DDR5 = 1;  //TX
    PD_DDR_bit.DDR6 = 0;  //RX

    //  Turn on the UART transmit, receive and the UART clock.
    UART1_CR2_TEN    = 1;
    UART1_CR2_REN    = 1;
    UART1_CR1_PIEN   = 0;
    UART1_CR4_LBDIEN = 0;
}
//******************************************************************************
// Отправка байта
//******************************************************************************
void uart_tx_byte(unsigned char data)
{
  while(!UART1_SR_TXE)
  {
    ///CLEAR_WDT;
  };
  UART1_DR = data;
}
//******************************************************************************
// Отправка массива данных
//******************************************************************************
void uart_tx_data(unsigned char * data, unsigned char len){
  while(len--)
  {
      CLEAR_WDT;
      uart_tx_byte(*data++);
  }
}
//******************************************************************************
// Прием байта
//******************************************************************************
/*
unsigned char uart_rx_byte()
{
    unsigned char data;
       
    while(!UART1_SR_RXNE)
    {
      CLEAR_WDT;
    };
    data = UART1_DR;
    return data;
}
*/
//******************************************************************************
// Прием байта
//******************************************************************************
bool uart_rx_byte(unsigned char *data){
    if(UART1_SR_RXNE){
      *data = UART1_DR;
      return true;
    }
    else
      return false;
}
//******************************************************************************
// Прием массива данных
//******************************************************************************
/*
void uart_rx_data(unsigned char * data, unsigned char len){
  while(len--){
      *data++ = uart_rx_byte();
  }
}*/
//*****************************************************************************
//*** Class TComPort
//*****************************************************************************
TComPort::TComPort():TTimer(2),ByteIndex(0),DataReceiving(false)
{
  //RxFlag = false;
  DATA_Rx = NULL;
}
//*****************************************************************************
//*** Class TComPort
//*****************************************************************************
TComPort::TComPort(Word IntBufSize):TTimer(1),ByteIndex(0),DataReceiving(false)
{
  //RxFlag = false;
  DATA_Rx = new TFifo<Byte>(IntBufSize);
}
//=============================================================================
// Destructor
//=======================================
TComPort::~TComPort()
{

}
//=============================================================================
// open com port 
//=======================================
HANDLE TComPort::open(Port port, DWord baudRate, DWord ReadIntervalTimeout)
{
  uart_init(baudRate, PROC_FREQ);
  time_outs.ReadIntervalTimeout    = ReadIntervalTimeout;
  return port; 
}
//=============================================================================
//
//=======================================
void TComPort::PushByteToDATA_Rx(Byte Data)
{
  if(DATA_Rx!=NULL)
  {
    DATA_Rx->push(Data);
  }
}
//=============================================================================
// 
//=======================================
#pragma inline
inline bool TComPort::RecvByte(Byte *data)
{
  if ( DATA_Rx == NULL || DATA_Rx->empty() )
  {  return false;}
  else
  {
    *data = DATA_Rx->pop();
    SetTimer(RX_BYTE_TIMEOUT, time_outs.ReadIntervalTimeout);
    return true;
  }  
}
//=============================================================================
// 
//=======================================
#pragma inline
inline bool TComPort::SendByte(Byte data)
{    
       uart_tx_byte(data);
        return true;
}
//=============================================================================
//
//=======================================
sWord   TComPort::Send(Byte *Buf, Word Len)
{
  if (Len > 0)
  {
     while( SendByte(Buf[ByteIndex]) )
     {  
       ///CLEAR_WDT;
       while( !UART1_SR_TC ){  
         ///CLEAR_WDT;
       }       
       if ( ++ByteIndex >= Len )
       {     
         DataReceiving = false;
         ByteIndex = 0;
         return Len;
       }    
     };
  }
  return Len;
}
//=============================================================================
// 
//=======================================
sWord TComPort::RecvTo(Byte *Buf, Word MaxLen, Byte StopSymbol)
{
      Byte data;
      if(DataReceiving == false)
      {
         DataReceiving = true;
         SetTimer(RX_BYTE_TIMEOUT,time_outs.ReadIntervalTimeout);
         ByteIndex = 0;
      }
      bool ext_flg = false;
      while( RecvByte(&data) )
      {
        CLEAR_WDT;
        Buf[ByteIndex++] = data;
        if( ByteIndex >= MaxLen ){ 
          ext_flg = true;
        }

        if(data == StopSymbol || ext_flg)
        {
             Word TmpWord = ByteIndex;
             ByteIndex       = 0x00;
             DataReceiving = false;
             return TmpWord;
        }          
      };
      if(DataReceiving == true)
      { 
           if ( GetTimValue(RX_BYTE_TIMEOUT) <= 0) //
           { 
             if(ByteIndex > 0){
               sWord t = GetTimValue(RX_BYTE_TIMEOUT);
               ByteIndex = ByteIndex;
               t = 0;
             }
             Word TmpWord = ByteIndex;
             ByteIndex       = 0x00;
             DataReceiving = false;
             return TmpWord;
           }  
      }
      return -1;    
}
//=================================================
// Usart.cpp
//=============================================================================