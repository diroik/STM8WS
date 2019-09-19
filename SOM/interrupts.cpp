//=============================================================================
//=============================================================================
#pragma vector = TIM1_CAPCOM_CC1IF_vector
__interrupt void TIM1_CAPCOM_CC1IF_handler(void)
{
  // Проверка, что же вызвало прерывание
  if (TIM1_SR1_CC1IF==1)
  {
    TIM1_SR1_CC1IF = 0;   // Очистка флага прерывания по обновлению
    TIM1_CCR1 += 50000;    /// 50000/2000000=0.025s

    
   _cval = _cval << 1;
   _cval |= !(bool)( PC_IDR&BIT7 );
  
      switch(_cval)
      {
          case 0xFFFFFFFF:
              _butonValue = 1;
              break;
          case 0x00:
              _butonValue = 0;
              break;     
      }
  }  
}
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
    
    AKB[ADC_Input].AKB+=((ADCRH & 0x03)<<8) | ADCRL;;
    if(++ADC_Input >= MAX_ADC_INPUTS){
      ADC_Input = 0;
      ADC_Counter++;
    }
    if(ADC_Counter >= 1023){  
          AKB[0].AKB_OUT = AKB[0].AKB; AKB[0].AKB=0;
          AKB[1].AKB_OUT = AKB[1].AKB; AKB[1].AKB=0;
          AKB[2].AKB_OUT = AKB[2].AKB; AKB[2].AKB=0;
          ADC_Ready   = true;          
          ADC_Counter = 0;
    }
    
    Start_ADC(ADC_Input);
}
//=============================================================================
//=============================================================================