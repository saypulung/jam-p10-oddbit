# How to Setup Arduino for Project

- Install Arduino IDE
- Install CH340 driver
- Copy all folder inside Libraries to the Arduino libraries folder. Usually it defined at C:\Users\SOME_USER\Documents\Arduino\Libraries in Windows OS
- Rename Sodaq_DS3231_x into Sodaq_DS3231
- Select chip to Arduino Uno and choose the COM Port that was connected into Arduino Uno
- Compile to Verify that the project work properly
- After that, you can modify and upload the code into Arduino Uno

# How to Troubleshot when the Time is Inaccurate

- Add library by Go to Tools menu -> Manage Libraries
- Find DS3231 by Andrew Wickert
- After that Open Sketch example by click File menu -> Examples -> DS3231 -> choose set_echo
- Build and Upload the Sketch
- Open Serial Monitor
- Send command to change date with format=YYMMDDwHHMMSS, example 2511123143045x

Explanation:

25 → 2025

11 → November

12 → 12th

3 → Wednesday

14 → 2 PM

30 → 30 minutes

45 → 45 seconds

x → end marker 

- Reupload the sketch project
