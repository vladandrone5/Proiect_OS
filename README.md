# EdgarieOS
Functional operating system for RISC-V 32 architecture, developed from scratch in C and Assembly.

**Authors:** Vlad Androne and Antoniu Andreas
**Languages:** RISC-V 32 Assembly, C
**Emulator:** QEMU (`qemu-system-riscv32`)
**Toolchain:** `riscv64-unknown-elf-gcc,ld,objcopy,gdb`

# Proiect OS - RISC-V Kernel

[cite_start]**Architecture:** RISC-V RV32IMA [cite: 3]  
[cite_start]**Platform:** QEMU (`qemu-system-riscv32`) [cite: 3]  
[cite_start]**Language:** C & Assembly [cite: 43]

## 📖 Overview

This repository contains a custom Operating System kernel developed from scratch for the RISC-V architecture. [cite_start]It implements core OS functionalities including preemptive scheduling, memory management, and system calls, strictly adhering to minimal dependencies (no standard `libc`)[cite: 61].

## ✨ Key Features

* [cite_start]**Boot Process:** Boots via SBI (OpenSBI) directly into `kmain()` and displays a boot banner via UART[cite: 4, 9].
* [cite_start]**Scheduling:** Implements preemptive scheduling (Round-Robin) driven by machine timer ticks[cite: 17]. [cite_start]Supports running multiple concurrent processes[cite: 16].
* [cite_start]**Memory Management:** Features a kernel heap allocator (bump or free-list) supporting `kmalloc` and `kfree`[cite: 19, 20].
* **System Calls:** Implements a trap mechanism for user-to-kernel interaction, including:
    * [cite_start]`write()`: Output to UART[cite: 24].
    * [cite_start]`yield()`: Voluntary CPU relinquishment[cite: 25].
    * [cite_start]`gettime()`: Read the system tick counter[cite: 26].
* [cite_start]**Program Loading:** Loads and executes ELF binaries (or flat binaries) from an embedded RAMFS[cite: 29].
* [cite_start]**Interactive Shell:** A built-in UART monitor for managing processes and executing commands[cite: 31].

## 🛠️ Prerequisites

[cite_start]To build and run this project, you need the following toolchain installed[cite: 5]:

* **Compiler:** `riscv64-unknown-elf-gcc`
* **Emulator:** `qemu-system-riscv32`
* **Tools:** `make`, `gdb`, `objcopy`

## 🚀 Building & Running

The project uses a `Makefile` for automated building and emulation.

1.  **Build the kernel:**
    ```bash
    make
    ```

2.  **Run in QEMU:**
    ```bash
    make run
    ```
    [cite_start]*This command compiles the project and launches the OS in QEMU[cite: 64].*

## 💻 Shell Commands

Once the OS is running, you can use the following commands in the UART terminal:

| Command | Description |
| :--- | :--- |
| `help` | [cite_start]List available commands[cite: 34]. |
| `ps` | [cite_start]Display process PID, state, and CPU time[cite: 35]. |
| `kill <pid>` | [cite_start]Terminate a specific process[cite: 36]. |
| `exec <prog>` | [cite_start]Launch a user program from the file system[cite: 37]. |

## 🧪 Demo Scenario

[cite_start]Following the project requirements, a standard run demonstrates[cite: 49]:
1.  **Boot:** Banner display and tick counter initialization.
2.  **Multitasking:** Execution of multiple processes (`exec u1`, `exec u2`).
3.  **Preemption:** Interleaved output from processes confirming context switching.
4.  [cite_start]**Management:** Successful use of `kill` and memory statistics reporting[cite: 54, 55].

## 📂 Project Structure

* `kernel/` - Core kernel source code (scheduler, memory, traps).
* `user/` - User-space library (`libu`) and programs.
* `include/` - System header files.
* `Makefile` - Build configuration.

<details>
  <summary> Bloopers(README original)</summary>
    ## Scrie Boot banner, da un "clear screen" si scrie ceva la terminal 
    ## Momentan scrierea intr un registru csr face ca programul sa intre intr un loop infinit ciudat 
    ## morti lui de opensbi ca ma baga in supervisor mode inloc sa ma lase in mmode; am pierdut ZILE IN PULA MEA INCERCAND SA IMID AU SEAMA 
    ## merge printf si sa lucram cu csr doar sa fie scsr(supervisor control and status registers) 
    ## working timer interrupts and print stuff 
    ## wrote func prototypes for plic  
    ## TO DO: add finite program trmpolin in start process and modify schedule so it works and removes programs that have finished 
    ## TO DO: create logic to call process done from inside schedule? think of how to find out that the last ret instrunction has been called 
    ## TO DO: add file system 
</details>
