#include <stdio.h>       
#include <stdlib.h>

#define TOTAL_REGISTROS 15220       //aqui define o total de registros que serão lidos

/*O struct Queimada armazena os dados sobre cada ocorrencia. 
Talvez seja desnecessario armazenar tudo isso ja que so precimos de data e municipio mas ta ai*/

typedef struct {
    long long id_bdq;
    char foco_id[37];
    double lat;
    double lon;
    char data_pas[20];
    char pais[50];
    char estado[50];
    char municipio[100];
    char bioma[50];
} Queimada;

int main() {
	//aqui é criado um ponteiro do tipo FILE (vem da biblioteca <stdio.h>), a função fopen abre o arquivo ACRE3 para leitura
    FILE *arquivo = fopen("ACRE3.csv", "r");
    
    //se der algum erro ao abrir, isso será impresso
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo ACRE3.csv");
        return 1;
    }

	//a função malloc reserva memória suficiente para 15220 registros do tipo Queimada (struct criado ali em cima).
    Queimada *vetorQueimadas = (Queimada *)malloc(TOTAL_REGISTROS * sizeof(Queimada));
    
    //se der algum erro ao abrir, isso será impresso
    if (vetorQueimadas == NULL) {
        fprintf(stderr, "Erro: Falha na alocação de memória.\n");
        fclose(arquivo);
        return 1;
    }
	
	//bufferLinha vai armazenar temporariamente cada linha do CSV (até 511 caracteres + '\0').
	//contador vai contar quantos registros foram realmente lidos
    char bufferLinha[512];
    int contador = 0;


	//fgets lê uma linha do arquivo e armazena em bufferLinha.
	//sscanf lê dados de uma string (bufferLinha) e coloca nas variáveis indicadas
	//itensLidos garante que todos os 9 campos foram lidos corretamente.
    while (fgets(bufferLinha, sizeof(bufferLinha), arquivo) && contador < TOTAL_REGISTROS) {
        int itensLidos = sscanf(bufferLinha,
               "%lld,%36[^,],%lf,%lf,%19[^,],%49[^,],%49[^,],%99[^,],%49[^\n]",
               &vetorQueimadas[contador].id_bdq,
               vetorQueimadas[contador].foco_id,
               &vetorQueimadas[contador].lat,
               &vetorQueimadas[contador].lon,
               vetorQueimadas[contador].data_pas,
               vetorQueimadas[contador].pais,
               vetorQueimadas[contador].estado,
               vetorQueimadas[contador].municipio,
               vetorQueimadas[contador].bioma);

        if (itensLidos == 9) {
            contador++; 
        }
    }

    fclose(arquivo);

    printf("Sucesso! %d registros foram lidos e armazenados no vetor.\n\n", contador);

    //Essa parte aqui pedi para o Gemini fazer para verificar se os dados foram realmente adicionados no vetor, pode remover.
    printf("--- Exibindo os 3 primeiros registros do vetor ---\n");
    for (int i = 0; i < 4 && i < contador; i++) {
        printf("  Registro %d:\n", i + 1);
        printf("  ID BDQ: %lld\n", vetorQueimadas[i].id_bdq);
        printf("  Foco ID: %s\n", vetorQueimadas[i].foco_id);
        printf("  Latitude: %f\n", vetorQueimadas[i].lat);
        printf("  Longitude: %f\n", vetorQueimadas[i].lon);
        printf("  Data: %s\n", vetorQueimadas[i].data_pas);
        printf("  País: %s\n", vetorQueimadas[i].pais);
        printf("  Estado: %s\n", vetorQueimadas[i].estado);
        printf("  Município: %s\n", vetorQueimadas[i].municipio);
        printf("  Bioma: %s\n", vetorQueimadas[i].bioma);
        printf("--------------------------------------------------\n");
    }

    free(vetorQueimadas);

    return 0;
}
