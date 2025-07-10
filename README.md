<details>
<summary>Click to view Documentation Images</summary>
  
| | | |
|---|---|---|
| ![1](https://github.com/Lunardy2509/arduino_multimeter/blob/main/documentation/WhatsApp%20Image%202024-11-13%20at%2011.24.08_102a96aa.jpg) | ![2](https://github.com/Lunardy2509/arduino_multimeter/blob/main/documentation/IMG-20240329-WA0021.jpg) | ![3](https://github.com/Lunardy2509/arduino_multimeter/blob/main/documentation/WhatsApp%20Image%202024-11-13%20at%2011.24.09_79977006.jpg) |

</details>

# 🔧 Arduino-Based Multimeter for Solar Power Monitoring

This is an open-source Arduino project designed to function as a **custom multimeter** capable of measuring DC and AC current (up to 5A) for solar PV systems. Developed under the *PKM MAN IC* initiative, this project supports solar education by enabling students to monitor real-time voltage, current, and power (watts) in small-scale solar setups.

---

## 📌 Project Objective

The multimeter is used to:

- ✅ Measure **input and output voltage/current** from solar PV systems
- ✅ Monitor components like **12V 3A battery**, **inverters**, and **loads** (e.g., electric fans)
- ✅ Hold/freeze values for user review via button interface

This was built for educational purposes in a high school community service program (*Pengabdian Kepada Masyarakat*) at **MAN Insan Cendekia Serpong** in Tangerang Selatan, Indonesia.

---

## ⚙️ Features

- 🧪 Measures both **DC and AC current** up to 5A  
- 🔋 Supports **solar PV + battery + inverter** system testing  
- 🖥️ Displays live voltage, current, and power  
- ⏸️ Includes **HOLD** function to freeze displayed values  
- 💾 Can log and compare values for calibration (with Excel sheet)  

---

## 📁 Folder Structure

```bash
arduino_multimeter/
├── circuit schematics/               # Wiring and circuit design (Fritzing/PNG)
├── code/                             # Arduino sketch (.ino)
│   └── multimeter.ino
├── documentation/                    # Educational docs or guides
├── Example_Sensor_Calibration.xlsx   # Calibration data for sensors
├── List of Materials and Apparatus.xlsx
└── README.md
```

## 🔌 Hardware Components
| Component                 | Quantity | Description                     |
| ------------------------- | -------- | ------------------------------- |
| Arduino Uno / Nano        | 1        | Microcontroller base            |
| ACS712 / INA219 Sensor    | 1–2      | Current sensor (for 5A current) |
| Voltage Divider           | 1        | For voltage measurement         |
| OLED / LCD Display        | 1        | For live data readout           |
| Push Button               | 1        | To hold/freeze values           |
| Load (e.g., Electric Fan) | 1        | Used as demonstration load      |
| Solar PV Panel (12V)      | 1        | Input source                    |
| Battery (12V, 3A)         | 1        | Intermediate storage            |
| Wires, Breadboard, etc.   | -        | For circuit assembly            |

## 🧰 Required Libraries
Please install the following libraries in your Arduino IDE:
- `Wire.h` (built-in)
- `LiquidCrystal_I2C.h` or `Adafruit_SSD1306.h` (based on display type)
- `ACS712.h` or `INA219.h` (based on current sensor)
Install via: **Sketch** → **Include Library** → **Manage Libraries**

## 🚀 Getting Started
### 1. Clone the Repository
```bash
git clone https://github.com/Lunardy2509/arduino_multimeter.git
cd arduino_multimeter/code
```

### 2. Upload Code
- Open `multimeter.ino` in Arduino IDE
- Install required libraries (see above)
- Select your board and COM port
- Upload the sketch to your Arduino

### 3. Build the Circuit
- Use the diagrams in `circuit schematics/` to connect your sensors and display
- Power the setup using USB or external 12V source (when appropriate)

## 📊 Calibration & Data Logging
Use `Example_Sensor_Calibration.xlsx` to:
- Calibrate raw sensor data against known values
- Adjust coefficients in your code accordingly for better accuracy

## 📚 Acknowledgments
This project was developed as part of a school community service program under PKM MAN IC (Pengabdian Kepada Masyarakat Madrasah Aliyah Negeri Insan Cendekia). Special thanks to the school and students involved in the solar energy education program.

## 📬 Contact
- GitHub: [@Lunardy2509](https://github.com/Lunardy2509)
- Email: ferdilunardy@gmail.com

If this helped your solar learning journey, please consider giving a ⭐️ and sharing with fellow makers!

---

Let me know if you want:
- A Bahasa Indonesia version
- Visual system diagrams or GIFs
- PDF export of documentation

I'll be happy to assist!
