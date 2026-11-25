# 🚀 Boost Now (`boost-it`)

A fast, lightweight, standalone C++17 command-line REPL utility built for Windows—especially low-spec or older machines—to clean temporary files, clear the Recycle Bin, clean the default browser's cache, and inspect basic system status without bloatware or external runtime dependencies.

---

## ✨ Features

- **⚡ Lightweight & Low-Overhead:** Built in modern C++17 as a standalone `.exe` with minimal memory footprint, designed to stay light on older CPUs, tight RAM, and crowded `C:` drives.
- **🧹 Multi-Target Cleanup:** Cleans `%TEMP%`, empties the Recycle Bin, and targets the default browser's cache (`Edge` or `Chrome` via Registry `ProgId`), skipping locked or in-use files safely.
- **📊 Quick Device Status:** Displays current RAM usage and `C:` drive capacity/free space at a glance.
- **💻 Interactive REPL Prompt:** Clean `>>>` interactive loop for quick commands without extra overhead.

---

## 🛠️ Commands

Launch `boost-it.exe` and enter any of the following commands at the `>>>` prompt:

| Command | Description |
| :--- | :--- |
| `boost` | Runs all cleanup routines (`temp`, `bin`, and default browser `cache`). |
| `boost temp` | Cleans safe temporary files in `%TEMP%` and reports freed space. |
| `boost bin` | Empties the Windows Recycle Bin silently. |
| `boost cache` | Cleans the default browser's cache (or `TEST_CACHE_PATH` if set for testing). |
| `status` | Shows RAM load/used/free space and `C:` drive total/used/free space. |
| `help` | Lists all available commands. |
| `help <command>` | Shows details for a specific command (`boost`, `temp`, `bin`, `cache`, `status`, `exit`, `help`). |
| `exit` | Exits the program cleanly. |

---

## 🚀 Getting Started

### Option 1: Download Ready-to-Use Binary (Recommended)

1. Go to the **Releases** section on the repository page.
2. Download the latest `boost-it.exe`.
3. Run `boost-it.exe` from your terminal or double-click it, then type `help` or `boost`.

> **Note:** Because the binary is unsigned, Windows SmartScreen may warn you on first launch. Select **More info** → **Run anyway**.

---

### Option 2: Build from Source

#### Prerequisites
- **Windows**
- **MSYS2 (MINGW64)** with a C++17-capable `g++` toolchain

#### Build & Run Steps
1. Clone the repository:
   ```bash
   git clone [https://github.com/Marwan-Void/PC-Booster.git](https://github.com/Marwan-Void/PC-Booster.git)
   cd PC-Booster