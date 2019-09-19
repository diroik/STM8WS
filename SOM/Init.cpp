#ifndef INIT_CPP
    #define INIT_CPP
// *************************************************             
// Инициализация                            
// *************************************************
//=================================================
#include <init.h>
#include <flash_map.h>
//=================================================
#pragma inline
inline void SYSTEM_INIT(void)
{ 
    Init_Ports();
    Init_OSC();
    
    //_delay_ms(500);
      
    Init_Flash_Const();
    Init_TIMER();
    Init_WDT();
    Init_ADC();
    Init_Flash_Const();
}
//=================================================
//======= Функции инициализации ===================
//=================================================
#pragma inline
inline void Init_Ports(void)
{
  //PC_DDR_bit.DDR6 = 1;   // Ножка PD0 конфигурируется на вывод
  //PC_CR1_bit.C16 = 1;
  
  PA_DDR &= ~(BIT1+BIT2+BIT3);//PA3 input
  PD_DDR &= ~BIT4;//PD4 input
  
  PC_DDR = BIT3 + BIT4 + BIT5 + BIT6;
  PC_CR1 = BIT3 + BIT4 + BIT5 + BIT6;
 
  //PC_DDR_bit.DDR6 = 1;
  //PC_CR1_bit.C16 = 1;
  //PC_ODR_bit.ODR6 = 1;
  
    //PB_DDR = 0xff;          //  All pins are outputs.
    //PB_CR1 = 0xff;          //  Push-Pull outputs.
    //PB_ODR = 0xff;             //  All pins are turned off.
    //PB_CR2 = 0xff;          //  Output speeds up to 10 MHz.
    
}
//=================================================
#pragma inline
inline void Init_OSC(void)// тактовой частоты                     
{  
 // After reset, the microcontroller restarts by default with an internal 2 MHz clock (HSI/8). 
    //CLK_ICKR = 0;                       //  Reset the Internal Clock Register.
    //CLK_ICKR_HSIEN = 1;                 //  Enable the HSI.
    //CLK_ECKR = 0;                       //  Disable the external clock.
    //while (CLK_ICKR_HSIRDY == 0);       //  Wait for the HSI to be ready for use.
    //CLK_CKDIVR = 0;                     //  Ensure the clocks are running at full speed.
    //CLK_PCKENR1 = 0xFF;                 //  Enable all peripheral clocks.
    //CLK_PCKENR2 = 0xFF;                 //  
    //CLK_CCOR = 0;                       //  Turn off CCO.
    //CLK_HSITRIMR = 0;                   //  Turn off any HSIU trimming.
    //CLK_SWIMCCR = 0;                    //  Set SWIM to run at clock / 2.
    //CLK_SWR = 0xe1;                     //  Use HSI as the clock source.
    //CLK_SWCR = 0;                       //  Reset the clock switch control register.
    //CLK_SWCR_SWEN = 1;                  //  Enable switching.
    //while (CLK_SWCR_SWBSY != 0);        //  Pause while the clock switch is busy.
} 
//=================================================
#pragma inline
void Init_WDT()
{
  IWDG_KR = 0xCC; // key_enable IWDG (Watchdog Timer)
  IWDG_KR = 0x55; // key_access to IWDG_PR and IWDG_RLR (config prescaler)
  IWDG_PR = 6;
  IWDG_RLR = 0xFF;
}

//=================================================
#pragma inline
inline void Init_TIMER(void)
{
  //TIM1_CR1    //Control register 1. Регистр управления 1. В нем находятся основные конфигурационные биты 
  //TIM1_IER    //Interrupt enable register. Регистр разрешения прерываний.
  
    //Настройка Таймера1
    TIM1_CR2  = 0;       // Синхронизация как ведущий с периферией отключена
    TIM1_SMCR = 0;      // Синхронизация как ведомый с периферией отключена
    TIM1_ETR  = 0;       // Внешнее тактирование отключено

    TIM1_IER = MASK_TIM1_IER_CC1IE;// Прерывание по обновлению счетного регистра разрешено
    //TIM1_IER = MASK_TIM1_IER_UIE;// Прерывание по обновлению счетного регистра разрешено
    
    // Предделитель - 0
    // ВАЖНО!!!
    // Порядок установки предделителя - старший регистр, потом младший
    TIM1_PSCRH = 0;
    TIM1_PSCRL = 3;

    TIM1_CCR1 = 50000;// 50000/2000000=0.025s
    
  // Режим непрерывного счета по возрастанию
  // Прерывание по переполнению разрешено и таймер запущен
    TIM1_CR1 = (MASK_TIM1_CR1_CEN);
 
}
//=================================================
#pragma inline
inline void Init_ADC(void)
{  
  ADC_CSR_EOCIE         = 1;    // ADC Interrupt Enable
  ADC_CSR_CH            = 4;    // Channel 4
  ADC_CR1_SPSEL         = 7;    // Fmaster/18
  ADC_CR1_CONT          = 0;    // Single Conversion
  ADC_CR2_ALIGN         = 1;    // Right Alignment
  //ADC_CR1_ADON        = 1;    // ADC ON
}
//=================================================
#pragma inline
inline void Start_ADC(Byte index)
{  
  
  ADC_CSR_EOCIE         = 1;    // ADC Interrupt Enable
  ADC_CSR_CH            = 4+index;    // Channel 4
  ADC_CR1_SPSEL         = 7;    // Fmaster/18
  ADC_CR1_CONT          = 0;    // Single Conversion
  ADC_CR2_ALIGN         = 1;    // Right Alignment
  
  ADC_CR1_ADON = 1;
  ADC_CR1_ADON = 1;
}
//=================================================
#pragma inline
inline void Stop_ADC(void)
{
  ADC_CR1_ADON = 0;
}
//=================================================
__noreturn void  Soft_Reset(void)//inline
{
    __disable_interrupt();

    IWDG_KR = 0xCC; // key_enable IWDG (Watchdog Timer)
    IWDG_KR = 0x55; // key_access to IWDG_PR and IWDG_RLR (config prescaler)
    IWDG_PR  = 0;
    IWDG_RLR = 0;
    
    while(1);
}
//=================================================
#pragma inline
inline void Init_Flash_Const(void)
{
   PtrToConstants = (Data_Params const*)&PARAMS; 
}
//=================================================
//=================================================
#endif
