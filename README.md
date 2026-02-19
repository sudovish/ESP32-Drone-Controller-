# 🚁 ESP32 DJI Tello Drone Controller

<p align="center">
  <img src="imag/Drone.jpeg" width="500"/>
</p>

## Overview

This project is a custom-built drone controller using an ESP32 microcontroller to control a DJI Tello drone over Wi-Fi.

The ESP32 connects directly to the Tello drone network and sends UDP SDK commands triggered by physical push buttons wired to the board.

Instead of using the standard mobile app, this system acts as a standalone embedded hardware controller.

---

## How It Works

1. ESP32 connects to Tello Wi-Fi network  
2. UDP communication initialized on port 8889  
3. Button press triggers SDK command  
4. Command is sent to 192.168.10.1:8889  

---

## Example Commands Sent

- command  
- takeoff  
- land  
- forward 50  
- left 30  
- right 30  
- cw 90  

---

## Core Logic Example

```cpp
if (digitalRead(TAKEOFF_BUTTON) == HIGH) {
    sendCommand("takeoff");
}

if (digitalRead(LAND_BUTTON) == HIGH) {
    sendCommand("land");
}
