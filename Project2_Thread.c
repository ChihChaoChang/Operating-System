#include <stdio.h> 
#include <pthread.h> 
#include <sys/shm.h>
#include <unistd.h> 
#include<sys/ipc.h>
#include <inttypes.h>

int64_t *glob_total;
int *glob_tag;
pthread_mutex_t mutex; 
pthread_cond_t cond; 

void *child(int arg) 
{

    FILE *fp;
    char str[50];
    int64_t total = 0;
    sprintf(str, "testdata%d.txt",arg+1);  
    
    fp = fopen(str,"r");
    while(fscanf(fp,"%s",str) != EOF) 
    {
        if(strcmp(str,"wait")==0) //compare the string if the strings equal to 'wait' and send to share memory
        {
            pthread_mutex_lock(&mutex);   //using mutex to avoid two or more thread enter the share memory
            *glob_total+=total;
            *glob_tag+=1;
            total = 0;
            pthread_cond_wait(&cond,&mutex); 
            pthread_mutex_unlock(&mutex); 
        }
        else           //if it does not meet the 'wait' ,then we add them.
        {
            int x = atoi(str);  
            total+=x;
        }
    }
    fclose(fp);
    exit(0);
}

int main(void) 
{
    int shmID1,shmID2;
    int tid[4];
    int i;
    FILE *out = fopen("Result.txt", "w");
    // shared memory 
    if((shmID1=shmget(IPC_PRIVATE, sizeof(int64_t), SHM_R | SHM_W))<0)
    //IPC_PRIVATE:產生一個新的共享記憶體分段
    //size:需要共享記憶體的大小 , 因為分配大小皆以page為單位 , 所以如果size = 1~4096 , 則實際上會分配到4k
    //SHM_R : 讀記憶體分段 SHM_W: 寫記憶體分段
  
    {
        return -1;
          //成功的話回傳記憶體的位置，失敗回傳-1
    }
    
    if((glob_total=shmat(shmID1,NULL,0))==(int64_t *)-1)
    {
        return -1;
    }
    
    if((shmID2=shmget(IPC_PRIVATE, sizeof(int), SHM_R | SHM_W))<0)
    {
        return -1;
    }
    
    if((glob_tag=shmat(shmID2,NULL,0))==(int *)-1)
    {
        return -1;
    }
    
    *glob_total = 0;
    *glob_tag = 0;
    
    pthread_mutex_init(&mutex,NULL); 
    pthread_cond_init(&cond,NULL); 
    
    for(i = 0;i < 4;i++)
    {
        pthread_create(&tid[i], NULL, child, i);
    }
    i = 0;
    while(1)
    {
        if(*glob_tag == 4) //when the 4 thread all finish their stage mission then print the subtotal
        {
            i++;
            fprintf(out, "No. %d output : %" PRId64 "\n", i, *glob_total);
            printf("No. %d output : %" PRId64 "\n", i,*glob_total);
            *glob_tag = 0;
            *glob_total = 0;
            pthread_cond_broadcast(&cond);
        }
    }
    fclose(out);
    pthread_exit(0);
}
