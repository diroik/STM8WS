///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR C/C++ Compiler V1.40.1.50106 for STM8            03/Aug/2016  19:08:53 /
// Copyright 2010-2013 IAR Systems AB.                                        /
//                                                                            /
//    Source file  =  D:\$PROJECTS\Microcontrollers\$STM8\Test\main.cpp       /
//    Command line =  D:\$PROJECTS\Microcontrollers\$STM8\Test\main.cpp -e    /
//                    -On --no_cse --no_unroll --no_inline --no_code_motion   /
//                    --no_tbaa --no_cross_call --debug --code_model small    /
//                    --data_model medium -o D:\$PROJECTS\Microcontrollers\$S /
//                    TM8\Test\Debug\Obj\ --dlib_config "C:\Program Files     /
//                    (x86)\IAR Systems\Embedded Workbench                    /
//                    6.5\stm8\LIB\dlstm8smn.h" -D STM8S003 -lC               /
//                    D:\$PROJECTS\Microcontrollers\$STM8\Test\Debug\List\    /
//                    -lA D:\$PROJECTS\Microcontrollers\$STM8\Test\Debug\List /
//                    \ -I D:\$PROJECTS\Microcontrollers\$STM8\Test\incl\     /
//                    --eec++ --vregs 16                                      /
//    List file    =  D:\$PROJECTS\Microcontrollers\$STM8\Test\Debug\List\mai /
//                    n.s                                                     /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME main

        RTMODEL "__SystemLibrary", "DLib"
        RTMODEL "__code_model", "small"
        RTMODEL "__core", "stm8"
        RTMODEL "__data_model", "medium"
        RTMODEL "__rt_version", "4"

        PUBLIC _Z10Init_Portsv
        PUBLIC _Z10Soft_Resetv
        PUBLIC _Z16Init_Flash_Constv
        PUBLIC _Z25MainLogicManagmentProcessv
        PUBLIC _Z8Init_ADCv
        PUBLIC _Z8Init_OSCv
        PUBLIC _Z8Stop_ADCv
        PUBLIC _Z9Start_ADCv
        PUBLIC main
        
          CFI Names cfiNames0
          CFI StackFrame CFA SP DATA
          CFI Resource A:8, XL:8, XH:8, YL:8, YH:8, SP:16, CC:8, PC:24, PCL:8
          CFI Resource PCH:8, PCE:8, ?b0:8, ?b1:8, ?b2:8, ?b3:8, ?b4:8, ?b5:8
          CFI Resource ?b6:8, ?b7:8, ?b8:8, ?b9:8, ?b10:8, ?b11:8, ?b12:8, ?b13:8
          CFI Resource ?b14:8, ?b15:8
          CFI ResourceParts PC PCE, PCH, PCL
          CFI EndNames cfiNames0
        
          CFI Common cfiCommon0 Using cfiNames0
          CFI CodeAlign 1
          CFI DataAlign 1
          CFI ReturnAddress PC CODE
          CFI CFA SP+2
          CFI A Undefined
          CFI XL Undefined
          CFI XH Undefined
          CFI YL Undefined
          CFI YH Undefined
          CFI CC Undefined
          CFI PC Concat
          CFI PCL Frame(CFA, 0)
          CFI PCH Frame(CFA, -1)
          CFI PCE SameValue
          CFI ?b0 Undefined
          CFI ?b1 Undefined
          CFI ?b2 Undefined
          CFI ?b3 Undefined
          CFI ?b4 Undefined
          CFI ?b5 Undefined
          CFI ?b6 Undefined
          CFI ?b7 Undefined
          CFI ?b8 SameValue
          CFI ?b9 SameValue
          CFI ?b10 SameValue
          CFI ?b11 SameValue
          CFI ?b12 SameValue
          CFI ?b13 SameValue
          CFI ?b14 SameValue
          CFI ?b15 SameValue
          CFI EndCommon cfiCommon0
        
// D:\$PROJECTS\Microcontrollers\$STM8\Test\main.cpp
//    1 /**********************************************/
//    2 /*              TM 2016                       */
//    3 /*                                            */
//    4 /*               fable2000@rambler.ru         */
//    5 /*                droik@techmonitor.ru        */
//    6 /**********************************************/
//    7 #include <iostm8s003f3.h>
//    8 #include <type_define.h>
//    9 #include <flash_map.h>
//   10 #include <sram_map.h>
//   11 #include "Init.cpp"

        SECTION `.near_func.text`:CODE:REORDER:NOROOT(0)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z10Init_Portsv
        CODE
// void Init_Ports()
_Z10Init_Portsv:
        RET
          CFI EndBlock cfiBlock0

        SECTION `.near_func.text`:CODE:REORDER:NOROOT(0)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z8Init_OSCv
        CODE
// void Init_OSC()
_Z8Init_OSCv:
        RET
          CFI EndBlock cfiBlock1

        SECTION `.near_func.text`:CODE:REORDER:NOROOT(0)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z8Init_ADCv
        CODE
// void Init_ADC()
_Z8Init_ADCv:
        RET
          CFI EndBlock cfiBlock2

        SECTION `.near_func.text`:CODE:REORDER:NOROOT(0)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z9Start_ADCv
        CODE
// void Start_ADC()
_Z9Start_ADCv:
        RET
          CFI EndBlock cfiBlock3

        SECTION `.near_func.text`:CODE:REORDER:NOROOT(0)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z8Stop_ADCv
        CODE
// void Stop_ADC()
_Z8Stop_ADCv:
        RET
          CFI EndBlock cfiBlock4

        SECTION `.near_func.text`:CODE:REORDER:NOROOT(0)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z10Soft_Resetv
        CODE
// void Soft_Reset()
_Z10Soft_Resetv:
??Soft_Reset_0:
        JRA       L:??Soft_Reset_0
          CFI EndBlock cfiBlock5

        SECTION `.near_func.text`:CODE:REORDER:NOROOT(0)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z16Init_Flash_Constv
        CODE
// void Init_Flash_Const()
_Z16Init_Flash_Constv:
        RET
          CFI EndBlock cfiBlock6
//   12 #include "interrupts.cpp"
//   13 //******************************************************************************
//   14 //**  Прототипы функций
//   15 //******************************************************************************
//   16 void MainLogicManagmentProcess(void);
//   17 
//   18 //******************************************************************************
//   19 //** Начало программы
//   20 //******************************************************************************

        SECTION `.near_func.text`:CODE:REORDER:NOROOT(0)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function main
        CODE
//   21 void main(void)
//   22 {
//   23   //SYSTEM_INIT(); 
//   24   //__enable_interrupt(); 
//   25   for(;;)
//   26   {
//   27    // CLEAR_WDT;
//   28     MainLogicManagmentProcess();  
main:
??main_0:
        CALL      L:_Z25MainLogicManagmentProcessv
        JRA       L:??main_0
          CFI EndBlock cfiBlock7
//   29   } 
//   30 }
//   31 //******************************************************************************
//   32 //**  
//   33 //******************************************************************************

        SECTION `.near_func.text`:CODE:REORDER:NOROOT(0)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z25MainLogicManagmentProcessv
        CODE
//   34 void MainLogicManagmentProcess()
//   35 {
//   36 }
_Z25MainLogicManagmentProcessv:
        RET
          CFI EndBlock cfiBlock8

        SECTION VREGS:DATA:REORDER:NOROOT(0)

        END
//   37 //******************************************************************************
//   38 
//   39 
// 
// 14 bytes in section .near_func.text
// 
// 14 bytes of CODE memory
//
//Errors: none
//Warnings: none
