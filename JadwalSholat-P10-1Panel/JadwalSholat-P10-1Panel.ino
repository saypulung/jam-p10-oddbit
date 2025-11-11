/*
 * Copy soruce code = https://github.com/busel7/Arduino
 * JADWAL WAKTU SHOLAT MENGGUNAKAN ARDUINO NANO, LED P10, RTC DS3241, BUZZER
 * FITUR :  JADWAL SHOLAT 5 WAKTU DAN TANBIH IMSAK, JAM BESAR, TANGGAL, ALARAM ADZAN DAN TANBIH IMSAK,
 *          DAN HITUNG MUNDUR IQOMAH.
 * 

Pin on  DMD P10     GPIO      Arduino Nano          Pin on  DS3231      Arduino Nano      Pin on  Buzzer       Arduino Nano 

        2  A        GPIO16    D6                            SCL         A5                        +            D3 (GPIO 3)
        4  B        GPIO12    D7                            SDA         A4                        -            GND
        8  CLK      GPIO14    D13                           VCC         3V
        10 SCK      GPIO0     D8                            GND         GND
        12 R-Data   GPIO13    D11
        1  NOE      GPIO15    D9
        3  GND      GND       GND

*/


#include <DMD.h>
#include <TimerOne.h>
#include <Wire.h>
#include <Sodaq_DS3231.h>
#include "PrayerTimes.h"
#include <SystemFont5x7.h>
#include "fonts/angka6x13.h"
#include "fonts/angka_2.h"
#include "fonts/huruf_kecilku.h"
#include "fonts/Font3x5.h"
#include <SPI.h>
// #include <EEPROM.h>

char weekDay[][7] = {" AHAD ","SENIN ","SELASA"," RABU ","KAMIS ","JUM'AT","SABTU"," AHAD "}; // array hari, dihitung mulai dari Ahad, hari Ahad angka nya =0.
char namaBulan[][13] = {" ","JAN","FEB","MAR","APR","MEI","JUN","JUL","AGU","SEP","OKT","NOV","DES"}; // Urutan nama bulan dihitung dari 0 sampai 12
int langkah;
int lama_tampilnya;
boolean Waduh_Setting = false;
//
int address = 0;
// value  ihti = penambahan waktu aman dari Kemenag. Saat ini penambahannya 2 menit
byte value_ihti=2, value_iqmh_subuh=10, value_iqmh_dzuhur=8, value_iqmh_ashar=7, value_iqmh_maghrib=7, value_iqmh_isya=7, value_hari, m_iqmh;
int addr = 0;
byte S_IQMH = 0, S_IHTI = 0, S_HARI = 0;

//BUZZER PIN
const int buzzer = 3; // Pin GPIO Buzzer - D2

///////////////////////////////// Mendifinisikan Pin Tombol ////////////////////////////////////////
#define Jam_bUP A1                  // Tombol setting sesuaikan dengan PIN di arduino anda mana  klo ini terhubung dengan PIN 3 Digital
#define tombol_bOK A2             // Tombol OK sesuaikan dengan PIN di arduino anda mana  klo ini terhubung dengan PIN 5 Digital
#define Jam_bDN A3           //tombol setting   sesuaikan dengan PIN di arduino anda mana  klo ini terhubung dengan PIN 4 Digital
///////////////////koding ini untuk setting rtc sesuai dengan laptop//// /////
//year, month, date, hour, min, sec and week-day(starts from 0 and goes to 6)
//writing any non-existent time-data may interfere with normal operation of the RTC.
//Take care of week-day also.
//DateTime dt(2018, 4, 8, 12, 52, 0, 6);
#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN); //untuk mengatur jumlah panel yang kamu pakai
double times[sizeof(TimeName) / sizeof(char*)];

  int GMT = 8; // Perbedaan zona waktu (WIB 7 / WITA 8 / WIT 9)
  float lat = -8.00000;  // Lintang lokasi Anda: hidden info for privacy reason
  float lon = 115.00000; // Bujur lokasi Anda: hidden info for privacy reason

const int DELAY_DISPLAY = 500;

void ScanDMD()
{
  dmd.scanDisplayBySPI();
}
//
void setup()
{
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);//----pin 2 untuk alarm adzan maghrib ---hubungkan dengan buzzer
  langkah = 0;
  rtc.begin();
  noTone(buzzer);
  //rtc.setDateTime(dt); //Adjust date-time as defined 'dt' above (ini untuk setting jam)
  Wire.begin();
  Timer1.initialize( 500 );
  Timer1.attachInterrupt( ScanDMD );
  dmd.clearScreen( true );
  BuzzerPendek();
}
////////////////////////////////// Fungsi Looping/Pengulangan ///////////////////////////////////////
void loop() {
  mulai();
}


//////////////////////==========================Program Jam==============

void mulai()
{
  TampilJam();
  AlarmSholat();

  TampilTanggal();
  AlarmSholat();

  TampilJadwalSholat();
  AlarmSholat();

  RunningText();
  delay(100);
  AlarmSholat();
}


//----------------------------------------------------------------------
//TAMPILKAN JAM BESAR

void TampilJam() {
  
  DateTime now = rtc.now();
  
  dmd.clearScreen( true );
  dmd.selectFont(angka6x13);
  int menit_e = now.minute();
  int jame = now.hour();
  
  String xjam = Konversi(jame) ; //tampilan jam
  int pjg = xjam.length() + 1;
  char sjam[pjg];
  xjam.toCharArray(sjam, pjg);
  dmd.drawString(1, 0, sjam, pjg, 0);
  
  /////menit///
  String xxjam = Konversi(now.minute()) ; //tampilan menit
  int xpjg = xxjam.length() + 1;
  char xsjam[xpjg];
  xxjam.toCharArray(xsjam, xpjg);
  dmd.drawString(18, 0, xsjam, xpjg, 0);
  
  ///titik dua :
  
  dmd.selectFont(SystemFont5x7);
  dmd.drawChar( 14,  4, ':', GRAPHICS_NOR   );   // clock colon overlay on
  delay( 1000 );
  dmd.drawChar( 14, 4, ':', GRAPHICS_OR   );   // clock colon overlay off
  delay( 1000 );
  /////////////////////
  dmd.selectFont(SystemFont5x7);
  dmd.drawChar( 14,  4, ':', GRAPHICS_NOR   );   // clock colon overlay on
  delay( 1000 );
  dmd.drawChar( 14, 4, ':', GRAPHICS_OR   );   // clock colon overlay off
  delay( 1000 );
  dmd.drawChar( 14,  4, ':', GRAPHICS_NOR   );   // clock colon overlay on
  delay( 1000 );
  dmd.drawChar( 14, 4, ':', GRAPHICS_OR   );   // clock colon overlay off
  delay( 1000 );
  /////////////////////
  dmd.selectFont(SystemFont5x7);
  dmd.drawChar( 14,  4, ':', GRAPHICS_NOR   );   // clock colon overlay on
  delay( 1000 );
  dmd.drawChar( 14, 4, ':', GRAPHICS_OR   );   // clock colon overlay off
  delay( 1000 );
  dmd.drawChar( 14,  4, ':', GRAPHICS_NOR   );   // clock colon overlay on
  delay( 1000 );
  dmd.drawChar( 14, 4, ':', GRAPHICS_OR   );   // clock colon overlay off
  delay( 1000 );
  /////////////////////
  dmd.selectFont(SystemFont5x7);
  dmd.drawChar( 14,  4, ':', GRAPHICS_NOR   );   // clock colon overlay on
  delay( 1000 );
  dmd.drawChar( 14, 4, ':', GRAPHICS_OR   );   // clock colon overlay off
  delay( 1000 );
  dmd.drawChar( 14,  4, ':', GRAPHICS_NOR   );   // clock colon overlay on
  delay( 1000 );
  dmd.drawChar( 14, 4, ':', GRAPHICS_OR   );   // clock colon overlay off
  delay( 1000 );
  dmd.selectFont(SystemFont5x7);
  dmd.drawChar( 14,  4, ':', GRAPHICS_NOR   );   // clock colon overlay on
  delay( 1000 );

}


//------------------------------------------------------
//RUNNING TEXT

void RunningText() {
  dmd.clearScreen( true );
  String scrollingText = "Tulis scrolling text disini";
  dmd.drawMarquee(scrollingText.c_str(), scrollingText.length(),(32*DISPLAYS_ACROSS)-1,3);
   long start=millis();
   long timer=start;
   boolean ret=false;
   while(!ret){
     if ((timer+100) < millis()) {
       ret=dmd.stepMarquee(-1,0);
       timer=millis();
     }
   }
}


//----------------------------------------------------------------------
//JADWAL SHOLAT

void JadwalSholat() {

  DateTime now = rtc.now();

  int tahun = now.year();
  int bulan = now.month();
  int tanggal = now.date();
  Serial.print("Tahun:"); Serial.println(tahun);
  Serial.print("Bulan: ");Serial.println(bulan);
  Serial.print("Tanggal: ");Serial.println(tanggal);

  int dst=8; // TimeZone
  
  set_calc_method(Custom);
  set_asr_method(Shafii);
  set_high_lats_adjust_method(AngleBased);
  set_fajr_angle(20);
  set_isha_angle(18);

  get_prayer_times(tahun, bulan, tanggal, lat, lon, GMT, times);
  // OutputSerialJamSholat();

}

void digitNol(int digits)
{
  if (digits < 10)
  {
    Serial.print('0');
    Serial.print(digits);
  }
  else
    Serial.print(digits);
}

void OutputSerialJamSholat() {
  int hours, minutes;
  get_float_time_parts(times[0], hours, minutes);
  Serial.print("Shubuh= ");
  digitNol(hours);
  Serial.print(":");
  digitNol(minutes);
  delay(DELAY_DISPLAY);
  Serial.println();

  get_prayer_times(2025, 11, 9, lat, lon, GMT, times);
  get_float_time_parts(times[2], hours, minutes);
  
  Serial.print("Zhuhur= ");
  digitNol(hours);
  Serial.print(":");
  digitNol(minutes);
  delay(DELAY_DISPLAY);
  Serial.println();

  get_prayer_times(2025, 11, 9, lat, lon, GMT, times);
  get_float_time_parts(times[3], hours, minutes);
  Serial.print("Ashar=");
  digitNol(hours);
  Serial.print(":");
  digitNol(minutes);
  delay(DELAY_DISPLAY);
  Serial.println();

  get_prayer_times(2025, 11, 9, lat, lon, GMT, times);
  get_float_time_parts(times[5], hours, minutes);
  Serial.print("Maghrib= ");
  digitNol(hours);
  Serial.print(":");
  digitNol(minutes);
  delay(DELAY_DISPLAY);
  Serial.println();

  get_prayer_times(2025, 11, 9, lat, lon, GMT, times);
  get_float_time_parts(times[6], hours, minutes);
  Serial.print("Isya= ");
  digitNol(hours);
  Serial.print(":");
  digitNol(minutes);
  delay(DELAY_DISPLAY);
  Serial.println();
}

void TampilJadwalSholat() {

  JadwalSholat();

  char sholat[7];
  char jam[5];
  char TimeName[][8] = {" SUBUH","TERBIT"," DZUHUR"," ASHAR","TERBENAM","MAGHRIB","  ISYA", "WAKTU"};
  int hours, minutes;

  for (int i=0;i<8;i++){

    get_float_time_parts(times[i], hours, minutes);
    minutes = minutes + value_ihti;

    if (minutes >= 60) {
      minutes = minutes - 60;
      hours ++;
    }
    
    if (i==0 || i==2 || i==3 || i==5 || i==6) { //Tampilkan hanya Subuh, Dzuhur, Ashar, Maghrib, Isya
      sprintf(sholat,"%s",TimeName[i]); 
      sprintf(jam,"%02d:%02d", hours, minutes);
      dmd.clearScreen(true);
      dmd.selectFont(Font3x5);
      dmd.drawString(1,-2, sholat, 7, 0);
      dmd.selectFont(angka_2);
      dmd.drawString(1,8,jam, 5, 0);
      Serial.print(sholat);
      Serial.print(" : ");
      Serial.println(jam);
      delay(2000);
      
    }
  }
  
}


//--------------------------------------------------
//ALARM ADZAN WAKTU SHOLAT

void AlarmSholat() {

  DateTime now = rtc.now();

  int Hari = now.dayOfWeek();
  int Hor = now.hour();
  int Min = now.minute();
  int Sec = now.second();

  JadwalSholat();
  
  int hours, minutes;

  // Subuh
  get_float_time_parts(times[0], hours, minutes);
  minutes = minutes + value_ihti;

  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }
  
  if (Hor == hours && Min == minutes) {
    dmd.clearScreen(true);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(1, 0, "ADZAN", 5, 0); //koordinat tampilan
    dmd.selectFont(Font3x5);
    dmd.drawString(6, 7, "SUBUH", 5, 0); //koordinat tampilan
    BuzzerPanjang();
    Serial.println("SUBUH");
    delay(180000);//180 detik atau 3 menit untuk adzan
    
    BuzzerPendek();
    //m_iqmh = EEPROM.read(1);    //Saat Subuh tambah 2 menit waktu Iqomah 
    //Iqomah();
  }

  // Dzuhur
  get_float_time_parts(times[2], hours, minutes);
  minutes = minutes  + value_ihti;

  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }
  
  if (Hor == hours && Min == minutes && Hari != 5) {
    
    dmd.clearScreen(true);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(1, 0, "ADZAN", 5, 0); //koordinat tampilan
    dmd.selectFont(Font3x5);
    dmd.drawString(4, 7, "DZUHUR", 6, 0); //koordinat tampilan
    BuzzerPanjang();
    Serial.println("DZUHUR");
    delay(180000);//180 detik atau 3 menit untuk adzan
    
    BuzzerPendek();
    //m_iqmh = EEPROM.read(2);
    //Iqomah();
    
  } else if (Hor == hours && Min == minutes && Hari == 5) { 
    
    dmd.clearScreen(true);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(1, 0, "ADZAN", 5, 0); //koordinat tampilan
    dmd.selectFont(Font3x5);
    dmd.drawString(4, 7, "JUM'AT", 6, 0); //koordinat tampilan
    BuzzerPanjang();
    Serial.println("Adzan Jum'at");
    delay(180000);//180 detik atau 3 menit untuk adzan
  }

  // Ashar
  get_float_time_parts(times[3], hours, minutes);
  minutes = minutes  + value_ihti;

  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }
  
  if (Hor == hours && Min == minutes) {
    dmd.clearScreen(true);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(1, 0, "ADZAN ", 6, 0); //koordinat tampilan
    dmd.selectFont(Font3x5);
    dmd.drawString(6, 7, "ASHAR", 5, 0); //koordinat tampilan
    BuzzerPanjang();
    Serial.println("ASHAR");
    delay(180000);//180 detik atau 3 menit untuk adzan
    
    BuzzerPendek();
    // m_iqmh = EEPROM.read(3);
    //Iqomah();
  }

  // Maghrib
  get_float_time_parts(times[5], hours, minutes);
  minutes = minutes  + value_ihti;

  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }
  
  if (Hor == hours && Min == minutes) {
    dmd.clearScreen(true);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(1, 0, "ADZAN ", 6, 0); //koordinat tampilan
    dmd.selectFont(Font3x5);
    dmd.drawString(1, 7, "MAGHRIB", 7, 0); //koordinat tampilan
    BuzzerPanjang();
    Serial.println("MAGHRIB");
    delay(180000);//180 detik atau 3 menit untuk adzan
    
    BuzzerPendek();
    //m_iqmh = EEPROM.read(4);
    //Iqomah();
  }

  // Isya'
  get_float_time_parts(times[6], hours, minutes);
  minutes = minutes  + value_ihti;

  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }
  
  if (Hor == hours && Min == minutes) {
    dmd.clearScreen(true);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(1, 0, "ADZAN", 5, 0); //koordinat tampilan
    dmd.selectFont(Font3x5);
    dmd.drawString(8, 7, "ISYA'", 5, 0); //koordinat tampilan
    BuzzerPanjang();
    Serial.println("ISYA");
    delay(180000);//180 detik atau 3 menit untuk adzan
    
    BuzzerPendek();
    //m_iqmh = EEPROM.read(5);  
    //Iqomah();
  }
  
}


//--------------------------------------------------
//IQOMAH

void Iqomah() {

  DateTime now = rtc.now();
  //iqomah----------------------------------
  int langkah;
  int waktu_iqomah;
  int panjang_iqomah;
  int iqomah;
  dmd.clearScreen( true );
  dmd.selectFont(Font3x5);
  //dmd.clearScreen( true );
  dmd.drawString(3, -2, "IQOMAH", 6, 0); //koordinat tampilan
  int tampil;
  int detik = 0, menit = m_iqmh;
  for (detik = 0; detik >= 0; detik--) {
    delay(1000);
    String iqomah = Konversi(menit) + ":" + Konversi(detik);
    int panjang_iqomah = iqomah.length() + 1;
    char waktu_iqomah[panjang_iqomah];
    iqomah.toCharArray(waktu_iqomah, panjang_iqomah);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(1, 9, waktu_iqomah, panjang_iqomah, 0);
    if (detik <= 0) {
      detik = 60;
      menit = menit - 1;
    } if (menit <= 0 && detik <= 1) {
      dmd.clearScreen( true );
      digitalWrite(2, HIGH);//alarm Iqomah
      delay(1000);
      digitalWrite(2, LOW);//alarm Iqomah
      delay(50);
      dmd.selectFont(Font3x5);
      dmd.drawString(2, -2, "LURUS 8", 7, 0); //koordinat tampilan
      dmd.drawString(0, 7, "RAPATKAN", 8, 0); //koordinat tampilan
      delay(10000);
      for (tampil = 0; tampil < 300 ; tampil++) { //< tampil selama 300 detik waktu saat sholat
        menit = 0;
        detik = 0;
        dmd.clearScreen( true );
        dmd.selectFont(Font3x5);
        dmd.drawString(4, -2, "SHOLAT", 6, 0); //koordinat tampilan
        ///////////////////////
        now = rtc.now();
        String xjam = Konversi(now.hour()) + ":" + Konversi(now.minute()) + ":" + Konversi(now.second()) ; //tampilan jam
        int pjg = xjam.length() + 1;
        char sjam[pjg];
        xjam.toCharArray(sjam, pjg);
        dmd.selectFont(Font3x5);
        dmd.drawString(2, 7, sjam, pjg, 0);
        /////////////////////
        delay (1000);
      }
    }
  }
}



// Tampil Tanggal Baris Atas, Tampil Hari Baris Bawah

void TampilTanggal() {
  
    DateTime now = rtc.now();    
    
    char tanggalan[5];
    
    sprintf(tanggalan,"%02d %s", now.date(), namaBulan[now.month()]);

    dmd.clearScreen( true );
    dmd.selectFont(Font3x5);
    dmd.drawString(4, -2, (weekDay[now.dayOfWeek()]), 6, 0);   //tampilan hari
    dmd.drawString(4, 7, tanggalan, 6, 0);                     //tampilan tanggal dan bulan contoh 08 APR
    
    delay(3000);
}

void TampilSuhu(){
  //Tampilkan Suhu
  dmd.clearScreen( true );
  dmd.selectFont(Font3x5);    
  dmd.drawString(9, -2, "SUHU", 5, 0);
  String suhuku = Konversi(rtc.getTemperature() - 1) + "C";
  int suhul = suhuku.length() + 1;
  char suhu[suhul];
  suhuku.toCharArray(suhu, suhul);
  dmd.selectFont(SystemFont5x7);
  dmd.drawString(8, 9, suhu, suhul, 0);
  delay(3000);
}


//----------------------------------------------------------------------
// BUNYIKAN BEEP BUZZER
// Tone pakai nada G4
void BuzzerPanjang() {
  tone(buzzer, 392, 1000);
  delay(1000);
  noTone(buzzer);
  delay(50);
  tone(buzzer, 392, 1000);
  delay(1000);
  noTone(buzzer);
  delay(50);
}

// Tone pake nada C4
void BuzzerPendek() {
  tone(buzzer, 262, 200);
  delay(200);
  noTone(buzzer);
  delay(100);
  tone(buzzer, 262, 200);
  delay(200);
  noTone(buzzer);
  delay(50);
}


//----------------------------------------------------------------------
///konversi angka agar ada nol didepannya jika diawah 10

String Konversi(int sInput) {
  if (sInput < 10)
  {
    return"0" + String(sInput);
  }
  else
  {
    return String(sInput);
  }
}
