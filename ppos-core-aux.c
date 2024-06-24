#include "disk.h"
#include "ppos-core-globals.h"
#include "ppos.h"
#include "ppos_data.h"
#include <signal.h>
#include <stdio.h>

// ****************************************************************************
// Coloque aqui as suas modificações, p.ex. includes, defines variáveis,
// estruturas e funções
#include "ppos_disk.h"
#include "queue.h"
#include <signal.h>
#include <sys/time.h>

struct sigaction action;
struct itimerval timer;

#define STANDARD_EXECUTION_TIME 99999
#define QUANTUM 20

void handler(int signum) {
  switch (signum) {
  case SIGALRM:
    systemTime++;
    if (taskExec == taskMain || taskExec == taskDisp) {
      // printf("queue size: %d\n", queue_size(disk.ready_queue));
      // printf("queue size: %d\n", queue_size(disk.suspend_queue));
      if (disk_cmd(DISK_CMD_STATUS, 0, 0) == DISK_STATUS_IDLE &&
          (queue_size(disk.ready_queue) > 0 ||
           queue_size(disk.suspend_queue) > 0)) {
        disk.wakeup = 1;
        task_resume(&disk.disk_task);
      }
      return;
    }
    if (taskExec->quantum == 0) {
      task_yield();
      taskExec->quantum = QUANTUM;
    }
    if (taskExec->ret == 0) {
      task_yield();
    }
    taskExec->quantum--;
    taskExec->ret--;
    taskExec->running_time++;
    break;
  case SIGUSR1:
    disk.wakeup = 1;
    // if (disk_cmd(DISK_CMD_STATUS, 0, 0) == DISK_STATUS_IDLE)
    task_resume(&disk.disk_task);
    break;
  case SIGINT:
  case SIGTERM:
  case SIGKILL:
    task_exit(0);
    break;
  default:
    break;
  }
}

int task_getprio(task_t *task) {
  /*
      Esta função devolve a prioridade estática da tarefa task (ou da tarefa
      corrente, se task for nulo). A prioridade é um valor entre -20 e +20,
     sendo que tarefas com prioridade negativa tem maior prioridade.
  */
  if (task == NULL) {
    return taskExec->priority;
  }

  return task->priority;
}

void task_setprio(task_t *task, int prio) {
  /*
      Esta função ajusta a prioridade estática da tarefa task (ou da tarefa
      corrente, se task for nulo). A prioridade é um valor entre -20 e +20,
     sendo que tarefas com prioridade negativa tem maior prioridade.
  */
  if (task == NULL) {
    taskExec->priority = prio;
  } else {
    task->priority = prio;
  }
}

void task_set_eet(task_t *task, int et) {
  /*
      Esta função ajusta a
      prioridade com base no tempo de execução total estimado para cada tarefa.
     Caso task seja nulo, ajusta a prioridade da tarefa atual. Quando a tarefa
     já está em execução, essa função deve sobrescrever tanto o valor estimado
     do tempo de execução como também o valor do tempo que ainda resta para a
     tarefa terminar sua execução.
  */
  if (task == NULL) {
    taskExec->eet = et;
    taskExec->ret = et - taskExec->running_time;
  } else {
    task->eet = et;
    task->ret = et - taskExec->running_time;
  }
}

int task_get_eet(task_t *task) {
  /*
      Esta função devolve o valor do tempo
      estimado de execução da tarefa task (ou da tarefa corrente, se task for
     nulo)
  */
  if (task == NULL) {
    return taskExec->eet;
  }

  return task->eet;
}

int task_get_ret(task_t *task) {
  /*
      Esta função devolve o valor do tempo
      restante de execução da tarefa task (ou da tarefa corrente, se task for
     nulo)
  */
  if (task == NULL) {
    return taskExec->ret;
  }

  return task->ret;
}

task_t *scheduler() {
  task_t *task = readyQueue;

  if (readyQueue == NULL)
    return readyQueue;

  task_t *aux = readyQueue->next;

  for (int i = 0; i < countTasks; i++, aux = aux->next) {
    if ((aux != taskMain && aux != taskDisp) &&
        (task == readyQueue || task_get_ret(task) > task_get_ret(aux))) {
      task = aux;
    }
  }

  return task;
}

// ****************************************************************************

void before_ppos_init() {
  // put your customization here
#ifdef DEBUG
  printf("\ninit - BEFORE");
#endif
}

void after_ppos_init() {
  // put your customization here
  action.sa_handler = handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;

  if (sigaction(SIGALRM, &action, NULL) < 0) {
    perror("Erro em sigaction: ");
    exit(1);
  }

  if (sigaction(SIGUSR1, &action, NULL) < 0) {
    perror("Erro em sigaction: ");
    exit(1);
  }

  timer.it_value.tv_usec = 1000;
  timer.it_value.tv_sec = 0;
  timer.it_interval.tv_usec = 1000;
  timer.it_interval.tv_sec = 0;

  if (setitimer(ITIMER_REAL, &timer, 0) < 0) {
    perror("Erro em setitimer: ");
    exit(1);
  }
#ifdef DEBUG
  printf("\ninit - AFTER");
#endif
}

void before_task_create(task_t *task) {
  // put your customization here
#ifdef DEBUG
  printf("\ntask_create - BEFORE - [%d]", task->id);
#endif
}

void after_task_create(task_t *task) {
  // put your customization here
  task_set_eet(task, STANDARD_EXECUTION_TIME);
  task->quantum = QUANTUM;
  task->activations = 0;
  task->processor_time = systemTime;
#ifdef DEBUG
  printf("\ntask_create - AFTER - [%d]", task->id);
#endif
}

void before_task_exit() {
  // put your customization here
  printf("Task %d exit: execution time %d ms, processor time %d ms, %d "
         "activations\n",
         taskExec->id, systemTime - taskExec->processor_time,
         taskExec->running_time, taskExec->activations);
#ifdef DEBUG
  printf("\ntask_exit - BEFORE - [%d]", taskExec->id);
#endif
}

void after_task_exit() {
  // put your customization here
#ifdef DEBUG
  printf("\ntask_exit - AFTER- [%d]", taskExec->id);
#endif
}

void before_task_switch(task_t *task) {
  // put your customization here
#ifdef DEBUG
  printf("\ntask_switch - BEFORE - [%d -> %d]", taskExec->id, task->id);
#endif
}

void after_task_switch(task_t *task) {
  // put your customization here
  task->activations++;
#ifdef DEBUG
  printf("\ntask_switch - AFTER - [%d -> %d]", taskExec->id, task->id);
#endif
}

void before_task_yield() {
  // put your customization here
#ifdef DEBUG
  printf("\ntask_yield - BEFORE - [%d]", taskExec->id);
#endif
}
void after_task_yield() {
  // put your customization here
#ifdef DEBUG
  printf("\ntask_yield - AFTER - [%d]", taskExec->id);
#endif
}

void before_task_suspend(task_t *task) {
  // put your customization here
#ifdef DEBUG
  printf("\ntask_suspend - BEFORE - [%d]", task->id);
#endif
}

void after_task_suspend(task_t *task) {
  // put your customization here
#ifdef DEBUG
  printf("\ntask_suspend - AFTER - [%d]", task->id);
#endif
}

void before_task_resume(task_t *task) {
  // put your customization here
#ifdef DEBUG
  printf("\ntask_resume - BEFORE - [%d]", task->id);
#endif
}

void after_task_resume(task_t *task) {
  // put your customization here
#ifdef DEBUG
  printf("\ntask_resume - AFTER - [%d]", task->id);
#endif
}

void before_task_sleep() {
  // put your customization here
#ifdef DEBUG
  printf("\ntask_sleep - BEFORE - [%d]", taskExec->id);
#endif
}

void after_task_sleep() {
  // put your customization here
#ifdef DEBUG
  printf("\ntask_sleep - AFTER - [%d]", taskExec->id);
#endif
}

int before_task_join(task_t *task) {
  // put your customization here
#ifdef DEBUG
  printf("\ntask_join - BEFORE - [%d]", taskExec->id);
#endif
  return 0;
}

int after_task_join(task_t *task) {
  // put your customization here
#ifdef DEBUG
  printf("\ntask_join - AFTER - [%d]", taskExec->id);
#endif
  return 0;
}

int before_sem_create(semaphore_t *s, int value) {
  // put your customization here
#ifdef DEBUG
  printf("\nsem_create - BEFORE - [%d]", taskExec->id);
#endif
  return 0;
}

int after_sem_create(semaphore_t *s, int value) {
  // put your customization here
#ifdef DEBUG
  printf("\nsem_create - AFTER - [%d]", taskExec->id);
#endif
  return 0;
}

int before_sem_down(semaphore_t *s) {
  // put your customization here
#ifdef DEBUG
  printf("\nsem_down - BEFORE - [%d]", taskExec->id);
#endif
  return 0;
}

int after_sem_down(semaphore_t *s) {
  // put your customization here
#ifdef DEBUG
  printf("\nsem_down - AFTER - [%d]", taskExec->id);
#endif
  return 0;
}

int before_sem_up(semaphore_t *s) {
  // put your customization here
#ifdef DEBUG
  printf("\nsem_up - BEFORE - [%d]", taskExec->id);
#endif
  return 0;
}

int after_sem_up(semaphore_t *s) {
  // put your customization here
#ifdef DEBUG
  printf("\nsem_up - AFTER - [%d]", taskExec->id);
#endif
  return 0;
}

int before_sem_destroy(semaphore_t *s) {
  // put your customization here
#ifdef DEBUG
  printf("\nsem_destroy - BEFORE - [%d]", taskExec->id);
#endif
  return 0;
}

int after_sem_destroy(semaphore_t *s) {
  // put your customization here
#ifdef DEBUG
  printf("\nsem_destroy - AFTER - [%d]", taskExec->id);
#endif
  return 0;
}

int before_mutex_create(mutex_t *m) {
  // put your customization here
#ifdef DEBUG
  printf("\nmutex_create - BEFORE - [%d]", taskExec->id);
#endif
  return 0;
}

int after_mutex_create(mutex_t *m) {
  // put your customization here
#ifdef DEBUG
  printf("\nmutex_create - AFTER - [%d]", taskExec->id);
#endif
  return 0;
}

int before_mutex_lock(mutex_t *m) {
  // put your customization here
#ifdef DEBUG
  printf("\nmutex_lock - BEFORE - [%d]", taskExec->id);
#endif
  return 0;
}

int after_mutex_lock(mutex_t *m) {
  // put your customization here
#ifdef DEBUG
  printf("\nmutex_lock - AFTER - [%d]", taskExec->id);
#endif
  return 0;
}

int before_mutex_unlock(mutex_t *m) {
  // put your customization here
#ifdef DEBUG
  printf("\nmutex_unlock - BEFORE - [%d]", taskExec->id);
#endif
  return 0;
}

int after_mutex_unlock(mutex_t *m) {
  // put your customization here
#ifdef DEBUG
  printf("\nmutex_unlock - AFTER - [%d]", taskExec->id);
#endif
  return 0;
}

int before_mutex_destroy(mutex_t *m) {
  // put your customization here
#ifdef DEBUG
  printf("\nmutex_destroy - BEFORE - [%d]", taskExec->id);
#endif
  return 0;
}

int after_mutex_destroy(mutex_t *m) {
  // put your customization here
#ifdef DEBUG
  printf("\nmutex_destroy - AFTER - [%d]", taskExec->id);
#endif
  return 0;
}

int before_barrier_create(barrier_t *b, int N) {
  // put your customization here
#ifdef DEBUG
  printf("\nbarrier_create - BEFORE - [%d]", taskExec->id);
#endif
  return 0;
}

int after_barrier_create(barrier_t *b, int N) {
  // put your customization here
#ifdef DEBUG
  printf("\nbarrier_create - AFTER - [%d]", taskExec->id);
#endif
  return 0;
}

int before_barrier_join(barrier_t *b) {
  // put your customization here
#ifdef DEBUG
  printf("\nbarrier_join - BEFORE - [%d]", taskExec->id);
#endif
  return 0;
}

int after_barrier_join(barrier_t *b) {
  // put your customization here
#ifdef DEBUG
  printf("\nbarrier_join - AFTER - [%d]", taskExec->id);
#endif
  return 0;
}

int before_barrier_destroy(barrier_t *b) {
  // put your customization here
#ifdef DEBUG
  printf("\nbarrier_destroy - BEFORE - [%d]", taskExec->id);
#endif
  return 0;
}

int after_barrier_destroy(barrier_t *b) {
  // put your customization here
#ifdef DEBUG
  printf("\nbarrier_destroy - AFTER - [%d]", taskExec->id);
#endif
  return 0;
}

int before_mqueue_create(mqueue_t *queue, int max, int size) {
  // put your customization here
#ifdef DEBUG
  printf("\nmqueue_create - BEFORE - [%d]", taskExec->id);
#endif
  return 0;
}

int after_mqueue_create(mqueue_t *queue, int max, int size) {
  // put your customization here
#ifdef DEBUG
  printf("\nmqueue_create - AFTER - [%d]", taskExec->id);
#endif
  return 0;
}

int before_mqueue_send(mqueue_t *queue, void *msg) {
  // put your customization here
#ifdef DEBUG
  printf("\nmqueue_send - BEFORE - [%d]", taskExec->id);
#endif
  return 0;
}

int after_mqueue_send(mqueue_t *queue, void *msg) {
  // put your customization here
#ifdef DEBUG
  printf("\nmqueue_send - AFTER - [%d]", taskExec->id);
#endif
  return 0;
}

int before_mqueue_recv(mqueue_t *queue, void *msg) {
  // put your customization here
#ifdef DEBUG
  printf("\nmqueue_recv - BEFORE - [%d]", taskExec->id);
#endif
  return 0;
}

int after_mqueue_recv(mqueue_t *queue, void *msg) {
  // put your customization here
#ifdef DEBUG
  printf("\nmqueue_recv - AFTER - [%d]", taskExec->id);
#endif
  return 0;
}

int before_mqueue_destroy(mqueue_t *queue) {
  // put your customization here
#ifdef DEBUG
  printf("\nmqueue_destroy - BEFORE - [%d]", taskExec->id);
#endif
  return 0;
}

int after_mqueue_destroy(mqueue_t *queue) {
  // put your customization here
#ifdef DEBUG
  printf("\nmqueue_destroy - AFTER - [%d]", taskExec->id);
#endif
  return 0;
}

int before_mqueue_msgs(mqueue_t *queue) {
  // put your customization here
#ifdef DEBUG
  printf("\nmqueue_msgs - BEFORE - [%d]", taskExec->id);
#endif
  return 0;
}

int after_mqueue_msgs(mqueue_t *queue) {
  // put your customization here
#ifdef DEBUG
  printf("\nmqueue_msgs - AFTER - [%d]", taskExec->id);
#endif
  return 0;
}