#include<semaphore.h>
#include<stdio.h>
#include<pthread.h>
# include<bits/stdc++.h>

using namespace std;
void *reader(void *);
void *writer(void *);
pthread_t r[15],w[3]; //array of thread id's
int shar_var = 2,reader_count = 0; // common data to the threads
sem_t in,muex,rwsem;  //semaphores used

//reader code
void *reader(void *i){
    /*
    --------------------
    ENTRY SECTION
    --------------------
    */
    
    sem_wait(&in);  //waiting on this semaphore to  any other reader or writer to complete its counting 
    sem_wait(&muex); //semaphore to update reader_count
    reader_count++;
    printf("Reader-%d counted\n",i);
    if(reader_count==1) 
    sem_wait(&rwsem); //rw semaphore to wait if writer is writing 
    sem_post(&muex); //signal muex
    sem_post(&in);  //signal in
    /*
    --------------------
    CRITICAL SECTION
    --------------------
    */
    printf("Reader-%d is reading value: %d\n",i,shar_var);
    /*
    --------------------
    EXIT SECTION
    --------------------
    */
    sem_wait(&muex); //wait(muex) to update reader_count
    
    reader_count--;
    if(reader_count==0)
    sem_post(&rwsem); //singal rwsem
    sem_post(&muex); //singal muex

}

void *writer(void *i){
     /*
    --------------------
    ENTRY SECTION
    --------------------
    */
    sem_wait(&in); //wait for currently executing reader to finish its counting or writer thread to finish 
    printf("writed-%d counted\n",i);
    sem_wait(&rwsem); //wait if some reader is reading or some writer is writing
    /*
    --------------------
    CRITICAL SECTION
    --------------------
    */
    shar_var +=200;
     printf("writer-%d is writing value: %d \n",i,shar_var);
     /*
    --------------------
    EXIT SECTION
    --------------------
    */
     sem_post(&rwsem); //singnal rwsem
     sem_post(&in);  //signak in
}
int main(){
    //intitialising semaphores
    sem_init(&in,0,1);
    sem_init(&rwsem,0,1);
    sem_init(&muex,0,1);
    //creating threads
    pthread_create(&r[0],NULL,reader,(void *)0);
    pthread_create(&w[0],NULL,writer,(void *)0);
    pthread_create(&r[1],NULL,reader,(void *)1);
    pthread_create(&r[2],NULL,reader,(void *)2);
    pthread_create(&r[3],NULL,reader,(void *)3);
    pthread_create(&r[4],NULL,reader,(void *)4);
    pthread_create(&r[5],NULL,reader,(void *)5);
    pthread_create(&r[6],NULL,reader,(void *)6);
     pthread_create(&w[1],NULL,writer,(void *)1);
    pthread_create(&r[7],NULL,reader,(void *)7);
    pthread_create(&r[8],NULL,reader,(void *)8);
    pthread_create(&r[9],NULL,reader,(void *)9);
    pthread_create(&r[10],NULL,reader,(void *)10);
    pthread_create(&r[11],NULL,reader,(void *)11);
     pthread_create(&w[2],NULL,writer,(void *)2);
     pthread_create(&r[12],NULL,reader,(void *)12);
     pthread_create(&r[13],NULL,reader,(void *)13);
    pthread_create(&r[14],NULL,reader,(void *)14);
    
    //waiting for the threads to finish
    pthread_join(r[0],NULL);
    pthread_join(w[0],NULL);
    pthread_join(r[1],NULL);
    pthread_join(r[2],NULL);
    pthread_join(r[3],NULL);
   
    pthread_join(r[4],NULL);
    pthread_join(r[5],NULL);
    pthread_join(r[6],NULL);
   
    pthread_join(w[1],NULL);
    pthread_join(r[7],NULL);
    pthread_join(r[8],NULL);
    pthread_join(r[9],NULL);
   
    pthread_join(r[10],NULL);
    pthread_join(r[11],NULL);
    pthread_join(w[2],NULL);
    pthread_join(r[12],NULL);
    pthread_join(r[13],NULL);
    pthread_join(r[14],NULL);
return 0;
}