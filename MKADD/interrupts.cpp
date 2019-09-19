//=============================================================================
//=============================================================================
#pragma vector = TIM1_CAPCOM_CC1IF_vector
__interrupt void TIM1_CAPCOM_CC1IF_handler(void)
{
  // Проверка, что же вызвало прерывание
  if (TIM1_SR1_CC1IF == 1)
  {
    TIM1_SR1_CC1IF = 0;   // Очистка флага прерывания по обновлению
    TIM1_CCR1 += 20000;    /// 20000/2000000/1=0.01s
    Usart.DecTimer(RX_BYTE_TIMEOUT, 10);   
    if( InputTimer++ >= DISCRETE_INPUTS_TIM)
    {
      InputTimer = 0;
      Input_Ready = true;
    }
    
    if( RebootTimer++ >= 100) // 1s
    {
      RebootTimer = 0;
      //DWord CNT = PtrToConstants->DATA[REBOOT_CNT] < 12000 ? 12000 : PtrToConstants->DATA[REBOOT_CNT];
      DWord CNT = PtrToConstants->DATA[REBOOT_CNT];
      if(RebootCounter++ > CNT)
      {
        RebootCounter = 0;
        Soft_Reset();
      }
    }
  }  
}
//=============================================================================
// Обработчик прерываний по приему UART
//=============================================================================
#pragma vector=UART1_R_RXNE_vector 
__interrupt void uart_rx_interrupt(void)
{
  Usart.PushByteToDATA_Rx( UART1_DR );
  UART1_SR_RXNE = 0;
}
//=============================================================================
// Обработчик прерываний по отправке UART
//=============================================================================
/*
#pragma vector=UART1_T_TXE_vector 
__interrupt void uart_tx_interrupt(void)
{
  UART1_DR = Usart.DATA_Tx;
  UART1_CR2_TIEN = 0;
}*/
//=============================================================================
//=============================================================================
#pragma vector=ADC1_EOC_vector
__interrupt void ADC_ISR(void)
{
   static Word ADCRL;
   static Word ADCRH;  
   
    ADC_CSR_EOC = 0;
    ADCRL       = ADC_DRL;
    ADCRH       = ADC_DRH;
    Stop_ADC();
    
    AKB[ADC_Input].AKB+=((ADCRH & 0x03)<<8) | ADCRL;
    if(++ADC_Input >= MAX_ADC_INPUTS){
      ADC_Input = 0;
      ADC_Counter++;
    }
    if(ADC_Counter >= 1023)
    {  
          AKB[0].AKB_OUT = AKB[0].AKB; AKB[0].AKB=0;
          AKB[1].AKB_OUT = AKB[1].AKB; AKB[1].AKB=0;
          AKB[2].AKB_OUT = AKB[2].AKB; AKB[2].AKB=0;
          AKB[3].AKB_OUT = AKB[3].AKB; AKB[3].AKB=0;
          ADC_Ready   = true;          
          ADC_Counter = 0;
    }
    
    Start_ADC(ADC_Input);
}
//=============================================================================
//=============================================================================