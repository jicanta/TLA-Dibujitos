#!/bin/bash

# Colores para la salida
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Directorios y ejecutables
TEST_DIR="./src/test/c/accept"
TREE_VISUALIZER="./build/tree_visualizer"

# Verificar que el visualizador existe
if [ ! -f "$TREE_VISUALIZER" ]; then
    echo -e "${YELLOW}El visualizador no existe, compilando...${NC}"
    make tree_visualizer
    
    if [ ! -f "$TREE_VISUALIZER" ]; then
        echo -e "${RED}No se pudo compilar el tree_visualizer. Abortando.${NC}"
        exit 1
    fi
fi

# Verificar que el directorio de tests existe
if [ ! -d "$TEST_DIR" ]; then
    echo -e "${RED}Directorio de tests no encontrado: $TEST_DIR${NC}"
    exit 1
fi

# Contar los tests (excluir archivos .svg)
total_tests=$(find "$TEST_DIR" -type f ! -name "*.svg" | wc -l)
echo -e "${GREEN}Ejecutando $total_tests tests...${NC}\n"

# Ejecutar tree_visualizer con cada archivo
count=1
for test_file in $(find "$TEST_DIR" -type f ! -name "*.svg" | sort); do
    echo -e "${YELLOW}============================================================${NC}"
    echo -e "${GREEN}[$count/$total_tests] Analizando: ${NC}$(basename "$test_file")"
    echo -e "${YELLOW}============================================================${NC}"
    
    # Ejecutar el visualizador y capturar el código de salida
    $TREE_VISUALIZER "$test_file"
    exit_code=$?
    
    if [ $exit_code -ne 0 ]; then
        echo -e "${RED}Error ejecutando tree_visualizer (código: $exit_code)${NC}"
    fi
    
    # Separador entre tests
    echo -e "\n\n"
    
    ((count++))
done

echo -e "${GREEN}Todos los tests han sido procesados.${NC}"