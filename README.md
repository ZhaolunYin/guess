# Guess

[Badge Workflow]: https://img.shields.io/github/actions/workflow/status/zhaolunyin/guess/WORKFLOW_FILE.yml?branch=main&style=for-the-badge
[Badge License]: https://img.shields.io/github/license/zhaolunyin/guess?style=for-the-badge
[Badge Language]: https://img.shields.io/github/languages/top/zhaolunyin/guess?style=for-the-badge
[Badge Pull Requests]: https://img.shields.io/github/issues-pr/zhaolunyin/guess?style=for-the-badge
[Badge Issues]: https://img.shields.io/github/issues/zhaolunyin/guess?style=for-the-badge
[Badge Hi Mom]: https://img.shields.io/badge/Hi-Mom!-ff69b4?style=for-the-badge

[Workflow]: https://github.com/zhaolunyin/guess/actions
[License]: https://github.com/zhaolunyin/guess/blob/main/LICENSE
[Pull Requests]: https://github.com/zhaolunyin/guess/pulls
[Issues]: https://github.com/zhaolunyin/guess/issues

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
