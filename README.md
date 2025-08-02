# RV32I Disassembler

This project is a disassembler for the **RISC-V 32-bit base integer instruction set (RV32I)** only.

## Features

* Takes a binary file containing RV32I machine code as input.
* Translates binary instructions into human-readable assembly code.
* Supports only the **RV32I base instruction set**.
* Does **not** support any RISC-V extensions (e.g., M, A, F, D, C).

## Usage

```bash
./rv32i-disassembler input.bin
```

* `input.bin` should be a binary file containing raw RV32I instructions.
* The disassembled output will be printed to the console (or redirected as needed).

## Limitations

* **Only RV32I** is supported — no other instruction set extensions are handled.
* No symbolic disassembly (labels, function names, or address resolution are not included).

## Purpose

This project is primarily educational, intended to provide insight into the structure of RV32I machine code and the basics of instruction decoding.

## License

This project is licensed under the MIT License (or another license of your choice).
