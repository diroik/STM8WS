//=============================================================================
//=============================================================================
#pragma vector = TIM1_CAPCOM_CC1IF_vector
__interrupt void TIM1_CAPCOM_CC1IF_handler(void)
{
  // Проверка, что же вызвало прерывание
  if (TIM1_SR1_CC1IF==1)
  {
    TIM1_SR1_CC1IF = 0;   // Очистка флага прерывания по обновлению
    TIM1_CCR1 += 20000;    /// 20000/2000000/1=0.01s //PROC_FREQ=2000000
    Usart.DecTimer(RX_BYTE_TIMEOUT, 10);   
    if( LedTimer++ >= 5)
    {
      LedTimer = 0;
      Led_Ready = true;
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
    
    AKB[0].AKB+=((ADCRH & 0x03)<<8) | ADCRL;
    if(ADC_Counter++ >= 1023){  
          AKB[0].AKB_OUT = AKB[0].AKB; AKB[0].AKB=0;
          ADC_Ready   = true;          
          ADC_Counter = 0;
    }
    
    Start_ADC();
}
//=============================================================================
//=============================================================================