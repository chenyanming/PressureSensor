/**
 * Subject: EE514 Real Time Computing
 * Topic: Application of Sensors and Medical analysis based on Azure
 * Alias: Pressure Sensors Simulation Program
 * Version: 1.0
 * Developed by CHEN Yanming - 14096635g, TAN Xiao - 14104813g
 * From: Hong Kong Polytechnic University
 * From: Department of Electronic Information Engineering
 */
// #include <Wire.h> //I2C needed for sensors
// #include "MPL3115A2.h" //Pressure sensor
// #include "HTU21D.h" //Humidity sensor

#define MYSERIAL Serial

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
char Measure1[] = "Pressure";
char Units1[] = "Units";
char Measure2[] = "Pressure";
char Units2[] = "Units";
char Measure3[] = "Pressure";
char Units3[] = "Units";
char Measure4[] = "Pressure";
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
  pinMode(0, INPUT);
  pinMode(4, OUTPUT);

  
  MYSERIAL.begin(9600);

  seconds = 0;
  lastSecond = millis();

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


/*
 * Calculate the pressure value from sensors
 */
void calcPressure()
{
    /**
     * Simulate the pressure sensors
     * And read the data
     */
    analogWrite(4, 230);
    pressure[0]=analogRead(0);
}



//Prints the various variables directly to the port
//I don't like the way this function is written but Arduino doesn't support floats under sprintf
int sequenceNumber =0;

void printPressure()
{
  calcPressure(); //Go calc all the various sensors

  // print string for Pressure, separated by line for ease of reading
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
  // memset(buffer,'\0',sizeof(buffer));
  // strcat(buffer,"{");
  // strcat(buffer,"\"guid\":\"");
  // strcat(buffer,GUID2);
  // strcat(buffer,"\",\"organization\":\"");
  // strcat(buffer,Org);
  // strcat(buffer,"\",\"displayname\":\"");
  // strcat(buffer,Disp);
  // strcat(buffer,"\",\"location\":\"");
  // strcat(buffer,Locn);  
  // strcat(buffer,"\",\"measurename\":\"");
  // strcat(buffer,Measure2);
  // strcat(buffer,"\",\"unitofmeasure\":\"");
  // strcat(buffer,Units2);
  // strcat(buffer,"\",\"value\":");
  // strcat(buffer,dtostrf(pressure[1],6,2,dtostrfbuffer));
  // strcat(buffer,"}");
  // MYSERIAL.println(buffer);

  // print string for light, separated by line for ease of reading
  // memset(buffer,'\0',sizeof(buffer));
  // strcat(buffer,"{");
  // strcat(buffer,"\"guid\":\"");
  // strcat(buffer,GUID3);
  // strcat(buffer,"\",\"organization\":\"");
  // strcat(buffer,Org);
  // strcat(buffer,"\",\"displayname\":\"");
  // strcat(buffer,Disp);
  // strcat(buffer,"\",\"location\":\"");
  // strcat(buffer,Locn);  
  // strcat(buffer,"\",\"measurename\":\"");
  // strcat(buffer,Measure3);
  // strcat(buffer,"\",\"unitofmeasure\":\"");
  // strcat(buffer,Units3);
  // strcat(buffer,"\",\"value\":");
  // strcat(buffer,dtostrf(pressure[2],6,2,dtostrfbuffer));
  // strcat(buffer,"}");
  // MYSERIAL.println(buffer);

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





  
  


