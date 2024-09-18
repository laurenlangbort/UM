# UM 
Universal Machine (UM) Project
This project implements a Universal Machine (UM), a theoretical computational model designed to execute machine-level programs with a simplified instruction set architecture. The goal of the project is to gain hands-on experience in low-level programming, computer architecture, and memory management. 

Link to our design file: https://docs.google.com/document/d/1xR0dicEuOe9QmYkQ4xQAl-VLisdsermEcx4IJq1iH2k/edit?usp=sharing 

Project Overview
Our implementation focuses on:

Memory Management: We created a custom memory module (Um_memory), which manages memory as a sequence of segments represented by C arrays. It handles mapping, unmapping, and recycling of memory segments.

Instruction Implementation: We implemented the UM’s core set of instructions, such as addition, multiplication, division, conditional moves, and bitwise operations. These instructions interact with memory and registers.

Execution Cycle: The program is driven by an execution cycle that reads instructions from memory, executes them, and manages registers and the program counter.

Key Features
Modular Design: The project is divided into modules for memory management, instruction handling, and the execution cycle, ensuring clarity and organization.
Testing: We have thoroughly unit-tested each instruction and run architecture tests to ensure the UM behaves correctly with .um binary files.

Design Decisions
All instructions were implemented in a single module for simplicity.
Memory is handled as a sequence of structs containing C arrays, which store the segment data and the number of elements in each array.
How to Run

Compile the UM using your preferred C compiler.
Run the UM with a .um binary file as input.
The program will execute the instructions from the binary file and produce output (if any).
Testing
We included several test programs to validate the functionality of the UM, such as:

add.um, multiply.um, divide.um: Test basic arithmetic operations.
halt.um: Tests the halt instruction.
map.um, unmap.um: Test memory segment mapping and unmapping.
