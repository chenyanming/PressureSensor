/* 
 Weather Shield code for http://connectthedots.msopentech.com end-to-end example of sending data to Microsoft Azure
 By: Microsoft Open Technologies, Inc.
 -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

 Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.

 The MIT License (MIT)

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 
 Based upon Nathan Seidle's Weather Shield Example, itself based upon MIke Grusin's USB Weather Board code, as stated below.

 Modifications by Microsoft Open Technologies, Inc include adding self-describing fields in each output string, and changing 
 output format to JSON string as expected by the CTD website.
 -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

 Original header follows.

 Weather Shield Example
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 16th, 2013
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 Much of this is based on Mike Grusin's USB Weather Board code: https://www.sparkfun.com/products/10586
 
 This code reads all the various sensors (wind speed, direction, rain gauge, humidty, pressure, light, batt_lvl)
 and reports it over the serial comm port. This can be easily routed to an datalogger (such as OpenLog) or
 a wireless transmitter (such as Electric Imp).
 
 Measurements are reported once a second but windspeed and rain gauge are tied to interrupts that are
 calcualted at each report.
 
 This example code assumes the GPS module is not used.
 
 */
// #include <Wire.h> //I2C needed for sensors
// #include "MPL3115A2.h" //Pressure sensor
// #include "HTU21D.h" //Humidity sensor

#define MYSERIAL Serial
// For Arduino Due Native Port:
//#define MYSERIAL SerialUSB

// MPL3115A2 myPressure; //Create an instance of the pressure sensor
// HTU21D myHumidity; //Create an instance of the humidity sensor

// Constants used for the ConnectTheDots project
// Disp value will be the label for the curve on the chart
// GUID ensures all the data from this sensor appears on the same chart
// You can use Visual Studio to create DeviceGUID and copy it here. In VS, On the Tools menu, click Create GUID. The Create GUID
// tool appears with a GUID in the Result box. Click Copy, and paste below.
//
char GUID1[] = "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx";
char GUID2[] = "yyyyyyyy-yyyy-yyyy-yyyy-yyyyyyyyyyyy";
char GUID3[] = "zzzzzzzz-zzzz-zzzz-zzzz-zzzzzzzzzzzz";
char GUID4[] = "wwwwwwww-wwww-wwww-wwww-wwwwwwwwwwww";
char Org[] = "My organization";
char Disp[] = "Arduino + Raspberry Pi";
char Locn[] = "here";
char Measure1[] = "Pressure of Sensor 1";
char Units1[] = "Units";
char Measure2[] = "Pressure of Sensor 2";
char Units2[] = "Units";
char Measure3[] = "Pressure of Sensor 3";
char Units3[] = "Units";
char Measure4[] = "Pressure of Sensor 4";
char Units4[] = "Units";
char buffer[300];
char dtostrfbuffer[15];


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//Hardware pin definitions
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// digital I/O pins
const byte a0 = 2;
const byte a1 = 4;
const byte a2 = 7;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//Global Variables
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
long lastSecond; //The millis counter to see when a second rolls by
byte seconds; //When it hits 60, increase the current minute
byte seconds_2m; //Keeps track of the "wind speed/dir avg" over last 2 minutes array of data
byte minutes; //Keeps track of where we are in various arrays of data
byte minutes_10m; //Keeps track of where we are in wind gust/dir over last 10 minutes array of data

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//Figure Variables that will display in website
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
float pressure[4]={0,0,0,0};




void setup()
{
  pinMode(a0,OUTPUT);
  pinMode(a1,OUTPUT);
  pinMode(a2,OUTPUT);
  // Serial.begin(115200);
  
  MYSERIAL.begin(9600);
  //Serial.println("Weather Shield Example");

  seconds = 0;
  lastSecond = millis();

  // attach external interrupt pins to IRQ functions
  // attachInterrupt(0, rainIRQ, FALLING);
  // attachInterrupt(1, wspeedIRQ, FALLING);

  // turn on interrupts
  interrupts();


}

void loop()
{
  //Keep track of which minute it is
  if(millis() - lastSecond >= 1000)
  {
    
    lastSecond += 1000;


    if(++seconds > 59)
    {
      seconds = 0;

      if(++minutes > 59) minutes = 0;
      if(++minutes_10m > 9) minutes_10m = 0;

    }

    //Report all readings every second
    printPressure();

  }

  delay(100);
}




//Prints the various variables directly to the port
//I don't like the way this function is written but Arduino doesn't support floats under sprintf
int sequenceNumber =0;

void printPressure()
{
  // calcPressure(); //Go calc all the various sensors

  // print string for temperature, separated by line for ease of reading
  // sent as one Serial.Print to reduce risk of serial errors
  
  memset(buffer,'\0',sizeof(buffer));
  strcat(buffer,"{");
  strcat(buffer,"\"guid\":\"");
  strcat(buffer,GUID1);
  strcat(buffer,"\",\"organization\":\"");
  strcat(buffer,Org);
  strcat(buffer,"\",\"displayname\":\"");
  strcat(buffer,Disp);
  strcat(buffer,"\",\"location\":\"");
  strcat(buffer,Locn);  
  strcat(buffer,"\",\"measurename\":\"");
  strcat(buffer,Measure1);
  strcat(buffer,"\",\"unitofmeasure\":\"");
  strcat(buffer,Units1);
  strcat(buffer,"\",\"value\":");
  strcat(buffer,dtostrf(pressure[0],6,2,dtostrfbuffer));
  strcat(buffer,"}");
  MYSERIAL.println(buffer);

  // print string for humidity, separated by line for ease of reading
  memset(buffer,'\0',sizeof(buffer));
  strcat(buffer,"{");
  strcat(buffer,"\"guid\":\"");
  strcat(buffer,GUID2);
  strcat(buffer,"\",\"organization\":\"");
  strcat(buffer,Org);
  strcat(buffer,"\",\"displayname\":\"");
  strcat(buffer,Disp);
  strcat(buffer,"\",\"location\":\"");
  strcat(buffer,Locn);  
  strcat(buffer,"\",\"measurename\":\"");
  strcat(buffer,Measure2);
  strcat(buffer,"\",\"unitofmeasure\":\"");
  strcat(buffer,Units2);
  strcat(buffer,"\",\"value\":");
  strcat(buffer,dtostrf(pressure[1],6,2,dtostrfbuffer));
  strcat(buffer,"}");
  MYSERIAL.println(buffer);

  // print string for light, separated by line for ease of reading
  memset(buffer,'\0',sizeof(buffer));
  strcat(buffer,"{");
  strcat(buffer,"\"guid\":\"");
  strcat(buffer,GUID3);
  strcat(buffer,"\",\"organization\":\"");
  strcat(buffer,Org);
  strcat(buffer,"\",\"displayname\":\"");
  strcat(buffer,Disp);
  strcat(buffer,"\",\"location\":\"");
  strcat(buffer,Locn);  
  strcat(buffer,"\",\"measurename\":\"");
  strcat(buffer,Measure3);
  strcat(buffer,"\",\"unitofmeasure\":\"");
  strcat(buffer,Units3);
  strcat(buffer,"\",\"value\":");
  strcat(buffer,dtostrf(pressure[2],6,2,dtostrfbuffer));
  strcat(buffer,"}");
  MYSERIAL.println(buffer);

  // print string for light, separated by line for ease of reading
  // memset(buffer,'\0',sizeof(buffer));
  // strcat(buffer,"{");
  // strcat(buffer,"\"guid\":\"");
  // strcat(buffer,GUID4);
  // strcat(buffer,"\",\"organization\":\"");
  // strcat(buffer,Org);
  // strcat(buffer,"\",\"displayname\":\"");
  // strcat(buffer,Disp);
  // strcat(buffer,"\",\"location\":\"");
  // strcat(buffer,Locn);  
  // strcat(buffer,"\",\"measurename\":\"");
  // strcat(buffer,Measure4);
  // strcat(buffer,"\",\"unitofmeasure\":\"");
  // strcat(buffer,Units4);
  // strcat(buffer,"\",\"value\":");
  // strcat(buffer,dtostrf(pressure[3],6,2,dtostrfbuffer));
  // strcat(buffer,"}");
  // MYSERIAL.println(buffer);
}





/*
 * Calculate the pressure value from sensors
 */
void calcPressure()
{
  for(int i=0;i<4;i++)
  {
    switch(i)
    {
    case 0:
      digitalWrite(a0,LOW);
      digitalWrite(a1,LOW);
      digitalWrite(a2,LOW);
      break;
    case 1:
      digitalWrite(a0,HIGH);
      digitalWrite(a1,LOW);
      digitalWrite(a2,LOW);
      break;
    case 2:
      digitalWrite(a0,LOW);
      digitalWrite(a1,HIGH);
      digitalWrite(a2,LOW);
      break;
    case 3:
      digitalWrite(a0,HIGH);
      digitalWrite(a1,HIGH);
      digitalWrite(a2,LOW);
      break;    
    default:
      digitalWrite(a0,LOW);
      digitalWrite(a1,LOW);
      digitalWrite(a2,LOW);
    }
    delay(10);
    pressure[i]=analogRead(0);
  }
  delay(10);
 
}

  
  


