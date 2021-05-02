# Starvation-Free-Reader-Writer-Problem Solution
**(*Name:Kishore KG, Enrollment No:17116033*)**
# Introduction
This repository contains a .cpp file named **starv_free.cpp** which is used to demonstrate a starvation free solution for the classical Reader-Wirter problem in process synchronisation. The program can be executed by runing the above cpp file using any ide or from the comand line.

The program contains two functions-reader and writer which implement the corresponding the functionalities. 15 reader threads and 3 writer threads have been used to demonstrate the solution. POSIX threads are used from the **pthread.h** library. 
The variable shar_var,a global variable is updated by the writer threads and read by reader threads.
Three semaphores named in, muex and rwsem are used. 
# Semaphores
A semaphore variable that, apart from initialization, is accessed only through two standard atomic operations: wait() and signal(). A semaphore is used to control access to a common resource by multiple processes and avoid critical section problems in a concurrent system such as a multitasking operating system. The semaphores used here are:

- in - Used to signal other threads(both readers and writers) that one of the threads is working. Any thread has to get a postive value for this semaphore to start its execution. This is intialised to 1. This way we can modify the reader-priority solution to equal priority to both readers and writers.
- muex - This semaphore is used to maintain mutual exclution amoung readers trying to update reader_count variable.
- rwsem - This semaphore is used to signal a reader if a writer is wirting and vice versa.

## Reader
An integer variable reader_count is used to keep track of no. of readers in the queue. reader_count is intialised to zero. Reader reads the value of shar_var and prints it in the output. Reader function is as shown bellow
```
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
```
This is slightly modified version of reader-priority solution to the reader wirter problem, where the writers starve until all the readers complete their execution. The only diffence between the reader-priority solution is that the semaphore **in** is used to avoid priority to the readers. 

## Writer

A writer increases the value of shar_var by 200. The writer code is shown below.
```
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
```
Similar to reader code only change is semaphore **in**.

# Sample Output

```
  Reader-0 counted
  Reader-0 is reading value: 2
  Reader-1 counted
  writed-0 counted
  Reader-1 is reading value: 2
  writer-0 is writing value: 202
  Reader-2 counted
  Reader-3 counted
  Reader-2 is reading value: 202
  Reader-4 counted
  Reader-3 is reading value: 202
  Reader-5 counted
  Reader-4 is reading value: 202
  Reader-5 is reading value: 202
  Reader-6 counted
  writed-1 counted
  Reader-6 is reading value: 202
  writer-1 is writing value: 402
  Reader-7 counted
  Reader-7 is reading value: 402
  Reader-8 counted
  Reader-9 counted
  Reader-8 is reading value: 402
  Reader-10 counted
  Reader-10 is reading value: 402
  Reader-9 is reading value: 402
  Reader-11 counted
  Reader-11 is reading value: 402
  writed-2 counted
  writer-2 is writing value: 602
  Reader-12 counted
  Reader-12 is reading value: 602
  Reader-13 counted
  Reader-13 is reading value: 602
  Reader-14 counted
  Reader-14 is reading value: 602
```

# Conclusion

We can see from the output that neither reader nor wirter is given priority. All the threads(readers and writers) are created parallely without any dealay. A writers have done the writing even if the readers are waiting. Similarly readers didn't wait for the wirters. Even after the reader-6 is counted the writer-1 is counted,before reader-6 has read. This is not the case with solution with reader having priority, where if a reader had entered the queue it is given priority. Here writer has done its job before even if a reader is present. Hence solution is starvation free.
