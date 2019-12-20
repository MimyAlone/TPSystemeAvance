#define NBOP 10
#define MAX 1000
#define RETRAIT 0
#define AJOUT 1

#include <stdio.h>
#include <stdlib.h>  
#include <time.h>
#include <pthread.h>


int solde = MAX;

typedef struct{
    int num, op, montant;
}OPERATION;

pthread_mutex_t mutex;

void operation(int num, int op, int montant){
    printf("operation n° %d, op = %d, montant = %d\n",num,op,montant);
    if(op == RETRAIT){
        if(solde>=montant){
            solde = montant;
            printf("nouveau solde après retrait : %d\n",solde);
        }else{
            printf("solde %d insuffisant pour effectuer un retrait de %d\n\n",solde,montant);
        }
    }else{
        solde += montant;
        printf("nouveau solde après ajout : %d\n\n",solde);
    }
}

void * operation1(void * arg){
    OPERATION *op = (OPERATION *)arg;
    
    printf("operation n° %d, op = %d, montant = %d\n",op->num,op->op,op->montant);
    if(op->op == RETRAIT){
        pthread_mutex_lock(&mutex);
        if(solde>=op->montant){
            solde = op->montant;
            printf("nouveau solde après retrait : %d\n",solde);
        }else{
            printf("solde %d insuffisant pour effectuer un retrait de %d\n\n",solde,op->montant);
        }
        pthread_mutex_unlock(&mutex);
    }else{
        pthread_mutex_lock(&mutex);
        solde += op->montant;
        pthread_mutex_unlock(&mutex);
        printf("nouveau solde après ajout : %d\n\n",solde);
    }
    
    free(op);
    
    return NULL;
}

int main(){
    pthread_mutex_init(&mutex,NULL);
    //initialisation du générateur de nombres aléatoires
    srand(time(NULL));
//     for(i = 1;i<=NBOP;i++){
//         operation(i,rand()%2,rand()%MAX);
//     }
    
    
    
    pthread_t tid;
    int err;
    
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    
    for(int i=0;i<NBOP;i++) {
        OPERATION * Ope = (OPERATION *)malloc(sizeof(OPERATION));
        Ope->num = i+1;
        Ope->op = rand()%2;
        Ope->montant = rand()%MAX;
        err=pthread_create(&tid,&attr,operation1,(void*)Ope);
        if(err!=0)
            perror("pthread_create");
    }
    
    pthread_exit(NULL);
    
    return 0;
}
