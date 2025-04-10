# ParkAI

**Smart, affordable, real-time parking detection** — built to work with *existing* CCTV, PCs, and without new infrastructure: No extra power setups, No sensors, no rewiring, no cloud.

## 🚗 The Problem

People waste time and fuel circling for parking — especially in schools, malls, and offices. Most lots don’t show real-time availability, and existing smart systems are too expensive or hard to retrofit.

## 🧠 The Concept

Use existing infrastructure:

- CCTV cameras
- A local PC or microcontroller
- Existing power

Run a lightweight AI model locally to detect if spots are free — no need for new hardware or cloud processing.

## 🛠️ Our Prototype 

To demo the concept, we built a **skeleton prototype** using:

- **ESP32-CAM** to simulate CCTV
- **Local Flask server** running YOLOv8 (trained on 2K+ images)
- **FRDM-K66F** for outputs (LEDs, LCD, servo motor gate)

This is an early proof-of-concept showing how the system could work. It’s modular, offline, and adaptable to different environments (schools, malls, etc.).

## 💡 Why This Approach?

- **Cheap** – no sensors, no cloud costs
- **No new infrastructure — uses what’s already there**
- **Offline** – better privacy, reliability
- **Flexible** – easy to maintain and expand: show output on screen, LEDs, or even mobile app

## 📹 Demo

[Watch the early-Skeleton prototype in action](https://drive.google.com/file/d/1MSu5mgmAo936-EsWqRB523nmbo4QnHnI/view?usp=sharing)

