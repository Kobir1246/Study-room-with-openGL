# 🏠 3D Study Room Simulation (OpenGL)

## 📌 Overview

This project is a 3D simulation of a modern study room built using OpenGL and GLUT in C++. It demonstrates fundamental concepts of computer graphics including 3D modeling, transformations, lighting, and animation.

The environment includes multiple objects such as furniture, lighting systems, and interactive elements like a rotating ceiling fan and toggleable lights.

---

## 🎯 Features

* 3D room environment with walls, floor, and ceiling
* Realistic objects:

  * Bed
  * Study table with monitor and accessories
  * Chair with detailed structure
  * Bookshelf and book rack
  * Window with curtain
  * Door with switch panel
  * Carpet and side table
  * Digital clock (real-time system clock)
* Lighting system:

  * Toggle between room light and lamp light
* Animation:

  * Rotating ceiling fan
* Camera movement controls

---

## 🧠 Concepts Used

* 3D transformations (translate, rotate, scale)
* Lighting and shading (OpenGL lighting model)
* Object modeling using primitives (cube, cylinder, cone, sphere)
* Hierarchical modeling using `glPushMatrix()` / `glPopMatrix()`
* Real-time animation with idle function
* Keyboard interaction

---

## 🎮 Controls

```id="ctrl1"
W / S → Move forward / backward  
A / D → Move left / right  
L → Toggle room light  
F → Toggle fan rotation  
ESC → Exit program
```

---

## 🏗️ Project Structure

```id="struct1"
study_room.cpp   # Main source file containing all rendering and logic
```

---

## 🚀 How to Run

### 🔹 Using g++ (Linux / Mac)

```id="run1"
g++ study_room.cpp -o room -lGL -lGLU -lglut
./room
```

### 🔹 Windows (MinGW)

```id="run2"
g++ study_room.cpp -o room.exe -lfreeglut -lopengl32 -lglu32
room.exe
```

---

## 💡 How it Works

* The scene is rendered inside the `display()` function
* Camera movement is handled via keyboard input
* Lighting is dynamically controlled based on user interaction
* Fan animation is updated continuously in the `idle()` function
* Real-time clock is displayed using system time

Example:

```cpp id="ex1"
if (fanOn) {
    fanAngle += 1.0f;
}
```

---

## ⚠️ Limitations

* No texture mapping (uses basic colors only)
* Fixed camera angle (no mouse control)
* No advanced physics or collision detection

---

## 🔮 Future Improvements

* Add texture mapping for realism
* Implement mouse-based camera control
* Add shadows and reflections
* Improve object detail with advanced modeling

---

## 👤 Author

Shahriar Kobir Sabbir
CSE Graduate | Aspiring Machine Learning Engineer

---
