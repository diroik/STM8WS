#ifndef INIT_H
    #define INIT_H
//=================================================
 void SYSTEM_INIT();
 void Init_Ports();
 void Init_OSC();
 void Init_TIMER();
 void Init_Flash_Const();
 void Init_ADC();
 void Init_WDT();
 
 void Start_ADC(); 
 void Stop_ADC(); 
 void Soft_Reset(); 
 void Init_Discrete(void);
//=================================================
#endif