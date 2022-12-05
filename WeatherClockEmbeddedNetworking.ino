#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows
LiquidCrystal_I2C lcd1(0x03F, 16, 2); // I2C address 0x03F, 16 column and 2 rows

#include <Wire.h>
#include "SPI.h" //I2C und SPI
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP280.h"
#include <DHT.h>;
#include "DS1307.h"
#include <RTClib.h>

//Setup connection of the sensor
Adafruit_BMP280 bmp; // I2C
DS1307 clock;//define a object of DS1307 class

//Constants
#define DHTPIN 13    // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22 
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

//Variables
float pressure;   //To store the barometric pressure (Pa)
int altimeter;    //To store the altimeter (m) (you can also use it as a float variable)

float hum;  //Stores humidity value
float temp; //Stores temperature value

//Botton
int P1=6; //menu
int P2=7; //increase
int P3=8; //decrease
int hourCnt;
int minagg;
int yearCnt;
int meseagg;
int dayagg;
int menu =0;

int inputPin = 2;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
void setup()
{
  Serial.begin(9600);
  lcd.init(); // initialize the lcd
  lcd.backlight();

  lcd.setCursor(0, 0);         // move cursor to   (0, 0)
  lcd.print("Weather Clock");        // print message at (0, 0)
  lcd.setCursor(2, 1);         // move cursor to   (2, 1)
  lcd.print("by Hosa&Imron"); // print message at (2, 1)

  lcd1.init(); // initialize the lcd
  lcd1.backlight();

  lcd1.setCursor(0, 0);         // move cursor to   (0, 0)
  lcd1.print("Weather Clock");        // print message at (0, 0)
  lcd1.setCursor(2, 1);         // move cursor to   (2, 1)
  lcd1.print("by Hosa&Imron"); // print message at (2, 1)

  bmp.begin(); 
  dht.begin();

   pinMode(inputPin, INPUT);     // declare sensor as input
  

  // Botton
  pinMode(P1,INPUT_PULLUP);
  pinMode(P2,INPUT_PULLUP);
  pinMode(P3,INPUT_PULLUP);
  int menu=0;

  //Clock
  clock.begin();
  clock.fillByYMD(2022, 1, 22); //Jan 19,2013
  clock.fillByHMS(20, 15, 0); //15:28 30"
  clock.fillDayOfWeek(SAT);//Saturday
  clock.setTime();//write time to the RTC chip
  
 
}

void loop()
{
  val = digitalRead(inputPin);  // read input value
  if(val == HIGH) {
    lcd.backlight();
    lcd1.backlight();
      if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
    }
    else {
      lcd.noBacklight();
      lcd1.noBacklight();
       if (pirState == HIGH){
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
      }
   
   //Read values from the sensor:
  pressure = bmp.readPressure();
  //temperature = bmp.readTemperature();
  altimeter = bmp.readAltitude (1030); //Change the "1050.35" to your city current barrometric pressure (https://www.wunderground.com)

  //Read data and store it to variables hum and temp
  hum = dht.readHumidity();
  temp= dht.readTemperature();  
  
  //Display 1
  lcd1.clear();
  lcd1.setCursor(0, 1);
  lcd1.print("hPa:");
  lcd1.print(pressure/100); 
  lcd1.setCursor(8, 1);
  lcd1.print(" alt:");
  lcd1.print(altimeter);

  lcd1.setCursor(0, 0);
  lcd1.print("tmp:");
  lcd1.print(temp); 
  lcd1.setCursor(6, 0);
  lcd1.print((char)223);
  lcd1.print("C hum:");
  lcd1.print(hum);
  lcd1.setCursor(15, 0);
  lcd1.print("%");
   
  if(digitalRead(P1) == LOW)
  {
   menu=menu+1;
  }
  
  if (menu==0)
  {
  DisplayDateTime();

  }
  if (menu==1)
  {
    DisplaySetHour();
   
  }
  if (menu==2)
  {
   DisplaySetMinute();
  }
  if (menu==3)
  {
    DisplaySetYear();
  
  }
  if (menu==4)
  {
   DisplaySetMonth();
   
  }
  if (menu==5)
  {
    DisplaySetDay();
    
  }
  if (menu==6)
  {
    StoreAgg(); 
    delay(500);
    menu=0;
  }
  delay(100);
}

void DisplayDateTime ()
{
   clock.getTime();
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Time: ");
   if (clock.hour<=9)
    {
    lcd.print("0");
    }
    lcd.print(clock.hour, DEC);
    hourCnt=clock.hour;
    lcd.print(":");
  if (clock.minute<=9)
  {
    lcd.print("0");
  }
  lcd.print(clock.minute, DEC);
  minagg=clock.minute;
  lcd.print(":");
  if (clock.second<=9)
  {
    lcd.print("0");
  }
  lcd.print(clock.second, DEC);

  lcd.setCursor(0, 1);
  lcd.print("Date: ");
  
  if (clock.dayOfMonth<=9)
  {
    lcd.print("0");
  }
  lcd.print(clock.dayOfMonth, DEC);
  dayagg=clock.dayOfMonth;
  lcd.print("/");
  if (clock.month<=9)
  {
    lcd.print("0");
  }
  lcd.print(clock.month, DEC);
  meseagg=clock.month;
  lcd.print("/");
  lcd.print(clock.year, DEC);
  yearCnt=clock.year;
}


void DisplaySetHour()
{
  lcd.clear();
  if(digitalRead(P2) == LOW)
  {
    if(hourCnt>=23)
    {
      hourCnt=0;
    }
    else
    {
      hourCnt=hourCnt+1;
    }
  }
   if(digitalRead(P3) == LOW)
  {
    if(hourCnt<=0)
    {
      hourCnt=23;
    }
    else
    {
      hourCnt=hourCnt-1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("Set Hour:");
  lcd.setCursor(0,1);
  lcd.print(hourCnt,DEC);
  delay(1000);
}


void DisplaySetMinute()
{
  lcd.clear();
  if(digitalRead(P2)==LOW)
  {
    if (minagg==59)
    {
      minagg=0;
    }
    else
    {
      minagg=minagg+1;
    }
  }
   if(digitalRead(P3)==LOW)
  {
    if (minagg==0)
    {
      minagg=59;
    }
    else
    {
      minagg=minagg-1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("Set Minutes:");
  lcd.setCursor(0,1);
  lcd.print(minagg,DEC);
  delay(200);
}
  
void DisplaySetYear()
{
  lcd.clear();
  if(digitalRead(P2)==LOW)
  {    
    yearCnt=yearCnt+1;
  }
   if(digitalRead(P3)==LOW)
  {
    yearCnt=yearCnt-1;
  }
  lcd.setCursor(0,0);
  lcd.print("Set Year:");
  lcd.setCursor(0,1);
  lcd.print(yearCnt,DEC);
  delay(200);
}

void DisplaySetMonth()
{
  lcd.clear();
  if(digitalRead(P2)==LOW)
  {
    if (meseagg==12)
    {
      meseagg=1;
    }
    else
    {
      meseagg=meseagg+1;
    }
  }
   if(digitalRead(P3)==LOW)
  {
    if (meseagg==12)
    {
      meseagg=12;
    }
    else
    {
      meseagg=meseagg-1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("Set Month:");
  lcd.setCursor(0,1);
  lcd.print(meseagg,DEC);
  delay(200);
}

void DisplaySetDay()
{
  lcd.clear();
  if(digitalRead(P2)==LOW)
  {
    if (dayagg==31)
    {
      dayagg=1;
    }
    else
    {
      dayagg=dayagg+1;
    }
  }
   if(digitalRead(P3)==LOW)
  {
    if (dayagg==1)
    {
      dayagg=31;
    }
    else
    {
      dayagg=dayagg-1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("Set Day:");
  lcd.setCursor(0,1);
  lcd.print(dayagg,DEC);
  delay(200);
}

void StoreAgg()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SAVING");
  lcd.setCursor(0,1);
  lcd.print("IN PROGRESS");
  clock.fillByYMD(yearCnt, meseagg, dayagg); //Jan 19,2013
  clock.fillByHMS(hourCnt, minagg, 0); //15:28 30"
  clock.setTime();//write time to the RTC chip
  delay(200);
}
