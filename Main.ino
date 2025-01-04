#define BLYNK_TEMPLATE_ID "TMPL6SfcvJ7MT"                    // Your Blynk IoT Template ID
#define BLYNK_TEMPLATE_NAME "Projek besar"                   // Your Blynk IoT Template Name
#define BLYNK_AUTH_TOKEN "9zH8fRFa2ly4OOjmtr79bTFflra6yGxN"  // Your Blynk IoT Auth Token

#include <Wire.h>
#include <PCF8574.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <BlynkSimpleEsp8266.h>  // Library Blynk untuk ESP8266
#include <LiquidCrystal_I2C.h>   // Library LCD I2C

// Ganti dengan informasi jaringan WiFi Anda
char ssid[] = "esp";
char pass[] = "00000000";

// Pin konfigurasi
#define SS_PIN D4
#define RST_PIN D3
#define SERVO_PIN_IN D0
#define SERVO_PIN_OUT D8

// PCF8574 I2C address
#define PCF8574_ADDR 0x20

// LCD I2C address
#define LCD_ADDR 0x27
LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2); // LCD dengan alamat 0x27, ukuran 16x2

// Konfigurasi parkir
#define MAX_CARS 4
#define V_PIN_CAR_COUNT V1
#define uuuu V2

// Objek PCF8574, RFID, dan Servo
PCF8574 pcf8574(PCF8574_ADDR);
MFRC522 rfid(SS_PIN, RST_PIN);
Servo servoIn, servoOut;

// Struktur kartu RFID
struct RFID_Card {
  byte uid[4];
  bool hasEntered;
};
RFID_Card cards[MAX_CARS];
int carCount = 0;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  servoIn.attach(SERVO_PIN_IN);
  servoOut.attach(SERVO_PIN_OUT);

  Wire.begin();
  pcf8574.begin();

  lcd.init();           // Inisialisasi LCD
  lcd.backlight();      // Aktifkan lampu latar LCD
  lcd.setCursor(0, 0);
  lcd.print("Sistem Parkir");
  lcd.setCursor(0, 1);
  lcd.print("Siap Digunakan");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  Serial.println("Sistem Parkir Siap. Silakan tempelkan kartu RFID.");
}

void loop() {
  Blynk.run();
  int parkedCarsByIR = countCarsByIR();

  Serial.print("Jumlah mobil terdeteksi oleh infrared: ");
  Serial.println(parkedCarsByIR);
  delay(1000);
  updateBlynk();
  updateLCD();

  if (parkedCarsByIR <= MAX_CARS) {
    carCount = parkedCarsByIR;
  }

  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    byte uid[4];
    for (byte i = 0; i < 4; i++) {
      uid[i] = rfid.uid.uidByte[i];
    }

    int cardIndex = findCardIndex(uid);

    if (cardIndex == -1 && carCount < MAX_CARS) {
      registerNewCard(uid);
      openGate(servoIn);
      delay(10000);
      closeGate(servoIn);
      carCount++;
      updateBlynk();
      updateLCD();
    } else if (cardIndex != -1) {
      if (cards[cardIndex].hasEntered) {
        openGate(servoOut);
        delay(10000);
        closeGate(servoOut);
        cards[cardIndex].hasEntered = false;
        carCount--;
        updateBlynk();
        updateLCD();
      } else if (carCount < MAX_CARS) {
        openGate(servoIn);
        delay(10000);
        closeGate(servoIn);
        cards[cardIndex].hasEntered = true;
        carCount++;
        updateBlynk();
        updateLCD();
      } else {
        Serial.println("Parkir penuh, tidak bisa masuk.");
        Blynk.virtualWrite(uuuu, "Penuh");
        lcd.setCursor(0, 1);
        lcd.print("Parkir Penuh    "); // Clear remaining characters
      }
    }

    rfid.PICC_HaltA();
  }
}

void openGate(Servo &servo) {
  servo.write(180);
  Serial.println("Gerbang terbuka.");
  lcd.setCursor(0, 1);
  lcd.print("Gerbang Terbuka");
}

void closeGate(Servo &servo) {
  servo.write(0);
  Serial.println("Gerbang tertutup.");
  lcd.setCursor(0, 1);
  lcd.print("Gerbang Tertutup");
}

int findCardIndex(byte uid[4]) {
  for (int i = 0; i < MAX_CARS; i++) {
    if (compareUID(cards[i].uid, uid)) {
      return i;
    }
  }
  return -1;
}

bool compareUID(byte uid1[4], byte uid2[4]) {
  for (int i = 0; i < 4; i++) {
    if (uid1[i] != uid2[i]) {
      return false;
    }
  }
  return true;
}

void registerNewCard(byte uid[4]) {
  for (int i = 0; i < MAX_CARS; i++) {
    if (!cards[i].hasEntered) {
      for (int j = 0; j < 4; j++) {
        cards[i].uid[j] = uid[j];
      }
      cards[i].hasEntered = true;
      break;
    }
  }
}

int countCarsByIR() {
  int carCountIR = 0;

  // Membaca pin dari PCF8574
  if (!pcf8574.read(0)) carCountIR++;
  if (!pcf8574.read(1)) carCountIR++;
  if (!pcf8574.read(2)) carCountIR++;
  if (!pcf8574.read(3)) carCountIR++;

  return carCountIR;
}

void updateBlynk() {
  Serial.print("Mengirim jumlah mobil: ");
  Serial.println(carCount);
  Blynk.virtualWrite(V_PIN_CAR_COUNT, carCount);
}

void updateLCD() {
  lcd.setCursor(0, 0);
  lcd.print("Mobil: ");
  lcd.print(carCount);
  lcd.print("/");
  lcd.print(MAX_CARS);
  lcd.print("    "); // Clear remaining characters
}
