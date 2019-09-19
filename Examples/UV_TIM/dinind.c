#include <iostm8s103f3.h>
#include <intrinsics.h>
#include "define.h"
#include "znggen.h"
#include "dinind.h"

#define segment // по сегментная индикация

#if colraz==2
#define mod 10
#elif colraz==3
#define mod 100
#elif colraz==4
#define mod 1000
#elif colraz==5
#define mod 10000
#elif colraz==6
#define mod 100000
#elif colraz==7
#define mod 1000000
#elif colraz==8
#define mod 10000000
#else
#define mod 1
#endif


#if colraz>5
#define lv unsigned long
#else 
#define lv uint
#endif


extern volatile uint tim1ms[coltim];
uchar AZN[colraz+1];
uchar BOOST=40;
const uchar SMZ[10]={SM_0,SM_1,SM_2,SM_3,SM_4,SM_5,SM_6,SM_7,SM_8,SM_9};
const struct
{
  uchar volatile *port;
  uchar pin; 
}raz[colraz]={{C3},{C2},{C1}};
   
const struct
{
  uchar volatile *port;
  uchar pin;
}seg[8]={{CA},{CB},{CC},{CD},{CE},{CF},{CG},{CH}};
  
#ifdef colvhd
const struct
{
  uchar volatile *port; 
  uchar pin;
}vhd[colvhd]={{UP},{DW},{SL}};
    
#define KUP 0
#define KDW 1

uchar timvhd[colvhd+1];
#endif
////////////////////////////////////////////////////////////////////////////
void init_din (void)
{
 uchar c; 
  for(c=0;c<8;c++)
    {
      *(seg[c].port)|=(1<<(seg[c].pin)); //_ODR=1
      *(seg[c].port+2)|=(1<<(seg[c].pin));//_DDR=1
      *(seg[c].port+3)&=~(1<<(seg[c].pin));
    }
     for(c=0;c<colraz;c++)
    {
       //#ifndef segment
      // *(raz[c].port)|=(1<<(raz[c].pin));
       //#endif
      *(raz[c].port)|=(1<<(raz[c].pin));
      *((raz[c].port)+2)|=(1<<(raz[c].pin));//_DDR=1
      *((raz[c].port)+3)|=(1<<(raz[c].pin));//_CR1=1
     
    
    }
    
    timdin=4;timsin=251;
}

//////////////////////////////////////////////////////////////////////////
void conv10_2 (uint a1,uchar point)
{ 
uchar ii = colraz-1; //(разряд)
lv a2;//,ma; 
//ma=a1;
  for (a2=mod;a2>1;a2/=10)    
  {                             
  AZN[ii]=0;                    
     while (a1>=a2)             
     {                          
     AZN[ii]++;                 
     a1-=a2;                    
     }                          
  --ii;                         
  }                             
  AZN[0]=a1; a2=mod;                
     for (ii=0;ii<colraz;ii++)
     {
       AZN[ii]=SMZ[AZN[ii]];

     }
    /*
     for(ii=3;ii>0;ii--)
     {
       if((a2>ma)&&(ii>point))AZN[ii]=SM_CLR;
      a2/=10; 
     }*/
  if(point) AZN[point-1]&=~(1<<7);   
}
///////////////////////////////////////////////////////////////////////////////
void ind7 (uchar segm, uchar raz)
{
  uchar c;
  static uchar b;
   b++;
    if((AZN[colraz]&(1<<raz))&& b>180)segm=SM_CLR;
  for(c=0;c<8;c++)
  {
    if(segm&(1<<c)) *seg[c].port|=(1<<(seg[c].pin));
    else   *seg[c].port&=~(1<<(seg[c].pin));  
  }
 
}
///////////////////////////////////////////////////////////////////////////////
void dinind (void)
{
#ifdef colvhd 
uchar c;
#endif 
 static uchar cndin, mraz;
#ifndef segment 
 if(++cndin>((colraz*2)-1))cndin=0;
   if((cndin&1)==0)
   {
     #ifdef colvhd 
     if(cndin==0) contr_vhd();
     #endif     
     mraz=cndin>>1;
     ind7(AZN[mraz],mraz);
     *raz[mraz].port&=~(1<<raz[mraz].pin);
     timdin=4;
   }
   else
   {
      *raz[mraz].port|=(1<<raz[mraz].pin);
      timdin=2;
    #ifdef colvhd      
       if(cndin==((colraz*2)-1))
       {
         for(c=0;c<colvhd;c++)
         {
           *(vhd[c].port+2)&=~(1<<(vhd[c].pin));//_DDR=0
           *(vhd[c].port+3)&=~(1<<(vhd[c].pin));//_CR1=0
         }
       }
    #endif       
   }
   
#endif  
#ifdef segment
 if(++cndin>((colraz<<3)+1))//*8segm
 {
   cndin=0;
   contr_vhd();
 }
 else
 {
     *raz[mraz>>3].port|=(1<<raz[mraz>>3].pin);// Погасить пред.знакоместо 
     *seg[mraz&BIN8(00000111)].port|=(1<<seg[mraz&BIN8(00000111)].pin);//Погасить пред.сегмент
     timdin=2;
        if(cndin==((colraz<<3)+1))
          {
            for(c=0;c<colvhd;c++)
             {
               *(vhd[c].port+2)&=~(1<<(vhd[c].pin));//_DDR=0
               *(vhd[c].port+3)&=~(1<<(vhd[c].pin));//_CR1=0
             }
          }
       else
       {
         mraz=(cndin-1);
         *raz[mraz>>3].port&=~(1<<raz[mraz>>3].pin);// Зажечь знакоместо
         
         c=((cndin-1)&BIN8(00000111));
           if(AZN[(cndin-1)>>3]&(1<<c)) *seg[c].port|=(1<<seg[c].pin);
           else *seg[c].port&=~(1<<seg[c].pin);
        }
 }   
#endif   
}
//////////////////////////////////////////////////////////////////////////////
void contr_vhd (void)
{
  uchar c;
  static uchar t;
     if((*(vhd[KUP].port+1)&(1<<vhd[KUP].pin))&&(*(vhd[KDW].port+1)&(1<<vhd[KDW].pin)))
          {
            if(TKUD!=TOPR)TKUD++;TKUP=0;TKDW=0;
          }
          else TKUD=0;
  if(!(*(vhd[KUP].port+1)&(1<<vhd[KUP].pin))&&!(*(vhd[KDW].port+1)&(1<<vhd[KDW].pin)))
  {BOOST=40;t=0;}
  else
  {
    if(++t==6)
    {
      if(BOOST>4)BOOST--;//Ускорение изменения параметров 1раз в 150мС
      t=0;
    }
  }
       for(c=0;c<colvhd;c++)
       {
         if(*(vhd[c].port+1)&(1<<vhd[c].pin))
         {
           if(timvhd[c]!=TOPR)timvhd[c]++;
           if(timvhd[c]==0)timvhd[c]=1;
         }
         else timvhd[c]=0;
        *(vhd[c].port+2)|=(1<<(vhd[c].pin));//_DDR=1
        *(vhd[c].port+3)|=(1<<(vhd[c].pin));//_CR1=1
       }
}