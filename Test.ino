/* HappyPig KVC Project */
/* Update 2.1 Code for newSensor module */
/* Fix bug!!! */
/* Libary */
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>

/* Constructor */
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
LiquidCrystal_I2C lcd(0x27, 16, 2);

/* Value */
float Ambient_forcon;
float Object_forcon;
char Ambient_Temp[4];
char Object_Temp[4];

/* Relay */
#define Relay 2

/* Time Stamp */
unsigned long period = 300000;
unsigned long last_time = 0;

void setup(void) {
  Serial.begin(9600);
  mlx.begin();
  setuplcd();
  pinMode(Relay,OUTPUT);
}

void setuplcd() {
  lcd.begin();
  lcd.backlight();
}

void 

void loop(void) {
  /* Serial Print */
  Serial.print("Ambient = "); 
  Serial.print(mlx.readAmbientTempC()); 
  Serial.print("*C\tObject = "); 
  Serial.print(mlx.readObjectTempC()); 
  Serial.println("*C");
  Serial.print("Ambient = "); 
  Serial.print(mlx.readAmbientTempF()); 
  Serial.print("*F\tObject = "); 
  Serial.print(mlx.readObjectTempF()); 
  Serial.println("*F");
  Serial.println();

  /* Read value and Change DB to String */
  Ambient_forcon = mlx.readAmbientTempC();
  Object_forcon = mlx.readObjectTempC();
  
  /* Loop ON */
  if ( millis() - last_time > period) //ดีเลย์แบบไม่บล็อคการทำงานของ Loop
  {
    last_time = millis();
    if (Ambient_forcon >= 32) // ถ้าอุนหภูมิ มากกว่า 32 รดน้ำ 2 วิ
    {
      digitalWrite(Relay, LOW); // เปิดน้ำ
      delay(2000);
    } else if (Ambient_forcon <= 30) //ถ้าน้ำชื้นมากกว่า 62 หรือ อุนหภูมิ น้อยกว่า 30
    {
      digitalWrite(Relay, HIGH); // ปิดน้ำ
      delay(2000);
    }
    digitalWrite(Relay, HIGH);  // ปิดน้ำ
  }
  digitalWrite(Relay, HIGH);
  delay(1000);
  digitalWrite(Relay, LOW); // เปิดน้ำ
  delay(1000);
}
