/* HappyPig KVC Project */
/* Update 2.1 Code for newSensor module */
/* Fix bug!!! */
/* Libary */
#include <Arduino.h>
#include <U8x8lib.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>

/* Constructor */
U8X8_SSD1327_EA_W128128_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

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

/* u8x8.begin() is required and will sent the setup/init sequence to the display */
void setup(void) {
  Serial.begin(9600);
  mlx.begin();
  u8x8.begin();
  u8x8.setPowerSave(0);
  pinMode(Relay,OUTPUT);
}

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
  /* Set font */
  u8x8.setFont(u8x8_font_chroma48medium8_r); //Font
  /* Name project */
  u8x8.setInverseFont(1); //ตัวทีบ
  u8x8.draw2x2String(0, 0, "HappyPig"); //Name Projecr
  /* Ambient Temp */
  u8x8.setInverseFont(0);//ตัวโปล่งแสง
  u8x8.draw1x2String(0, 2, "Ambient Temp:"); //Ambient Temp
  u8x8.draw1x2String(10, 4, String(Ambient_forcon).c_str());
  u8x8.draw1x2String(15, 4, "C"); //Ambient Temp C
  /* Object Temp */
  u8x8.draw1x2String(0, 7, "Object Temp :"); //Object Temp
  u8x8.draw1x2String(10, 9, String(Object_forcon).c_str());
  u8x8.draw1x2String(15, 9, "C"); //Object temp C
  /* KVC Footer */
  u8x8.setInverseFont(1);
  u8x8.draw2x2String(7, 11, "KVC"); //KVC footter
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
