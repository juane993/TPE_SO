#!/bin/bash

# Encuentra y formatea todos los archivos .c y .h a partir del directorio actual
find "$(pwd)" -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -i {} \;

echo "Formateo completo."
