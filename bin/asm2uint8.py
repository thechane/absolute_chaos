#!/usr/bin/env python3
import sys, re

def parse_bytes(text: str):
    # Grab $FF, 0xFF, or decimal numbers anywhere in the text
    tokens = re.findall(r'\$[0-9A-Fa-f]+|0x[0-9A-Fa-f]+|\b\d+\b', text)
    vals = []
    for t in tokens:
        if t.startswith('$'):
            v = int(t[1:], 16)
        elif t.lower().startswith('0x'):
            v = int(t, 16)
        else:
            v = int(t, 10)
        if not 0 <= v <= 255:
            sys.exit(f"Error: value {t} parsed as {v} is out of range 0..255.")
        vals.append(v)
    return vals

def main():
    data = sys.stdin.read()
    if not data.strip():
        sys.exit("No input detected. Pipe/paste your DEFB lines into stdin.")

    bytes_ = parse_bytes(data)
    n = len(bytes_)
    if n % 32 != 0:
        r = n % 32
        need = 32 - r
        sys.exit(f"Error: found {n} bytes; not a multiple of 32 (remainder {r}). "
                 f"Add {need} more bytes or remove {r} bytes.")

    # Emit t1, t2, t3... each with 32 bytes
    for i in range(0, n, 32):
        chunk = bytes_[i:i+32]
        idx = i // 32 + 1
        hexes = ",".join(f"0x{b:02X}" for b in chunk)
        print(f"static const uint8_t t{idx}[32]={{" + hexes + "};")

if __name__ == "__main__":
    main()
