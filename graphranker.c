#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct EL //elemento della coda
{
    unsigned int score; //priorita
    unsigned int index; //nome
} elem;
typedef elem * coda;
typedef struct SCORE //elemento dell'array dei grafi
{
    unsigned int myScore;
    char isPresent;    
} scoreEl;
typedef struct ElClassifica{
    struct ElClassifica * prox;
    unsigned int numeroGrafo;
    unsigned int punti;
} classs;
typedef classs * ListaClassifica;


//--------------------------------------- DICHIARAZIONE FUNZIONI --------------------------------------
//gestione matrice
unsigned int ** allocaMatrice(unsigned int);
char * readInput();
void loadMatrixLine(unsigned int *);
unsigned int getInt(char *);

//gestione coda
coda allocaCoda(unsigned int);
void insCoda(coda, elem);
void eliminaCoda(coda, unsigned int);
void aggiornaCoda(coda, elem);

//dijkstra
unsigned int dijk(unsigned int **, unsigned int);

//inserimento in classifica
ListaClassifica insClassifica(ListaClassifica, unsigned int,unsigned int);
//----------------------------------------- FINE DICHIARAZIONE ------------------------------------------

unsigned int nodeNum; //numero di nodi
unsigned int rankNum; //lunghezza della classifica

int main(){

    unsigned int ** matriceTmp; //matrice di adiacenza del grafo
    ListaClassifica classList=NULL; //inizializzo la classifica

    char * inputString; //stringa di input comandi
    unsigned int graphId=0; //id del grafo

    if(scanf("%u %u",&nodeNum,&rankNum)==1); //leggo la dimensione della matrice e il numero di grafi
    matriceTmp = allocaMatrice(nodeNum); //alloco la matrice
 
    while (1){ //WHILE TRUE-> gestione dell'input
        inputString=readInput(); //leggo il comando in ingresso

//----------------------------- AGGIUNGI GRAFO -----------------------------------------------
        if(strcmp(inputString,"AggiungiGrafo")==0){
            free(inputString); //libero la memoria occupata dalla stringa in input

            unsigned int i;
            for(i=0;i<nodeNum;i++){ //leggo una riga per volta da stdin e la carico
                loadMatrixLine(matriceTmp[i]);
            }

            elem e;
            e.index=graphId;
            e.score=dijk(matriceTmp,nodeNum); //eseguo dijkstra sulla matrice ottenuta
            
            classList=insClassifica(classList,e.index,e.score); //inserisco in classifica il risultato

            graphId++; //incremento il numero del grafo elaborato
   
        }

//------------------------------- END OF FILE --------------------------------------------------
        else if(*inputString==EOF){
            free(inputString);
            return 0;
        }

//----------------------------------- TOP K ----------------------------------------------------
        else if(strcmp(inputString,"TopK")==0){ 
            free(inputString);

            //STAMPA DELLA CLASSIFICA
            int i;
            ListaClassifica tmp=classList;
            for(i=0;tmp!=NULL&&tmp->prox!=NULL&&i<rankNum-1;i++){
                printf("%u ",tmp->numeroGrafo);
                tmp=tmp->prox;
            }
            if(tmp!=NULL) printf("%u\n",tmp->numeroGrafo); //stampo l'ultimo carattere senza lo spazio
            else printf("\n");
        }

//----------------------- LETTO QUALSIASI ALTRO CARATTERE ---------------------------------------
        else{
            free(inputString);
            return 0;
        }

    }
    
    return 0;
}

ListaClassifica insClassifica(ListaClassifica myLista, unsigned int nGrafo,unsigned int punt){
    ListaClassifica tmpLista=myLista; //puntatore alla lista in input
    ListaClassifica prec=NULL; //puntatore temporaneo elemento precedente
    unsigned int i=0;

    while(tmpLista!=NULL && i<=rankNum && tmpLista->punti <= punt) { //trovo la posizione di inserimento
        prec=tmpLista;
        tmpLista=tmpLista->prox;
        i=i+1;
    }
    
    if(i>rankNum) return myLista; //se sfora la classifica non inserisco nemmeno

    classs * tmpEl=(classs*)malloc(sizeof(classs)); //alloco l'elemento
    
    if(prec==NULL){ //inserisco in testa
        tmpEl->numeroGrafo=nGrafo;
        tmpEl->punti=punt;
        tmpEl->prox=tmpLista;
        return tmpEl;
    }

    else{ //inserisco in mezzo/in fondo
        tmpEl->numeroGrafo=nGrafo;
        tmpEl->punti=punt;
        tmpEl->prox=tmpLista;
        prec->prox=tmpEl;
        return myLista;
    }

    return NULL;
 

}

unsigned int ** allocaMatrice(unsigned int dim){
    unsigned int ** matrix;
    unsigned int i;
    
    matrix=(unsigned int **)malloc(dim*sizeof(unsigned int*));    //alloco un array di puntatori, uno per riga

    for(i=0;i<dim;i++){ //alloco le colonne e salvo i relativi puntatori
        matrix[i]=(unsigned int*)malloc(dim*sizeof(unsigned int));
    }

    return matrix;
}

coda allocaCoda(unsigned int len){
    coda array;
    array = (coda)calloc(len+1,sizeof(elem));
    array[0].score=len; //salvo la lunghezza allocata
    array[0].index=0; //salvo il numero di elementi in coda
    return array;
}

void insCoda(coda Q, elem e){
    unsigned int len = Q[0].score; //massima lunghezza della coda
    unsigned int codaNum = Q[0].index; //lunghezza attuale della coda

    unsigned int elemScore=e.score; //peso dell'elemento
    //unsigned int elemIndex=e.index; //numero dell'elemento

    if(codaNum==0){ //coda vuota
        Q[1]=e;
        Q[0].index=1;
    }

    else{ //coda gia riempita
        unsigned int i=1;
        while(i<=codaNum && Q[i].score<=elemScore) i++; //trovo dove devo inserire l'elemento

        if(i==codaNum+1 && codaNum<len){ //se l'elemento va in fondo e non sfora lo inserisco
            Q[i]=e;
            Q[0].index = ++codaNum;
        }

        else{ //l'elemento va in mezzo
            unsigned int j;
            elem tmp,ins=e;

            for(j=i;j<=codaNum+1 && j<=len;j++){ //shifto ogni elemento stando attento a non sforare
                
                tmp=Q[j]; //copio l'elem da spostare
                Q[j]=ins; //sovrascrivo
                ins=tmp; //indico l'elemento copiato come elem da inserire
            }

            if(codaNum+1<=len) Q[0].index = codaNum +1; //incremento la lunghezza della coda se non sforo
        }

    }

}

void eliminaCoda(coda Q, unsigned int Eindex){
    unsigned int codaLen = Q[0].score;
    unsigned int codaNum = Q[0].index;
    
    unsigned int i=1,j;

    while(Q[i].index != Eindex) i++; //trovo l'elemento da eliminare

    for(j=i;j<=codaNum && j<codaLen;j++){ //sposto a sinistra tutti gli altri
        Q[j]=Q[j+1];
    }

    --Q[0].index; //decremento il numero di elementi della coda
}

void aggiornaCoda(coda Q, elem e){
    eliminaCoda(Q,e.index); //cancello e reinserisco l'elemento
    insCoda(Q,e);
}

char * readInput(){
    char * result;
    char tmp;
    int len=0; //lunghezza letta

    result=(char *)malloc(14*sizeof(char)); //alloco una prima stringa
    tmp=getchar(); //leggo il primo carattere
    if(tmp=='\n') tmp = getchar();

    while(len<13 && tmp!='\n'){ //finche non ho finito
        len++;
        result[len-1]=tmp;
        tmp=getchar(); //leggo il prossimo carattere
    }

    result[len]='\0'; //termino la stringa
    return result;
}

void loadMatrixLine(unsigned int * array){
    char tmp;
    char buff[11]; //buffer di 10 caratteri
    unsigned int pos=0; //posizione nella riga della matrice
    unsigned int buffPos=0; //posizione nel buffer
    
    tmp=getchar(); //leggo il primo carattere
    if(tmp=='\n') tmp = getchar(); //ripulisco l'a capo se c'è

    while(1){
        while(tmp!=','&& tmp!='\n'){ //ripeto finche trovo una virgola o a capo
            buff[buffPos]=tmp; //CARICO IL NUMERO NEL BUFFER
            buffPos++;
            
            tmp=getchar(); //leggo il prossimo carattere
        }

        buff[buffPos]='\0'; //quando ho finito chiudo la stringa del buffer
        buffPos=0; //azzero la posizione

        array[pos]=getInt(buff); //converto la stringa in intero e la salvo nella riga della matrice
        pos++;

        if(tmp=='\n') break; //se ho finito la riga termino
        tmp=getchar(); //altrimenti continuo a leggere
    }
    return;
}

unsigned int getInt(char * stringa){
    unsigned int result=0;
    int i=0;
    while(stringa[i]!='\0'){
        result=result*10 + (stringa[i]-'0');
        i++;
    }
    return result;
}

unsigned int dijk(unsigned int ** grafo ,unsigned int nodeNumb){
    elem tmpEl;
    unsigned int j, somma=0;

    scoreEl * scoreArray = (scoreEl *)malloc(nodeNumb*sizeof(scoreEl)); //alloco l'array punteggi per velocizzare l'algoritmo
    coda nodeQueue = allocaCoda(nodeNumb); //alloco la coda
    
    tmpEl.index=0;
    tmpEl.score=0;
    insCoda(nodeQueue,tmpEl); //inserisco la distanza 0 per il nodo 0
    
    scoreArray[0].isPresent='1'; //il nodo zero esiste e ha peso 0
    scoreArray[0].myScore=0;

    for(j=1;j<nodeNumb;j++){
        tmpEl.index=j;
        tmpEl.score=UINT_MAX;
        insCoda(nodeQueue,tmpEl); //inserisco distanza infinita per gli altri nodi

        scoreArray[j].isPresent='1'; //do peso infinito agli altri nodi nell'array punteggi
        scoreArray[j].myScore=UINT_MAX;
    }

    while(nodeQueue[0].index != 0){ //finche non ho passato tutti i nodi continuo
        unsigned int newDist,oldDist;
        unsigned int i;
        unsigned int currNode = nodeQueue[1].index; //prendo il primo nodo della coda (valore minimo)
        unsigned int currDist = nodeQueue[1].score;
        
        if(currDist!=UINT_MAX) somma=somma+currDist; //se non è infinito lo sommo, infinito = sommo zero

        eliminaCoda(nodeQueue,currNode); //lo elimino dalla coda
        scoreArray[currNode].isPresent='0'; //metto il flag eliminato nell'array punteggi

        for(i=0;i<nodeNumb;i++){
            oldDist=scoreArray[i].myScore; //prendo il valore del punteggio dall'array punteggi

            if(grafo[currNode][i]!=0 && scoreArray[i].isPresent=='1'){

                if(currDist!=UINT_MAX) newDist=currDist+grafo[currNode][i];
                else newDist=UINT_MAX;

                if(oldDist>newDist){
                    elem tmpEl;
                    tmpEl.index=i;
                    tmpEl.score=newDist;
                    aggiornaCoda(nodeQueue,tmpEl);

                    scoreArray[i].myScore=newDist; //aggiorno l'array dei punteggi
                }
            }

        }

    }

    free(nodeQueue);
    free(scoreArray);
    return somma;
}
