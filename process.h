#define THREAD_RUNNING 0 //thread is currently running
#define THREAD_READY 1 //thread is ready to be run
#define THREAD_SLEEPING 2 //set from call to thread_sleep()
#define THREAD_WAITING 3 //waiting on mutex or semaphore

struct process {
    int status;
    char name[10];
};