Name: Megha Sravani Lavu(username:melavu)
CSU ID: 2762646(CIS 545)
~~~~~~~~~~~~~~~~~~~~
Project description:
~~~~~~~~~~~~~~~~~~~~ 

Implemented all 8 functions in sync.c and sync.h

Variables used:
 
readcount and writecount is used to check active readers and writers respectively.
lock: variable which is to be checked before acquiring lock.
rqueue[100],wqueue[100]: The blocked thread are stored in this array. This implementation supports Max 100 waiting/blocked threads.
rqueuecount, wqueuecount: count of read and write waiting/blocked threads in the array.

In init: All variables are initialised to zero.
In destroy: I didn't allocate any memory for any variable so didn't free any resource.
In read_try_lock and write_try_lock: if lock is zero it will return 0 or else if lock is one it will return 1 or else return -1.

read_lock(supports multiple readers): It will check whether lock is zero if it is zero the test and set(this function is atomic) will set lock to 1 and increment the readcount.
If lock is one checks whether there are any write threads if there is no such thread, it will allow read thread else suspend it giving priority to write.

read_unlock: It will check whether lock is one and read count is >0 and write count is zero and then it will decrement read count  and if read count is
is zero it will set lock to zero and then checks for blocked thread in wqueue first to wake up all the write threads and then rqueue to wake up read threads
or else it will return -1.

similarly, write_lock and write_unlock are implemented

~~~~~~~~~~~~~~~~~~~~~~
Compiling Instruction:
~~~~~~~~~~~~~~~~~~~~~~
either gcc -m32 bank.c sync.c sthread.c 

or type make and then gcc -m32 bank.c libsthread.a
~~~~~~~~~~~~
Test case 1:
~~~~~~~~~~~~
 checking for multiple readers (I didn't do unlock in balance enquiry to test whether multiple readers can access critical section code or not)


#define _REENTRANT
#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include"sthread.h"

sthread_rwlock_t rwlock;
sthread_rwlock_t *rwlockptr=&rwlock;
float balance=10000;


int balanceenquiry(void *args);


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

	//int uret=sthread_read_unlock(&rwlock);
	//if(uret==0)
	//{
		//printf("unlock successful\n");
	//}
}

int main(int argc,char*argv[])
{
	sthread_rwlock_init(&rwlock);
	sthread_t thread1,thread2,thread3;

       if (sthread_init() == -1)
       {
       		fprintf(stderr, "Error in init\n");
       }

     if(sthread_create(&thread1,balanceenquiry,(void *)1)==-1)
       {
        	fprintf(stderr, "Error in create\n");
       }
    if(sthread_create(&thread2,balanceenquiry,(void *)2)==-1)
       {
       		fprintf(stderr, "Error in create\n");
       }
    if(sthread_create(&thread3,balanceenquiry,(void *)3)==-1)
       {
 		fprintf(stderr, "Error in create\n");
       }

       
	return 0;
}

~~~~~~~
Output:
~~~~~~~
% gcc -m32 bank.c sync.c sthread.c
% ./a.out
The balance is:10000.000000
The balance is:10000.000000
The balance is:10000.000000
%

~~~~~~~~~~~~
Test case 2:   
~~~~~~~~~~~~

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
       if(sthread_create(&thread3,cashwithdrawal,(void *)3)==-1)
       {
       		fprintf(stderr, "Error in create\n");
       } 
	return 0;
}

~~~~~~~
Output:
~~~~~~~

% gcc -m32 bank.c sync.c sthread.c
% ./a.out
Remaining balance:11000.000000
unlock successful in cashdeposit
The balance is:11000.000000
unlock successful in balance enquiry
Remaining balance:10000.000000
unlock successful in cashwithdrawal
