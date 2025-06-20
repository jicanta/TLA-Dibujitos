#! /bin/bash

set -euo pipefail

BASE_PATH="$(dirname "$0")/../.."
cd "$BASE_PATH"

INPUT="$1"
OUTPUT_FILE="${INPUT}.svg"
shift 1

echo "Compiling $INPUT to $OUTPUT_FILE..."
cat "$INPUT" | build/Compiler -o "$OUTPUT_FILE" "$@"
echo "SVG generated: $OUTPUT_FILE"
