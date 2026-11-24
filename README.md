# 🚀 PC Booster (`pc-booster`)

A fast, lightweight, standalone REPL utility built in modern C++17 to safely clean up Windows temporary files (`%TEMP%`) and free up disk space without bloatware or external dependencies.

---

## ✨ Features

- **⚡ Blazing Fast & Lightweight:** Compiled as a single standalone `.exe` with zero external runtime dependencies.
- **🛡️ Safe Cleanup:** Targets temporary files safely while skipping locked files currently in use by Windows or running apps.
- **💻 Interactive REPL Interface:** Simple `>>>` prompt for quick, distraction-free execution.
- **🪟 Native Windows Support:** Built specifically for Windows environments.

---

## 🛠️ Commands

Once you launch `pc-booster.exe`, use the following commands inside the `>>>` prompt:

| Command | Description |
| :--- | :--- |
| `boost` | Scans and cleans safe temporary files in your `%TEMP%` directory. |
| `help` | Displays available commands and usage instructions. |
| `help [commands]` | Describe the command you selected. |
| `exit` | Exits the application cleanly. |

---

## 🚀 Getting Started

### Option 1: Download Ready-to-Use Binary (Recommended)
1. Go to the **Releases** section on the right.
2. Download the latest `pc-booster.exe`.
3. Double-click `pc-booster.exe` (or run it from your terminal) and type `boost`.

> **Note:** Since the executable is unsigned, Windows SmartScreen may show a warning on first launch. Click **More info** → **Run anyway**.

---

### Option 2: Build from Source

#### Prerequisites
- **Windows**
- **MSYS2 (MINGW64)** with `g++` supporting C++17

#### Build Steps
1. Clone the repository:
   ```bash
   git clone [https://github.com/Marwan-Void/PC-Booster.git](https://github.com/Marwan-Void/PC-Booster.git)
   cd PC-Booster