/*********************************************************************
Optical Power Meter
version : 1.3 beta
Date    : 27 Nov 2016
By      : oakkar7, Myanmar
blog    ; okelectronic.wordpress.com
code    : https://github.com/oakkar7

Credit to:
1) Nokia 5110 LCD is powered by Adafruit, Adafruit-PCD8544-Nokia-5110-LCD-library
https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library

2) SPI DDM interface is based on the works of 
SFPDDM (Arduino library for interfacing SFP modules and reading DDM information as per SFF-8472)
https://github.com/SloMusti/sfpddm

*********************************************************************/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Wire.h>
#include <math.h>

#include "ddm.h"
#include "catbin.h"

// Creating an object of SFP DDM library
ddm sfp;

// input pins for ALM
const int TX_fault =  8;      // TX en LED pin
const int LOS =  9;           // TX en LED pin
// output pin for CTL
const int TX_disable =  10;   // TX en, 0 = TX ensble ,1= disable ,

// status LEDs
const int TX_led =  A2;     // TX en LED pin
const int RX_led = A3;      // LOS LED pin

const int TX_sw =  11;      // LOS LED pin
const int ES_sw =  12;      // LOS LED pin
const int UP_sw =  A0;      // LOS LED pin
const int DN_sw =  A1;      // LOS LED pin
const int FN_sw =  A6;      // LOS LED pin
const int EN_sw =  A7;      // LOS LED pin

boolean debug_flag = 1;
boolean TX_status = 0;
boolean RX_status = 0;
boolean V_status = 0;

float temp_t;
float volts;
float optical_tx;
float optical_rx;

boolean temp_hi, temp_low, vcc_hi, vcc_low, tx_hi, tx_low, rx_hi, rx_low;
boolean temp_hi_warn, temp_low_warn, vcc_hi_warn, vcc_low_warn, tx_hi_warn, tx_low_warn, rx_hi_warn, rx_low_warn;

unsigned char  cur_menu = 5;

// Software SPI (slower updates, more flexible pin options): 
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

// Setup/Init Everything
void setup()   {

  pinMode(TX_disable, OUTPUT);        // TX en, GND = TX end,
  pinMode(TX_fault, INPUT_PULLUP);    // TxFault , 1 = OK, 0 = Fault
  pinMode(LOS, INPUT_PULLUP);         // LOS, 1 = OK, 0 = LOS
  pinMode(TX_sw, INPUT_PULLUP); 
  pinMode(TX_led, OUTPUT); 
  pinMode(RX_led, OUTPUT); 
  pinMode(FN_sw, INPUT); 
  pinMode(EN_sw, INPUT);
  pinMode(UP_sw, INPUT_PULLUP);
  pinMode(DN_sw, INPUT_PULLUP);
  pinMode(ES_sw, INPUT_PULLUP);
   
  Wire.begin();  
  Serial.begin(9600);

  display.begin();
  // LCD init done
  // you can change the contrast around to adapt the display
  display.setContrast(60);
  
  showSplash();
  delay(1000);

  digitalWrite(TX_disable, HIGH);    // Enable Laster TX
}

void loop() {

 while (1) {

  checkSW();

  sfp.DDM_All_Read ();
  volts = sfp.getVolts();
  temp_t = sfp.getTemp();
  optical_rx = sfp.getRX();
  optical_tx = sfp.getTX();

  checkALM();

  if (cur_menu == 5) showTXRX();
  if (cur_menu == 4) showRX();
  if (cur_menu == 3) showTX();
  if (cur_menu == 2) showDetails();
  if (cur_menu == 1) showSystem();
  
  delay(500);

  if (debug_flag)
  sfp.debugDDM();

  //getINFO();
  sfp.getALM(temp_hi, temp_low, vcc_hi, vcc_low, tx_hi, tx_low, rx_hi, rx_low);
  sfp.getWARN(temp_hi_warn, temp_low_warn, vcc_hi_warn, vcc_low_warn, tx_hi_warn, tx_low_warn, rx_hi_warn, rx_low_warn);  
  }
}


// Check Switches
void checkSW()
{
  // check TX SW
  if (digitalRead(TX_sw) == LOW)
  {
    delay(100);
    if (TX_status == 1) 
      TX_status = 0;
    else
      TX_status = 1;
    Serial.println(TX_status);

  if (TX_status == 1)
  {
    digitalWrite(TX_disable, HIGH);
    digitalWrite(TX_led, LOW); 
  }
  if (TX_status == 0)
  {
    digitalWrite(TX_disable, LOW);
    digitalWrite(TX_led, HIGH); 
  }
  }
  
  //check menu
   int FN_sw = analogRead(A6) > 256 ? 0 : 1;
  if (FN_sw == 1)
  {
    processMenu();
    //delay(100);
  }
  //check Escape/Cancel SW
  if (digitalRead(ES_sw) == LOW)
  {
    showSplash();
    delay(1000);
  }

  if (cur_menu > 5) cur_menu = 5;
  if (cur_menu < 1) cur_menu = 1;
}

// Check Switches-2
void checkSW2()
{
  //check UP SW
  int UP_sw = analogRead(A0) > 256 ? 0 : 1;
  if (UP_sw == 1)
  { 
    cur_menu++;
  }
  //check Down SW
  int DN_sw = analogRead(A1) > 256 ? 0 : 1;
  if (DN_sw == 1)
  {
    cur_menu--;
   }
  //check Escape/Cancel SW
  if (digitalRead(ES_sw) == LOW)
  {
    showSplash();
    delay(1000);
  }

  if (cur_menu > 5) cur_menu = 5;
  if (cur_menu < 1) cur_menu = 1;
}

// Check Alarms
void checkALM()
{
    if ( 30 > optical_rx  && optical_rx > -40)
    {  
      digitalWrite(RX_led, HIGH); 
      RX_status = 1;
    }
  else
  {
   digitalWrite(RX_led, LOW); 
   RX_status = 0;
  } 
}

// process menu
void processMenu()
{ int EN_sw = 0;
  while (EN_sw == 0)
  {
    checkSW2();
    showMenu();
    EN_sw = analogRead(A7) > 256 ? 0 : 1;
    delay(300); 
  }
}

// display menu
void showSplash()
{
    // miniature splash display
  display.clearDisplay();
  display.drawBitmap(0, 0,  cat1, 84, 48, 1);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("I");
  display.setCursor(0,10);
  display.println("CAN");
  display.setCursor(0,20);
  display.println("HAZ");
  display.setCursor(0,30);
  display.println("P0wer");
  display.setCursor(0,40);
  display.println("?!");
  display.display();
  delay(1000); 

  // text about
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("    Optical");
  //display.setTextSize(2);
  display.setTextColor(WHITE, BLACK); // 'inverted' text
  display.println(" POWER METER  ");
  display.setTextColor(BLACK);
  display.println("     v1.3  ");
  //display.setTextSize(1);
  display.println("      By");
  display.println("Oakkar,Myanmar");
  display.display();
  delay(1000);
}

void showMenu()
{
  display.clearDisplay();
  // text display tests
  display.setTextSize(1);
  
  if (cur_menu == 5)
   display.setTextColor(WHITE, BLACK);
  else
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("1.TX/RX Power");

  if (cur_menu == 4)
   display.setTextColor(WHITE, BLACK);
  else
  display.setTextColor(BLACK);
  display.setCursor(0,10);
  display.println("2.TX Power");

  if (cur_menu == 3)
   display.setTextColor(WHITE, BLACK);
  else
  display.setTextColor(BLACK);
  display.setCursor(0,20);
  display.println("3.TX Power");

  if (cur_menu == 2)
   display.setTextColor(WHITE, BLACK);
  else
  display.setTextColor(BLACK);
  display.setCursor(0,30);
  display.println("4.Details");

  if (cur_menu == 1)
   display.setTextColor(WHITE, BLACK);
  else
  display.setTextColor(BLACK);
  display.setCursor(0,40);
  display.println("5.System Setup");
  display.display(); 
}

void showTX()
{
  display.clearDisplay();
  // text display tests
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.print("TX Power:");
   display.println("dbm");
  display.setTextSize(2.5);
  display.setCursor(0,15);
  display.print(optical_tx);
  display.display();
  showStatus();
}

void showRX()
{
  display.clearDisplay();
  // text display tests
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.print("RX Power:");
  display.println("dbm");
  display.setTextSize(2.5);
  display.setCursor(0,15);
  display.print(optical_rx);
  display.display();
  showStatus();
}

void showTXRX()
{
  display.clearDisplay();
  // text display tests
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("RX Power:");
  display.setTextSize(2);
  display.setCursor(0,9);
  display.print(optical_rx);
  display.setTextSize(1);
  display.setCursor(64,15);
  display.println("dbm");
  
  display.setCursor(0,27);
  display.print("TX: ");
  //display.setTextSize(2);
  //display.setCursor(0,33);
  display.print(optical_tx);
  display.setCursor(64,27);
  display.println("dbm");
  display.display();
  showStatus();
}

// show status/alarm in bottom bar
void showStatus()
{
  display.setTextSize(1);
  display.setCursor(0,40);
  display.println("TX:    RX:");
  display.setCursor(18,40);
  if (TX_status == 0)
  display.print("EN");
  else
  display.print("NA");

  display.setCursor(60,40);
  if (RX_status == 1)
  display.print("OK ");
  else
  display.print("LOS"); 
   
  display.display();
  
}
// show detailed one screen
void showDetails()
{
   display.clearDisplay();
  // text display tests
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("Temp   :");
  display.setCursor(0,10);
  display.println("Volt   :");
  display.setCursor(0,20);
  display.println("TXPower:");
  display.setCursor(0,30);
  display.println("RXPower:");
  display.setCursor(0,40);
  display.println("Alarms :");
  display.display();

  //update
   display.setCursor(48,0);
  display.println(temp_t);
  display.setCursor(48,10);
  display.println(volts);
  display.setCursor(48,20);
  display.println(optical_tx);
  display.setCursor(48,30);
  display.println(optical_rx);
  display.display();
}

void  showSystem()
{
   display.clearDisplay();
  // text display tests
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("Unit   :");
  display.setCursor(0,10);
  display.println("Volt   :");
  display.setCursor(0,20);
  display.println("Current:");
  display.setCursor(0,30);
  display.println("Temp   :");
  display.setCursor(0,40);
  display.println("Version:");
  display.display();
}
// EOF
