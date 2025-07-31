# Automated-Sunshade-with-IoT
To **download and install Node-RED** for your ESP32-based smart sunshade project (or any IoT dashboarding), follow these steps based on your operating system. Node-RED runs on **Windows, macOS, Linux**, and even **Raspberry Pi**.

---

## 🌐 What is Node-RED?

**Node-RED** is a flow-based programming tool for wiring together hardware devices, APIs, and online services. You can use it to:

* Subscribe to your ESP32’s MQTT messages
* Visualize data like light intensity and motor state
* Send manual control commands (`UP`, `DOWN`) via MQTT

---

## 🧰 Installation Instructions

### 🔵 Option 1: Using Node.js (Recommended for All Platforms)

Node-RED runs on Node.js. You need to install Node.js first.

### ✅ Step 1: Install Node.js

Download Node.js LTS version from the official site:

🔗 [https://nodejs.org/en/download/](https://nodejs.org/en/download/)

Make sure Node.js and npm (Node Package Manager) are installed:

```bash
node -v
npm -v
```

---

### ✅ Step 2: Install Node-RED

After installing Node.js, run:

```bash
npm install -g --unsafe-perm node-red
```

This installs Node-RED globally on your system.

---

### ✅ Step 3: Start Node-RED

```bash
node-red
```

By default, it runs at:

🌐 [http://localhost:1880](http://localhost:1880)

You can open this URL in your browser to access the Node-RED editor.

---

## 🖥️ Platform-Specific Notes

### 🪟 Windows (PowerShell or CMD)

1. Install Node.js (LTS version)
2. Run `npm install -g --unsafe-perm node-red`
3. Start it using `node-red`

### 🍏 macOS (Terminal)

Use Homebrew to install Node.js:

```bash
brew install node
```

Then follow the steps above.

### 🐧 Linux (Debian/Ubuntu)

```bash
sudo apt update
sudo apt install -y nodejs npm
sudo npm install -g --unsafe-perm node-red
```

Start with:

```bash
node-red
```

### 🍓 Raspberry Pi

Node-RED comes pre-installed in Raspberry Pi OS (since 2015). To upgrade or reinstall:

```bash
bash <(curl -sL https://raw.githubusercontent.com/node-red/linux-installers/master/deb/update-nodejs-and-nodered)
```

Start Node-RED with:

```bash
node-red-start
```

---

## 📦 Install MQTT Nodes (inside Node-RED)

Once Node-RED is running:

1. Go to [http://localhost:1880](http://localhost:1880)
2. Click the **menu (☰) → Manage palette → Install**
3. Search for: `node-red-node-mqtt`
4. Click **Install**

This lets you connect to your ESP32’s MQTT broker.

---

* MQTT input node
* Text display for lux, mode, motor state
* Buttons to publish `UP` or `DOWN` to the MQTT topic?

Let me know — I can create and export that for you.

---

# ESP32 Smart Light-Controlled Sunshade System

This project implements a smart automatic sunshade controller using an ESP32 microcontroller. It adjusts a motorized shade system based on real-time light intensity data and allows for manual or automatic operation modes. The system also publishes sensor and system state data to an MQTT broker, allowing integration with platforms like Node-RED for data visualization and remote control.

## 🔧 Features

- 🌞 Light-based automatic control using DFRobot B_LUX_V30B sensor
- 🤖 Manual control buttons and MQTT remote control commands
- 📊 Real-time data publishing to MQTT topic
- 🔁 Motor control with limit switches
- 💡 Mode indication via LEDs (AUTO or MANUAL)
- 📡 WiFi connectivity and MQTT communication

## 📦 Hardware Requirements

- ESP32s board
- DFRobot B_LUX_V30B light sensor
- Stepper Motor or DC Motor
- Limit switches (top and bottom)
- Push buttons for manual up/down and mode toggle
- Indicator LEDs for AUTO/MANUAL modes
- Power supply suitable for ESP32 and motor

## 🧠 Software Requirements

- Arduino IDE with ESP32 board support
- Libraries:
  - `WiFi.h`
  - `PubSubClient.h`
  - `Wire.h`
  - `DFRobot_B_LUX_V30B.h`

## 📡 MQTT Configuration

- **Broker:** `mqtt-dashboard.com`
- **Port:** `1883`
- **Publish Topic:** `esp32/lightSystem`
- **Subscribe Topic:** `esp32/manual/control`
- **MQTT Commands:** `"UP"` and `"DOWN"` (only in MANUAL mode)


## 🧠 System Behavior

- **AUTO Mode:**
  - If light level > 200 lux and shade is not fully up → move up.
  - If light level < 200 lux and shade is not fully down → move down.

- **MANUAL Mode:**
  - Control via push buttons or MQTT commands.
  - Limit switches prevent overrun at both ends.

## 📈 Data Payload Example (MQTT)
```json
{
  "lux": 235.6,
  "mode": "AUTO",
  "motor": "IDLE"
}

