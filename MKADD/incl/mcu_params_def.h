#ifndef IO_CFG_H
    #define IO_CFG_H


      #define  CRITICAL_SEC_ON  __disable_interrupt();
      #define  CRITICAL_SEC_OFF __enable_interrupt();
//= IO_CFG =============================================
      #define P1_PORT   P1OUT
      #define P1_DDR    P1DIR
      #define P1_PIN    P1IN
      #define P1_SEL    P1SEL
    
      #define P2_PORT   P2OUT
      #define P2_DDR    P2DIR
      #define P2_PIN    P2IN
      #define P2_SEL    P2SEL
    
      #define P3_PORT   P3OUT
      #define P3_DDR    P3DIR
      #define P3_PIN    P3IN
      #define P3_SEL    P3SEL
    
      #define P4_PORT   P4OUT
      #define P4_DDR    P4DIR
      #define P4_PIN    P4IN
      #define P4_SEL    P4SEL
    
      #define P5_PORT   P5OUT
      #define P5_DDR    P5DIR
      #define P5_PIN    P5IN
      #define P5_SEL    P5SEL
    
      #define P6_PORT   P6OUT
      #define P6_DDR    P6DIR
      #define P6_PIN    P6IN
      #define P6_SEL    P6SEL

      #define P7_PORT   P7OUT
      #define P7_DDR    P7DIR
      #define P7_PIN    P7IN
      #define P7_SEL    P7SEL

      #define P8_PORT   P8OUT
      #define P8_DDR    P8DIR
      #define P8_PIN    P8IN
      #define P8_SEL    P8SEL

      #define P9_PORT   P9OUT
      #define P9_DDR    P9DIR
      #define P9_PIN    P9IN
      #define P9_SEL    P9SEL

      #define P10_PORT   P10OUT
      #define P10_DDR    P10DIR
      #define P10_PIN    P10IN
      #define P10_SEL    P10SEL

//= OSC ================================================
      #define SMCLK    14745600
//= WatchDog Timer =====================================

    #define WDT_INT_VECTOR WDT_VECTOR
    /**** Макросы для WDT ****/
    #define CLEAR_WDT     WDTCTL =  WDT_MRST_32                            
    #define HOLD_WDT      WDTCTL = (WDTPW | WDTHOLD)  

//======= TIMER0 =======================================
      #define T0_IV              TAIV
      #define T0_INT1            0x02  	//Захват/сравнение 1  	TACCR1 CCIFG  	
      #define T0_INT2            0x04 	//Захват/сравнение 2 	TACCR2 CCIFG 	  
      #define T0_INT3            0x0A 	//Переполнение таймера 	TAIFG
//======= TIMER1 =======================================
      #define T_INT1            0x02  	//Захват/сравнение 1  	TBCCR1 CCIFG  	
      #define T_INT2            0x04 	//Захват/сравнение 2 	TBCCR2 CCIFG 	
      #define T_INT3            0x06 	//Захват/сравнение 3 	TBCCR3 CCIFG 	
      #define T_INT4            0x08 	//Захват/сравнение 4 	TBCCR4 CCIFG 	
      #define T_INT5            0x0A 	//Захват/сравнение 5 	TBCCR5 CCIFG
      #define T_INT6            0x0C 	//Захват/сравнение 6 	TBCCR6 CCIFG
      #define T_INT7            0x0E 	//Переполнение таймера 	
//======================================================
//======================================================

#endif//IO_CFG_H
