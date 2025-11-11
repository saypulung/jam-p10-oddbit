# Arduino Project Jam Sholat OddBit

## JWS - Jadwal Waktu Sholat

Jadwal Waktu Sholat menggunakan:

Arduino Uno
LED Panel P10
RTC DS3231
Passive Buzzer
LED Panel P10

### FITUR

- Jam Besar
- Jadwal Sholat 5 Waktu
- Tanggal
- Suhu

### Pin on DMD LED P10 Panel

| DMD P10 | Nano  | 
| ------- | ----- |
| A       | D6    |                                                 
| B       | D7    |                                                  
| CLK     | D13   |                           
| SCK     | D8    |                           
| R       | D11   |
| NOE     | D9    |
| GND     | GND   |

### Pin on RTC DS3231

| DS3231 | Nano     |
| ------ | -------- |
| SCL    | A5/SCL   |
| SDA    | A4/SDA   |
| VCC    | 3V       |
| GND    | GND      |

### Pin on Buzzer

| Buzzer | Nano  |
| ------ | ----- |
| +      | D3    |
| -      | GND   |

### Eksternal Library
- DMD : https://github.com/freetronics/DMD
- PrayerTime : https://github.com/asmaklad/Arduino-Prayer-Times
- RTC DS3231 : https://github.com/SodaqMoja/Sodaq_DS3231
        
### Catatan : 
- Power bisa langsung dari USB Arduino Uno
