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
  
  /****** Set Date Time ***********************************/
  void setDateTime()
  {
    /*******  Config to time *****************************/
    byte set_second =      0;    // Set Second 0-59
    byte set_minute =      43;   // Set Minute 0-59
    byte set_hour =        22;    // Set Hour   0-23
    byte set_weekDay =     1;    // Set Day of Week  1-7
    byte set_monthDay =    19;   // Set Day of Month 1-31
    byte set_month =       10;   // Set Month  1-12
    byte set_year  =       14;   // Set Year   0-99
    
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
  
    // Write to address ds1307
    Wire.beginTransmission(DS1307_ADDRESS);  
    Wire.write(zero);  //stop Oscillator
    Wire.write(decToBcd(set_second));
    Wire.write(decToBcd(set_minute));
    Wire.write(decToBcd(set_hour));
    Wire.write(decToBcd(set_weekDay));
    Wire.write(decToBcd(set_monthDay));
    Wire.write(decToBcd(set_month));
    Wire.write(decToBcd(set_year));
    Wire.write(control);
    //Wire.write(12);    //data address 08
    //Wire.write(34);    //data address 09
    Wire.write(zero);  //start 
    Wire.endTransmission();
  
  }
  
  void now_datetime()
  {
    //  Read from address ds1307
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(zero);
    Wire.endTransmission();
    
    Wire.requestFrom(DS1307_ADDRESS, 10);
    _second = bcdToDec(Wire.read());
    _minute = bcdToDec(Wire.read());
    _hour = bcdToDec(Wire.read() &0b111111);  // 24 hour time
    _weekDay = bcdToDec(Wire.read());         // 1-7 ->Sunday - Saturday
    _monthDay = bcdToDec(Wire.read());
    _month = bcdToDec(Wire.read());
    _year = bcdToDec(Wire.read());
    control = Wire.read();
    //d1 = Wire.read();
    //d2 = Wire.read();
  }
  
  void printDateTime()
  {
    now_datetime();
    
    //print the data from DS1307
    Serial.print("Day : ");
    if(_weekDay == 1) Serial.print("Sunday");
    if(_weekDay == 2) Serial.print("Monday");
    if(_weekDay == 3) Serial.print("Tuesday");
    if(_weekDay == 4) Serial.print("Wednesday");
    if(_weekDay == 5) Serial.print("Thursday");
    if(_weekDay == 6) Serial.print("Friday");
    if(_weekDay == 7) Serial.print("Saturay");
    
    /*switch (_weekDay) 
    {
      case 1:
        Serial.print("Sunday");
      break;
      case 2:
        Serial.print("Monday");
      break;
      case 3:
        Serial.print("Tuesday");
      break;
      case 4:
        Serial.print("Wednesday");
      break;
      case 5:
        Serial.print("Thursday");
      break;
      case 6:
        Serial.print("Friday");
      break;
      case 7:
        Serial.print("Saturay");
      break;
    }*/
    
    Serial.print(", ");
    Serial.print(_monthDay);
    Serial.print("/");
    Serial.print(_month);
    Serial.print("/20");
    Serial.print(_year);
    Serial.print(", ");
    Serial.print(_hour);
    Serial.print(":");
    Serial.print(_minute);
    Serial.print(":");
    Serial.print(_second);
    Serial.print(", ");
    Serial.print(control,BIN);
    Serial.print(", ");
    Serial.print(d1);
    Serial.print(", ");
    Serial.println(d2);
 
  }
