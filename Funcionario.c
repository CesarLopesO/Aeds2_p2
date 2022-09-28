#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funcionario.h"

void toString(TFunc *func){

    printf("\n\nNome: %s", func->nome);
    printf("\nCPF: %s", func->cpf);
    printf("\nCodigo: %i", func->cod);
    printf("\nData de nascimento: %s", func->data_nascimento);
    printf("\nSalario: %.2f", func->salario);
    printf("\n");

}

void salva_arq(TFunc *func, FILE *arq){

    fwrite(&func->cod, sizeof(int), 1, arq);
    fwrite(func->nome, sizeof(char), sizeof(func->nome), arq);
    fwrite(func->cpf, sizeof(char), sizeof(func->cpf), arq);
    fwrite(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento), arq);
    fwrite(&func->salario, sizeof(float), 1, arq);

}

TFunc *le(FILE *in){

    TFunc *func = (TFunc*) malloc(sizeof(TFunc));

    if(0 >= fread(&func->cod, sizeof(int), 1, in)){
        free(func);
        return NULL;
    }
    fread(func->nome, sizeof(char), sizeof(func->nome), in);
    fread(func->cpf, sizeof(char), sizeof(func->cpf), in);
    fread(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento), in);
    fread(&func->salario, sizeof(float), 1, in);

    return func;
}

void cria_base_de_dados(FILE *arq, int nFunc){
int vetorrandom[100]= {11,25,97,80,35,79,67,19,96,81,4,46,27,50,23,9,3,91,29,69,99,65,57,
98,16,21,66,5,49,62,72,83,92,47,89,87,32,88,74,90,15,85,17,26,22,8,59,28,42,
18,71,37,70,75,93,55,53,30,45,48,95,1,86,33,43,7,31,60,2,63
,68,82,38,84,64,56,24,44,61,41,100,78,12,73,51,54,20,6,52,76,34,58,14,77,13,39,36,10,40,94};
    

    for(int i=1; i<= nFunc; i++){
        int rand_num =vetorrandom[i];
        TFunc func;
        func.cod = rand_num;
        sprintf(func.nome, "Funcionario %i", rand_num);
        sprintf(func.cpf, "000.000.000-00");
        sprintf(func.data_nascimento, "01/01/2000");
        func.salario = 1000 + rand_num;
        fseek(arq, (i-1)*sizeof(TFunc), SEEK_SET);
        salva_arq(&func, arq);

    }
}

TFunc *busca_sequencial(int cod, FILE *arq, int tam_arq, int *comp){

    for(int i=0; i<tam_arq; i++){
        *comp +=1;
        fseek(arq, i*sizeof(TFunc), SEEK_SET);
        TFunc *func = le(arq);
        if(func->cod == cod){
            return func;
        }
    }

    return NULL;

}

TFunc *buscaBinaria (int cod, int qtd_func, FILE *arq, int *comp){

    int l = 0, r = qtd_func-1;

    while(l <= r){
        *comp +=1;
        int meio = (l+r)/2;
        fseek(arq, meio*sizeof(TFunc), SEEK_SET);
        TFunc *func = le(arq);
        if(cod == func->cod){
            return func;
        }else if(func->cod < cod){
            l = meio+1;
        }else{
            r = meio-1;
        }
    }
    return NULL;
}

void key_sorting(FILE *arq, int *key_array, int qtd_func){

    for(int i=0; i<qtd_func; i++){
        fseek(arq, i*sizeof(TFunc), SEEK_SET);
        TFunc *func = le(arq);
        key_array[i] = func->cod;
    }

}

void ordena_array(Key_sorting *array, int qtd_func){
    //ordena��o simples

    for (int i=0; i< qtd_func; i++) {
        for (int j=i+1; j< qtd_func; j++) {
            if (array[i].cod > array[j].cod) {
                Key_sorting aux = array[i];
                array[i] = array[j];
                array[j] = aux;
            }
        }
    }
}

void Key_sorting_file(FILE *arq, FILE *arq_ordenado, int qtd_func){

    printf("\nReaizando Key Sorting ... \n");

    clock_t start_time, end_time;
    double temp_exe = 0.0;

    start_time = clock();

    Key_sorting tag_array[qtd_func];

    rewind(arq);

    for(int i=0; i<qtd_func; i++){ //for pra preencher array do rrn
        fseek(arq, i*sizeof(TFunc), SEEK_SET);
        tag_array[i].RRN = ftell(arq);//retornar a quantidade de bytes/endere�o de memoria
        TFunc *func = le(arq);//recebe o arquivo naquela posi��o/le o func naquela posi��o
        tag_array[i].cod = func->cod;
    }

    ordena_array(&tag_array, qtd_func);// ORDENA O ARRAY

    rewind(arq);

    for(int i=0; i<qtd_func; i++){

        fseek(arq,tag_array[i].RRN, SEEK_SET);//funciona analogamente a sizeoff, ja que � a quantidade de bytes de um tfunc
        TFunc *func = le(arq);//arquivo ainda desordenado

        fseek(arq_ordenado, i*sizeof(TFunc), SEEK_SET);//BUSCA ATRAV�S DA RRN, BUSCA MALUCA
        salva_arq(func, arq_ordenado);
    }

    end_time = clock();
    temp_exe += (double)(end_time-start_time)/CLOCKS_PER_SEC;
    printf("\n## Tempo gasto na Execucao do Key Sorting: %.8f s\n", temp_exe);
}
void insertion_sort_disco(FILE *arq, int qtd_func){
    printf("\nRealizando Insertion sort ... \n");
    rewind(arq);
    int i;
    for (int j = 2; j <= qtd_func; j++) {
        fseek(arq, (j-1) * sizeof(TFunc), SEEK_SET);//posiciona o cursor no segundo valor do arquivo
        TFunc *func_j = le(arq);//le o valor do file naquela posição
        i = j - 1;
        fseek(arq, (i-1) * sizeof(TFunc), SEEK_SET);//posiciona o cursor na primeira posição do arquivo do arquivo
        TFunc *func_i = le(arq);// le e recebe o valor daquela posição
        while ((i > 0) && (func_i->cod > func_j->cod)) {//comparando os 2 tfunc lidos, se o segundo valor for maior que o primeiro, ele ja manda a troca dos valores, por meio dos fseeks abaixo.
            fseek(arq, i * sizeof(TFunc), SEEK_SET);
            salva_arq(func_i, arq);// ja salva o func i na posição nova do arquivo, mais a esquerda
            i = i - 1;
            fseek(arq, (i-1) * sizeof(TFunc), SEEK_SET);//move cursor pra posição a esquerda
            func_i = le(arq);// salva o func i
         }
        fseek(arq, (i) * sizeof(TFunc), SEEK_SET);// coloca o funcj no mais a direita
        salva_arq(func_j, arq);
    }
    fflush(arq);
}