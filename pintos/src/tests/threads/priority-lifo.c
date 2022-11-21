/* Creates several threads all at the same priority and ensures
   that they consistently run in the same round-robin order.

   Based on a test originally submitted for Stanford's CS 140 in
   winter 1999 by by Matt Franklin
   <startled@leland.stanford.edu>, Greg Hutchins
   <gmh@leland.stanford.edu>, Yu Ping Hu <yph@cs.stanford.edu>.
   Modified by arens. */

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "devices/timer.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"

struct simple_thread_data
{
  int id;            /* Sleeper ID. */
  int iterations;    /* Iterations so far. */
  struct lock *lock; /* Lock on output. */
  int **op;          /* Output buffer position. */
};

#define THREAD_CNT 16
#define ITER_CNT 16

static thread_func simple_thread_func;

void test_priority_lifo(void)
{
  struct simple_thread_data data[THREAD_CNT];
  struct lock lock;
  int *output, *op;
  int i, cnt;

  /* This test does not work with the MLFQS. */
  ASSERT(!thread_mlfqs);

  /* Make sure our priority is the default. */
  ASSERT(thread_get_priority() == PRI_DEFAULT);

  msg("%d threads will iterate %d times in the same order each time.",
      THREAD_CNT, ITER_CNT);
  msg("If the order varies then there is a bug.");

  output = op = malloc(sizeof *output * THREAD_CNT * ITER_CNT * 2);
  ASSERT(output != NULL);
  lock_init(&lock);

  /* 부모 thread가 자식 thread보다 높은 우선순위를 가지게 하여 preempt를 방지함 */
  thread_set_priority(PRI_DEFAULT + THREAD_CNT + 1);

  for (i = 0; i < THREAD_CNT; i++)
  {
    char name[16];

    /**
     * i번째 자식 thread는 data[i]를 할당받음.
     * 모든 자식 thread는 동일한 lock과 output buffer를 공유함.
     */
    struct simple_thread_data *d = data + i;
    snprintf(name, sizeof name, "%d", i);
    d->id = i;
    d->iterations = 0;
    d->lock = &lock;
    d->op = &op;

    /**
     * priority를 1씩 높여가며 자식 thread를 생성.
     * 모든 자식들이 동일한 SIMPLE_THREAD_DATA를 공유함.
     */
    thread_create(name, PRI_DEFAULT + 1 + i, simple_thread_func, d);
  }

  /* 부모의 priority를 다시 낮추면, 자식 thread들에게 우선순위가 밀리므로 READY_LIST로 밀려남. */
  thread_set_priority(PRI_DEFAULT);

  /* All the other threads now run to termination here. */
  ASSERT(lock.holder == NULL);

  cnt = 0;
  for (; output < op; output++)
  {
    struct simple_thread_data *d;

    ASSERT(*output >= 0 && *output < THREAD_CNT);

    /* output buffer에 먼저 접근한 thread 순으로 ID가 저장되어있음. */
    d = data + *output;
    if (cnt % THREAD_CNT == 0)
      printf("(priority-lifo) iteration:");
    printf(" %d", d->id);
    if (++cnt % THREAD_CNT == 0)
      printf("\n");
    d->iterations++;
  }
}

static void
simple_thread_func(void *data_)
{
  struct simple_thread_data *data = data_;
  int i;

  /**
   * 모든 자식들에게 공유되어있는 DATA의 output buffer에 현재 접근한 thread ID를 저장함.
   * 해당 테스트가 정상적으로 수행된다면, 우선순위가 높은 thread의 ID부터 차례로 저장됨.
   */
  for (i = 0; i < ITER_CNT; i++)
  {
    lock_acquire(data->lock);
    *(*data->op)++ = data->id;
    lock_release(data->lock);
    thread_yield();
  }
}
