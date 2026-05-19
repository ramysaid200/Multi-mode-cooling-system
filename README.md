# 🌀 Multi-Mode Cooling System

A microcontroller-based smart fan control system developed for the **8th Semester Embedded Systems** course. The system allows users to control a DC fan motor with multiple speed levels and operating modes (Continuous / Timer) via a 4x4 keypad, with real-time status display on an LCD and UART serial monitoring.

---

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Hardware Components](#hardware-components)
- [Pin Configuration](#pin-configuration)
- [System Modes](#system-modes)
- [Keypad Mapping](#keypad-mapping)
- [How It Works](#how-it-works)
- [Project Files](#project-files)
- [Tools Used](#tools-used)

---

## 📖 Overview

The **Multi-Mode Cooling System** is an embedded C project built on the **8051 microcontroller (AT89C52)**. It controls a DC fan motor using **PWM (Pulse Width Modulation)** to achieve 6 distinct speed levels. Users interact with the system through a **4×4 matrix keypad**, while a **16×2 LCD** provides live feedback and a **UART serial interface** logs all events.

---

## ✨ Features

- ✅ **6 Fan Speed Levels** — Controlled via PWM duty cycle
- ✅ **Continuous Mode** — Fan runs indefinitely at selected speed
- ✅ **Timer Mode** — Fan auto-stops after 5, 10, or 15 seconds
- ✅ **Start / Stop Control** — Manual fan control via keypad
- ✅ **LCD Display** — Real-time speed and mode feedback
- ✅ **UART Serial Monitor** — Logs all events at 9600 baud rate
- ✅ **LED Status Indicators** — Green (ON) / Red (OFF) LEDs

---

## 🔧 Hardware Components

| Component | Description |
|-----------|-------------|
| AT89C52 (8051) | Main microcontroller |
| DC Motor + L298N | Fan motor and H-Bridge driver |
| 16×2 LCD | Status display (data bus on Port 2) |
| 4×4 Matrix Keypad | User input interface (Port 1) |
| Green LED | Indicates fan is running |
| Red LED | Indicates fan is stopped |
| Crystal Oscillator | 11.0592 MHz (for accurate UART baud rate) |
| UART / USB-Serial | Serial monitoring at 9600 baud |

---

## 📌 Pin Configuration

| Signal | Port / Pin | Description |
|--------|------------|-------------|
| `LCD` (data bus) | `P2` | 8-bit LCD data lines |
| `RS` | `P3.5` | LCD Register Select |
| `EN_LCD` | `P3.7` | LCD Enable |
| `GREEN` LED | `P3.0` | Fan running indicator |
| `RED` LED | `P3.6` | Fan stopped indicator |
| `IN1` (Motor) | `P3.2` | H-Bridge direction pin 1 |
| `IN2` (Motor) | `P3.3` | H-Bridge direction pin 2 |
| `EN1` / PWM | `P3.4` | Motor enable / PWM signal |
| `TX` (UART) | `P3.1` | Serial transmit |
| Keypad | `P1` | 4×4 matrix keypad |

---

## ⚙️ System Modes

### 🔁 Continuous Mode
The fan runs at the selected speed level indefinitely until the user manually presses **`D`** (Stop).

### ⏱️ Timer Mode
The fan runs at the selected speed for a preset time, then stops automatically:

| Key | Duration |
|-----|----------|
| `A` | 5 seconds |
| `B` | 10 seconds |
| `C` | 15 seconds |

---

## 🎹 Keypad Mapping

| Key | Function |
|-----|----------|
| `1` – `6` | Set fan speed level (1 = slowest, 6 = fastest) |
| `A` | Set timer to 5 seconds |
| `B` | Set timer to 10 seconds |
| `C` | Set timer to 15 seconds |
| `*` | **START** fan |
| `D` | **STOP** fan |

---

## ⚡ How It Works

1. On startup, the system initializes the LCD, UART, and GPIO pins.
2. The user selects a **speed** (keys `1`–`6`) and optionally a **timer** (keys `A`/`B`/`C`).
3. Pressing **`*`** starts the fan; the green LED lights up.
4. The motor runs using **software PWM** — `EN1` is toggled on/off with calculated duty cycles:

| Speed Level | Duty Cycle |
|-------------|------------|
| 1 | 2/10 (20%) |
| 2 | 4/10 (40%) |
| 3 | 6/10 (60%) |
| 4 | 7/10 (70%) |
| 5 | 8/10 (80%) |
| 6 | 9/10 (90%) |

5. In **Timer Mode**, a countdown runs and the fan stops automatically when time expires.
6. Pressing **`D`** at any time stops the fan manually.
7. All events (start, stop, speed change, timer) are logged via **UART** at 9600 baud.

---

## 📁 Project Files

```
Multi-Mode-Cooling-System/
│
├── fan_project.c              # Main C source code
├── STARTUP.A51                # 8051 startup assembly file
├── fan_project.uvproj         # Keil µVision project file
├── fan_project.uvopt          # Keil project options
├── Multi Mode Cooling System.pdsprj   # Proteus simulation project
├── Smar Fan Project Report.pdf        # Full project report (PDF)
├── Listings/                  # Compiler listing output directory
└── Objects/                   # Compiled object files directory
```

---

## 🛠️ Tools Used

| Tool | Purpose |
|------|---------|
| **Keil µVision 5** | C code editing, compilation, and debugging |
| **Proteus 8** | Circuit schematic design and simulation |
| **8051 C Compiler (Keil)** | Embedded C compilation targeting AT89C52 |
| **UART Terminal** | Serial output monitoring |

---

## 👨‍💻 Author

**Ramy Said**  
8th Semester — Embedded Systems Course  
Faculty of Engineering
