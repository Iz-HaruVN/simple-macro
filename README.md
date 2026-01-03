# simple-macro
basic keymapper coded in c++ with readable config file
# 🖱️ C++ Macro Application

A lightweight **Windows macro application written in C++**, designed to automate keyboard and mouse actions with configurable delays and key bindings.  
All settings are controlled via external configuration files, allowing easy customization without recompiling.

---

## ✨ Features

- Simulate keyboard key presses
- Simulate mouse left & right clicks
- Configurable base delay and random delay
- Custom macro action list
- Toggle macro on/off using a keybind
- Simple configuration files (`config.json`, `key.txt`)

---

## 📂 Project Structure

```
.
├── main.cpp
├── config.json
├── key.txt
└── ReadMe.md
```

---

## ⚙️ Configuration: `config.json`

This file controls macro timing and the activation key.

### Example

```json
{
    "delay": 100,
    "random_delay": 50,
    "keybind": "q"
}
```

### Fields

| Field          | Type    | Description |
|----------------|---------|-------------|
| `delay`        | number  | Base delay (milliseconds) between actions |
| `random_delay` | number  | Additional random delay (0 → random_delay) |
| `keybind`      | string  | Key used to toggle the macro on/off |

**Final delay formula:**
```
delay + random(0, random_delay)
```

### Supported keybind values

- Single characters: `"q"`, `"e"`, `"f"`, `"1"`
- Function keys: `"F6"`, `"F7"`, `"F8"`
- Special keys: `"ESC"`

> ⚠️ `config.json` must be placed in the same directory as the executable.

---

## ⌨️ Macro Keys Setup: `key.txt`

This file defines the sequence of actions the macro will execute.

### Example

```
left_click
right_click
q
e
f
```

### Supported actions

#### 🖱️ Mouse
- `left_click`  → Left mouse click
- `right_click` → Right mouse click

#### ⌨️ Keyboard
- Single characters: `a`, `b`, `q`, `1`, `2`, etc.

### Rules

- One action per line
- Actions are executed **top to bottom**
- Avoid empty lines

---

## ▶️ How to Use

1. Configure `config.json`
2. Define actions in `key.txt`
3. Run the application
4. Press the configured **keybind** to start/stop the macro

---

## 🛠 Requirements

- Windows OS
- C++ compiler (MinGW or MSVC)
- Windows API (`windows.h`)

---

## ⚠️ Disclaimer

This project is intended for **educational purposes only**.  
Use responsibly and follow the terms of service of any application or game.

---

⭐ If you find this project useful, feel free to improve or expand it!
