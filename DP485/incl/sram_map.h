#ifndef SRAM_MAP_H
#define SRAM_MAP_H

        #include <Usart.h>


        Data_Params const*      PtrToConstants;

        //T_DISCRETE     DISCRETE[MAX_DISCRETE_INPUTS];
        T_ADC_AKB      AKB[MAX_ADC_INPUTS];
        Word           ADC_Counter              = 0;
        bool           ADC_Ready                = false;
        
        Word           LedTimer       = 0;
        bool           Led_Ready      = false;
        sWord          LedCounter     = 0;  
        
        sWord           DataLen;
        Byte            Buffer[MAX_BUFF_SIZE];
        TComPort        Usart(16);
        
        Data_Values     DATA;
        
        //Word          TestCounter     = 0;

#endif