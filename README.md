
# uni_converter

`uni_converter` is a command-line tool for converting between different data types: **ASCII**, **Binary**, **Octal**, **Decimal**, and **Hexadecimal**.

It supports a **single input type** and **multiple output types**. You can mix and match output flags to see results in multiple formats at once.

---

## Features

- Convert between:
  - ASCII
  - Binary
  - Octal
  - Decimal (Integer)
  - Hexadecimal
- Multiple outputs per run
- Byte-width control for hex and binary
- Supports multiple inputs at once

---

## Usage

```bash
uni_converter [Input...] [Flags]
```

- Input flags must be **uppercase** (only **one** input type allowed per run)
- Output flags are **lowercase**
- Flags must be passed back-to-back (no spaces in between)

### Example

```bash
uni_converter 0xFF -Xai
```

This command converts `0xFF` (hexadecimal) into:
- ASCII (`-a`)
- Decimal integer (`-i`)

### Multiple Inputs

For multiple values, enclose them in quotes and separate by spaces:

```bash
uni_converter "0xFF 0xAB 0xC0" -Xa
```

---

## Flags

### Input Flags (uppercase — **only one allowed**)
| Flag | Description              |
|------|--------------------------|
| `-X` | Input is hexadecimal     |
| `-I` | Input is integer         |
| `-A` | Input is ASCII           |
| `-B` | Input is binary          |
| `-O` | Input is octal           |

### Output Flags (lowercase — **use as many as you want**)
| Flag | Description                  |
|------|------------------------------|
| `-x` | Output as hexadecimal        |
| `-i` | Output as decimal (int64)    |
| `-a` | Output as ASCII              |
| `-b` | Output as binary             |
| `-o` | Output as octal              |

### Other Flags
| Flag         | Description                                                                 |
|--------------|-----------------------------------------------------------------------------|
| `--bytes=N`  | Number of bytes to parse at a time (only for hex or binary input)          |
| `-h`         | Show short help                                                             |
| `--help`     | Show full help                                                              |

---

## Notes

- **Only one input type can be used per execution**
- Binary and hex input defaults to 1-byte parsing unless `--bytes` is specified
- Invalid combinations of flags will result in an error

---

## Example Usages

```bash
uni_converter "A B C" -Axi    # ASCII input to hex and decimal
uni_converter "10101010 11110000" -Bb           # Binary input to binary output
uni_converter "0xFA 0xFB" -Xo                   # Hex input to octal
uni_converter 42 -Ii                            # Integer input to decimal
```

---

## Help Command

```bash
uni_converter -h
```

or

```bash
uni_converter --help
```
