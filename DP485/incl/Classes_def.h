#ifndef CLASSES_DEF_H
  #define CLASSES_DEF_H
     
    //======================================================
    #define MAX_ADC_INPUTS          1//13    
    #define ADC_TIMER_PERIUD        10 // период включения АЦП в cек
    #define MAX_DISCRETE_INPUTS     16  
    #define MAX_RELE_OUTPUTS        4  
    #define MAX_BUFF_SIZE           128
    //======================================================
    #define STX ':'
    #define ETX 0x0A
    //======================================================
    #define DEVICE_CURR_VER  2      //
    #define DEVICE_SUB_VER   0      //Подверсия устройства.    
        
    
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
    #define DISCRETE_INPUTS_TIM  10 
    #define FILTER_CNT 30
    //======================================================


    #define RESISTOR 1000.0


   #define PROC_FREQ 2000000
    // comport
    #define INVALID_HANDLE_VALUE  0
    // timeouts
    #define RX_BYTE_TIMEOUT       0x00      // 
    //#define RX_TOTAL_TIMEOUT      0x01      //  
    //#define TX_BYTE_TIMEOUT       0x02  
    //#define TX_TOTAL_TIMEOUT      0x03 
    //======================================================  
    #define START_COIL_STATUS_ADDR      100
    #define START_DISCRETE_INPUTS_ADDR  200
    #define START_HOLDIG_REGISTERS_ADDR 300
    #define START_INPUT_REGISTERS_ADDR  400
    #define START_DATA_REGISTERS_ADDR   16384
    #define REBOOT_REGISTERS_ADDR       30001
    //======================================================  
#endif
