#include "ppos_disk.h"
#include "disk.h"
#include "ppos-core-globals.h"
#include "ppos.h"
#include "ppos_data.h"
#include "queue.h"
#include <stdio.h>

disk_t disk;

request_t *request_create(int operation, int block, void *buffer,
                          task_t *task) {
  request_t *request = (request_t *)malloc(sizeof(request_t));
  request->operation = operation;
  request->block = block;
  request->buffer = buffer;
  request->task = task;
  return request;
}

request_t *fcfs() {
  request_t *request = (request_t *)queue_remove(&disk.ready_queue, disk.ready_queue);
  return request;
}

request_t *sstf() {
  request_t *first, *closest, *aux;
  first = closest = (request_t *)queue_remove(&disk.ready_queue, disk.ready_queue);

  int distance = abs(first->block - disk.head_pos);

  while (aux != first || aux != NULL) {
    if (abs(aux->block - disk.head_pos) < distance) {
      closest = aux;
      distance = abs(aux->block - disk.head_pos);
    }
    aux = aux->next;
  }
  
  return closest;
}

request_t *cscan() {
  request_t *first, *lowest, *aux;
  first = lowest = (request_t *)queue_remove(&disk.ready_queue, disk.ready_queue);
  aux = first->next;

  while (aux != first || aux != NULL) {
    if (aux->block < lowest->block) {
      lowest = aux;
    }
    aux = aux->next;
  }

  return lowest;
}

request_t *disk_scheduler() {
  switch (disk.scheduler) {
  case FCFS:
    return fcfs();
  case SSTF:
    return sstf();
  case CSCAN:
    return cscan();
  }
}

void disk_queue_manager(void *arg __attribute__((unused))) {
  while (1) {
    sem_up(&disk.semaphore);
    if (disk.wakeup) {
      disk.wakeup = 0;
      request_t *task_request =
          (request_t *)queue_remove(&disk.suspend_queue, disk.suspend_queue);
      task_resume(task_request->task);
    }
    

    if (disk_cmd(DISK_CMD_STATUS, 0, 0) == DISK_STATUS_IDLE &&
        queue_size(disk.ready_queue) > 0) {
      request_t *request = disk_scheduler();
      if (disk_cmd(request->operation, request->block, request->buffer) == -1)
        exit(1);
      queue_append(&disk.suspend_queue, (queue_t *)request);
    }
    task_suspend(&disk.disk_task, NULL);
    sem_down(&disk.semaphore);
  }
}

/*inicializacao do gerente de disco
  retorna -1 em erro ou 0 em sucesso
  numBlocks: tamanho do disco, em blocos
  blockSize: tamanho de cada bloco do disco, em bytes*/
int disk_mgr_init(int *numBlocks, int *blockSize) {
  /*
      A tarefa principal (main) inicializa o gerente/driver de disco através da
     chamada int disk_mgr_init (&num_blocks, &block_size); Ao retornar da
     chamada, a variável num_blocks contém o número de blocos do disco
      inicializado, enquanto a variável block_size contém o tamanho de cada
     bloco do disco, em bytes. Essa chamada retorna 0 em caso de sucesso ou -1
     em caso de erro.
  */

  if (disk_cmd(DISK_CMD_INIT, 0, 0))
    return -1;

  if ((*numBlocks = disk_cmd(DISK_CMD_DISKSIZE, 0, 0)) == -1)
    return -1;

  if ((*blockSize = disk_cmd(DISK_CMD_BLOCKSIZE, 0, 0)) == -1)
    return -1;

  disk.ready_queue = NULL;
  disk.wakeup = 0;
  disk.suspend_queue = NULL;
  disk.scheduler = FCFS;
  // disk.scheduler = SSTF;
  // disk.scheduler = CSCAN;

  task_create(&disk.disk_task, disk_queue_manager, NULL);
  sem_create(&disk.semaphore, 0);

  return 0;
}

/*
    As tarefas podem ler e escrever blocos de dados no disco virtual através das
    seguintes chamadas (ambas bloqueantes): int disk_block_read (int block,
   void* buffer); int disk_block_write (int block, void* buffer); onde: • block:
   posição (número do bloco) a ler ou escrever no disco (deve estar entre 0 e
   numblocks-1); • buffer: endereço dos dados a escrever no disco, ou onde devem
   ser colocados os dados lidos do disco; esse buffer deve ter capacidade para
   block_size bytes. • retorno: 0 em caso de sucesso ou -1 em caso de erro.
*/
int disk_block_read(int block, void *buffer) {
  if (block < 0 || block >= disk_cmd(DISK_CMD_DISKSIZE, 0, 0))
    return -1;

  if (!buffer)
    return -1;
  

  sem_up(&disk.semaphore);
  request_t *request = request_create(DISK_CMD_READ, block, buffer, taskExec);

  queue_append(&disk.ready_queue, (queue_t *)request);
  task_resume(&disk.disk_task);

  task_suspend(taskExec, NULL);
  task_yield();

  sem_down(&disk.semaphore);
  return 0;
}


/*
    As tarefas podem ler e escrever blocos de dados no disco virtual através das
    seguintes chamadas (ambas bloqueantes): int disk_block_read (int block,
   void* buffer); int disk_block_write (int block, void* buffer); onde: • block:
   posição (número do bloco) a ler ou escrever no disco (deve estar entre 0 e
   numblocks-1); • buffer: endereço dos dados a escrever no disco, ou onde devem
   ser colocados os dados lidos do disco; esse buffer deve ter capacidade para
   block_size bytes. • retorno: 0 em caso de sucesso ou -1 em caso de erro.
*/
int disk_block_write(int block, void *buffer) {
  /*escrita de um bloco, do buffer para o disco*/
  if (block < 0 || block >= disk_cmd(DISK_CMD_DISKSIZE, 0, 0))
    return -1;

  if (!buffer)
    return -1;

  sem_up(&disk.semaphore);
  request_t *request = request_create(DISK_CMD_WRITE, block, buffer, taskExec);

  queue_append(&disk.ready_queue, (queue_t *)request);
  task_resume(&disk.disk_task);

  task_suspend(taskExec, NULL);
  task_yield();
  sem_down(&disk.semaphore);

  return 0;
}