/*
 * This is the data logger code in the simplest form. It doesn't use Mosfet or a count. It just 
 */

#include <SD.h>
#include <SPI.h>

// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.

// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
// Teensy audio board: pin 10
// Teensy 3.5 & 3.6 on-board: BUILTIN_SDCARD
// Wiz820+SD board: pin 4
// Teensy 2.0: pin 0
// Teensy++ 2.0: pin 20
const int chipSelect = BUILTIN_SDCARD;
File dataFile;

void setup()
{
  //UNCOMMENT THESE TWO LINES FOR TEENSY AUDIO BOARD:
  //SPI.setMOSI(7);  // Audio shield has MOSI on pin 7
  //SPI.setSCK(14);  // Audio shield has SCK on pin 14
  
 // Open serial communications and wait for port to open:
  Serial.begin(9600);

  pinMode(0, INPUT); //vibration sensor
  pinMode(13, OUTPUT); //LED
  
  Serial.print("Initializing SD card...");
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

   dataFile = SD.open("datalog.txt", FILE_WRITE);
   if(dataFile){
      dataFile.println("TIME PRESSURE TEMP");
      dataFile.close();
   }
   Serial.println("TIME PRESSURE TEMP");
   digitalWrite(13, HIGH);
}

void loop()
{
  // make a string for assembling the data to log:
  String pressureString = "";
  String tempString = "";
  String timeString = "";
  unsigned long time;

  // read three sensors and append to the string:
  int pressureSensor = analogRead(A9);
  pressureString += String(pressureSensor);
  
  int tempSensor = analogRead(A8);
  tempString += String(tempSensor);

  bool vibSensor = digitalRead(0);

  time = millis();
  timeString += String(time);

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  delay(2);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(timeString);
    dataFile.print("    ");
    dataFile.print(pressureString);
    dataFile.print("    ");
    dataFile.print(tempString);
    if(vibSensor == HIGH){
      dataFile.print("    ");
      dataFile.print("vibration sensor triggered");
    }
    dataFile.println();
    dataFile.close();
    // print to the serial port too:
    Serial.print(timeString);
    Serial.print("    ");
    Serial.print(pressureString);
    Serial.print("    ");
    Serial.print(tempString);
    if(vibSensor == HIGH){
      Serial.print("    ");
      Serial.print("vibration sensor triggered");
    }
    Serial.println();
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  } 
}
