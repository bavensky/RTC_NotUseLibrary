  #include "Wire.h"
  #define DS1307_ADDRESS 0x68   // Address IC 0x68
  byte zero = 0x00; 
  
  void setup()
  {
    Wire.begin();
    Serial.begin(9600);
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
    byte second =      0;    // Set Second 0-59
    byte minute =      12;   // Set Minute 0-59
    byte hour =        0;   // Set Hour   0-23
    byte weekDay =     1;    // Set Day of Week  1-7
    byte monthDay =    19;   // Set Day of Month 1-31
    byte month =       10;   // Set Month  1-12
    byte year  =       14;   // Set Year   0-99
    
    /*******************************************************
    * BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0
    * ------------------------------------------------------
    * OUT  |  X   |  X   | SQWE |  X   |  X   | RS1  | RS2
    ********************************************************/
    byte control  =    0b10010011; 
    // out 0 0 sqwe 0 0 rs1 rs0  >>> led blink 1Hz  
    byte  d1;
    byte  d2;
  
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(zero); //stop Oscillator
    Wire.write(decToBcd(second));
    Wire.write(decToBcd(minute));
    Wire.write(decToBcd(hour));
    Wire.write(decToBcd(weekDay));
    Wire.write(decToBcd(monthDay));
    Wire.write(decToBcd(month));
    Wire.write(decToBcd(year));
    Wire.write(control);
    Wire.write(12);  //data address 08
    Wire.write(34);  //data address 09
    Wire.write(zero); //start 
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
  
  void printDateTime()
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

