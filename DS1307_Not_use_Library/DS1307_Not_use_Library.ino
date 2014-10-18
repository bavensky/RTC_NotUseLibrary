  /*******************************************************************************
  * Project  : Real Time Clock DS1307                                           *
  * Compiler : Arduino 1.5.5-r2                                                 *
  * Board    : Arduino UNO                                                      *
  * Device   : Tiny RTC I2C module                                              *
  * Link     : -                                                                * 
  * Author   : Bavensky :3                                                      *
  * E-Mail   : Aphirak_Sang-ngenchai@hotmail.com                                *
  * Date     : 19/10/2014 [dd/mm/yyyy]                                          *
  *******************************************************************************/ 
  
  #include "Wire.h"
  #define DS1307_ADDRESS 0x68   // Address IC 0x68
  byte zero = 0x00; 
  int _weekDay;
  int _second, _minute, _hour;
  int _monthDay, _month, _year; 
  int control, d1, d2; 

  
  void setup()
  {
    Wire.begin();
    Serial.begin(9600);
    
    /*** uncomment when Seting Time ***/
    //setDateTime();   
  }
  
  void loop()
  {
    printDateTime();
    delay(1000);
  }
  
  /****** Set Date Time ***********************************/
  void setDateTime()
  {
    /*******  Config to time *****************************/
    byte second =      0;    // Set Second 0-59
    byte minute =      12;   // Set Minute 0-59
    byte hour =        0;    // Set Hour   0-23
    byte weekDay =     1;    // Set Day of Week  1-7
    byte monthDay =    19;   // Set Day of Month 1-31
    byte month =       10;   // Set Month  1-12
    byte year  =       14;   // Set Year   0-99
    
    /*******************************************************
    * BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0
    * ------------------------------------------------------
    * OUT  |  X   |  X   | SQWE |  X   |  X   | RS1  | RS0
    *  0   |  X   |  X   |  0   |  X   |  X   |  0   |  0
    ********************************************************/
    /* Comment :
        OUT  Mean  Output control level of the SQW out
        SQWE Mean  Square Wave Enable output depends upon the rs1 rs0 
        RS   Mean  Rate Select control square wave output when enable
                   |  RS1  RS0  SQWoutput  |
                   |   0    0      1 Hz    |
                   |   0    1   4.096 kHz  |
                   |   1    0   8.192 kHz  |
                   |   1    1  32.768 kHz  |  */
    byte control  =    0b10010011; 

    byte  d1;
    byte  d2;
  
    
    Wire.beginTransmission(DS1307_ADDRESS);  
    Wire.write(zero);    //stop Oscillator
    Wire.write(decToBcd(second));
    Wire.write(decToBcd(minute));
    Wire.write(decToBcd(hour));
    Wire.write(decToBcd(weekDay));
    Wire.write(decToBcd(monthDay));
    Wire.write(decToBcd(month));
    Wire.write(decToBcd(year));
    Wire.write(control);
    Wire.write(12);    //data address 08
    Wire.write(34);    //data address 09
    Wire.write(zero);  //start 
    Wire.endTransmission();
  
  }
  
  byte decToBcd(byte val)
  {
    // Convert normal decimal numbers to binary coded decimal
    return ( (val/10*16) + (val%10) );
  }
  
  byte bcdToDec(byte val)  
  {
    // Convert binary coded decimal to normal decimal numbers
    return ( (val/16*10) + (val%16) );
  }
  
  void now_datetime()
  {
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(zero);
    Wire.endTransmission();
    Wire.requestFrom(DS1307_ADDRESS, 10);
    
    int second = bcdToDec(Wire.read());
    int minute = bcdToDec(Wire.read());
    int hour = bcdToDec(Wire.read() &0b111111); //24 hour time
    int weekDay = bcdToDec(Wire.read()); //1-7 ->sunday - Saturday
    int monthDay = bcdToDec(Wire.read());
    int month = bcdToDec(Wire.read());
    int year = bcdToDec(Wire.read());
    int control = Wire.read();
    int d1 = Wire.read();
    int d2 = Wire.read();
  }
  
  void printDateTime()
  {
    now_datetime();
    
    switch (weekDay) 
    {
      case 1:
        Serial.print(" Sunday")
      break;
      case 2:
        Serial.print(" Monday")
      break;
      case 3:
        Serial.print(" Tuesday")
      break;
      case 4:
        Serial.print(" Wednesday")
      break;
      case 5:
        Serial.print(" Thursday")
      break;
      case 6:
        Serial.print(" Friday")
      break;
      case 7:
        Serial.print(" Saturay")
      break;

  }

       
    //print the data
    Serial.println(" ");
    Serial.print("Day : ");
    Serial.print(weekDay);
    Serial.print(", ");
    Serial.print(monthDay);
    Serial.print("/");
    Serial.print(month);
    Serial.print("/20");
    Serial.print(year);
    Serial.print(", ");
    Serial.print(hour);
    Serial.print(":");
    Serial.print(minute);
    Serial.print(":");
    Serial.print(second);
    Serial.print(", ");
    Serial.print(control,BIN);
    Serial.print(", ");
    Serial.print(d1);
    Serial.print(", ");
    Serial.println(d2);
  }
