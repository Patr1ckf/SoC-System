# SoC System for Prime Number Calculation

## Project Description
The project involves creating a complete System-on-Chip (SoC) capable of calculating prime numbers in real-time. It consists of three key components: a Verilog module, a Linux kernel module, and a user application. Each element plays a crucial role, and their interaction enables efficient prime number generation and verification.

## System Architecture
The system is divided into three main layers:

1. **Verilog Module (FPGA)**
2. **Linux Kernel Module**
3. **User Application (Test Tool)**

These layers communicate via the AXI interface and sysfs files, allowing seamless data exchange between hardware and software.

## Verilog Module
This is the core of the system, implementing the prime number calculation algorithm. It runs on an FPGA board and interacts with the rest of the system through the AXI bus.

**Inputs:**
- `n_reset`: reset signal
- `saddress`: register address
- `srd`, `swr`: read and write signals
- `sdata_in`: input data (number of primes to calculate)
- `clk`: clock signal

**Outputs:**
- `sdata_out`: computation result
- `gpio_out`: count of calculated prime numbers

### Logic Flow:
The algorithm works as a finite state machine:

1. **State 0 (Initialization):** Reset variables and set initial parameters.
2. **State 1 (Computation):**
   - Check consecutive natural numbers.
   - Divide by numbers up to the square root (prime check).
   - If prime, save as result and increment counter.
3. **State 2 (Completion):** Transfer result to the register and wait for new input.

**Example Results:**
- A = 5 → Result: 11
- A = 10 → Result: 29
- A = 100 → Result: 541
- A = 450 → Result: 3181

## Linux Kernel Module
The kernel module acts as a driver that communicates with the Verilog module through sysfs files, enabling data exchange between the user application and hardware.

**Sysfs Files:**
- `rejA`: write the number A (count of primes to find)
- `rejW`: read the result (last found prime number)
- `rejS`: read the status (0 - init, 1 - calculating, 2 - done)

Error handling ensures system stability by rejecting invalid inputs.

## User Application
The application, written in C, automates system testing.

**Functionality:**
- Writes the value of A to the register via `echo`.
- Polls the status register, waiting for computation to finish.
- Retrieves and verifies the result.

**Example Tests:**
- For A = 25 → Result: 97
- For A = 60 → Result: 281
- For A = 1000 → Result: 7919

The app handles incorrect data and reports invalid results, ensuring robustness.

## Summary
This project is a complete solution for real-time prime number calculation using FPGA and Linux. The combination of low-level hardware logic with user-space software creates a system that is both fast and easy to test.

The project not only demonstrates hardware-software integration but also provides a real-world implementation that could be scaled for more complex computational tasks.

