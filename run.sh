clear
make clean
make
if [ $? -eq 0 ]; then
    echo "Compilação bem-sucedida. Executando o programa..."
    ./queimadas_tui
else
    echo "Erro na compilação."
fi
