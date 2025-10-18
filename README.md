# Modbus TCP Client – Industrial Communications

A **Modbus TCP client** in C, built in **two layers**:

- **ModbusAP** – builds/validates **APDUs** (functions `0x03` and `0x10`), parses responses/exceptions.  
- **ModbusTCP** – adds/strips the **MBAP** header (7 bytes) and handles **TCP sockets** (`connect`, `send`, `recv`, timeouts).

Includes example programs: **`clienteMB`** (write+read) and **`clienteA`** (simple demo).

---
---

## ✨ Features
- `0x03` **Read Holding Registers** (up to **125** 16-bit registers, big-endian)  
- `0x10` **Write Multiple Registers** (up to **123** registers)  
- Validation: **TI/PI/UnitId/Length**, `byteCount` (0x03), and **Modbus exceptions** (FC bit 7)  
- Robust TCP I/O (**send/recv loops**, configurable **timeout**)

## 🔧 Build

**Linux / macOS / WSL / Cygwin**

clientMB
gcc modbusTCP.c modbusAP.c clienteMB.c -o clienteMB
---
clientA
gcc modbusTCP.c modbusAP.c clienteA.c -o clienteA
---
clientteste
gcc modbusTCP.c modbusAP.c clienteA.c -o clienteA

## ▶️ Run
./clienteMB    
./clienteA     

