# EdgarieOS
Functional operating system for RISC-V 32 architecture, developed from scratch in C and Assembly.

**Authors:** Vlad Androne and Antoniu Andreas

**Architecture:** RISC-V RV32 ISA 
**Platform:** QEMU (`qemu-system-riscv32`)  
**Languages:** C & RISC-V Assembly


## Key Features

* **Boot Process:** Boots via SBI (OpenSBI) directly into `kmain()` and displays a boot banner via UART.
* **Scheduling:** Implements preemptive scheduling (Round-Robin) with time slice. Supports running multiple concurrent processes (maximum of 8).
* **Memory Management:** Features a kernel heap allocator supporting `kmalloc` and `kfree`.
* **System Calls:** Implements a trap mechanism for user-to-kernel interaction (`USER_MODE_EXCEPTION`), including:
    * `write()`: Output to UART.
    * `yield()`: Voluntary CPU relinquishment.
    * `get_time()`: Show the system tick counter.
* **Interactive Shell:** A built-in UART monitor for managing processes and executing commands (`<EdgarieOS>:`).

## Prerequisites

To build and run this project, you need the following toolchain installed:

* **Compiler toolchain:** `riscv64-unknown-elf-gcc`
* **Emulator:** `qemu-system-riscv32`
* **Tools:** `make`, `gdb`, `objcopy`

## Building & Running

The project uses a `run.sh` file for automated building and emulation.
 
To run, make sure the `run.sh` file has the execute right (`chmod +x run.sh`). Then run in terminal the command `./run.sh` 

*This command compiles the project and launches the OS in QEMU.*

## Shell Commands

Once the OS is running, you can use the following commands in the UART terminal:

| Command | Description |
| :--- | :--- |
| `help` | List available commands. |
| `ps` | Display process PID, state, and CPU time. |
| `kill <prog_id>` | Terminate a specific process. |
| `exec <prog>` | Launch a user program. |

## What a demo should do

Following the project requirements, a standard run demonstrates:
1.  **Boot:** Banner display and tick counter initialization, interrupt initialization and reset.
2.  **Multitasking:** Execution of multiple processes (ex: when calling `exec prog1`, `exec prog2`).
3.  **Checking process list:** After calling `ps` command a list of processes should appear.
4.  **Preemption:** Process scheduling visible (by using `uart_printf` in example programs) the terminal.
5.  **Management:** Successful use of `kill` and memory statistics reporting (heap before/after allocation and after free).

## Project Structure (important directories)

* `kernel/` - Core kernel source code.
* `userlib/` - User-space library for user interface commands.
* `interrupts/` - Interrupt handling functions.
* `memory/` - Directory for memory management.
* `keyboard/` - Keyboard exeptions handling (special keys + buffer processing).
* `uart/` - UART printing functions for M mode and S mode.
* `run.sh` - Build configuration + opens QEMU and runs the OS.

<details>
  <summary> Bloopers(README original)</summary>
    ##  Scrie Boot banner, da un "clear screen" si scrie ceva la terminal 
    ## Momentan scrierea intr un registru csr face ca programul sa intre intr un loop infinit ciudat 
    ## morti lui de opensbi ca ma baga in supervisor mode inloc sa ma lase in mmode; am pierdut ZILE IN PULA MEA INCERCAND SA IMID AU SEAMA 
    ## merge printf si sa lucram cu csr doar sa fie scsr(supervisor control and status registers) 
    ## working timer interrupts and print stuff 
    ## wrote func prototypes for plic  
    ## TO DO: add finite program trmpolin in start process and modify schedule so it works and removes programs that have finished 
    ## TO DO: create logic to call process done from inside schedule? think of how to find out that the last ret instrunction has been called 
    ## TO DO: add file system 
</details>
