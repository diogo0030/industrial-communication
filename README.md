# Modbus TCP Client – Industrial Communications

A **Modbus TCP client** in C, built in **two layers**:

- **ModbusAP** – builds/validates **APDUs** (functions `0x03` and `0x10`), parses responses/exceptions.  
- **ModbusTCP** – adds/strips the **MBAP** header (7 bytes) and handles **TCP sockets** (`connect`, `send`, `recv`, timeouts).

Includes example programs: **`clienteMB`** (write+read) and **`clienteA`** (simple demo).

---

## ✨ Features
- `0x03` **Read Holding Registers** (up to **125** 16-bit registers, big-endian)  
- `0x10` **Write Multiple Registers** (up to **123** registers)  
- Validation: **TI/PI/UnitId/Length**, `byteCount` (0x03), and **Modbus exceptions** (FC bit 7)  
- Robust TCP I/O (**send/recv loops**, configurable **timeout**)

## 🔧 Build

**Linux / macOS / WSL / Cygwin**

**clientMB.c** : gcc modbusTCP.c modbusAP.c clienteMB.c -o clienteMB

**clientA.c** : gcc modbusTCP.c modbusAP.c clienteA.c -o clienteA

**clientteste.c** : gcc modbusTCP.c modbusAP.c clienteA.c -o clientteste

## ▶️ Run
./clienteMB    
./clienteA 

## 🧪 Using with Modbus Slave (server simulator)
**Start Modbus Slave** (or any Modbus TCP server):
- **Connection:** TCP/IP
- **Port:** `502`
- **Host/IP:** `127.0.0.1` (same PC) **or** the server machine’s IP (e.g., `192.168.1.45`)
- **Data model:** create a block of **Holding Registers** that covers the addresses you’ll use (e.g., 1..200)
  **Run your client:**
-./clienteA   # writes one register (check value in Modbus Slave UI)
-./clienteMB  # writes N registers then reads them back (table should update)

## 🧩 API
**0x03 — Read n registers**

// Returns: n (success), -E (Modbus exception; E = exception code), -1 (non-Modbus error).

int Read_h_regs(const char* ip, int port, uint16_t start, uint16_t n, uint16_t* out);

**0x10 — Write n registers**

// Returns: n (success), -E (Modbus exception), -1 (non-Modbus error).

int Write_multiple_regs(const char* ip, int port, uint16_t start, uint16_t n, const uint16_t* vals);









