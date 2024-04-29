# k8d - Kristoffer's Chip-8 Disassembler

k8d is a simple program for disassembling Chip-8 binary files.

## Installation

Clone the k8d repository to your computer. The main branch contains the latest 
version.

    $ git clone https://github.com/kawright/k8d.git

Compile the binary.

    $ make all

Install the compiled binary.
    
    $ make install

You can clean up the build artifacts once you have finished installation.

    $ make clean

You can also uninstall k8d using the provided make script.

    $ make uninstall


## Usage

    $ k8d FILENAME [-a] [-h] [-o PATH]


### Arguments

- **`FILENAME`**: The path to the file being disassembled.


### Options

- **`-a`**: Print information about k8d to the console, and exit.

- **`-h`**: Print help text to the console, and exit.

- **`-o PATH`**: Write output to the file at PATH.


## Exit Codes

- **`0`**: OK. No error occurred.

- **`1`**: General error.

- **`2`**: Malformed argument vector.

- **`3`**: Out-of-memory.

- **`4`**: Sub-system initialization.

- **`5`**: Out-of-range memory access.

- **`6`**: IO error.

- **`7`**: Corrupt data.

## Authors

- Kristoffer A. Wright <kris.al.wright@gmail.com>


## Reporting Bugs

Please send bug reports to kris.al.wright@gmail.com with the subject line
"k8e BUG REPORT: ..."

