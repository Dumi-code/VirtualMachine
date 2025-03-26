# Virtual Machine Simulator
A simple virtual machine that executes custom assembly code with process scheduling capabilities.

## Features   
   **Basic CPU Simulation**: Registers, memory, and instruction execution.<br>
   **Instruction Set**: Supports MOV, LOAD, STORE, ARITHMETIC, JUMPS, and more.<br>
   **Process Scheduling**: Implements a basic round-robin scheduler.<br>
   **Interrupt Handling**: Timer-based interrupts for process switching.<br>
   **Memory Management**: 100-location memory space with stack operations.<br>

## Instruction Set Overview

| Instruction | Description                          | Example           |
|-------------|--------------------------------------|-------------------|
| mov         | Register to register move            | `mov a b`         |
| movv        | Immediate value to register          | `movv a 10`       |
| load        | Load from memory address             | `load a 50`       |
| loadr       | Load from register-indirect address  | `loadr a b`       |
| store       | Store to memory address              | `store 50 a`      |
| storer      | Store to register-indirect address   | `storer b a`      |
| add         | Add two registers (result in acc)    | `add a b`         |
| sub         | Subtract two registers               | `sub a b`         |
| mod         | Modulus operation                    | `mod a b`         |
| push        | Push register to stack               | `push a`          |
| pop         | Pop stack to register                | `pop a`           |
| jmp         | Unconditional jump                   | `jmp 20`          |
| jnz         | Jump if register not zero            | `jnz 20 a`        |
| out         | Output register value                | `out a`           |
| halt        | Stop execution                       | `halt`            |

## Sample Program (prog.asm)
asm
# Process 0 - counts down from 20
    80 movv a 20
    81 movv b 1
    82 sub a b
    83 mov a acc
    84 out acc
    85 jnz 82 a
    86 halt

# Process 1 - counts down from 10000
    90 movv a 10000
    91 movv b 1
    92 sub a b
    93 mov a acc
    94 out acc
    95 jnz 92 a
    96 halt

## Running
## compile the virtual machine: 
    g++ -o vm vm.cpp
## Run the  program: 
    ./vm

## Implementation 
  **Memory**: 100-location memory initialized to 0.<br>
  **Registers**:<br>
          General purpose: a, b, c, d, e, f.<br>
          Special: sp (stack pointer), acc (accumulator), pc (program counter).<br>
          System: ivec (interrupt vector), int (interrupt flag), timer.<br>
  **Scheduling**:<br>
          Timer interrupt every 5 cycles.<br>
          Round-robin switching between 2 processes.<br>
          Process contexts saved on stack.<br>

## Limitations
   Fixed memory size (100 locations),
   Limited to 2 processes ,
   No memory protection,
   Simple round-robin scheduling,
   Basic error handling.

## Future Enhancements
   Add more instructions (multiply, divide, etc.),
   Implement virtual memory,
   Add system calls,
   Support more processes,
   Improve error reporting.

## Output Interpretation
   The program will output:
                       Countdown from 20 (Process 0),
                       Countdown from 10000 (Process 1),
                       Final register and memory states,
                       The counts will be interleaved due to process switching.

## Debugging Tips
  Check register states after halt,
    Examine memory contents for stack values,
    Verify timer and interrupt setting,
    Ensure stack operations don't overflow.

##  Author
   **Dumindu**
