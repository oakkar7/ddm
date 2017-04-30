/*
  ddm.h - Test library for digital diagnostic monitoring - description
  Copyright (c) 2017 OAKKAR7.  All right reserved.

  By      : oakkar7, Myanmar
  blog    ; okelectronic.wordpress.com
  code    : https://github.com/oakkar7

*/

// ensure this library description is only included once
#ifndef Test_h
#define Test_h

// include types & constants of Wiring core API
#include <Arduino.h>
//#include "WConstants.h"
//#define byte uint8_t

// library interface description
class ddm
{
  // user-accessible "public" interface
  public:
    float getRX();
    float getTX();
    float getTemp();
    float getVolts();
    void getALM(boolean & _temp_hi, boolean & _temp_low, boolean & _vcc_hi, boolean & _vcc_low,boolean & _tx_hi, boolean & _tx_low,boolean & _rx_hi, boolean & _rx_low );
    void getWARN(boolean & _temp_hi_warn, boolean & _temp_low_warn, boolean & _vcc_hi_warn, boolean & _vcc_low_warn,boolean & _tx_hi_warn, boolean & _tx_low_warn,boolean & _rx_hi_warn, boolean & _rx_low_warn );
    uint16_t calibrateRXpower();
    void getINFO();
    void debugDDM();    
    //uint8_t DDM_Read(unsigned int address);
    void DDM_All_Read(void);
    //Test(int);
    //void doSomething(void);

  // library-accessible "private" interface
  private:
    //int value;
    //void doSomethingSecret(void);
    uint8_t DDM_Read(unsigned int address);
    
};

#endif

