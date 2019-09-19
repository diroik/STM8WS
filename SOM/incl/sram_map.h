#ifndef SRAM_MAP_H
#define SRAM_MAP_H

        Data_Params const*      PtrToConstants;
        
        T_ADC_AKB      AKB[MAX_ADC_INPUTS];
        Word           ADC_Counter              = 0;
        bool           ADC_Ready                = false;
        Word           ADC_Input                = 0;
        
        long           _cval                    = 0;
        bool           _butonValue              = false;
        Byte           _ledValues[2]            = {1,1};
        
       // Byte testVal = 0;
        

#endif