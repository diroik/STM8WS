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
    //Init_Flash_Const();
    Init_Discrete();
}
//=================================================
//======= Функции инициализации ===================
//=================================================
    //PB_DDR = 0xff;          //  All pins are outputs.
    //PB_CR1 = 0xff;          //  Push-Pull outputs.
    //PB_ODR = 0xff;          //  All pins are turned off.
    //PB_CR2 = 0xff;          //  Output speeds up to 10 MHz.
//=================================================
#pragma inline
inline void Init_Ports(void)
{
  //PC_DDR_bit.DDR6 = 1;   // Ножка PD0 конфигурируется на вывод
  //PC_CR1_bit.C16 = 1;
  //PC_ODR_bit.ODR6 = 1;
  
  // input
  PA_DDR &= ~(BIT1+BIT3);
  PB_DDR &= ~(BIT4+BIT5+BIT6+BIT7);
  PC_DDR &= ~(BIT1+BIT2+BIT3);
  PD_DDR &= ~(BIT0+BIT2+BIT3+BIT4+BIT7);
  PE_DDR &= ~(BIT5);
  PF_DDR &= ~(BIT4);
  
  // output
  PC_DDR = BIT4+BIT5+BIT6+BIT7;
  PC_CR1 = BIT4+BIT5+BIT6+BIT7; //подтяжка к верху (обязательно!!)
  

    
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
  IWDG_PR = 0x6; //IWDG_PR_256 
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
    
    // Предделитель -  fCK_CNT = fCK_PSC / (PSCR[15:0]+1)
    // ВАЖНО!!!
    // Порядок установки предделителя - старший регистр, потом младший
    //fCK_CNT = fCK_PSC / (PSCR[15:0]+1)
    TIM1_PSCRH = 0;
    TIM1_PSCRL = 0;

    TIM1_CCR1 = 50000;// 
    //TIM1_CCR1 = 50000;// 50000/2000000=0.025s
    
  // Режим непрерывного счета по возрастанию
  // Прерывание по переполнению разрешено и таймер запущен
    TIM1_CR1 = (MASK_TIM1_CR1_CEN);
 
}
//=================================================
#pragma inline
inline void Init_ADC(void)
{  
  ADC_CSR_EOCIE         = 1;    // ADC Interrupt Enable
  ADC_CSR_CH            = 0;    // Channel 0
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
  ADC_CSR_CH            = index;    // Channel 4
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
    IWDG_PR  = 1;//0
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
#pragma inline
inline void Init_Discrete(void)
{
  DISCRETE[0].REG = &PE_IDR; DISCRETE[0].BIT = BIT5;
  DISCRETE[1].REG = &PC_IDR; DISCRETE[1].BIT = BIT1;
  DISCRETE[2].REG = &PC_IDR; DISCRETE[2].BIT = BIT2;
  DISCRETE[3].REG = &PC_IDR; DISCRETE[3].BIT = BIT3;
  DISCRETE[4].REG = &PB_IDR; DISCRETE[4].BIT = BIT7;
  DISCRETE[5].REG = &PB_IDR; DISCRETE[5].BIT = BIT6;
  DISCRETE[6].REG = &PB_IDR; DISCRETE[6].BIT = BIT5;
  DISCRETE[7].REG = &PB_IDR; DISCRETE[7].BIT = BIT4;

  DISCRETE[8].REG = &PD_IDR; DISCRETE[8].BIT = BIT0;
  DISCRETE[9].REG = &PD_IDR; DISCRETE[9].BIT = BIT2;
  DISCRETE[10].REG = &PD_IDR; DISCRETE[10].BIT = BIT3;
  DISCRETE[11].REG = &PD_IDR; DISCRETE[11].BIT = BIT4;
  DISCRETE[12].REG = &PD_IDR; DISCRETE[12].BIT = BIT7;
  DISCRETE[13].REG = &PA_IDR; DISCRETE[13].BIT = BIT1;
  DISCRETE[14].REG = &PA_IDR; DISCRETE[14].BIT = BIT3;
  DISCRETE[15].REG = &PF_IDR; DISCRETE[15].BIT = BIT4;
}

//=================================================
#endif
