#include <iostm8s103f3.h>
#include <intrinsics.h>
#include "define.h"
#include "znggen.h"
#include "dinind.h"

volatile uint tim1ms[coltim];  //Массив таймеров
uint TIMER,MTIME,INDEX;
volatile uchar RUN;
extern uchar timvhd[colvhd+1];    //Массив входных сигналов
extern uchar AZN[colraz+1];
extern uchar BOOST;
#pragma location=0x4000 //адрес в eeprom
__no_init uchar MSEC[450];
#pragma location=0x41C8 //адрес в eepromh
__no_init uchar HOUR;
#pragma location=0x41D0 //адрес в eeprom
__no_init uint MARKER;
 #pragma location=0x4803 //адрес в eeprom (в данном случае - OPT)
__no_init uchar OPT2;
 #pragma location=0x4804 
__no_init uchar NOPT2;
volatile uint itst;

void ticsec (void);
void selind (void);
void settim (void);
void restim (void);
void init_time (void);
uchar readbin (uint i);
//////////////////////////////////////////////////////////////////
#pragma vector=TIM1_OVR_UIF_vector
__interrupt void TIM1OVR(void)
{
  //if(RUN!=0){timsec=1;}
  timsec=1;
  TIM1_SR1_bit.UIF=0;//Сброс флага
}
///////////////////////////////////////////////////////////////////
#pragma vector=TIM4_OVR_UIF_vector 
__interrupt void TIM4OVR(void)
{ uchar c;
  for(c=0;c<coltim;c++)
  {
    if(tim1ms[c]>1)tim1ms[c]--;
  }
  TIM4_SR_bit.UIF=0;//Сброс флага
}
////////////////////////////////////////////////////////////////////////////////
void ticsec (void)
{
  uchar r;
    if(RUN)
    {
       MTIME=(HOUR*3600)+(INDEX<<3)+readbin(INDEX);
       if(MTIME==MARKER){restim();}
       else
       {
        r=~MSEC[INDEX];
          if(r<BIN8(10000000))r<<=1;
          else
          {
            r=1;
             MSEC[INDEX]=0xFF;
              if(INDEX<449)INDEX++;
              else
              {
               INDEX=0;
               HOUR++;
              }
          }
        MSEC[INDEX]=~r; 
        AZN[0]=SM_CLR;AZN[1]=SM_CLR;AZN[2]=SM_CLR;
       }
    }
  timsin=251; 
  timsec=0;
}
////////////////////////////////////////////////////////////////////////////////
void selind (void)
{
  uint time;
  uchar min,sec;
    if(RUN==0)time=TIMER;
    else
    {
      if(MARKER>MTIME)time=MARKER-MTIME;
      else time=MTIME-MARKER;
    }
      if(time)
      {
       min=time/60;
       sec=time-(min*60);
       if(min>99) conv10_2(min,1);
       else if(min<10) conv10_2(((min*100)+sec),3);
       else conv10_2(((min*10)+(sec/10)),2);
      }
      else
      {
        AZN[2]=SM_E;AZN[1]=SM_n;AZN[0]=SM_d;
      }
   timsin=0;
}
////////////////////////////////////////////////////////////////////////////////
void settim (void)
{
 if(TIMER)
 {
  MARKER=MTIME+TIMER;
  ITHL(TIM1_CNTR,100)
  TIM1_SR1_bit.UIF=1;
  timsec=0;
  RUN=0xFF;
  TIMER=0;
  OUT=0;
 }
 else
 {
   RUN=0;OUT=0;
      if(MARKER>MTIME)TIMER=MARKER-MTIME;
      else TIMER=MTIME-MARKER;
 }
}
////////////////////////////////////////////////////////////////////////////////
void restim (void)
{
  if(RUN)
  {
    MTIME=((HOUR*3600)+(INDEX<<3)+readbin(INDEX));
    MARKER=MTIME;
    RUN=0;
    OUT=1;
  }
  else TIMER=0;
  timsin=1;
}
////////////////////////////////////////////////////////////////////////////////
void init_time (void)
{
  INDEX=0;
   while(INDEX<450)
   {
     if(MSEC[INDEX]<0xFF) break;
     INDEX++;
   }
   if(INDEX==450){INDEX=0;MSEC[INDEX]=BIN8(11111110);}
   MTIME=((HOUR*3600)+(INDEX<<3)+readbin(INDEX));
   if(MTIME!=MARKER){RUN=0xFF;OUT=0;} 
}
////////////////////////////////////////////////////////////////////////////////
uchar readbin (uint i)
{
  uchar r,c=0;
 r=~MSEC[i]; 
  while(r>0)
  {
    r>>=1;
    c++;
  }
return c;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int main( void )
{
  
  FLASH_DUKR = 0xAE;//Разблокировка
  FLASH_DUKR = 0x56;// EEPROM and Options byte
  //FLASH_CR2_bit.OPT=1; //Разрешение записи Options byte
  //FLASH_NCR2_bit.NOPT=0; //Разрешение записи Options byte
  //OPT2|=(1<<0);
 // NOPT2&=~(1<<0); //C7 alternate function = TIM1_CH2.
  //FLASH_CR2_bit.OPT=0; //
  //FLASH_NCR2_bit.NOPT=1;
  CLK_PCKENR1=0;
  CLK_PCKENR2=0;
  //Инициализируем TIM1
  CLK_PCKENR1|=(1<<7);
  TIM1_CR1_bit.DIR=1; //down-counter
  TIM1_CR1_bit.ARPE=1;
  ITHL(TIM1_ARR,100);        //Регистр автозагрузки таймера при переполнении 10x100=1S
  ITHL(TIM1_CNTR,100); 
  TIM1_CCMR2_bit.CC2S=1;     //channel 2 to detect rising edges
  TIM1_CCMR2|=BIN8(01110000);//filter 7 takt
  TIM1_CCER1_bit.CC2P=0;     //rising edge polarity 
  TIM1_SMCR_bit.SMS=7;       //External clock source mode 1
  TIM1_SMCR_bit.TS=6;        //TI2 as the input source
  TIM1_IER_bit.UIE=1;       //Разрешаем генерацию прерывания при переполнеини
  PC_CR1_bit.C17=1;
  TIM1_CR1_bit.CEN=1;       //Разрешаем счет 
  TIM1_SR1_bit.UIF=1;
        //Инициализируем TIM4
       CLK_PCKENR1|=(1<<4);//BIN8(00010000);
       TIM4_SR_bit.UIF=0;        //Сбросим признак прерывания
       TIM4_PSCR=0x03;           //Предделитель на 2^3=8  4uS (F_CPU=2000000)
       TIM4_CR1_bit.ARPE=1;
       TIM4_ARR=250;             //Регистр автозагрузки таймера при переполнении 1mS
       TIM4_IER_bit.UIE=1;       //Разрешаем генерацию прерывания при переполнеини
       TIM4_CR1_bit.CEN=1;       //Разрешаем счет 
       TIM4_SR_bit.UIF=1;
 PA_DDR_bit.DDR3=1;
 PA_CR1_bit.C13=1;
 OUT=1;
 ///*  при дебаге закоментировать
 //CPU_CFG_GCR_bit.SWD=1;              //SWIM->IO 
 //Инициализация ст.тайм.
 IWDG_KR=0x55;
 IWDG_PR=4;IWDG_RLR=0xff;//(4,255) 255mS
  __watchdog_reset();
 IWDG_KR=0xCC;
 //*/  
 init_din();
 timdin=100;
 timsin=1;
 init_time();
  __enable_interrupt();

  while(1)
  {
   __watchdog_reset();
    if(timdin==1)dinind();
    else if(timsin==1)selind();
    else if(TKUD==TOPR){restim();TKUD+=1;} 
    else if(TKSL==TOPR){settim();TKSL+=1;}
    else if(RUN==0&&TKDW==TOPR&&TIMER<59940){TIMER++;TKDW-=BOOST;timsin=1;}//max 999min
    else if(RUN==0&&TKUP==TOPR&&TIMER<(59940-60)){TIMER+=60;TKUP-=BOOST;timsin=1;}
    else if(timsec==1)ticsec();
  }
}
