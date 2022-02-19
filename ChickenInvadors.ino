/*
* LED 8x8 DotMarix using Arduino
* Created by TheCircuit
*/
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978 

#include <LedControl.h>


// notes in the melody:
int melody[] = {
  NOTE_AS4, NOTE_C5 , NOTE_DS5, NOTE_DS5,0, NOTE_AS4, NOTE_C5, 
  NOTE_DS5, NOTE_F5, 0, NOTE_G5, NOTE_F5, NOTE_DS5, NOTE_C5,
  NOTE_G5, NOTE_F5, NOTE_DS5, NOTE_DS5, 0
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 3, 8, 2, 3, 4, 4, 4, 2, 3, 4, 8, 8, 4, 4, 2, 4, 2, 4
};

int introMelody[] = {
  NOTE_C3, NOTE_E3, NOTE_G3, NOTE_C4,0, NOTE_G3, NOTE_C4, 0,
};
int spawnMelody[]{ NOTE_G3,0,0};
int spawnMelodyDurations[] = {4,8,8};
// note Nurations: 4 = quarter note, 8 = eighth note, etc.:
int introNoteDurations[] = {
  8, 8, 8, 8, 8, 8, 2, 8
};

int DIN = 8; 
int CS =  9;
int CLK = 10; 

byte spaceship[8] = {B00111000,B00010000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000};
byte screenAtemp[8] = {B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000};
byte screenBtemp[8] = {B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000};
byte a[8] = {B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000};

LedControl lc=LedControl(DIN,CLK,CS,1);
LedControl lc2=LedControl(12,13,11,1);

const int VRyPin = A4;
const int VRxPin = A3;
const int SWPin  = 3;

int VRx = 0;        // value read from the horizontal pot
int VRy = 0;        // value read from the vertical pot
int SW = 0;         // value read from the switch
int Period = 1000;
int LevelUp = 3000;
unsigned long time_now_two = 0;
int PeriodTwo = 2000;
unsigned long time_now = 0;

unsigned long time_now_three = 0;
int LED1 = 4;
int LED2 = 5;
int LED3 = 7;

int tempInt = 0;
bool temp = false;
bool gameOn = true;

void setup(){
   pinMode(LED1, OUTPUT);
   pinMode(LED2, OUTPUT);
   pinMode(LED3, OUTPUT);

   analogWrite(LED1, 5);
   analogWrite(LED2, 5);
   analogWrite(LED3, 5);
     
   Serial.begin(9600);
   pinMode(SWPin,INPUT_PULLUP);
   
   lc.shutdown(0,false);       //The MAX72XX is in power-saving mode on startup
   lc.setIntensity(0,2);      // Set the brightness to maximum value
   lc.clearDisplay(0);         // and clear the display
  
   lc2.shutdown(0,false);       //The MAX72XX is in power-saving mode on startup
   lc2.setIntensity(0,2);      // Set the brightness to maximum value
   lc2.clearDisplay(0);         // and clear the display
   playSong(introMelody,introNoteDurations);
   spawnAsteroids();
   printByte(spaceship);

}

void loop(){ 
    //downSpaceShip();
    //delay(1000);
    if (gameOn){
        VRx = analogRead(VRxPin);
        //VRy = analogRead(VRyPin);
        //SW = digitalRead(SWPin);
        //Serial.println(millis());
        if(millis() >= time_now_three + LevelUp){
          if ( Period>100){
            time_now_three += LevelUp;
            Period = Period - 100;
            PeriodTwo = PeriodTwo - 200;
          }
          else{
            }
        }
        // update the board every Period seconds
        if(millis() >= time_now + Period){
             temp = true;
             time_now += Period;
             tickScreen();
             
          }
        if(millis() >= time_now_two + PeriodTwo){
            time_now_two += PeriodTwo;
            spawnAsteroids(); 
        }
        moveSpaceShip(VRx);
        delay(100);
    }
    else{
      Serial.println("GameOver");
      }
}

void displayOneTick(){
    for (int displayOne = 7; displayOne>=0; displayOne --){
      if(displayOne > 3){
            screenAtemp[displayOne-2] = spaceship[displayOne];
       }
      if(displayOne <= 3){ 
        //interactions with the spaceship
        // just learned how to compare bits at specific addressses using bit masks
          if(((spaceship[1] & spaceship[2]) == spaceship[1])&& temp == true){
            takeDamage();
            temp = false;
            }
            }
        }
    }
void takeDamage(){
  if(tempInt == 0){
     analogWrite(LED1, 0);
     tone(2, NOTE_G3, 250);
     delay(100);
     noTone(2);
     tempInt = tempInt + 1;
  }
  else if(tempInt == 1){
     analogWrite(LED2, 0);
     tone(2, NOTE_G3, 250);
     delay(100);
     noTone(2);
     tempInt = tempInt + 1;
  }
  else if(tempInt == 2){
     analogWrite(LED3, 0);
     tempInt = tempInt + 1;
     tone(2, NOTE_G3, 250);
     delay(100);
     noTone(2);
     gameOn = false;
  }
  }
     
void displayTwoTick(){
  for (int displayTwo = 7; displayTwo>=0; displayTwo --){
    if(displayTwo >= 2){
        screenBtemp[displayTwo-2] = a[displayTwo];
     }
    else if (displayTwo==1){
        screenAtemp[7] = a[displayTwo];
    }
    else if (displayTwo==0){
        screenAtemp[6] = a[displayTwo];
    }
  }
}

void tickScreen(){
   displayTwoTick();
   displayOneTick();
   
   for(int temp = 7; temp>=0; temp-- ){
       a[temp] = screenBtemp[temp];
   }
   for(int temp2 = 7; temp2>=2; temp2-- ){
       spaceship[temp2] = screenAtemp[temp2];
   }
   
   printByte(spaceship);
   printByte2(a);
   resetVars();
  }
  
void resetVars(){
  byte ScreenAtemp[8] = {B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000};
  byte screenBtemp[8] = {B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000};
  }
  
void printByte(byte character [])
{
  int i = 0;
  for(i=0;i<8;i++)
  {
    lc.setRow(0,i,character[i]);
  }
}
void printByte2(byte character [])
{
  int i = 0;
  for(i=0;i<8;i++)
  {
    lc2.setRow(0,i,character[i]);
  }
}

void spawnAsteroids(){
  int seed = random(0, 4);
  //Serial.println(seed);
  if(seed == 0){
      a[7] = B00000011;
      a[6] = B00000011;
      printByte2(a);
  }
  else if(seed == 1){
      a[7] = B00001100;
      a[6] = B00001100;
      printByte2(a);
  }
  else if(seed == 2){
      a[7] = B00110000;
      a[6] = B00110000;
      printByte2(a);
  }
  else if(seed == 3){ 
      a[7] = B11000000;
      a[6] = B11000000;
      printByte2(a);
  }
}
  
void playSong(int melodi[], int durations[]){
    // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / durations[thisNote];
    tone(2, melodi[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(2);
    }
}
void moveSpaceShip(int vector){
  if (vector>700){
    downSpaceShip();
    }
  if (vector<300){
    upSpaceShip();
    }
  }
void bulletTravel(){
  for(int i = 2; i<=7; i++){
    if (millis() == time_now + Period){
    time_now += Period;
      if (i!=2){
        spaceship[i] = spaceship[i] + spaceship[i-1];
        spaceship[i-1] = B00000000;
        printByte(spaceship);
        }
      else{
        spaceship[i] = spaceship[i] + spaceship[i-1];
        printByte(spaceship);
      }
    }
  }
}   
void upSpaceShip(){
  if (spaceship[0] == B00000011){
    //spaceship is at the top of the screen
  }
  else if (spaceship[0] == B11000000){
    //spaceship is at the bottom of the screen
    spaceship[0] = B11100000;
    spaceship[1] = spaceship[1] >> 1;
    printByte(spaceship);
  }
  else{
    spaceship[0] = spaceship[0] >> 1;
    spaceship[1] = spaceship[1] >> 1;
    printByte(spaceship);
  } 
}
void downSpaceShip(){
  if (spaceship[0] == B11000000){
  }
  else if (spaceship[0] == B00000011){
    spaceship[0] = B00000111;
    spaceship[1] = spaceship[1] << 1;
    printByte(spaceship);
  }
  else{
    spaceship[0] = spaceship[0] << 1;
    spaceship[1] = spaceship[1] << 1;
    printByte(spaceship);
  }
}
