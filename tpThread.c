#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define MAX 		10

int tab[MAX];
int nb=0;

typedef struct{
    int * min,*max;
}interval;

pthread_mutex_t mutex;


void lire() {
	int i;
	printf("saisir %d entiers\n", MAX);
	for (i=0; i<MAX ; i++) scanf("%d", &tab[i]);
}
void affiche(){
	int i;
	printf("entiers saisis :");
	for (i=0; i<MAX ; i++) printf("%d ",tab [i]);
	printf("\n");
}
void* moyenne(void* arg){
	int i, moy=tab[0];
	for (i=1; i<MAX; i++) moy = moy + tab[i];
	printf("moyenne des entiers = %f\n", (float)moy/MAX);
    return NULL;
}
void* supSeuil(void* seuil1){
	int i, seuil =  * (int *)seuil1;
//     free(seuil1);
	for (i=0; i<MAX; i++) {
        if (tab[i]>seuil)
        {
            pthread_mutex_lock(&mutex);
            nb++;
            pthread_mutex_unlock(&mutex);
        }
    }
// 	printf("fin de fonction supSeuil : nb = %d\n", nb);
    return NULL;
}
void* infSeuil(void* seuil1){
	int i, seuil = * (int *)seuil1;
//     free(seuil1);
	for (i=0; i<MAX; i++) {
        if (tab[i]<seuil) 
        {
            pthread_mutex_lock(&mutex);
            nb++;
            pthread_mutex_unlock(&mutex);
        }
    }
// 	printf("fin de fonction infSeuil : nb = %d\n", nb);
    return NULL;
}

void minMax(int *min, int *max) {
	int i;
	*min=tab[0];
	*max=tab[0];
	for (i=1; i<MAX; i++) {
	    if (tab[i]<*min) *min=tab[i];
	    else if (tab[i]>*max) *max=tab[i];
	}
}


void* minMax2(void *arg) {
	int i;
	*((interval *)arg)->min=tab[0];
	*((interval *)arg)->max=tab[0];
	for (i=1; i<MAX; i++) {
	    if (tab[i]<*((interval *)arg)->min)
            *((interval *)arg)->min=tab[i];
	    else if (tab[i]>*((interval *)arg)->max)
            *((interval *)arg)->max=tab[i];
	}
	return NULL;
}

int main(){
	int min, max;
    int seuilInf, seuilSup;
    interval minmax;
    pthread_t tid1,tid2,tid3,tid4;
    pthread_attr_t attr;
    pthread_mutex_init(&mutex,NULL);


    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	lire();
	affiche();
	pthread_create(&tid1,&attr,moyenne, NULL);
    
	printf("saisir les seuils sup et inf : ");
	scanf("%d%d", &seuilSup, &seuilInf);
    
	pthread_create(&tid2,NULL,supSeuil, (void*)&seuilSup);

    pthread_create(&tid3,NULL,infSeuil, (void*)&seuilInf);

// 	supSeuil(seuilSup);
// 	infSeuil(seuilInf);
// 	minMax(&min, &max);
    minmax.min = &min;
    minmax.max= &max;
    pthread_create(&tid4,NULL,minMax2, (void*)&minmax);
    pthread_join(tid4,NULL);
	printf("min = %d, max = %d\n", *(minmax.min), *(minmax.max));
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);
    printf("fin de fonction infSeuil : nb = %d\n", nb);
    pthread_exit(NULL);
	return(0);
}
