//////////////////////////////////////////////////////////////////////////////////////
//  Happy pi project from KVC
//  Update 1.2 add time mills
//////////////////////////////////////////////////////////////////////////////////////
#include <SPI.h> //  Serial Peripheral Interface 
#include <Wire.h> // WIre connect
#include <Adafruit_AM2315.h> // T/H Sensor
#include <Adafruit_GFX.h> //OLED display 1
#include <Adafruit_SSD1306.h> //OLED discplay 2

//เปิดขา-ตัวแปร
Adafruit_AM2315 am2315; //Call T/H sensor

const int sensor_pin = A0; // Call Soil Moduel on A0 pin
#define Relay 2 // Call relay on 2 pin

//OLED
//OLED define
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//Time
//delay
unsigned long period = 300000;
unsigned long last_time = 0;
///////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(10);  
  }
  Serial.println("AM2315 Test!");

  if (! am2315.begin()) {
    Serial.println("Sensor not found, check wiring & pullups!");
    while (1);
  }
  pinMode(Relay, OUTPUT);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display(); //Display logo
  //   delay(dht.getMinimumSamplingPeriod());
  //  delay(1000);
  display.clearDisplay();
}

float temperature, humidity; // Global Value T/H
float moisture_percentage; // Global Value Moisture

void loop() {
  if (! am2315.readTemperatureAndHumidity(&temperature, &humidity)) { //Check T/H is work
    Serial.println("Failed to read data from AM2315");
    return;
  }
  // IF T/H sensor work Print T/H
  Serial.print("Temp *C: "); Serial.print(temperature);
  Serial.print("  Hum %: "); Serial.println(humidity);

  //Moisture Sensor
  moisture_percentage = ( 100.00 - ( (analogRead(0) / 1023.00) * 100.00 ) ); //การคำนวนให้เป็น %
  Serial.print("Soil Moisture(in Percentage) = ");
  Serial.print(moisture_percentage);
  Serial.println("%");

  display.clearDisplay();
  oledDisplayHeader();

  oledDisplay(3, 5, 28, humidity, "%");
  oledDisplay(2, 70, 16, temperature, "C");
  oledDisplay(2, 70, 44, moisture_percentage, "%");
  display.display();

  ///////////////////////////////////////////////////////////////////////////////////////////
  // Relay Low = เปิด HIGH = ปิด
  // การคำนวนทั้งหมด
  /*
    if (temperature >= 32) // ถ้าอุนหภูมิ มากกว่า 32 รดน้ำ 2 วิ
    {
    digitalWrite(Relay, LOW); // เปิดน้ำ
    delay(2000);
    } else if (moisture_percentage >= 62 or temperature <= 30) //ถ้าน้ำชื้นมากกว่า 62 หรือ อุนหภูมิ น้อยกว่า 30
    {
    digitalWrite(Relay, HIGH); // ปิดน้ำ
    delay(2000);
    }
    digitalWrite(Relay, HIGH);  // ปิดน้ำ
    delay(1800000); // ดีเลย์ 30 นาที
  */
  ///////////////////////////////////////////////////////////////////////////////////////////
  if ( millis() - last_time > period) //ดีเลย์แบบไม่บล็อคการทำงานของ Loop 
  {
    last_time = millis();
    if (temperature >= 32) // ถ้าอุนหภูมิ มากกว่า 32 รดน้ำ 2 วิ
    {
      digitalWrite(Relay, LOW); // เปิดน้ำ    
      delay(2000);
    } else if (moisture_percentage >= 62 or temperature <= 30) //ถ้าน้ำชื้นมากกว่า 62 หรือ อุนหภูมิ น้อยกว่า 30
    {
      digitalWrite(Relay, HIGH); // ปิดน้ำ
      delay(2000);
    }
    digitalWrite(Relay, HIGH);  // ปิดน้ำ
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////
//ต่อไปเป็นเป็นคำนวนเกี่ยวกับจอ  ==ซึงผมก็ยังไม่ค่อยเข้าใจมันอยู่ดี เลยปล่อยๆไว้ไม่เป็นยุ่งกับมันครับ
//ถ้าจะแก้พวก อุนหภูมิ แก้ข้างบนได้เลย จริงๆทำเป็น void ก็ได้ มีคค่าเท่ากัน

void oledDisplayHeader() {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Humidity");
  display.setCursor(60, 0);
  display.print("Temperature");
}
void oledDisplay(int size, int x, int y, float value, String unit) {
  int charLen = 12;
  int xo = x + charLen * 3.2;
  int xunit = x + charLen * 3.6;
  int xval = x;
  display.setTextSize(size);
  display.setTextColor(WHITE);

  if (unit == "%")
  {
    display.setCursor(x, y);
    display.print(value, 0);
    display.print(unit);
  } else
  {
    if (value > 99)
    {
      xval = x;
    } else
    {
      xval = x + charLen;
    }
    display.setCursor(xval, y);
    display.print(value, 0);
    display.drawCircle(xo, y + 2, 2, WHITE); // print degree symbols (  )
    display.setCursor(xunit, y);
    display.print(unit);
  }

}
