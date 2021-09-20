#ifndef SRAM_MAP_H
#define SRAM_MAP_H

        #include <Usart.h>


        Data_Params const*      PtrToConstants;

        T_DISCRETE     DISCRETE[MAX_DISCRETE_INPUTS];
        T_ADC_AKB      AKB[MAX_ADC_INPUTS];
        Word           ADC_Counter              = 0;
        bool           ADC_Ready                = false;
        Word           ADC_Input                = 0;
        
        Word           InputTimer       = 0;
        bool           Input_Ready      = false;
        
        
        sWord           DataLen;
        Byte            Buffer[MAX_BUFF_SIZE];
        TComPort        Usart(16);
        
        Data_Values     DATA;

        Word            RebootTimer             = 0;
        DWord           RebootCounter           = 0;
        //DWord           RebootConstant          = 12000;//12000 sec = 200 min
        /*
        Word           TermoDatchiks[MAX_ADC_INPUTS];
        Word           InputValues  = 0;
        Word           ReleValues  = 0;
        */

#endif