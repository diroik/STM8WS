#ifndef FLASH_MAP_H
#define FLASH_MAP_H


      #pragma location=0x4000
     __root const Data_Params  PARAMS = 
      {
        .DATA[BOOT_FLG] = 0,
        .DATA[DEV_ADDR] = 1,
        .DATA[TIME_OUT] = 500,
        .DATA[BOUD_RATE] = 9600,
        .DATA[RELE_STAT] = 0x00,
        .DATA[INPUT_INV] = 0xFF00,
        .DATA[KOEF1] = 1000,//1000=1.000
        .DATA[KOEF2] = 1000,//1000=1.000
        .DATA[KOEF3] = 1000,//1000=1.000
        .DATA[KOEF4] = 1000,//1000=1.000
        
        .CRC                    = 0xFF,
        .KeyByte                = 0xAA
      };
      //#pragma memory = default


     
#endif

