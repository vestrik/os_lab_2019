#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


void do_one_thing();
void do_another_thing();
int common1 = 0; 
int common2 = 0; 
pthread_mutex_t mut1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut2 = PTHREAD_MUTEX_INITIALIZER;

int main() {
  pthread_t thread1, thread2;

  if (pthread_create(&thread1, NULL, (void *)do_one_thing,
			  NULL) != 0) {
    perror("pthread_create");
    exit(1);
  }

  if (pthread_create(&thread2, NULL, (void *)do_another_thing,
                     NULL) != 0) {
    perror("pthread_create");
    exit(1);
  }

  if (pthread_join(thread1, NULL) != 0) {
    perror("pthread_join");
    exit(1);
  }

  if (pthread_join(thread2, NULL) != 0) {
    perror("pthread_join");
    exit(1);
  }

  return 0;
}

void do_one_thing() {
  pthread_mutex_lock(&mut1);
  common2 = 1; 
  printf("before deadlock for pthread1\n");
  pthread_mutex_lock(&mut2);
  common1 = 1;
  printf("after deadlock for pthread1\n");
  pthread_mutex_unlock(&mut2);
  pthread_mutex_unlock(&mut1);
}

void do_another_thing() {
  pthread_mutex_lock(&mut2);
  common2 = 1;  
  printf("before deadlock for pthread2\n");
  pthread_mutex_lock(&mut1);
  common1 = 1;
  printf("after deadlock for pthread2\n");
  pthread_mutex_unlock(&mut1);
  pthread_mutex_unlock(&mut2);
}