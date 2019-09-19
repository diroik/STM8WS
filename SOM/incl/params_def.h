#ifndef IO_CFG_H
    #define IO_CFG_H


      #define  CRITICAL_SEC_ON  __disable_interrupt();
      #define  CRITICAL_SEC_OFF __enable_interrupt();

    //= OSC ================================================
    #define F_CPU    2000000
    //= WatchDog Timer =====================================
    #define CLEAR_WDT IWDG_KR = 0xAA;                          
    #define HOLD_WDT 

/************************************************************
* STANDARD BITS
************************************************************/

      #define BIT0                (0x01u)
      #define BIT1                (0x02u)
      #define BIT2                (0x04u)
      #define BIT3                (0x08u)
      #define BIT4                (0x10u)
      #define BIT5                (0x20u)
      #define BIT6                (0x40u)
      #define BIT7                (0x80u)
/*
      #define BIT8                (0x0100u)
      #define BIT9                (0x0200u)
      #define BITA                (0x0400u)
      #define BITB                (0x0800u)
      #define BITC                (0x1000u)
      #define BITD                (0x2000u)
      #define BITE                (0x4000u)
      #define BITF                (0x8000u)*/

//= IO_CFG =============================================
      #define P1_PORT   PC_ODR
      #define P1_DDR    PC_DDR
      #define P1_PIN    PC_IDR
      #define P1_SEL    PC_CR1
      #define P1_SEL2   PC_CR2
    
      #define P2_PORT   PD_ODR
      #define P2_DDR    PD_DDR
      #define P3_PIN    PD_IDR
      #define P3_SEL    PD_CR1
      #define P4_SEL2   PD_CR2
  

//======= TIMER0 =======================================



      //#define T0_IV              TAIV
      #define T0_INT1            0x02  	//Захват/сравнение 1  	TACCR1 CCIFG  	
      #define T0_INT2            0x04 	//Захват/сравнение 2 	TACCR2 CCIFG 	  
      #define T0_INT3            0x0A 	//Переполнение таймера 	TAIFG
//======================================================

/*---------------------------------------------
 * Define NAME as an I/O 16 bit reg
 * Access of 16 bit reg:  NAME
 *--------------------------------------------*/
#define __IO_REG16(NAME, ADDRESS, ATTRIBUTE)              \
                  __near __no_init volatile ATTRIBUTE unsigned short NAME @ ADDRESS;


/* TIM1 capture/compare register 1 high */
__IO_REG16    (TIM1_CCR1,  0x5265, __READ_WRITE);


#endif//IO_CFG_H
