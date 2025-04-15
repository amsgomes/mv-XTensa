#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int PC; 
int a[4]; //Registradores (a0, a1, a2, a3)

struct Memoria{
    int cod; 
    int op1;
    int op2;
    int op3;
};

struct Memoria mem[320];

void inicializar_mv();
int converter_registradores(char *s);
void carregar_programa();
void simular_programa();

// Função de inicialização da MV
void inicializar_mv() 
{   
    int i;

    PC = 0; 

    for (i = 0; i < 4; i++) {
        a[i] = 0;
    }

    for (i = 0; i < 320; i++) {
        mem[i].cod = 0; 
        mem[i].op1 = 0; 
        mem[i].op2 = 0; 
        mem[i].op3 = 0; 
    }
}

// Função para converter string em decimal
int converter_registradores(char *s) 
{
    if (strcmp(s, "a0") == 0){
        return 0;
    }    
    if (strcmp(s, "a1") == 0){
        return 1;
    }    
    if (strcmp(s, "a2") == 0){
        return 2;
    }    
    if (strcmp(s, "a3") == 0){ 
        return 3;
    }

    return atoi(s); // se não for registrador, assume número
}

// Função de ler arquivo e carregar na memória
void carregar_programa() 
{
    FILE *f;
    char nomeArq[30];
    int i = 0;

    printf("\nDigite o nome do arquivo: ");
    scanf("%s", nomeArq);

    f = fopen(nomeArq, "r");
    if (!f) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    while (!feof(f) && i < 320) {
        char cod[10], op1[10], op2[10], op3[10];
        int lidos; 

        while (i < 320 && (lidos = fscanf(f, "%s %s %s %s", cod, op1, op2, op3)) != EOF) {
            if (lidos == 4) {
                if (i == 0) {
                    PC = atoi(cod);
                } else {
                    mem[i - 1].cod = atoi(cod);
                    mem[i - 1].op1 = converter_registradores(op1);
                    mem[i - 1].op2 = converter_registradores(op2);
                    mem[i - 1].op3 = converter_registradores(op3);
                }
                i++;
            }
        }
    }

    fclose(f);
}

// Função que realiza a simulação do programa
void simular_programa()
{
    while (1) {
        switch (mem[PC].cod) {
            case 0: // add
                a[mem[PC].op1] = a[mem[PC].op2] + a[mem[PC].op3];
            break;

            case 1: //sub
                a[mem[PC].op1] = a[mem[PC].op2] - a[mem[PC].op3];
            break;

            case 2: //mul
                a[mem[PC].op1] = a[mem[PC].op2] * a[mem[PC].op3];
            break;

            case 3: //div
                if (a[mem[PC].op3] == 0) {
                    printf("Divisão por zero.\n");
                    return;
                }
                a[mem[PC].op1] = a[mem[PC].op2] / a[mem[PC].op3];
            break;

            case 4: //mv
                a[mem[PC].op1] = mem[mem[PC].op2].cod;
            break;

            case 5: //st
                mem[mem[PC].op2].cod = a[mem[PC].op1];
            break;

            case 6: //jmp
                PC = mem[PC].op1;
                continue;

            case 7: //jeq
                if (a[mem[PC].op1] == a[mem[PC].op2]){
                        PC = mem[PC].op3;
                }else{
                    PC++;
                }    
                continue;

            case 8: //jgt
                if (a[mem[PC].op1] > a[mem[PC].op2]){
                    PC = mem[PC].op3;
                }else{
                    PC++;
                }
                continue;

            case 9: //jlt
                if (a[mem[PC].op1] < a[mem[PC].op2]){
                    PC = mem[PC].op3;
                }else{
                    PC++;
                }
                continue;

            case 10: //w
                printf("mem[%d] = %d\n", mem[PC].op1, mem[mem[PC].op1].cod);
            break;

            case 11: //r
                printf("Digite um valor para mem[%d]: ", mem[PC].op1);
                scanf("%d", &mem[mem[PC].op1].cod);
            break;

            case 12: //stp
                printf("Execução finalizada.\n");
                return;
            break;

            default:
                printf("\nInstrução inválida: %d\n", mem[PC].cod);
                return;
        }

        PC++;
    }
}

int main()
{
    inicializar_mv(); 
    carregar_programa();
    simular_programa();
    return 0;
}

