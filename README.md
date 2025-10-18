# Raw UDP Socket Example in C

This repository demonstrates a minimal example of using **raw sockets** in C to send and receive UDP packets, with manually constructed **IP and UDP headers** and checksum calculations.

---

## Files

- `utils.h` : Contains IP/UDP header structures and checksum functions.
- `sender.c` : Sends a UDP packet using raw sockets.
- `receiver.c` : Receives raw UDP packets and prints them to the console.

---

## Features

- Manual construction of **IPv4 header**.
- Manual construction of **UDP header**.
- Calculation of **IP checksum** and **UDP checksum**.
- Minimal and easy-to-understand code structure.
- Works with **root/admin privileges** due to raw socket usage.
- Can be tested with Wireshark or on a local network.

---

## Requirements

- GCC or Clang compiler
- Linux, macOS, or WSL environment
- Root/Administrator privileges to run raw sockets

---

## Usage

### 1. Compile

```bash
gcc -o sender sender.c
gcc -o receiver receiver.c
```

### 2. Run receiver
```bash
sudo ./receiver
```

### 3. Run sender
```bash
sudo ./sender
```

---

## Notes
Make sure to set correct IP addresses in sender.c:
```c
iph->saddr = inet_addr("YOUR_SOURCE_IP");
iph->daddr = inet_addr("DESTINATION_IP");
```
