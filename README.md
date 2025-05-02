
# tDCS Controller System using ESP8266

A complete guide for setting up and using a tDCS (transcranial Direct Current Stimulation) Controller system powered by an ESP8266 microcontroller and a web-based interface.

---

## ESP8266 মডিউলে কোড ইনস্টল ও সেটআপ প্রক্রিয়া

### প্রয়োজনীয় জিনিসপত্র:
- Arduino IDE (ইনস্টল করা থাকতে হবে)
- ESP8266 বোর্ড (যেমন NodeMCU / Wemos D1 Mini)
- USB Cable (ডাটা সাপোর্টেড)

### ধাপ ১: Arduino IDE সেটআপ করুন

1. Arduino IDE খুলুন  
2. ESP8266 Board Manager অ্যাড করতে:
   - **File > Preferences** এ যান  
   - “Additional Boards Manager URLs” বক্সে দিন:  
     `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
   - OK চাপুন
3. এখন:
   - **Tools > Board > Boards Manager** এ যান  
   - “ESP8266” লিখে সার্চ করে `esp8266 by ESP8266 Community` ইনস্টল করুন

### ধাপ ২: বোর্ড ও পোর্ট নির্বাচন করুন

- **Tools > Board >** আপনার ESP8266 ডিভাইস নির্বাচন করুন  
  উদাহরণ: `NodeMCU 1.0 (ESP-12E Module)`
- **Tools > Port >** আপনার সংযুক্ত COM port নির্বাচন করুন

### ধাপ ৩: কোড লোড ও আপলোড

1. [tdcs_controller.ino](https://github.com/HumayunShariarHimu/tDCS-Controller-Application/blob/main/tdcs_controller.ino) ফাইল খুলুন (বা কোডটি নতুন ফাইলে কপি করুন)
2. বোর্ড সংযুক্ত করে **Upload** (তীর চিহ্ন) বাটনে ক্লিক করুন
3. "Done uploading" মেসেজ দেখালে বোঝা যাবে সফলভাবে ইনস্টল হয়েছে

---

## tDCS কনট্রোলার ওয়েব এপ্লিকেশন গাইড

### ধাপ ১: ESP8266 ডিভাইস প্রস্তুত করুন

- আগের ধাপের কোড ESP8266 তে আপলোড করুন
- ESP8266 ও মোবাইল/কম্পিউটার একই Wi-Fi নেটওয়ার্কে যুক্ত থাকতে হবে

### ধাপ ২: ডিভাইসের IP বের করুন

- Arduino IDE > Serial Monitor খুলুন (বাউড রেট 115200)
- ESP8266 চালু হলে দেখা যাবে:
  ```
  Connected to WiFi
  IP address: 192.168.x.x
  ```
- এই IP address (যেমন: `192.168.0.105`) ওয়েব অ্যাপে ব্যবহার করবেন

### ধাপ ৩: ওয়েব অ্যাপে IP বসান

1. index.html ফাইলে এই লাইনটি খুঁজুন:
   ```js
   const url = `http://tdcs.local/start?mode=${mode}&current=${current}&duration=${duration}&polarity=${polarity}`;
   ```
2. এখানে `tdcs.local` এর জায়গায় আপনার ESP8266 এর IP বসান:
   ```js
   const url = `http://192.168.0.105/start?mode=${mode}&current=${current}&duration=${duration}&polarity=${polarity}`;
   ```

### ধাপ ৪: ওয়েব অ্যাপ ওপেন করুন

- [index.html](https://github.com/HumayunShariarHimu/tDCS-Controller-Application/blob/main/index.html) ফাইল ব্রাউজারে ডাবল ক্লিক করে চালু করুন
- মোড, কারেন্ট, সময়, পোলারিটি নির্বাচন করে **"স্টিমুলেশন শুরু করুন"** বাটনে চাপুন
- ESP8266 অনুযায়ী কাজ করবে

---

## tDCS Controller System Wiring & Electrode Placement

### ১. ESP8266 Pin Configuration:

- `D1 (GPIO5)` : tDCS Output Control Pin (PWM বা Analog)
- `GND` : ক্যাথোড ইলেকট্রোডের সাথে কানেক্ট

### ২. Power Management:

- বাইরের Constant Current Source Circuit প্রয়োজন (যেমন: LM334, L200, বা Adjustable Buck Converter + Current Limiter)
- ESP8266 সরাসরি tDCS কারেন্ট (1-2mA) জেনারেট করতে পারে না
- এটি শুধু control signal (PWM ON/OFF বা analog) সরবরাহ করে

### ৩. Electrode Placement According to Mode:

| Mode        | Anode Position                      | Cathode Position                      |
|-------------|-------------------------------------|----------------------------------------|
| memory      | Left DLPFC (F3)                    | Right supraorbital (Fp2)              |
| focus       | Prefrontal Cortex (F3/F4)          | Opposite supraorbital area            |
| mood        | Left DLPFC (F3)                    | Right DLPFC (F4)                      |
| pain        | Somatosensory Cortex (C3/C4)       | Shoulder or contralateral area        |
| social      | Orbitofrontal Cortex (Fp1 or Fp2)  | Behind the ear or contralateral side  |
| creativity  | Right Temporal (T4)                | Left Temporal (T3)                    |
| anxiety     | Left Shoulder or extracephalic     | Right DLPFC (F4)                      |

### ৪. ব্যবহৃত ম্যাপিং (10-20 EEG System অনুযায়ী):

- `F3`: Left Dorsolateral Prefrontal Cortex
- `F4`: Right DLPFC
- `Fp1/Fp2`: Frontal Pole / Orbitofrontal
- `C3/C4`: Somatosensory
- `T3/T4`: Temporal

### ৫. নিরাপত্তা নির্দেশনা:

- কারেন্ট সীমা: **সর্বোচ্চ 2mA**
- সময়সীমা: **সর্বোচ্চ 30 মিনিট**
- ত্বকে জ্বালাপোড়া লাগলে সাথে সাথে বন্ধ করুন
- **বাচ্চা বা মানসিক রোগীর উপর প্রয়োগ করবেন না**

---

## License

This project is open-source and licensed under the [MIT License](LICENSE).

## Author

Developed by **Humayun Shariar Himu**  
GitHub: [@HumayunShariarHimu](https://github.com/HumayunShariarHimu)
