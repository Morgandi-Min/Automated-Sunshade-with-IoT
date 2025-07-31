
## 🌞 ESP32 Smart Light-Controlled Sunshade System

This project implements a smart sunshade system powered by an **ESP32**, which adjusts based on real-time light intensity using a **DFRobot B_LUX_V30B** sensor. It supports both **automatic and manual modes**, and communicates data via **MQTT** for remote monitoring and control, such as through **Node-RED**.

---

## 🛠️ Features

- 🔆 Automatic control using real-time light intensity
- 🧠 Manual override via physical buttons or MQTT
- 🚦 Stepper motor control with limit switches for position safety
- 📡 MQTT data publishing for integration with dashboards (e.g., Node-RED)
- 💡 LED indicators for MANUAL and AUTO modes
- 📶 WiFi-enabled via ESP32

---

## 🧩 Hardware Requirements

- ESP32 microcontroller
- DFRobot B_LUX_V30B light sensor
- Stepper motor and driver
- Limit switches (top and bottom)
- Push buttons: Manual UP, Manual DOWN, Mode Toggle
- LEDs: AUTO mode, MANUAL mode
- Power source for ESP32 and motor

---

## 🧠 System Behavior

### AUTO Mode:
- Moves sunshade **UP** if light > 200 lux (unless already at the top)
- Moves sunshade **DOWN** if light < 200 lux (unless already at the bottom)

### MANUAL Mode:
- Operate via:
  - Physical buttons (UP, DOWN)
  - MQTT commands (`"UP"` / `"DOWN"`)
- Movement blocked by limit switches for safety

---

## 📡 MQTT Setup Guide

This project uses **MQTT** to allow the ESP32 to communicate with external interfaces like **Node-RED**, dashboards, or mobile apps.

---

### ✅ What is MQTT?

MQTT (Message Queuing Telemetry Transport) is a lightweight messaging protocol designed for low-power and low-bandwidth devices. It's ideal for IoT systems like this automatic sunshade.

---

### 🔌 MQTT Broker Configuration

By default, this project uses a **public MQTT broker** for testing:

- **Broker:** `mqtt-dashboard.com`
- **Port:** `1883`
- **Username/Password:** *Not required* (open connection)
- **Publish Topic:** `esp32/lightSystem`
- **Subscribe Topic:** `esp32/manual/control`

You can change the broker details in `main.cpp` if needed.

---

### 📲 ESP32 MQTT in Code

In the code, the ESP32:

- **Publishes** sensor and status data every 3 seconds:
  ```json
  {
    "lux": 235.6,
    "mode": "AUTO",
    "motor": "IDLE"
  }

---

## 🚀 Installing Node-RED Dashboard (for Data + Control)

Node-RED is used to visualize ESP32 sensor data and send MQTT commands (manual UP/DOWN).

### ✅ Step 1: Install Node.js

Download and install the **LTS version** of Node.js from:

👉 [https://nodejs.org/en/download/](https://nodejs.org/en/download/)

Verify installation:

```bash
node -v
npm -v
```

### ✅ Step 2: Install Node-RED

Use npm to install globally:

```bash
npm install -g --unsafe-perm node-red
```

### ✅ Step 3: Start Node-RED

```bash
node-red
```

Open the Node-RED UI at:

🌐 [http://localhost:1880](http://localhost:1880)

---

### 🧱 Install MQTT Nodes

Inside Node-RED:

1. Go to the editor → **Menu (☰) → Manage Palette → Install**
2. Search for: `node-red-node-mqtt`
3. Click **Install**

---
