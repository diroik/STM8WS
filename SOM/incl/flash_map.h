#ifndef FLASH_MAP_H
#define FLASH_MAP_H


      //#pragma constseg = INFOD 
      #pragma location=0x4000
     __root const Data_Params  PARAMS = 
      {
        .PorogValue1      = 210,
        .PorogValue2      = 210,
        
        .CRC                = 0xFF,
        .KeyByte            = 0xAA
      };
      //#pragma memory = default


     
#endif

