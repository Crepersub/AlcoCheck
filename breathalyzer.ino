#include <U8g2lib.h>

#define csPin 10
#define dcPin 8
#define resPin 9

#define alcoPin 0

#define buttonPin 6
#define buzzerPin 7

#define redledPin 2
#define yellowledPin 3
#define greenledPin 4

// Constructor for SH1106 128x64 SPI display
U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ csPin, /* dc=*/ dcPin, /* reset=*/ resPin);

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin,OUTPUT);
  pinMode(redledPin,OUTPUT);
  pinMode(yellowledPin,OUTPUT);
  pinMode(greenledPin, OUTPUT);
  tone(buzzerPin,750,1000);
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_9x15_t_cyrillic);  // Choose a suitable font
  u8g2.drawStr(0,10,"Warming up.");
  u8g2.sendBuffer();
  delay(20000);
  u8g2.clearBuffer();
  u8g2.drawStr(0, 10, "Waiting...");
  u8g2.sendBuffer();
}
void loop() {                  // Transfer internal memory to the display
  if(digitalRead(buttonPin) == HIGH){
    stopLEDs();
    digitalWrite(yellowledPin, HIGH);
    int counter = 1;
    int measurements[3];
    for(int i = 0; i < 3; i++){
      measurements[i] = analogRead(alcoPin);
      printTimer(counter);
      counter++;
      delay(1000);
    }
    float avgAlco = ArrAvg(measurements, 3);
    float alcovalue = ((avgAlco / 10) * (5.0 / 1024.0))*0.67;
    u8g2.clearBuffer();
    u8g2.drawStr(0, 10, String(measurements[0]).c_str());
    u8g2.drawStr(0, 25, String(measurements[1]).c_str());
    u8g2.drawStr(0, 40, String(measurements[2]).c_str());
    u8g2.drawStr(0, 55, String(alcovalue).c_str());
    u8g2.sendBuffer();
    digitalWrite(yellowledPin,LOW);
    if(alcovalue<0.5){
      digitalWrite(greenledPin,HIGH);
      tone(buzzerPin,1000,1000);
    }
    else{
      digitalWrite(redledPin,HIGH);
      tone(buzzerPin,500,1000);
      delay(500);
      tone(buzzerPin,1500,500);
      delay(500);
      tone(buzzerPin,500,1000);
      delay(500);
      tone(buzzerPin,1500,500);
    }
  }
}
void stopLEDs(){
  digitalWrite(redledPin,LOW);
  digitalWrite(yellowledPin,LOW);
  digitalWrite(greenledPin,LOW);
}
void printTimer(int seconds){
  u8g2.clearBuffer();
  u8g2.drawStr(0, 10, String(seconds).c_str());
  u8g2.drawStr(5, 10, " out of 3 seconds");
  u8g2.sendBuffer();
}
void printResults(int orgvalue, float idk){
  u8g2.clearBuffer();
  u8g2.drawStr(0 ,20, String(idk).c_str());
  u8g2.drawStr(30, 20, "mgL");
  u8g2.drawStr(0, 40, String(orgvalue).c_str());
  u8g2.sendBuffer();
}
float ArrAvg(int a[], int n){
  int sum = 0;
  for(int i = 0; i < n;i++){
    sum += a[i];
  }
  return (float)sum/n;
}
