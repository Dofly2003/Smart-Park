# Sistem Parkir Pintar dengan RFID dan Blynk

Proyek ini adalah sistem parkir otomatis berbasis **RFID**, **servo motor**, **infrared sensor**, dan **Blynk**. Sistem ini mampu mendeteksi kartu RFID untuk membuka dan menutup gerbang parkir, menghitung jumlah kendaraan di area parkir, serta menampilkan informasi jumlah kendaraan di aplikasi **Blynk** dan LCD.

## Fitur Utama
- **Deteksi Kartu RFID**: Membuka gerbang saat kartu RFID yang valid dipindai.
- **Hitung Kendaraan**: Menghitung kendaraan menggunakan sensor infrared.
- **Kontrol Servo Motor**: Menggerakkan servo untuk membuka dan menutup gerbang.
- **Integrasi Blynk**: Menampilkan jumlah kendaraan parkir di aplikasi Blynk.
- **LCD Display**: Menampilkan status sistem dan jumlah kendaraan parkir.
- **Indikasi Parkir Penuh**: Menampilkan status "Parkir Penuh" jika kapasitas penuh.

## Perangkat Keras yang Dibutuhkan
1. **ESP8266** (misalnya NodeMCU)
2. **Modul RFID** (MFRC522)
3. **Servo Motor** (2 buah, untuk gerbang masuk dan keluar)
4. **Sensor Infrared** (terhubung melalui PCF8574)
5. **LCD I2C** (16x2)
6. **PCF8574** (Ekspander pin I2C)
7. **Koneksi WiFi**

## Instalasi
1. **Kloning Repositori**
   ```bash
   git clone https://github.com/username/parkir-pintar.git
   cd parkir-pintar
   ```

2. **Tambahkan Library yang Dibutuhkan**
   Instal library berikut di Arduino IDE melalui **Library Manager**:
   - **MFRC522**
   - **Servo**
   - **LiquidCrystal_I2C**
   - **BlynkSimpleEsp8266**
   - **PCF8574**

3. **Konfigurasi WiFi dan Blynk**
   Ubah informasi berikut di kode:
   ```cpp
   #define BLYNK_TEMPLATE_ID "TMPL6SfcvJ7MT"
   #define BLYNK_TEMPLATE_NAME "Projek besar"
   #define BLYNK_AUTH_TOKEN "9zH8fRFa2ly4OOjmtr79bTFflra6yGxN"
   char ssid[] = "esp";      // Nama WiFi
   char pass[] = "00000000"; // Password WiFi
   ```

4. **Unggah Kode**
   - Hubungkan ESP8266 ke komputer.
   - Pilih board **NodeMCU 1.0 (ESP-12E Module)** di Arduino IDE.
   - Unggah kode ke ESP8266.

## Wiring Diagram
Berikut adalah pin yang digunakan dalam proyek ini:
- **RFID**
  - `SS_PIN` -> D4
  - `RST_PIN` -> D3
- **Servo**
  - Servo Masuk -> D0
  - Servo Keluar -> D8
- **PCF8574**
  - I2C Address -> 0x20
- **LCD**
  - I2C Address -> 0x27

## Cara Kerja
1. Sistem menyambungkan ESP8266 ke jaringan WiFi.
2. Sistem menunggu kartu RFID dipindai.
   - Jika kartu baru: Tambahkan kendaraan dan buka gerbang masuk.
   - Jika kartu yang terdaftar keluar: Kurangi kendaraan dan buka gerbang keluar.
3. Sistem menghitung jumlah kendaraan di area parkir menggunakan sensor infrared.
4. Sistem memperbarui status kendaraan di **Blynk** dan **LCD**.
5. Jika kapasitas penuh, sistem akan menampilkan status "Parkir Penuh".

## Video Demo
Silakan kunjungi tautan berikut untuk melihat demo proyek: [Link Demo](#)

## Masalah yang Dapat Muncul
- **Parkir penuh**: Jika kapasitas kendaraan sudah penuh, kartu baru tidak bisa masuk.
- **Kesalahan Koneksi WiFi**: Pastikan jaringan WiFi stabil dan informasi SSID/Password benar.

## Kontribusi
Kontribusi sangat diterima! Harap buat _pull request_ atau buka _issue_ jika Anda menemukan bug atau memiliki ide untuk pengembangan lebih lanjut.
