#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

#define Password_Length 7
String Data;
String Master = "022004A";
LiquidCrystal lcd(12,11,A2,A3,A4,A5);
Servo myservo;
int lockOutput = 8;
byte data_count = 0;
char customKey;
int pos = 0;


const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};


byte rowPins[ROWS] = {0, 1, 2, 3};
byte colPins[COLS] = {4, 5, 6, 7};

Keypad customKeypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup(){
  lcd.begin(16,2);
  myservo.attach(9, 2000, 2400);
  ServoClose();
  lcd.print("Protected Door");
  Serial.println("Protected Door");
  loading("Loading");
  lcd.clear();
  pinMode(lockOutput,OUTPUT);
}

void loop(){
  lcd.setCursor(0,0);
  lcd.print("Enter Password");
  customKey = customKeypad.getKey();
  if(customKey){
    Data += customKey;
    lcd.setCursor(data_count,1);
    lcd.print(Data[data_count]);
    data_count++;
  }
  if (data_count == Password_Length){
    lcd.clear();
    if (Data == Master){
      //Correct Password
      Open();
   
    }
    else{
      lcd.print("Wrong Password");
      delay(500);

    }
    lcd.clear();
    clear_data();
  }
}
void clear_data(){
  data_count = 0;
  Data = "";
}
void ServoClose()
{
  for (pos = 90; pos >= 0; pos -= 10) { 
    myservo.write(pos);
  }
}

void ServoOpen()
{
  for (pos = 0; pos <= 90; pos += 10) {
    myservo.write(pos);  
  }
}
void loading (char msg[]) {
  lcd.setCursor(0, 1);
  lcd.print(msg);
  Serial.println(msg);

  for (int i = 0; i < 9; i++) {
    delay(1000);
    lcd.print(".");
    Serial.println(".");
  }
}
void Open(){
      ServoOpen();
      digitalWrite(lockOutput,HIGH);
      lcd.print("Door is Opened!");
      delay(5000);
      //delay(1000);
      loading("Waiting");
      lcd.clear();
      lcd.print("Time is up!");
      delay(500);
      lcd.clear();
      ServoClose();
      loading("Closed!");
      delay(500);
      digitalWrite(lockOutput,LOW);
}
