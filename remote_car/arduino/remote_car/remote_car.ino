#include <Servo.h>  // подключаем библиотеку для работы с сервоприводом
#include <SoftwareSerial.h>

#define pinX A2    // ось X джойстика
#define pinY A1    // ось Y джойстика
#define swPin 2    // кнопка джойстика
#define ledPin 13  // светодиод на Pin 13

SoftwareSerial BTSerial (2,3);

Servo servo;
int dX = 0;
int dY = 0;

#define PIN_IN1 4
#define PIN_IN2 7
#define PIN_IN3 5
#define PIN_IN4 6

void LOG(String str, int val)
{
    //Serial.print(str);
    //Serial.println(val);
}

void setup() {
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);

  // Команда остановки двум моторам
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);

  pinMode(ledPin, OUTPUT);
  // настройка входных пинов от джойстика
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);

  pinMode(swPin, INPUT);
  digitalWrite(swPin, HIGH);

  servo.attach(9);  // привязываем сервопривод к аналоговому выходу 11

  Serial.begin(9600);
  Serial.println("Enter AT Commands");
  BTSerial.begin(9600);
}

int btValue = 0;
int X = 512;
int Y = 90;

void loop() {
  // put your main code here, to run repeatedly:
  if(BTSerial.available()){
    btValue = BTSerial.read();
    Serial.write(btValue);
  }

  if(Serial.available()){
    BTSerial.write(Serial.read());
  }

  // читаем значение с джойстика
  // Y влево-вправо, X вперёд-назад
  //int Y = analogRead(pinY);
  // if (Y < 512) {
  //   Y = map(Y, 0, 512, 50, 90);
  // } else {
  //   Y = map(Y, 512, 1023, 90, 130);
  // }

  // int X = analogRead(pinX);
  int movingForward = 0;
 

  // Left 0, Right 1024
  if(btValue == 'L')
  {
      Y = 50;
  }
  else if(btValue == 'R')
  {
    Y = 130;
  }

   // Moving forward / backward
  if(btValue == 'T')
  {
    X = 1024;
    movingForward = 1;
  }
  else if(btValue == 'X')
  {
    X = 0;
    movingForward = 1;
  }

  if(btValue == '0')
  {
    if(Y !=90)
    {
      Y = 90;
      btValue = 'Q';
    }
    else if( X != 512)
    {
      X = 512;
      btValue = 'Q';
    }
  }


  if (dY != Y) {
    dY = Y;

    LOG("dY: ", dY);

    servo.write(Y);  // поворачиваем серво от джойстик
  }

  if (dX != X) {
    dX = X;
  
    LOG("dX: ", dX);

    // Forward 1024, Backward 0
    // PIN_IN1, PIN_IN2 - RIGHT WHEEL
    // PIN_IN3, PIN_IN4 - LEFT WHEEL
    if (X < 412) {
      digitalWrite(PIN_IN1, HIGH);
      digitalWrite(PIN_IN2, LOW);
      digitalWrite(PIN_IN3, LOW);
      digitalWrite(PIN_IN4, HIGH);
    } else if (X > 612) {
      digitalWrite(PIN_IN1, LOW);
      digitalWrite(PIN_IN2, HIGH);
      digitalWrite(PIN_IN3, HIGH);
      digitalWrite(PIN_IN4, LOW);
    } else {
      digitalWrite(PIN_IN1, LOW);
      digitalWrite(PIN_IN2, LOW);
      digitalWrite(PIN_IN3, LOW);
      digitalWrite(PIN_IN4, LOW);
    }
  }
}
