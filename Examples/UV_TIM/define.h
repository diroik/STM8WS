#define F_CPU 4000000
#define __watchdog_reset(); IWDG_KR=0xAA;

#define BIN__N(x) (x) | x>>3 | x>>6 | x>>9
#define BIN__B(x) (x) & 0xf | (x)>>12 & 0xf0
#define BIN8(v) (BIN__B(BIN__N(0x##v)))

#define uchar unsigned char
#define uint unsigned int

#define _ithl(h,l,v) h=v>>8;l=v&0x00ff;
#define ITHL(x,v) _ithl(x,v)

#define _set(port,pin,reg) port##_##reg##|=(1<<pin)
#define SET(x,y) _set(x,y)
#define _res(port,pin,reg) port##_##reg##&=~(1<<pin)
#define RES(x,y) _res(x,y)
#define _cpl(port,pin,reg) port##_##reg##^=(1<<pin)
#define CPL(x,y) _cpl(x,y)


#define colraz 3
#define colvhd 3
#ifdef colvhd
#define TKUP timvhd[0]
#define TKSL timvhd[2]
#define TKDW timvhd[1]
#define TKUD timvhd[3]
#endif
#define TOPR 6

#define coltim 5
#define timdin tim1ms[0] //
#define timsec tim1ms[1] //
#define timprg tim1ms[2]//program
#define timsin tim1ms[3]//Sel_ind
#define timtst tim1ms[4]

#define OUT PA_ODR_bit.ODR3

#define CA &PC_ODR,5
#define CB &PB_ODR,4
#define CC &PC_ODR,6
#define CD &PD_ODR,1
#define CE &PD_ODR,2
#define CF &PC_ODR,4
#define CG &PC_ODR,3
#define CH &PD_ODR,3
#define C1 &PD_ODR,6
#define C2 &PD_ODR,5
#define C3 &PD_ODR,4
#define UP CA
#define DW CH
#define SL CC

#define TIM1_ARR  TIM1_ARRH,TIM1_ARRL
#define TIM1_CNTR  TIM1_CNTRH,TIM1_CNTRL