#include<stdio.h>
#include<errno.h>
#include<pthread.h>

#define MAX_RESOURCES 5
int available_resources = MAX_RESOURCES;
pthread_mutex_t mtx;

int decrease_count(int count)
{
    pthread_mutex_lock(&mtx);    
    if (available_resources < count)
    {
        pthread_mutex_unlock(&mtx);
        return -1;
    }
    else
    {
        available_resources -= count;
        printf("%s %d %s %d %s\n", "Got", count, "resources", available_resources, "remaining");
        pthread_mutex_unlock(&mtx);
    }
    return 0;
}

int increase_count(int count)
{
    pthread_mutex_lock(&mtx);    
    available_resources += count;
    printf("%s %d %s %d %s\n", "Released", count, "resources", available_resources, "remaining");
    pthread_mutex_unlock(&mtx);
    return 0;
}

void* f(void* param)
{
    int* i = (int *)param;
    printf("%d\n", *i);  
   
   //guard code 
    if(decrease_count(3) != -1)
        increase_count(3);
       
    return param;
}

int main()
{
    pthread_mutex_init(&mtx, NULL);
    printf("%s %d\n", "MAX_RESOURCES =", MAX_RESOURCES);
    int thread_count = 5;
    pthread_t threads[thread_count];
    
    for(int i = 0; i < thread_count; i++)
    {
        int a = i;        
        if(pthread_create(&threads[i], NULL, f, &a))
        {
            perror(NULL);
            return errno;
        }
    }
    
    for(int i = 0; i < thread_count; i++)
    {
        if(pthread_join(threads[i], NULL))
        {
            perror(NULL);
            return errno;
        }
    }

    pthread_mutex_destroy(&mtx);
    return 0;
}
