//GABRIELA CHAVEZ 10295440
//RAFAEL ROQUE 10295412
//VITOR AVIAN 10295392

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//A estrutura abaixo possui todos os campos do registro e servirá para a manipulação do registro,
//nas operações implementadas neste código. O numero de elementos é 11, nos dois campos fixos, pois o último é o '\0'.
typedef struct dados{
    int codEscola; //Código da escola.
    char dataInicio[11]; //Data de início do ano letivo.
    char dataFinal[11]; //Data do final do ano letivo.
    char *nomeEscola; //Nome da escola.
    char *municipio; //Nome do município.
    char *endereco; //Endereço da escola.
    int tamEsc, tamMun, tamEnd; //Variáveis auxiliares que serão utilizadas para saber o tamanho dos campos variáveis. 
}Dados;

//Função que inicia a struct Dados, com os respectivos valores inicializadores de cada variável para poder manipular os
//dados ao longo do trabalho. 
void iniciaDado(Dados *d){ 
    d->codEscola=0;
    strcpy(d->dataInicio, "0000000000");
    strcpy(d->dataFinal, "0000000000");
    d->nomeEscola=NULL;
    d->municipio=NULL;
    d->endereco=NULL;
    d->tamEsc=0;
    d->tamMun=0;
    d->tamEnd=0;
}

//A função abaixo retornará o campo de tamanho variável. Recebe como parâmetros um ponteiro para o arquivo e um ponteiro 
//de inteiro, que conterá o tamanho do campo variável. Deste modo, obtêm-se a partir desta função, a string do campo e 
//seu tamanho.
char *leCampo(FILE *fe, int *aux){
    char *vet;
    vet = (char*)malloc(sizeof(char)*100);
    int x=fscanf(fe,"%[^;\n]", vet); //A função fscanf() lerá o que está arquivo até encontrar um ';' ou '\n'.
    fgetc(fe); //É consumido o char ';' ou '\n' encontrado na linha anterior.
    if(x==0) //Checa-se se o tamanho do campo é 0, ou seja, nulo. Caso seja, aux recebe esse valor. 
        *aux=0;
    else //Caso contrário, recebe o tamanho do campo.
        *aux=strlen(vet);
    return vet;    
}

//A função a seguir lê os campos de tamanho fixo. Os parâmetros são o ponteiro que manipula o arquivo e a string de tamanho
//fixo.
void leCampoFixo(FILE *fe, char *data){
    int aux=fscanf(fe,"%[^;\n]", data);
    if(aux==0) //É tratado neste ponto o caso do registro ser nulo.
        strcpy(data, "0000000000");
    fgetc(fe); 
}

//Função [10]:
void insercaoArvore(char *entrada){
    char status = '0';
    int noRaiz;
    int altura;
    int ultimoRRN;
}


//Função [1]: 
void insercao(char *entrada){ 
    //Variaveis do cabeçalho.
    char status='0';
    int topoPilha=-1;

    FILE *fd=fopen("arq.bin", "wb");//Criar o arquivo de dados binário.
    fwrite(&status, 1, 1, fd);//Escrever o status do cabeçalho.
    fwrite(&topoPilha, 4, 1, fd);//Escrever o topo da pilha.
    fclose(fd);//Fechar o arquivo.
    
    FILE *fe=fopen(entrada, "r");//Abrir o arquivo de entrada.csv para leitura.
    
    if(fe!=NULL){//Se a abertura foi sucedida.
        while(feof(fe)==0){//Enquanto o arquivo de entrada não acabou, continuar.
            Dados d;//Registro de dados para facilitar a passagem dos campos de um arquivo pro outro.
            iniciaDado(&d);//Iniciar o registro de dados.
            char aux[10];//Char auxiliar para ler.
            leCampoFixo(fe, aux);//Le o codigo da escola.
            d.codEscola= atoi(aux);//Transforma em inteiro. 
            //Le os campos variaveis:       
            d.nomeEscola=leCampo(fe, &d.tamEsc);        
            d.municipio=leCampo(fe, &d.tamMun);        
            d.endereco=leCampo(fe, &d.tamEnd);        
            //Le as datas:
            leCampoFixo(fe, d.dataInicio);
            leCampoFixo(fe, d.dataFinal);       

            fd=fopen("arq.bin", "ab");//Abre o arquivo de dados para escrever as informaçoes lidas.
            int flag=0;//Flag para sinalizar que o registro existe.
            //Escrever todas as informações do registro no arquivo de dados:
            fwrite(&flag, sizeof(int), 1, fd);
            fwrite(&d.codEscola, sizeof(int), 1, fd);
            fwrite(d.dataInicio, sizeof(char)*10, 1, fd);
            fwrite(d.dataFinal, sizeof(char)*10, 1, fd);
            fwrite(&d.tamEsc, sizeof(int), 1, fd);
            //Somente escrever os campos de tamanho variavel se eles forem diferente de 0:
            if(d.tamEsc!=0)
                fwrite(d.nomeEscola, d.tamEsc, 1, fd);
            fwrite(&d.tamMun, sizeof(int), 1, fd);
            if(d.tamMun!=0)
                fwrite(d.municipio,  d.tamMun, 1, fd); 
            fwrite(&d.tamEnd, sizeof(int), 1, fd);
            if(d.tamEnd!=0)
                fwrite(d.endereco, d.tamEnd, 1, fd);
            //Se o registro não ocupar o total de 112 bytes, escrever o restante dos bytes com 0.
            if(d.tamEsc+d.tamMun+d.tamEnd+36<112){//Verificar o tamnho.
                int a=112-(d.tamEsc+d.tamMun+d.tamEnd+36);//Ver o quanto falta.
                for(int i=0; i<a; i++){//Escrever o restante com 0
                    char ch='0';
                    fwrite(&ch, sizeof(char), 1, fd);
                }
            }
            fclose(fd);//Fechar o arquivo de dados.
        }

        //Abrir o arquivo de dados e colocar seu status como consistente.
        fd=fopen("arq.bin", "r+b");    
        status='1';
        fwrite(&status, 1, 1, fd);
        fclose(fd);//Fechar o arquivo de dados.

        fclose(fe);//Fechar o arquivo de entrada.
        printf("Arquivo carregado.\n");//Concluir imprimindo que o arquivo foi carregado.
    }
    else//Falha.
        printf("Falha no carregamento do arquivo.\n");
}

//Função [2]:
void recuperar(){ 
    FILE *fd = fopen("arq.bin", "rb");//Arquivo aberto apenas em modo de leitura.
    if(fd!=NULL){
        char cons;
        fread(&cons, sizeof(char), 1, fd); //Checa se o arquivo esta consistente.
        if(cons=='1'){//Se o arquivo estiver consistente, lê os dados.
            fseek(fd, 5, SEEK_SET);//Pula o cabeçalho para a leitura dos registros.
            int valido;//Variavel que recebe o estado lógico do registro.
            fread(&valido, sizeof(int), 1, fd); 
            int count=0;//Auxilia a "pular" para o proximo registro.
            int flag=0;//Indica se algum registro foi lido.
            while(feof(fd) == 0){//Enquanto o arquivo não acabar, continua lendo.
                if(valido==0){
                    int cod=0;//Recebe o codEscola e imprime na tela.
                    fread(&cod, sizeof(int), 1, fd);
                    printf("%d ", cod);

                    char aux[100];//Variavel que recebe temporariamente as strings para a impressão.
                    fread(aux, sizeof(char), 10, fd);//Lê a dataInicio.
                    aux[10]='\0';//Coloca fim de string nos bytes lidos para usar o "%s" na impressão.
                    printf("%s ", aux);
                
                    fread(aux, sizeof(char), 10, fd);//Lê a dataFinal.
                    aux[10]='\0';
                    printf("%s ", aux);

                    for (int i = 0; i < 3; ++i){
                        int a=0;//Variavel que recebe o tamanho dos campos variaveis.
                        fread(&a, sizeof(int), 1, fd);//Lê o tamanho dos campos.
                        if(a!=0){            //Se eles não forem nulos,
                            printf("%d ", a);//imprime os valores e as strings.
                            fread(aux, sizeof(char), a, fd);
                            aux[a]='\0';
                            printf("%s ", aux);
                        }
                        else
                            printf("%d ", a);  //Senão imprime zero.
                    }
                    printf("\n");
                    flag=1;//Indica que foi lido ao menos um registro.
                }
                count++;//Count somado para acessar o proximo registro atraves de seu RRN.
                fseek(fd, 5+(count*116), SEEK_SET);//fseek para o RRN do proximo elemento.
                fread(&valido, sizeof(int), 1, fd);//Lê o estado lógico do proximo registo
            }
            if(flag==0)//Caso não seja encontrado nenhum registro, imprime erro.
                printf("Registro inexistente.\n");
            fclose(fd);
        }
        else
            printf("Falha no processamento do arquivo.\n");//Caso o aquivo esteja incosistente ou não foi aberto, indica erro.
    }
    else
        printf("Falha no processamento do arquivo.\n");
}

//Função [3]:
void recuperarCriterio(char *criterio, char* valor){
    FILE *fd = fopen("arq.bin", "rb");//Arquivo aberto apenas em modo de leitura.
    if(fd!=NULL){
        char cons;
        fread(&cons, sizeof(char), 1, fd);//Checa se o arquivo esta consistente.
        if(cons=='1'){
            int erro=0;//Variavel que indica erro caso não seja encontrado nenhum registro.
            fseek(fd, 5, SEEK_SET); //Pula o cabeçalho para a leitura do registro.
            int valido;
            fread(&valido, sizeof(int), 1, fd); 
            int count=0;//Variavel que auxilia a checar os proximos registros.
            while(feof(fd) == 0){
            //Enquanto não for fim de arquivo e o registro não foi removido, compara o criterio de busca com os registros.
            //Se for for igual, imprime o registro atual. 
                if(valido==0){
                    int flagigual=0;//Variavel que indica que o registro deve ser impresso.
                    if(strcmp(criterio, "codEscola")==0){//se o criterio for o codEscola,
                        int cod=0;
                        fread(&cod, sizeof(int), 1, fd);//lê os 4 primeiros bytes do registro.
                        if(cod==atoi(valor))//Se o valor for igual ao buscado, permite a impressão.
                            flagigual=1;
                    }
                    else if(strcmp(criterio, "dataInicio")==0){//Se o criterio for dataInicio,
                        fseek(fd, 4, SEEK_CUR);//vai para o segundo campo,lê, e compara com o que é procurado.
                        char aux[11];
                        fread(aux, sizeof(char), 10, fd);
                        aux[10]='\0';//É colocado "\0" no fim para usar a função strcmp.
                        if(strcmp(aux, valor)==0)
                            flagigual=1;//se verdadeiro, imprime registro 
                    }
                    else if(strcmp(criterio, "dataFinal")==0){//Similar a dataInicio, porém pulando para o terciro campo.
                        fseek(fd, 14, SEEK_CUR);
                        char aux[11];
                        fread(aux, sizeof(char), 10, fd);
                        aux[10]='\0';
                        if(strcmp(aux, valor)==0)
                            flagigual=1;
                    }
                    else if(strcmp(criterio, "nomeEscola")==0){//Se o criterio for nomeEscola,
                        fseek(fd, 24, SEEK_CUR);//pula os campos de tamanho fixo,
                        int a=0;
                        fread(&a, sizeof(int), 1, fd);//lê o tamanho do campo,
                        char aux[a+1];
                        fread(aux, sizeof(char), a, fd);//lê o campo,
                        aux[a]='\0';
                        if(strcmp(aux, valor)==0)//e compara com o valor desejado.
                            flagigual=1;
                    }
                    else if(strcmp(criterio, "municipio")==0){//Se o criterio for municipio,
                        fseek(fd, 24, SEEK_CUR);//pula os campos de tamnho fixo,
                        int a=0;
                        fread(&a, sizeof(int), 1, fd);//lê o tamanho do quarto campo,
                        fseek(fd, a, SEEK_CUR);//pula o campo,
                        fread(&a, sizeof(int), 1, fd);//lê o tamanho do municipio,
                        char aux[a+1];
                        fread(aux, sizeof(char), a, fd);//lê o municipio,
                        aux[a]='\0';
                        if(strcmp(aux, valor)==0)//e compara com o valor buscado.
                            flagigual=1;
                    }
                    else if(strcmp(criterio, "endereco")==0){//Se o criterio for endereco,
                        fseek(fd, 24, SEEK_CUR);//pula os campos de tamanho fixo,
                        int a=0;
                        fread(&a, sizeof(int), 1, fd);//lê o tamanho e pula o campo duas vezes, para chegar ao sexto campo,
                        fseek(fd, a, SEEK_CUR);
                        fread(&a, sizeof(int), 1, fd);
                        fseek(fd, a, SEEK_CUR);
                        fread(&a, sizeof(int), 1, fd); 
                        char aux[a+1];
                        fread(aux, sizeof(char), a, fd);
                        aux[a]='\0';
                        if(strcmp(aux, valor)==0)//e compara com o valor buscado
                            flagigual=1;
                    }
                    if(flagigual==1){//Se o registro for igual ao buscado, imprime o registro.
                        erro=1;//Indica que foi lido pelo menos um registro.
                        fseek(fd, 9+(count*116), SEEK_SET);//Volta ao inicio do registro.
                        int cod=0;//Lê codigo da escola.
                        fread(&cod, sizeof(int), 1, fd);
                        printf("%d ", cod);//Imprime o valor lido.

                        char aux[100];//Variavel que armazena as strings para a impressão.
                        fread(aux, sizeof(char), 10, fd);//Lê a dataInicio
                        aux[10]='\0';//Coloca "\0" para o uso do "%s" na função print.
                        printf("%s ", aux);
                    
                        fread(aux, sizeof(char), 10, fd);//Lê dataFinal
                        aux[10]='\0';
                        printf("%s ", aux);

                        for (int i = 0; i < 3; ++i){//Lê os campos de tamanho variavel.
                            int a=0;
                            fread(&a, sizeof(int), 1, fd);//Lê o tamanho do campo.
                            if(a!=0){//Se não for nulo,
                                printf("%d ", a);//imprime o valor e a string.
                                fread(aux, sizeof(char), a, fd);
                                aux[a]='\0';
                                printf("%s ", aux);
                            }
                            else//Senão imprime nulo(0).
                                printf("%d ", a);     
                        }
                        printf("\n");
                    }
                        
                }
                count++;//Soma o count para a acessar o proximo RRN.
                fseek(fd, 5+(count*116), SEEK_SET);//Vai para o proximo registro atraves do RRN.
                fread(&valido, sizeof(int), 1, fd);//Lê se o estado logico do proximo registro.
            }
            fclose(fd);
            if(erro==0)//Se nenhum registro for lido, impreme o erro.
                printf("Registro inexistente.\n");
        }
        else//Caso o arquivo esteja instável ou não foi aberto, imprime erro.
            printf("Falha no processamento do arquivo.\n");
    }
    else
        printf("Falha no processamento do arquivo.\n");
}


//Função [4]:
void recuperarRRN(int RRN){//Recupera o registro a partir do RRN.
    FILE *fd = fopen("arq.bin", "rb");//Abrir o aqruivo de dados.
    if(fd!=NULL){//Checa se ele foi aberto.
        char cons;//Char para ler o status do arquivo.
        fread(&cons, sizeof(char), 1, fd);//Le se o status do cabeçalho do arquivo.
        if(cons=='1'){//Se o arquivo estiver consistente. 
            fseek(fd, 0, SEEK_END);//Coloca o ponteiro no final do arquivo.
            long int tam = ftell(fd);//Usa a funcão ftell para verificar quantos bytes o arquivo tem.
            tam = (tam-5)/116;//Calcula quantos registros tem no arquivo.
            if(RRN >= tam || RRN < 0){//Verifica se o registro não esta no arquivo.
                printf("Registro inexistente.\n");//Se nao estiver, imprimir que não existe e fechar o arquivo.
                fclose(fd);
                return;
            }
            fseek(fd, 5+(RRN*116), SEEK_SET);//Vai pro locar do RRN.
            int valido;//Int para armazenar se ele existe ou não.
            fread(&valido, sizeof(int), 1, fd);//Le o int antes do registro pra ver se ele foi removido ou não 
            if(valido==0){//Se o registro existir.
                int a=0;//Variavel para armazenar o tamanho dos campos variaveis.
                int cod=0;//Variavel para ler o codigo da escola.
                //Leitura do codigo da escola.
                fread(&cod, sizeof(int), 1, fd);
                printf("%d ", cod);

                //Leitura das datas.
                char aux[100];//Variavel para armazenar os campos.
                fread(aux, sizeof(char), 10, fd);
                aux[10]='\0';
                printf("%s ", aux);
                fread(aux, sizeof(char), 10, fd);
                aux[10]='\0';
                printf("%s ", aux);

                //Leitura dos campos variaveis.
                for (int i = 0; i < 3; ++i){
                    fread(&a, sizeof(int), 1, fd);//Lê o tamanho.
                    if(a!=0){//Lê o campo variavel.
                        printf("%d ", a);
                        fread(aux, sizeof(char), a, fd);
                        aux[a]='\0';
                        printf("%s ", aux);
                    }
                    else
                        printf("%d ", a);     
                }
                printf("\n");
            }
            else//Se o registro foi removido
                printf("Registro inexistente.\n");
        }
        else//Falha no arquivo.
            printf("Falha no processamento do arquivo.\n");
    }
    else//Falha no arquivo.
        printf("Falha no processamento do arquivo.\n");
    fclose(fd);//Fechar o arquivo de dados.
}

//Funcão [5]:
void remocaoLogica(int RRN){
    FILE *fd=fopen("arq.bin", "r+b"); //Arquivo aberto no modo que permite a escrita e leitura de arquivos binários.
    if(fd!=NULL){ //Checa-se se o ponteiro é nulo. Caso não seja, é possível prosseguir com a função.
        char cons; //Variável criada para saber se o arquivo é consistente
        fread(&cons, sizeof(char), 1, fd); //A variável 'cons' recebe o primeiro byte do arquivo, para ver se ele é consistente.
        if(cons=='1'){            
            fseek(fd, 0, SEEK_SET); //Modifica-se o status do arquivo para instavel.
            char status='0'; //Variável recebe '0'.
            fwrite(&status, 1, 1, fd); //O conteúdo de status é colocado no primeiro byte do arquivo, mudando seu status 
            //para inconsistente, enquanto é manipulado.

            //É preciso verificar se o RRN buscado está entre o intervalo de RRNs do arquivo em questão.
            fseek(fd, 0, SEEK_END); //Leva-se o ponteiro ao final do arquivo para poder executar a proxima linha.
            long int tam = ftell(fd); //A variável recebe o numero de bytes do arquivo, para que seja possivel fazer a verificação.
            tam = (tam-5)/116;
            if(RRN >= tam || RRN < 0){
                printf("Registro inexistente.\n");
                fseek(fd, 0, SEEK_SET); //Modifica-se o status do arquivo para estável.
                status='1';
                fwrite(&status, 1, 1, fd);
                fclose(fd);
                return;
            }
     
            fseek(fd, 5+(RRN*116), SEEK_SET); //Encontra o registro que será removido.
     
            int valido; 
            fread(&valido, sizeof(int), 1, fd);
            if(valido==-1){ //Verifica-se se o registro que será removido é valido.
                printf("Registro inexistente.\n");
                //Caso não seja valido, modifica-se o status do arquivo para consistente e encerra-se a função, pois o registro não existe.
                fseek(fd, 0, SEEK_SET); 
                status='1';
                fwrite(&status, 1, 1, fd);
                fclose(fd);
                return;
            }       
            else{ //Caso o registro exista, executa-se a remoção. 
                fseek(fd, -4, SEEK_CUR); //Volta-se 4 bytes com o ponteiro, para poder modificar o campo que marca se o registro é valido.
                int aux=-1; //A variável aux será utilizada para poder escrever no arquivo que o registro foi removido.
                fwrite(&aux, sizeof(int), 1, fd); //Marca-se o registro como removido. 
                
                fseek(fd, 1, SEEK_SET); //O ponteiro é colocado no local do cabeçalho que indica o topo da pilha,
                int topoPilha; //para que seja possível alterar o topo da pilha. 
                fread(&topoPilha, sizeof(int), 1, fd); //Obtem-se o topo da pilha.
                
                fseek(fd, 9+(RRN*116), SEEK_SET); //Encontra o registro que será removido.
                fwrite(&topoPilha, sizeof(int), 1, fd); //Escreve-se o topo da pilha no registro logicamente removido,
                //logo após o campo que sinaliza que o registro é valido.
                
                fseek(fd, 1, SEEK_SET); //Agora é preciso atualizar o valor a pilha para o RRN que acabou de ser removido.
                fwrite(&RRN, sizeof(int), 1, fd); 
            }          

            fseek(fd, 0, SEEK_SET); //Modifica-se o status do arquivo para estável.
            status='1'; //Status é setado para 1, 
            fwrite(&status, 1, 1, fd); //e escrito no arquivo, para deixá-lo consistente novamente.
            fclose(fd);
            printf("Registro removido com sucesso.\n"); //Imprime-se a mensagem para confirmar que a operação foi realizada com sucesso.
        }
        else //Imprime-se a mensagem de erro, caso o arquivo esteja incosistente.
            printf("Falha no processamento do arquivo.\n");
    }
    else //Imprime-se mensagem de erro, caso o ponteiro do arquivo seja NULL.
        printf("Falha no processamento do arquivo.\n");   
}

//Função [6]:
void adicionaReg(Dados d){
    FILE *fd = fopen("arq.bin", "r+b");//Abre o arquivo para a escrita e leitura.
    if(fd!=NULL){
        char cons;
        fread(&cons, sizeof(char), 1, fd); //Checa se o arquivo esta consistente
        if(cons=='1'){ 

            fseek(fd, 0, SEEK_SET); //Modifica o status do arquivo para instavel.
            char status='0';
            fwrite(&status, 1, 1, fd);

            int topo;//Lê o topo da pilha para determinar onde será adicionado o novo registro.
            fread(&topo, sizeof(int), 1, fd);

            if(topo!=-1){//Se pilha não estiver vazia.
                fseek(fd, 9+(topo*116), SEEK_SET);//Vai para o local indicado no topo da pilha.
                fread(&topo, sizeof(int), 1, fd);//Armazena o valor da proximo elemento da pilha. 
                fseek(fd, -8, SEEK_CUR);//Volta ao inicio do registro para sobrescrever o conteudo.
                int flag=0;
                fwrite(&flag, sizeof(int), 1, fd);//Marca o registro como valido.
                fwrite(&d.codEscola, sizeof(int), 1, fd);//Sscreve o codEscola.
                fwrite(&d.dataInicio, sizeof(char)*10, 1, fd);//Escreve as datas.
                fwrite(&d.dataFinal, sizeof(char)*10, 1, fd);
                fwrite(&d.tamEsc, sizeof(int), 1, fd);//Escreve os tamanhos dos campos.
                if(d.tamEsc!=0)//Se não forem nulos, imprime os campos.
                    fwrite(d.nomeEscola, d.tamEsc, 1, fd);
                fwrite(&d.tamMun, sizeof(int), 1, fd);
                if(d.tamMun!=0)
                    fwrite(d.municipio,  d.tamMun, 1, fd); 
                fwrite(&d.tamEnd, sizeof(int), 1, fd);
                if(d.tamEnd!=0)
                    fwrite(d.endereco, d.tamEnd, 1, fd);
                if(d.tamEsc+d.tamMun+d.tamEnd+36<112){//Calcula o tamanho gasto pelo registro.
                    //Se for menor que 112, completa com "0" até atingir 112.
                    int a=112-(d.tamEsc+d.tamMun+d.tamEnd+36);
                    for(int i=0; i<a; i++){
                        char ch='0';
                        fwrite(&ch, sizeof(char), 1, fd);
                    }
                }
                fseek(fd, 1, SEEK_SET);//Volta ao cabeçalho e atualiza o topo da pilha.
                fwrite(&topo, sizeof(int), 1, fd);
            }
            else{//Se a pilha estiver vazia, adiciona o registro no final do arquivo.
                fseek(fd, 0, SEEK_END);
                int flag=0;//Flag que indica que o estado do registro.
                fwrite(&flag, sizeof(int), 1, fd);//Escreve os dados no arquivo.
                fwrite(&d.codEscola, sizeof(int), 1, fd);
                fwrite(&d.dataInicio, sizeof(char)*10, 1, fd);
                fwrite(&d.dataFinal, sizeof(char)*10, 1, fd);
                fwrite(&d.tamEsc, sizeof(int), 1, fd);
                if(d.tamEsc!=0)//Se o campo não for nulo, imprime.
                    fwrite(d.nomeEscola, d.tamEsc, 1, fd);
                fwrite(&d.tamMun, sizeof(int), 1, fd);
                if(d.tamMun!=0)
                    fwrite(d.municipio,  d.tamMun, 1, fd); 
                fwrite(&d.tamEnd, sizeof(int), 1, fd);
                if(d.tamEnd!=0)
                    fwrite(d.endereco, d.tamEnd, 1, fd);
                if(d.tamEsc+d.tamMun+d.tamEnd+36<112){//Completa o registro com "zero" para completar 112 bytes.
                    int a=112-(d.tamEsc+d.tamMun+d.tamEnd+36);
                    for(int i=0; i<a; i++){
                        char ch='0';
                        fwrite(&ch, sizeof(char), 1, fd);
                    }
                }
            } 

            fseek(fd, 0, SEEK_SET); //Modifica o status do arquivo para estavel.
            status='1';
            fwrite(&status, 1, 1, fd);                     
            fclose(fd);
            printf("Registro inserido com sucesso.\n");
        }
        else//Se o arquivo for instavel ou não foi aberto corretamente, imprime erro.
            printf("Falha no processamento do arquivo.\n");
    }
    else
        printf("Falha no processamento do arquivo.\n");
}  

//Função [7]:
void atualizacaoRRN(int RRN, Dados d){
    FILE *fd=fopen("arq.bin", "r+b"); //Arquivo aberto, no modo de escrita e leitura.
    if(fd!=NULL){ //Checa-se se o ponteiro é nulo. Caso não seja, é possível prosseguir a execução da função.  
        char cons;
        fread(&cons, sizeof(char), 1, fd); //A partir do primeiro byte do arquivo,
        if(cons=='1'){ //confere-se se o arquivo é consistente.
            fseek(fd, 0, SEEK_SET); //Modifica-se o status do arquivo para instavel.
            char status='0'; //Variável status recebe '0'.
            fwrite(&status, 1, 1, fd); //
            
            //Verifica-se se o registro está entre os RRN que existem no arquivo.
            fseek(fd, 0, SEEK_END); //Leva-se o ponteiro ao final do arquivo para poder executar a proxima linha.
            long int tam = ftell(fd); //A variável recebe o numero de bytes do arquivo, para que seja possivel fazer a verificação.
            tam = (tam-5)/116;
            if(RRN >= tam || RRN < 0){
                printf("Registro inexistente.\n");
                fseek(fd, 0, SEEK_SET); //Modifica-se o status do arquivo para estável.
                status='1';
                fwrite(&status, 1, 1, fd);
                fclose(fd);
                return; 
            }

            fseek(fd, 5+(116*RRN), SEEK_SET); //Encontra o registro, a partir de seu RRN.

            int valido;
            fread(&valido, sizeof(int), 1, fd); //Checa-se se o registro existe.
            if(valido==-1){
                printf("Registro inexistente.\n");
                fseek(fd, 0, SEEK_SET); //Modifica-se o status do arquivo para estável.
                status='1';
                fwrite(&status, 1, 1, fd);
                fclose(fd);
                return;        
            }
            else{ //Caso o registro exista, a atualização será executada.
                fseek(fd, 9+(116*RRN), SEEK_SET); //Encontra o registro, a partir de seu RRN.
        
                //As novas informações são passadas para o arquivo.
                //Os campos fixos são escritos no arquivo sem fazer nenhuma verificação, pois já foi tratado o caso de o registro ser nulo na main.
                fwrite(&d.codEscola, sizeof(int), 1, fd);
                fwrite(&d.dataInicio, sizeof(char), 10, fd);  
                fwrite(&d.dataFinal, sizeof(char), 10, fd);
                
                if(d.tamEsc==0) //Verifica-se se o campo é nulo. 
                    fwrite(&d.tamEsc, sizeof(int), 1, fd); //Caso seja, escreve-se '0' no arquivo.
                else{ 
                    fwrite(&d.tamEsc, sizeof(int), 1, fd); //Caso não seja, é escrito o tamanho do registro, 
                    fwrite(d.nomeEscola, sizeof(char), d.tamEsc, fd); //e logo depois a informação do nome da escola.
                }

                //É feita a verificação para todos os campos de tamanho variável.
                if(d.tamMun==0) 
                    fwrite(&d.tamMun, sizeof(int), 1, fd); //Aqui checa-se o minicpio.
                else{
                    fwrite(&d.tamMun, sizeof(int), 1, fd);
                    fwrite(d.municipio, sizeof(char), d.tamMun, fd);
                }

                if(d.tamEnd==0) 
                    fwrite(&d.tamEnd, sizeof(int), 1, fd); //Aqui checa-se endereço.
                else{
                    fwrite(&d.tamEnd, sizeof(int), 1, fd);
                    fwrite(d.endereco, sizeof(char), d.tamEnd, fd);
                }
                
                if(d.tamEsc+d.tamMun+d.tamEnd+36<112){ //É verificado se o tamanho do registro é menor que 112 bytes, que é o tamanho fixo do registro.
                    int a=112-(d.tamEsc+d.tamMun+d.tamEnd+36); 
                    for(int i=0; i<a; i++){ //Caso seja, se preenche com 0 os espaços restantes.
                        char ch='0';    
                        fwrite(&ch, sizeof(char), 1, fd);
                    }
                }
            }
            
            fseek(fd, 0, SEEK_SET); //Modifica-se o status do arquivo para estável.
            status='1';
            fwrite(&status, 1, 1, fd);

            fclose(fd);
            printf("Registro alterado com sucesso.\n");
        }
        else //Se o arquivo for inconsistente, imprime-se a mensagem de erro.
            printf("Falha no processamento do arquivo.\n");
    }
    else //Se o ponteiro for nulo, imprime-se a mensagem de erro.
        printf("Falha no processamento do arquivo.\n");
}

//Função [8]:
void desfragmenta(){
    FILE *fd = fopen("arq.bin", "r+b"); //Arquivo aberto no modo que a escrita e a leitura são permitidas par aum arquivo binário.
    if(fd!=NULL){ //Checa-se se o ponteiro para o arquivo é nulo, caso não seja, prossegue-se para a função.
        char cons; //A variável é criada para verificar se o arquivo é consistente.
        fread(&cons, sizeof(char), 1, fd); //'cons' recebe o primeiro valor do cabeçalho que diz se o arquivo é ou não consistente.
        if(cons=='1'){ //Caso seja, continua-se a função.
            FILE *fn = fopen("arq2.bin", "wb"); //O 'arq2.bin' é criado e aberto em modo de escrita para arquivos binários.
            
            char status='0'; //As variáveis status e topoPilha serão colocadas no cabeçalho do arquivo novo. 
            int topoPilha=-1;

            fwrite(&status, sizeof(char), 1, fn); //É setado o cabeçalho em 'arq2.bin' com o auxilio das variáveis status e topoPilha.
            fwrite(&topoPilha, sizeof(int), 1, fn); 
            fclose(fn);

            int valido; //Esta variável servirá para verificar se o registro analisado existe ou não.
            fseek(fd, 5, SEEK_SET);             
            fread(&valido, sizeof(int), 1, fd); //'valido' recebe o valor 0 ou -1, do campo do registro que diz se este é ou não válido. 
            int count=0; //A variável count serivirá para auxiliar a manipular dos registros, pois representará o RRN dentro do while.
            //Enquanto não chegar no final do arquivo, realiza-se a operação de passar os registros válidos do arquivo 'arq.bin' para o 'arq2.bin'.
            while(feof(fd)==0){ 
                if(valido==0){ //Caso o registro exista, executa-se a operação de copia para o arquivo 'arq2.bin'.
                    Dados d; //A struct d auxiliará na passagem dos dados de um arquivo para outro.
                    iniciaDado(&d); //É iniciada a struct.
                    
                    //Lê-se os campos do registro do arquivo 'arq.bin' e são colocados em d, para, depois serem passados para 'arq2.txt'.
                    fread(&d.codEscola, sizeof(int), 1, fd); 
                    fread(d.dataInicio, sizeof(char), 10, fd);
                    fread(d.dataFinal, sizeof(char), 10, fd); 

                    fread(&d.tamEsc, sizeof(int), 1, fd); 
                    d.nomeEscola=(char*)malloc(sizeof(char)*d.tamEsc);
                    fread(d.nomeEscola, sizeof(char), d.tamEsc, fd);

                    fread(&d.tamMun, sizeof(int), 1, fd);
                    d.municipio=(char*)malloc(sizeof(char)*d.tamMun);
                    fread(d.municipio, sizeof(char), d.tamMun, fd);

                    fread(&d.tamEnd, sizeof(int), 1, fd);
                    d.endereco=(char*)malloc(sizeof(char)*d.tamEnd);
                    fread(d.endereco, sizeof(char), d.tamEnd, fd);

                    fn=fopen("arq2.bin", "ab"); //Abre-se o arquivo no modo a, para que sempre seja aberto com o ponteiro fn no fim do arquivo.
                    fwrite(&valido, sizeof(int), 1, fn); //Escreve-se o conteúdo da struct d no arquivo 'arq2.bin'.
                    fwrite(&d.codEscola, sizeof(int), 1, fn); //Primeiro os campos fixos.7
                    fwrite(d.dataInicio, sizeof(char)*10, 1, fn);
                    fwrite(d.dataFinal, sizeof(char)*10, 1, fn);
                    
                    fwrite(&d.tamEsc, sizeof(int), 1, fn); //Trata-se a informação, no caso de os campos de tamanho variável serem nulos. 
                    if(d.tamEsc!=0) //Caso não seja nulo, passa-se o nome da escola para 'arq2.bin'.
                        fwrite(d.nomeEscola, d.tamEsc, 1, fn);
                    fwrite(&d.tamMun, sizeof(int), 1, fn);
                    if(d.tamMun!=0) //Caso não seja nulo, passa-se o municipio para 'arq2.bin'.
                        fwrite(d.municipio,  d.tamMun, 1, fn); 
                    fwrite(&d.tamEnd, sizeof(int), 1, fn);
                    if(d.tamEnd!=0) //Caso não seja nulo, passa-se o endereço para 'arq2.bin'.
                        fwrite(d.endereco, d.tamEnd, 1, fn);
                    if(d.tamEsc+d.tamMun+d.tamEnd+36<112){ //Compara-se o tamanho do registro com 112 bytes, que é o tamanho máximo que o registro pode ter.
                        int a=112-(d.tamEsc+d.tamMun+d.tamEnd+36); //Caso seja menor, completa-se o registro com o caracter '0'.
                        for(int i=0; i<a; i++){
                            char ch='0';
                            fwrite(&ch, sizeof(char), 1, fn);
                        }
                    }
                    fclose(fn);                    
                }
                count++; //Adiciona-se uma unidade a count, para ir para o próximo RRN.
                fseek(fd, 5+(count*116), SEEK_SET); //O ponteiro fd é setado para o início do próximo RRN que será analisado, do arquivo 'arq.bin'.
                fread(&valido, sizeof(int), 1, fd); //'valido' recebe o primeiro byte do registro, e será verificado a seguir se o reguistro é valido ou não.
            }

            fn=fopen("arq2.bin", "r+b"); //Abre-se o arquivo para modificar o cabeçalho.
            status='1'; //A variável status recebe '1', e auxilirá em passar o valor 1 para o primeiro campo do cabeçalho.
            fwrite(&status, 1, 1, fn); //Agora o arquivo 'arq2.bin' passa a ser consistente.
            fclose(fn);

            fclose(fd); 

            remove("arq.bin"); //o arquivo 'arq.bin' é deletado.
            rename("arq2.bin", "arq.bin"); //O arquivo 'arq2.bin' é renomeado para 'arq.bin'. 
            
            printf("Arquivo de dados compactado com sucesso.\n");
        }
        else //Caso o arquivo nao seja consistente, imprime-se a mensagem de erro.
            printf("Falha no processamento do arquivo.\n");
    }
    else //Caso o ponteiro seja nulo, imprime-se a mensagem de erro.
        printf("Falha no processamento do arquivo.\n");
}
	
//Função [9]:
void recuperarRemovidos(){
    FILE *fd=fopen("arq.bin", "rb");//Abrir o arquivo de dados no modo leitura binaria.
    if(fd!=NULL){//Verificar se a abertura foi sucedida.
        char cons;//Variavel para checar se o arquivo esta estavel.
        fread(&cons, sizeof(char), 1, fd);//Ler o status no cabeçalho do arquivo.
        if(cons == '1'){//Se o arquivo estiver consistente.
            int pilha;//Inteiro para receber o topo da pilha.
            fseek(fd, 1, SEEK_SET);//Ir para o locar do cabeçalho que armazena o topo da pilha.
            fread(&pilha, sizeof(int), 1, fd);//Ler o topo da pilha no cabeçalho e armazenar no int.
            if(pilha == -1){//Se não tiver registros removidos.
                printf("Pilha vazia.\n");//Imprimir que a pilha está vazia.
                fclose(fd);//Fechar o arquivo.
                return;
            }
            printf("%d ", pilha);//Imprimir o topo da pilha.
			
            while(pilha != -1){//Percorrer a pilha.
                fseek(fd, 5+(pilha*116)+4, SEEK_SET);//Ir no RRN da pilha.
                fread(&pilha, sizeof(int), 1, fd);//Ler o RRN que esta armazenado.
                if(pilha != -1)//Se ele não for o final da pilha.
                    printf("%d ", pilha);//Imprimir a pilha.
            }
            printf("\n");
            fclose(fd);//Fechar o arquivo.
        }else//Se o arquivo não estiver consistente.
            printf("Falha no processamento do arquivo.\n");
    }else//Se o arquivo não abrir.
        printf("Falha no processamento do arquivo.\n");
}


int main(int argc, const char * argv[]) {
    int op=atoi(argv[1]); //É definida a operação que será executada.
    char *aux;  //Variáveis que serão utilizadas para manipular os dados recebidos do terminal, para que seja possível passá-los para as funções.
    char *aux2;
    int RRN;
    Dados d;
    switch (op){
        case 1:   //Leitura do arquivo de entrada e escrita no arquivo de dados.
            aux=(char *)argv[2];
            insercao(aux);   
            break;
        case 2:   //Recuperação de todos dados do arquivo de dados.
            recuperar();
            break;
        case 3:   //Recuperação dados a partir de um critério.
            aux=(char *)argv[2];
            aux2=(char *)argv[3];
            recuperarCriterio(aux, aux2);
            break;
        case 4:   //Recuperação dados a partir do RNN.
            RRN = atoi(argv[2]);//Transformar o argv[2] em int, o RRN.
            recuperarRRN(RRN);
            break;
        case 5:   //Remoção lógica de registros.
            RRN = atoi(argv[2]);
            remocaoLogica(RRN);
            break;
        case 6:   //Adição de registros adicionais, reaproveitando espaços de registros logicamante removidos.
            //Para que seja possível passar o registro que será adicionado para a função, tratamos os argumentos passados através terminal
            //e colocamos as informações dentro da estrutura Dados d.
            iniciaDado(&d);
            d.codEscola=atoi(argv[2]); //O segundo argumento do terminal será o código da escola.

            aux=(char*)argv[3]; //O terceiro é a data inicial.
            if(strcmp(aux, "0")==0) //Caso o campo seja nulo, a string passa a ser "0000000000". 
                strcpy(d.dataInicio, "0000000000");                 
            else
                strcpy(d.dataInicio, aux);

            aux=(char*)argv[4]; //O quarto é a data final.
            if(strcmp(aux, "0")==0) //Caso o campo seja nulo, a string passa a ser "0000000000".
                strcpy(d.dataFinal, "0000000000");                 
            else
                strcpy(d.dataFinal, aux);

            aux=(char*)argv[5]; //O quinto é o nome da escola.
            d.nomeEscola=(char *)malloc(sizeof(char)*strlen(aux));
            strcpy(d.nomeEscola, aux);

            aux=(char*)argv[6]; //O sexto é o município.
            d.municipio=(char *)malloc(sizeof(char)*strlen(aux));
            strcpy(d.municipio, aux);

            aux=(char*)argv[7]; //O sétimo, finalmente, é o endereço.
            d.endereco=(char *)malloc(sizeof(char)*strlen(aux));
            strcpy(d.endereco, aux);
            
            //Os tamanhos dos campos de tamanho variável são atribuídos aos campos da struct reservados para conter tal informação.
            d.tamEsc=strlen(d.nomeEscola); 
            d.tamMun=strlen(d.municipio);
            d.tamEnd=strlen(d.endereco);

            adicionaReg(d); //A operação é executada.
            
            free(d.nomeEscola);
            free(d.municipio);
            free(d.endereco);

            break;
        case 7:   //Atualização de todos os campos de um registro, a partir de seu RRN. 
            //Trata-se, novamente, as informações recebidas do terminal, para que seja possível passá-las para a função.
            iniciaDado(&d);
            RRN=atoi(argv[2]); //Nesta operação, o argumento 2 é o RRN.
            d.codEscola=atoi(argv[3]); //O terceiro é o código da escola.

            aux=(char*)argv[4]; //O quarto a data de início.
            if(strcmp(aux, "0")==0) //Ocorre o tratamento do campo caso este seja nulo.
                strcpy(d.dataInicio, "0000000000");                 
            else
                strcpy(d.dataInicio, aux);

            aux=(char*)argv[5]; //O quinto é a data final.
            if(strcmp(aux, "0")==0) //Ocorre o tratamento do campo caso este seja nulo.
                strcpy(d.dataFinal, "0000000000");                 
            else
                strcpy(d.dataFinal, aux);

            aux=(char*)argv[6]; //O sexto é o nome da escola. 
            d.nomeEscola=(char *)malloc(sizeof(char)*strlen(aux));
            strcpy(d.nomeEscola, aux);

            aux=(char*)argv[7]; //O sétimo é o município.
            d.municipio=(char *)malloc(sizeof(char)*strlen(aux));
            strcpy(d.municipio, aux);

            aux=(char*)argv[8]; //E o oitavo o endereço.
            d.endereco=(char *)malloc(sizeof(char)*strlen(aux));
            strcpy(d.endereco, aux);
            
            //As variáveis da struct Dados reservadas para os tamanhos dos campos variáveis são setadas.
            d.tamEsc=strlen(d.nomeEscola);
            d.tamMun=strlen(d.municipio);
            d.tamEnd=strlen(d.endereco);

            atualizacaoRRN(RRN, d); //Chama-se a função que realizará a operação.

            free(d.nomeEscola);
            free(d.municipio);
            free(d.endereco);
            break;
        case 8:  //Desfragmentação do arquivo de dados.
            desfragmenta();
            break;
        case 9:  //Recuperação dos RRN dos registros logicamente removidos.
            recuperarRemovidos();
            break;
      	case 10: //Inserir junto no indice da Arvore-B.
            aux = (char*)argv[2];
            insercaoArvore(aux);
        	break;
      	case 11: //Parte 6 + arvore
        	break;
      	case 12: //Recuperar registro a partir da Arvore-B.
      	  	break;
      	case 13: //Remover o registro a partir da Arvore-B. 
        	break;
      	case 14: //Atualizar o registro a partir da Arvore-B.
       	 	break;
        
        default :   //Caso seja um valor diferente dos estabelecidos.
            printf("Valor Invalido\n");
            break;
    }
        
    return 0;
}
