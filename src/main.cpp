#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_SHT31.h>

// กำหนดขาพอร์ตสำหรับ Relay และ INPUT
#define RELAY_PIN 26
#define INPUT_PIN 35

// สร้างตัวแปรสำหรับ SHT31
Adafruit_SHT31 sht31 = Adafruit_SHT31();

void setup() {
  // ตั้งค่าขา RELAY_PIN เป็น OUTPUT
  pinMode(RELAY_PIN, OUTPUT);
  // ตั้งค่าขา INPUT_PIN เป็น INPUT
  pinMode(INPUT_PIN, INPUT);
  
  // เริ่มการสื่อสาร Serial
  Serial.begin(9600);
  
  // เริ่มการทำงานของ SHT31
  if (!sht31.begin(0x44)) { // 0x44 คือ I2C address ของ SHT31
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
}

void loop() {
  // อ่านสถานะจาก INPUT
  int inputState = digitalRead(INPUT_PIN);

  // สถานะของ Relay
  bool relayState;

  // หาก INPUT ถูกกด (สถานะเป็น HIGH)
  if (inputState == HIGH) {
    // เปิด Relay
    digitalWrite(RELAY_PIN, HIGH);
    relayState = true;
  } else {
    // ปิด Relay
    digitalWrite(RELAY_PIN, LOW);
    relayState = false;
  }

  // อ่านค่าอุณหภูมิและความชื้นจาก SHT31
  float temperature = sht31.readTemperature();
  float humidity = sht31.readHumidity();

  // ตรวจสอบการอ่านค่าจาก SHT31 ว่าสำเร็จหรือไม่
  if (!isnan(temperature) && !isnan(humidity)) {
    // แสดงค่าอุณหภูมิและความชื้นที่ Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" *C, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %RH");
  } else {
    Serial.println("Failed to read from SHT31");
  }

  // แสดงสถานะของ INPUT และ Relay ที่ Serial Monitor
  Serial.print("INPUT State: ");
  Serial.print(inputState == HIGH ? "HIGH" : "LOW");
  Serial.print(", Relay State: ");
  Serial.println(relayState ? "ON" : "OFF");

  // หน่วงเวลาเล็กน้อยเพื่อป้องกันการสั่นของ INPUT และการอ่านค่าซ้ำซ้อน
  delay(500);
}
