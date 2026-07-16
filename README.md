# Guess

[![Badge Workflow]][Workflow]
[![Badge License]][License] 
![Badge Language] 
[![Badge Pull Requests]][Pull Requests] 
[![Badge Issues]][Issues] 
![Badge Hi Mom]

> *A C program that creates a guessing game based on any csv file.*

# Features

- Portable guessing game capable of using any text-based dataset.
- Many flags for complete control of randomness and function.
- Can show all fields to aid as a study tool.
- Written in C for maximum speed for large datasets.

# Install

```bash
# Clone this repository
git clone https://github.com/zhaolunyin/guess.git && cd guess

# Compile
make
```

# Usage

```bash
# Run with test file
./guess fruits.csv

# Run with max 7 attempts
./guess -n 7 fruits.csv

# Show all fields by default
./guess -a fruits.csv

# Run case-sensitive
./guess -c fruits.csv

# Run using row 5 excluding header
./guess -l 5 fruits.csv

# Run with set seed
./guess -s 6767 fruits.csv
```
