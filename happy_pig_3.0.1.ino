/////////////////////////////////
//  Happy pig project from KVC //
//  Update 3.0.1 new codeing //
/////////////////////////////////
#include <SPI.h> //  Serial Peripheral Interface 
#include <Wire.h> // WIre connect
#include <Adafruit_AM2315.h> // T/H Sensor
#include <LiquidCrystal_I2C.h> // LCD

//เปิดขา-ตัวแปร
Adafruit_AM2315 am2315; //Call T/H sensor

#define Relay 8

LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long period = 300000; // 5 minit
unsigned long last_time = 0;

void setuplcd() {
  lcd.begin();
  lcd.backlight();
}

void setuplcd_value() {
  lcd.setCursor(0, 0);
  lcd.print("Temperature:");
  lcd.setCursor(0, 1);
  lcd.print("Humidity:");
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(10);
  }
  Serial.println("Happy Pig Preject");

  if (! am2315.begin()) {
    Serial.println("Sensor not found, check wiring & pullups!");
    while (1);
  }

  // begin() does a test read, so need to wait 2secs before first read
  delay(2000);
  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, HIGH);
  setuplcd();
  setuplcd_value();
}

void loop() {
  float temperature, humidity;

  if (! am2315.readTemperatureAndHumidity(&temperature, &humidity)) {
    Serial.println("Failed to read data from AM2315");
    return;
  }
  // IF T/H sensor work Print T/H
  Serial.print("Temp *C: "); Serial.print(temperature);
  Serial.print("  Hum %: "); Serial.println(humidity);

  //Lcd Print
  lcd.setCursor(13, 0);
  lcd.print(temperature);
  lcd.setCursor(15, 0);
  lcd.print("C");
  lcd.setCursor(13, 1);
  lcd.print(humidity);
  lcd.setCursor(15, 1);
  lcd.print("%");

  if ( millis() - last_time > period) //ดีเลย์แบบไม่บล็อคการทำงานของ Loop
  {
    last_time = millis();
    if (temperature >= 32) // ถ้าอุนหภูมิ มากกว่า 32 รดน้ำ 2 วิ
    {
      digitalWrite(Relay, LOW); // เปิดน้ำ
      delay(10000);
    } else if (temperature <= 30 or humidity >= 80) //ถ้าน้ำชื้นมากกว่า 62 หรือ อุนหภูมิ น้อยกว่า 30
    {
      digitalWrite(Relay, HIGH); // ปิดน้ำ
      delay(10000);
    }
    digitalWrite(Relay, HIGH);  // ปิดน้ำ
  }
  delay(2000);
}
