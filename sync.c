/*
 * NAME, etc.
 *
 * sync.c
 *
 * Synchronization routines for SThread
 */

#define _REENTRANT
#include "sthread.h"


/*
 * rwlock routines
 */
void readqueue(sthread_t s,sthread_rwlock_t *rwlock);
void writequeue(sthread_t s,sthread_rwlock_t *rwlock);



int sthread_rwlock_init(sthread_rwlock_t *rwlock)
{
        /* FILL ME IN! */
               if(1)
               {
                    // initialising all variables to zero
                    rwlock->readcount=0;
                    rwlock->writecount=0;
                    rwlock->lock=0;
                    int i;
                    for(i=0;i<100;i++)
                    {
                        rwlock->rqueue[i]=0;
                        rwlock->wqueue[i]=0;
                    }
                    rwlock->rqueuecount=0;
                    rwlock->wqueuecount=0;
                    return 0;

               }
         return -1;
}

int sthread_rwlock_destroy(sthread_rwlock_t *rwlock)
{
        /* FILL ME IN! */
    if(1)
    {
        return 0;
    }
    // didn't allocate any memory and hence nothing to free
  return -1;      
}

int sthread_read_lock(sthread_rwlock_t *rwlock)
{
        /* FILL ME IN! */
       
          
            if(test_and_set((volatile unsigned long *)&rwlock->lock)==0)
            {
                //when no waiting write threads in queue 
             
                    rwlock->readcount= rwlock->readcount+1;
                    return 0;  
            }
           else if(test_and_set((volatile unsigned long *)&rwlock->lock)==1)
            {
                if(rwlock->writecount==0 && rwlock->wqueuecount==0)
                {
                    //multiple readers and no active write threads, blocked write threads
                    rwlock->readcount= rwlock->readcount+1;
                    return 0;
                }
                else if(rwlock->writecount==1 || rwlock->wqueuecount>=1)
                {
                    // giving priority to write thread it suspends the read thread
                  
                         readqueue(sthread_self(),rwlock);
                         sthread_suspend();
                         return 1;                   
                }
                 return -1;

            }

}

int sthread_read_try_lock(sthread_rwlock_t *rwlock)
{
        /* FILL ME IN! */
       // no suspend of threads since no blocking


        if(rwlock->lock==0)
        {   
            return 0;
        }     
        else if(rwlock->lock==1)
        {
           //  printf("locck unavailable\n");
            return 1;
        }
        else
        {
             return -1;
        }
    
       
}

int sthread_read_unlock(sthread_rwlock_t *rwlock)
{
        /* FILL ME IN! */
    if(rwlock->lock==1 && rwlock->readcount>=1 && rwlock->writecount==0)
    {
        if(test_and_set((volatile unsigned long *)&rwlock->lock)==1);
       
        rwlock->readcount= rwlock->readcount-1;
        if(rwlock->readcount==0)
        {
            rwlock->lock=0; 
       
// given priority to write thread it will check whether they are any write thread in writequeue and then proceed to read thread
               if(rwlock->wqueuecount!=0)
               {
                while(rwlock->wqueuecount>0)
                {
                    sthread_wake(rwlock->wqueue[rwlock->wqueuecount-1]);
                    rwlock->wqueuecount=rwlock->wqueuecount-1; 
                }
                
              }   
             if(rwlock->rqueuecount!=0)
            {
                while(rwlock->rqueuecount>0)
                {
                      sthread_wake(rwlock->rqueue[rwlock->rqueuecount-1]);
                      rwlock->rqueuecount=rwlock->rqueuecount-1;
                }
              
            }
         if(rwlock->rqueuecount==0 && rwlock->wqueuecount==0)
            {
                // printf("no threads\n");
            }

        }
        return 0;
    }

        return -1;
}

int sthread_write_lock(sthread_rwlock_t *rwlock)
{
        /* FILL ME IN! */
    

    if(test_and_set((volatile unsigned long *)&rwlock->lock)==0)
    {
        // no threads
        rwlock->writecount=rwlock->writecount+1;
            return 0;        
    }
    else if(test_and_set((volatile unsigned long *)&rwlock->lock)==1)
    {
        if(rwlock->writecount==1 || rwlock->readcount>=1)
        {
            //when there is Active write thread or read Thread
              
                writequeue(sthread_self(),rwlock);
                sthread_suspend(); 
                return 1;    

        }
        return -1;
     }
}

int sthread_write_try_lock(sthread_rwlock_t *rwlock)
{
        /* FILL ME IN! */

// no suspend of threads since no blocking
 
        if(rwlock->lock==0)
        {           
            // printf("locck available\n");
            return 0;
        }     
        else if(rwlock->lock==1)
        {
           //  printf("locck unavailable\n");
            return 1;
        }
        else
        {
             return -1;
        }
    
}

int sthread_write_unlock(sthread_rwlock_t *rwlock)
{
        /* FILL ME IN! */

    if(rwlock->lock==1 && rwlock->writecount==1)
    {
      if(test_and_set((volatile unsigned long *)&rwlock->lock)==1);
          rwlock->lock=0;
          rwlock->writecount=rwlock->writecount-1;

        //while(test_and_set((volatile unsigned long *)&rwlock->lock)==0);

         if(rwlock->wqueuecount!=0)
            {
              while(rwlock->wqueuecount>0)
                {
                    sthread_wake(rwlock->wqueue[rwlock->wqueuecount-1]);
                    rwlock->wqueuecount=rwlock->wqueuecount-1; 
                }
            }   
           if(rwlock->rqueuecount!=0)
            {
                while(rwlock->rqueuecount>0)
                {

                      sthread_wake(rwlock->rqueue[rwlock->rqueuecount-1]);
                      rwlock->rqueuecount=rwlock->rqueuecount-1;
                }
              
            }
         if(rwlock->rqueuecount==0 && rwlock->wqueuecount==0)
            {
                // printf("no threads\n");
            }
    
      return 0;
    }
    return -1;
}

void readqueue(sthread_t s,sthread_rwlock_t *rwlock)
{
    //printf("added in readqueue\n");
    rwlock->rqueuecount= rwlock->rqueuecount+1;
    rwlock->rqueue[rwlock->rqueuecount-1]=s;
   
}
void writequeue(sthread_t s,sthread_rwlock_t *rwlock)
{
    rwlock->wqueuecount=rwlock->wqueuecount+1;
    rwlock->wqueue[rwlock->wqueuecount-1]=s;
    
}
