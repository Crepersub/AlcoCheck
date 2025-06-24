
#include <U8g2lib.h>

#define alcoPin 0

#define buttonPin 6
#define buzzerPin 7

#define redledPin 8
#define yellowledPin 9
#define greenledPin 3

char buffer[30];
const char outofseconds[] PROGMEM = " out of 3 seconds";

float RL = 10000.0; 
float R0 = 41000.0;  
float a = -0.48;    
float b = 1.58;

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0);

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin,OUTPUT);
  pinMode(redledPin,OUTPUT);
  pinMode(yellowledPin,OUTPUT);
  pinMode(greenledPin, OUTPUT);
  delay(1000);

  tone(buzzerPin,750,1000);

  u8g2.begin();
  u8g2.firstPage();
  do{
    
  } while(u8g2.nextPage());
  u8g2.setFont(u8g2_font_6x12_tf);

  for(int i = 0; i<128; i++){
    u8g2.firstPage();
    do{
      u8g2.drawFrame(0, 40, 128, 10); 
      u8g2.setFont(u8g2_font_6x12_tf);
      u8g2.drawStr(30,35,"Preparing..."); 
      u8g2.drawBox(0, 40, i, 10);
    } while(u8g2.nextPage());
    delay(1000);
  }
  delay(2000);
u8g2.firstPage();
    do{
      u8g2.drawFrame(10, 10, 110, 40);
      u8g2.drawStr(25, 35, "Ready to test.");
    } while(u8g2.nextPage());
}
 
void loop() {       
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
    int analogValue = ArrMax(measurements,3);
  	float RS = RL * (1023.0 - analogValue) / analogValue;
    float ratio = RS / R0;
    float alcoholPPM = pow(10, a * log10(ratio) + b);
    float BrAC = alcoholPPM / 500.0;
    float BAC = BrAC * 0.21 * 10;

    Serial.println(BAC);
    Serial.println(analogValue);
    u8g2.firstPage();
    do{
      u8g2.setFont(u8g2_font_6x12_tf);
      u8g2.drawFrame(10, 10, 110, 40);
      u8g2.setCursor(55, 35);
      u8g2.print(BAC);
    } while(u8g2.nextPage());
    digitalWrite(yellowledPin,LOW);
    if(BAC<0.5){
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
  u8g2.firstPage();
  do{
    snprintf(buffer,sizeof(buffer),"%d",seconds);
    u8g2.drawFrame(10, 40, 100, 10); 
    u8g2.drawBox(10, 40, seconds * 33, 10);
    u8g2.drawStr(5, 20, buffer);
    strcpy_P(buffer,outofseconds);
    u8g2.drawStr(10, 20, buffer);
  } while(u8g2.nextPage());

}
float ArrMax(int a[], int n){
  int max = a[0];
  for(int i = 0; i < n;i++){
    if(a[i]>max){
      max = a[i];
    }
  }
  return max;
}
