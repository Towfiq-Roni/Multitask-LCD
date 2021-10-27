#include <LiquidCrystal.h>

const int btn = 8;
int sensorPin = 0;

int game = 0;
int buttonPress = 0; 
int buttonState = 0;
int obstaclePos;
int obsDelay;
int score = 0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; 
int lastButtonState = LOW;
int second = 0, minute = 0, hour = 0;

//Graphics
byte runnin1[8] = {
  0b01100,
  0b01100,
  0b00000,
  0b11100,
  0b01110,
  0b01100,
  0b11110,
  0b00011
};
byte runnin2[8] = {
  0b00110,
  0b00110,
  0b00000,
  0b00111,
  0b01110,
  0b00110,
  0b01110,
  0b00101
};
byte jump[8] = {
  0b00100,
  0b01100,
  0b11100,
  0b01110,
  0b01100,
  0b11111,
  0b00000,
  0b00001
};
byte def1[8] = {
  0b00100,
  0b00110,
  0b00111,
  0b01110,
  0b00110,
  0b01111,
  0b01000,
  0b00000
};
byte def2[8] = {
  0b00100,
  0b00110,
  0b00111,
  0b01110,
  0b00110,
  0b00110,
  0b00110,
  0b00010
};
byte slide[8] = {
  0b00000,
  0b00000,
  0b00011,
  0b00011,
  0b00000,
  0b01111,
  0b11111,
  0b00000
};

 
LiquidCrystal lcd(4,6,10,11,12,13);
void setup(){

  Serial.begin(9600);
  pinMode(btn,INPUT);
  lcd.begin(16,2);
  lcd.createChar(0,runnin1);
  lcd.createChar(1,runnin2);
  lcd.createChar(2,jump);
  lcd.createChar(3,def1);
  lcd.createChar(4,def2);
  lcd.createChar(5,slide);
  buttonState=digitalRead(btn);
}

void loop(){
  intro();
  while(game==1){
      ballActive();
  }
  endGame(); 
}

 void intro(){
   score=0;
   lcd.clear();
   lcd.setCursor(7,1);
   lcd.clear();
   while(button()!=1){
     lcd.setCursor(0,0);
     delay(100);
     int reading=analogRead(sensorPin);
     float voltage=reading*4.68;
     voltage/=1024.0;
     
     float temp=(voltage-0.5)*100;
     lcd.setCursor(9,0);
     lcd.print(temp);
     lcd.print("C");
     
     lcd.setCursor(0,1);
     lcd.print(19);
     
     lcd.setCursor(2,1);
     lcd.print("/");
     
     lcd.setCursor(3,1);
     lcd.print("08");
     
     lcd.setCursor(5,1);
     lcd.print("/");
     
     lcd.setCursor(6,1);
     lcd.print("2021");
     
     lcd.setCursor(0,0);
     lcd.print(hour);
     
     lcd.setCursor(2,0);
     lcd.print(":");
     
     lcd.setCursor(3,0);
     lcd.print(minute);
     
     lcd.setCursor(5,0);
     lcd.print(":");
     
     lcd.setCursor(6,0);
     lcd.print(second);
     
     second+=1;
     if(second==60){
       minute+=1;
       second=0;
     }
     if(minute==60){
       hour+=1;
       minute=0;
     }
     if(hour==24){
       hour=0;
       minute=0;
       second=0;
     }
     delay(1000);
   }
   if(button()==1){
     game=1;
   }
 }

 void reset(){
      if(obstaclePos%2==0)
      {
          lcd.clear();
          lcd.setCursor(0,1);
          lcd.write(byte(0));
      }
      else
      {
          lcd.clear();
          lcd.setCursor(0,1);
          lcd.write(1);
      }
 }

 void ballActive()
 {
   
  obsDelay=200;
  obstaclePos=15;
  while(game==1)
  {
       reset();
       if(button()==1)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.write(2);
          obstacle(obstaclePos);
          delay(400);
          lcd.clear();
          lcd.setCursor(0,1);
          lcd.write(1);
          obstacle(obstaclePos);  
          
        }
        else
        {
          if(obstaclePos!=0){
              reset();
              obstacle(obstaclePos);
          }
          else if(obstaclePos==0)
          {
             game=0;
             break;
          }
        }
        obstaclePos--;
        if(obstaclePos<0)
        {                             
          obsDelay=obsDelay-20;
          obstaclePos=15;
          score++;
        }
        if(obsDelay==0)
            game=0;
             
        delay(obsDelay);
    }
  }
int button() 
{
     int reading = digitalRead(btn);
     if (reading != lastButtonState) {
       lastDebounceTime = millis();
     } 
     
     if ((millis() - lastDebounceTime) > debounceDelay) {
       buttonState = reading;     
       if (buttonState == HIGH) {
           lastButtonState = reading;
           return 1;
         }
       }
     lastButtonState = reading;
}
void obstacle(int i)
{ 
          if(obstaclePos%2==0&& obstaclePos>2)
          {
              lcd.setCursor(i,1);
              lcd.write(3);
          }
          else if(obstaclePos%2!=0&& obstaclePos>2)
          {
              lcd.setCursor(i,1);
              lcd.write(4);
          }
          else
          {
              lcd.setCursor(i,1);
              lcd.write(5);
          }
}
void endGame()
{
    if(score==10)
    {
        lcd.clear();
        lcd.print("Congratulations!");
        lcd.setCursor(0,1);
        lcd.print("You beat 'em all");
        delay(3000);
    }
    else if(score<3)
    {
        lcd.clear();
        lcd.print("Boooo");
        lcd.setCursor(0,1);
        lcd.print("Score: ");
        lcd.print(score);
        delay(3000);
    }
    else if(score<6)
    {
        lcd.clear();
        lcd.print("Not bad!");
        lcd.setCursor(0,1);
        lcd.print("Score: ");
        lcd.print(score);
        delay(3000);
    }
    else
    {
        lcd.clear();
        lcd.print("Great!");
        lcd.setCursor(0,1);
        lcd.print("Score: ");
        lcd.print(score);
        delay(3000);
    }
}
