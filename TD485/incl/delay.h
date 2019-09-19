/* 
 * delay utilite for STM8 family
 * COSMIC and SDCC
 * Terentiev Oleg
 * t.oleg@ymail.com
 */

#ifndef _UTIL_DELAY_H_
#define _UTIL_DELAY_H_ 1
#include <Classes_def.h>
   
#define T_COUNT(x) (unsigned short)( (( F_CPU * x / 1000000UL )-10)/20)    
   
#pragma inline 
static void inline _delay_cycl( volatile unsigned short __ticks)
{
	//asm("nop"); 
     /* asm("label:nop\n"    
          "jra label");*/
  
  
	// ASM: ldw X, #tick; lab$: decw X; tnzw X; jrne lab$
        //      2c;                 1c;     2c    ; 1/2c   
        while ( --__ticks );

}             

#pragma inline
static inline void _delay_us( volatile const unsigned short __us )
{
	_delay_cycl(  T_COUNT(__us) );
}

#pragma inline
static inline void _delay_ms( volatile unsigned short __ms )
{
	while ( __ms-- )
	{
                CLEAR_WDT;
		_delay_us( 1000 );
	}
}


#endif

