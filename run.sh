gcc -o queimadas_cli src/main.c -lmenu -lncurses
if [ $? -eq 0 ]; then
    echo "Compilação bem-sucedida. Executando o programa..."
    ./queimadas_cli
else
    echo "Erro na compilação."
fi
