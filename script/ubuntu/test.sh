#! /bin/bash

set -u

BASE_PATH="$(dirname "$0")/../.."
cd "$BASE_PATH"

GREEN='\033[0;32m'
RED='\033[0;31m'
OFF='\033[0m'
STATUS=0

# Build the tree visualizer if it doesn't exist and SHOW_AST is true
if [ "${SHOW_AST:-false}" = "true" ] && [ ! -f "build/tree_visualizer" ]; then
	echo "Building tree visualizer..."
	gcc -o build/tree_visualizer src/main/c/tools/tree_visualizer.c -I src/main/c -L build -lCompiler
fi

echo "Compiler should accept..."
echo ""

for test in $(ls src/test/c/accept/); do
	echo -e "\n${GREEN}Testing: $test${OFF}"
	cat "src/test/c/accept/$test" | build/Compiler >/dev/null 2>&1
	RESULT="$?"
	if [ "$RESULT" == "0" ]; then
		echo -e "    $test, ${GREEN}and it does${OFF} (status $RESULT)"
		# Show the tree for accepted tests only if SHOW_AST is true
		if [ "${SHOW_AST:-false}" = "true" ]; then
			build/tree_visualizer "src/test/c/accept/$test"
		fi
	else
		STATUS=1
		echo -e "    $test, ${RED}but it rejects${OFF} (status $RESULT)"
	fi
done
echo ""

echo "Compiler should reject..."
echo ""

for test in $(ls src/test/c/reject/); do
	cat "src/test/c/reject/$test" | build/Compiler >/dev/null 2>&1
	RESULT="$?"
	if [ "$RESULT" != "0" ]; then
		echo -e "    $test, ${GREEN}and it does${OFF} (status $RESULT)"
	else
		STATUS=1
		echo -e "    $test, ${RED}but it accepts${OFF} (status $RESULT)"
	fi
done
echo ""

echo "All done."
exit $STATUS
