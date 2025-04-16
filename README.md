# IMX Assembler/Disassembler

**IMX** is a simple assembler and disassembler designed for the SDMA RISC CPU of the i.MX6 series. This tool allows developers to convert assembly code into machine code and vice versa, facilitating low-level programming and analysis for the i.MX6's SDMA engine.

## Features

- **Assembler**: Converts SDMA assembly code into machine code.
- **Disassembler**: Translates machine code back into human-readable assembly.
- **Custom Instruction Set Support**: Tailored for the specific instruction set of the SDMA RISC CPU.
- **Command-Line Interface**: Easily integrate into scripts and development workflows.

## Getting Started

### Prerequisites

- **Flex**: For lexical analysis.
- **Bison**: For parsing.
- **GCC**: C compiler.
- **Make**: Build automation tool.

Ensure these tools are installed on your system before proceeding.

### Building the Project

Clone the repository and build the project using the provided Makefile:

```bash
git clone https://github.com/yairgd/imx.git
cd imx
make
```

This will generate the assembler and disassembler binaries.

## Usage

### Assembler

To assemble an SDMA assembly file:

```bash
./asm input.asm -o output.bin
```

- `input.asm`: Your assembly source file.
- `output.bin`: The resulting machine code output.

### Disassembler

To disassemble a machine code file:

```bash
./disasm input.bin -o output.asm
```

- `input.bin`: Machine code input file.
- `output.asm`: The resulting assembly code output.

## Project Structure

- `asm.lex`: Lexical analyzer definitions for tokenizing assembly code.
- `asm.yacc`: Grammar rules for parsing assembly code.
- `instructions.c` & `instructions.h`: Definitions and implementations of the SDMA instruction set.
- `Makefile`: Build instructions for compiling the assembler and disassembler.
- `README.md`: Project documentation.
- `info.txt`: Additional information about the project.

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request with your enhancements.

## License

This project is licensed under the MIT License.

