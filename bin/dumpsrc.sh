#!/bin/bash

# Recursively find .c and .h files in ./src
find ./src -type f \( -name "*.c" -o -name "*.h" \) | while read -r file; do
    echo "=============================="
    echo "FILE: $file"
    echo "=============================="
    cat "$file"
    echo -e "\n"
done
