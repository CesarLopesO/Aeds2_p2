#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Funcionario.h"


int main(){

    FILE *arq = fopen("dados.dat", "wb+");
    FILE *arq_ordenado = fopen("dadosOrdenados.dat", "wb+");

    clock_t start_time_seq, end_time_seq, start_time_bin, end_time_bin,start_time_key,end_time_key,start_time_ins,end_time_ins;

    int quantidadeFuncionarios = 100; ///MUDE PARA AUMENTAR OU DIMINUIR A QUANTIDADE DE FUNCIONARIOS
    int procurarCodigo = 32; ///MUDE PARA ALTERAR QUAL CODIGO DO FUNCIONARIO PROCURAR
    int selecao=15;
    cria_base_de_dados(arq, quantidadeFuncionarios);



    int comparacoes = 0, comparacoesBinaria = 0;
    double tempoExecucao = 0.0, temp_exe_bin = 0.0,temp_exe_key =0.0,temp_exe_ins;
    if(arq == NULL){
        printf("\nARQUIVO INEXISTENTE");
        return 1;
    }

    if(arq_ordenado == NULL){
        printf("\nARQUIVO INEXISTENTE");
        return 1;
    }


    while(selecao!= 0){
printf("0- SAIR DO PROGRAMA\n");
printf("1-ORDENAR VIA KEY SORTING\n");
printf("2-ORDENAR VIA INSERTION SORT\n");
printf("3-BUSCA BINARIA\n");
printf("4-SEQUENCIAL\n");
scanf("%d",&selecao);
switch (selecao){
 case 0:
printf("SAINDO DO PROGRAMA...");
break;
case 1:
start_time_key = clock();
Key_sorting_file(arq, arq_ordenado, quantidadeFuncionarios);
end_time_key = clock();
temp_exe_key += (double)(end_time_key-start_time_key)/CLOCKS_PER_SEC;
break;
case 2:
start_time_ins = clock();
insertion_sort_disco(arq, quantidadeFuncionarios);
end_time_ins = clock();
temp_exe_ins += (double)(end_time_ins-start_time_ins)/CLOCKS_PER_SEC;
break;
case 3:
start_time_bin = clock();
    func = buscaBinaria(procurarCodigo, quantidadeFuncionarios, arq_ordenado, &comparacoesBinaria);
    end_time_bin = clock();

    temp_exe_bin += (double)(end_time_bin-start_time_bin)/CLOCKS_PER_SEC;
       printf("\nBusca binaria\n");
    printf("\nBuscando funcionario: %d",procurarCodigo);
    if(func == NULL){
        printf("\nFuncionario inexistente na base de dados\n");
        return 1;
    }else{
        printf("\nFuncionario encontrado: Imprimindo...");
        toString(func);
    }
    printf("\nNo de comparacoes da busca Binaria: %i", comparacoesBinaria);
    printf("\nTempo gasto na Execucao da busca Binaria: %.10f s\n", temp_exe_bin);

break;
case 4:
start_time_seq = clock();
TFunc *func = busca_sequencial(procurarCodigo, arq, quantidadeFuncionarios, &comparacoes);
end_time_seq = clock();
tempoExecucao += (double)(end_time_seq-start_time_seq)/CLOCKS_PER_SEC;
printf("Buscando funcionario: %d\n", procurarCodigo);
    if(func == NULL){
        printf("Funcionario inexistente na base de dados\n");
        return 1;
    }else{
        printf("Funcionario encontrado: Imprimindo...");
        toString(func);
    }

    printf("\nComparacoes da busca sequencial: %i",comparacoes);
    printf("\nTempo de busca sequencial: %.10f s\n", tempoExecucao);


break;
}


}
    
    

    start_time_seq = clock();
    TFunc *func = busca_sequencial(procurarCodigo, arq, quantidadeFuncionarios, &comparacoes);
    end_time_seq = clock();

    tempoExecucao += (double)(end_time_seq-start_time_seq)/CLOCKS_PER_SEC;

    printf("Buscando funcionario: %d", procurarCodigo);
    if(func == NULL){
        printf("\nFuncionario inexistente na base de dados\n");
        printf("\n");
        return 1;
    }else{
        printf("\nFuncionario encontrado: Imprimindo...");
        toString(func);
    }

    printf("\nComparacoes da busca sequencial: %i",comparacoes);
    printf("\nTempo de busca sequencial: %.10f s\n", tempoExecucao);

    Key_sorting_file(arq, arq_ordenado, quantidadeFuncionarios);

    start_time_bin = clock();
    func = buscaBinaria(procurarCodigo, quantidadeFuncionarios, arq_ordenado, &comparacoesBinaria);
    end_time_bin = clock();

    temp_exe_bin += (double)(end_time_bin-start_time_bin)/CLOCKS_PER_SEC;

    printf("\n\n------------------------------------------------------\n\n");
    printf("\nBusca binaria\n");
    printf("\nBuscando funcionario: %d",procurarCodigo);
    if(func == NULL){
        printf("\nFuncionario inexistente na base de dados\n");
        return 1;
    }else{
        printf("\nFuncionario encontrado: Imprimindo...");
        toString(func);
    }
    printf("\nComparacoes da busca binario: %i", comparacoesBinaria);
    printf("\nTempo de busca binario: %.10f s\n", temp_exe_bin);

    free(func);
    fclose(arq);
    fclose(arq_ordenado);

    return 0;
}
