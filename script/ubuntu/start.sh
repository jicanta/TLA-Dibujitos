#! /bin/bash

set -euo pipefail
export ASAN_OPTIONS=detect_leaks=0
BASE_PATH="$(dirname "$0")/../.."
cd "$BASE_PATH"

INPUT="$1"
OUTPUT_FILE="${INPUT}.svg"
shift 1

echo "Compiling $INPUT to $OUTPUT_FILE..."
cat "$INPUT" | build/Compiler -o "$OUTPUT_FILE" "$@"
echo ""
echo "SVG generated: $OUTPUT_FILE"

# Print usage information if no additional arguments provided
if [ $# -eq 0 ]; then
    echo ""
    echo "Additional options:"
    echo "  -w <width>     Set SVG width (default: 1000)"
    echo "  -h <height>    Set SVG height (default: 1000)"
    echo "  --width <w>    Set SVG width (default: 1000)"
    echo "  --height <h>   Set SVG height (default: 1000)"
    echo ""
    echo "Example: ./start.sh myfile.dib -w 800 -h 600"
fi
