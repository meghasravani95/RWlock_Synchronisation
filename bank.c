#define _REENTRANT
#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include"sthread.h"

sthread_rwlock_t rwlock;
sthread_rwlock_t *rwlockptr=&rwlock;
float balance=10000;


int balanceenquiry(void *args);
int cashwithdrawal(void *args);
int cashdeposit(void *args);

int balanceenquiry(void *args)
{
	int ret=sthread_read_lock(&rwlock);
	if(ret==0)
	{
		//critical code
		printf("The balance is:%f\n",balance);

	}
	else if(ret==1)
	{
		printf("suspended in\n");
	}
	else
	{
		printf("errror");
	}

	int uret=sthread_read_unlock(&rwlock);
	if(uret==0)
	{
		printf("unlock successful in balance enquiry\n");
	}
}
int cashwithdrawal(void *args)
{
	int ret1=sthread_write_lock(&rwlock);
	
	if(ret1==0)
	{
		// critical code
		
	  float amount;
	//printf("Enter the amount to be withdrawed\n");
	//scanf("%f",&amount);
	balance=balance-1000;
	printf("Remaining balance:%f\n",balance);

	}
		else if(ret1==1)
	{
		printf("suspended cash withdrwal\n");
	}
	else
	{
		printf("errror");
	}

	
	int wret=sthread_write_unlock(&rwlock);
	if(wret==0)
	{
		printf("unlock successful in cashwithdrawal\n");
	}
	else
	{
		printf("error in unlocking cash withdrawal\n");
	}
 return 0;
}
int cashdeposit(void *args)

{
	int ret2=sthread_write_lock(&rwlock);

	if(ret2==0)
	{
		//critical code
	float amount;
    //printf("Enter the amount to be deposited\n");
	//scanf("%f",&amount);
	balance=balance+1000;
	printf("Remaining balance:%f\n",balance);

	}
	else if(ret2==1)
	{
		printf("suspended cash deposit\n");
	}
	else
	{
		printf("error\n");
	}
	

	int wret1=sthread_write_unlock(&rwlock);
	
	if(wret1==0)
	{
	  printf("unlock successful in cashdeposit\n");
	}
	else
	{
		printf("error in unlocking cash deposit\n");
	}
 
	return 0;
}

int main(int argc,char*argv[])
{
	sthread_rwlock_init(&rwlock);
	sthread_t thread1,thread2,thread3;

       if (sthread_init() == -1)
       {
       		fprintf(stderr, "Error in init\n");
       }

    if(sthread_create(&thread1,cashdeposit,(void *)1)==-1)
       {
        	fprintf(stderr, "Error in create\n");
       }
   if(sthread_create(&thread2,balanceenquiry,(void *)2)==-1)
       {
       		fprintf(stderr, "Error in create\n");
       }
       if(sthread_create(&thread2,cashwithdrawal,(void *)2)==-1)
       {
       		fprintf(stderr, "Error in create\n");
       }
	
 
       
	return 0;
}