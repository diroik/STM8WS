#ifndef CLASSES_DEF_H
  #define CLASSES_DEF_H
     
    //======================================================
    #define MAX_ADC_INPUTS          3//13    
    #define ADC_TIMER_PERIUD        10 // периуд включения АЦП в cек
    //======================================================
    #define DEVICE_CURR_VER  1      //
    #define DEVICE_SUB_VER   1      //Подверсия устройства.    
        
    
    #define U_DIVIDER  250.0/10.0//(120+120+10)/10

    #define I_DIVIDER  1.0      //20.0/10.0//(10+10)/10

    #define I_SENS     2.0 // 2В/A    
    #define U_SENS_MAX 5.0
    #define I_NULL_POROG  0.05
    #define NULL_ERROR35  0.06
    #define NULL_ERROR75  0.15
    #define NULL_TEMP     35.0 
    #define TEMP_KOEF     0.00005  
    #define MAX_U         2.55 
    #define MIN_U         2.45
        
    
    //======================================================  

#endif
