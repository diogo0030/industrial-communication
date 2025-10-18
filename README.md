# Modbus TCP Client – Industrial Communications

![C](https://img.shields.io/badge/lang-C-informational)
![Sockets](https://img.shields.io/badge/TCP-Sockets-blue)
![Modbus](https://img.shields.io/badge/Protocol-Modbus%20TCP-orange)
![License](https://img.shields.io/badge/License-MIT-green)

A **Modbus TCP client** in C, built in **two layers**:

- **ModbusAP** – builds/validates **APDUs** (functions `0x03` and `0x10`), parses responses/exceptions.  
- **ModbusTCP** – adds/strips the **MBAP** header (7 bytes) and handles **TCP sockets** (`connect`, `send`, `recv`, timeouts).

Includes example programs: **`clienteMB`** (write+read) and **`clienteA`** (simple demo).

---

## 📚 Table of Contents
- [Features](#-features)
- [Architecture](#-architecture)
- [Repo Structure](#-repo-structure)
- [Build](#-build)
- [Run](#-run)
- [API (ModbusAP)](#-api-modbusap)
- [Recommended Tests](#-recommended-tests)
- [Wireshark](#-wireshark)
- [Key Theory Notes](#-key-theory-notes)
- [Troubleshooting](#-troubleshooting)
- [Limitations & Next Steps](#-limitations--next-steps)
- [License](#-license)
- [Author](#-author)

---

## ✨ Features
- `0x03` **Read Holding Registers** (up to **125** 16-bit registers, big-endian)  
- `0x10` **Write Multiple Registers** (up to **123** registers)  
- Validation: **TI/PI/UnitId/Length**, `byteCount` (0x03), and **Modbus exceptions** (FC bit 7)  
- Robust TCP I/O (**send/recv loops**, configurable **timeout**)
