/***************************************************************
*
*   Programme de gestion d'une Horloge Temps Réel DS3231
*   https://www.maximintegrated.com/en/products/digital/real-time-clocks/DS3231.html
*   https://learn.adafruit.com/adafruit-ds3231-precision-rtc-breakout/arduino-usage
*
*   Auteur : MS
*   Date   : 21/06/2018
*   Version: 0.2
*
*   Connexion :
*    - I2C
*
*   Librairie :
*    - Wire.h : gestion du bus I2C
*    - DS3231.h : Gestion de la RTC et des classes RTClib et DateTime
*
*   Description : Deux modes de fonctionnement :
*   - Mettre à jour la RTC en lui passant un timestamp
*     suivit d'un retour à la ligne sur le port série
*     -> Activer #define TIMESTAMP
*   - Mettre à jour la RTC en lui passant une chaine formatée
*     suivie d'un retour à la ligne : YYMMDDHHmmSS\n
*     -> Activer #define DATETIME
*
*   Exemple :
*    1499800260\n : TIMESTAMP  => 11/07/2017 19:11:00
*    180621172040\n : DATETIME => 21/06/2018 17:20:40
*
********************************************************************/
#include <DS3231.h>
#include <Wire.h>

#define DATETIME
//#define TIMESTAMP

RTClib RTC;
DateTime now;
DS3231 Clock;



byte Year;
byte Month;
byte Date;
byte Hour;
byte Minute;
byte Second;


void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {

  // If something is coming in on the serial line, it's
  // a time correction so set the clock accordingly.
  if (Serial.available())
  {
    String frame = Serial.readStringUntil('\n');
    Serial.println(frame);

#ifdef DATETIME
    Year  = (byte)(frame.substring(0, 2).toInt());
    Month = (byte)(frame.substring(2, 4).toInt());
    Date  = (byte)(frame.substring(4, 6).toInt());
    Hour  = (byte)(frame.substring(6, 8).toInt());
    Minute = (byte)(frame.substring(8, 10).toInt());
    Second = (byte)(frame.substring(10, 12).toInt());
#endif

#ifdef TIMESTAMP
    uint32_t t = atol(frame.c_str());
    DateTime myDate(t);

    Year  = (byte)(myDate.year() - 2000);
    Month = (byte)myDate.month();
    Date  = (byte)myDate.day();
    Hour  = (byte)myDate.hour();
    Minute = (byte)myDate.minute();
    Second = (byte)myDate.second();
#endif

    Clock.setClockMode(false);	// set to 24h
    //setClockMode(true);	// set to 12h

    Clock.setYear(Year);
    Clock.setMonth(Month);
    Clock.setDate(Date);
    Clock.setHour(Hour);
    Clock.setMinute(Minute);
    Clock.setSecond(Second);
  }
  now = RTC.now();
  uint32_t ts = now.unixtime();
  String horodate = (String)ts + " = " + (String)now.day() + "/" + (String)now.month() + 
                           "/" + (String)now.year() + " " + (String)now.hour() + 
                           ":" + (String)now.minute() + ":" + (String)now.second();
  Serial.println(horodate);

  delay(1000);
}
