#include <SD.h>
#include <SPI.h>
const int chipSelect = BUILTIN_SDCARD;

File countFile;
int cnt = 0;

void setup() {
  //UNCOMMENT THESE TWO LINES FOR TEENSY AUDIO BOARD:
  SPI.setMOSI(7);  // Audio shield has MOSI on pin 7
  SPI.setSCK(14);  // Audio shield has SCK on pin 14
  Serial.begin(9600);

  Serial.print("Initializing SD card... ");
  if(!SD.begin(chipSelect)){
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("Card initialized.");
  //open and read exisiting count then delete
  countFile = SD.open("Count.txt", FILE_READ);
  delay(2);
  if(countFile){
    while(countFile.available()){
      cnt = countFile.read() + 1;
    }
    countFile.close();
    SD.remove("Count.txt");
    if(countFile){
      Serial.print("Fail to delete");
    }else{
      Serial.println("deleted");
    }
  }else{
    Serial.println("fail to read count");
  }
  //Create new count file with current count
  //SD.mkdir("Count.txt");
  delay(2);
  countFile = SD.open("Count.txt", FILE_WRITE);
  countFile.close();
  delay(2);
  if(countFile){
  countFile.write(cnt);
  countFile.close();
  }else{
    Serial.println("Fail to save count");
  }
}

void loop() {

  

}
