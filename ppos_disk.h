// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 1.2 -- Julho de 2017

// interface do gerente de disco rígido (block device driver)

#ifndef __DISK_MGR__
#define __DISK_MGR__
#include "ppos_data.h"
#include "queue.h"

// estruturas de dados e rotinas de inicializacao e acesso
// a um dispositivo de entrada/saida orientado a blocos,
// tipicamente um disco rigido.

// estrutura que representa um disco no sistema operacional

typedef struct request_t {
  struct request_t *prev, *next;
  void *buffer;
  int block;
  int operation;
  task_t *task;
} request_t;

enum scheduler {
  FCFS,
  SSTF,
  CSCAN
};

typedef struct {
  // completar com os campos necessarios
  int wakeup;
  int head_pos;
  int distance;
  int last_pos;
  enum scheduler scheduler;
  queue_t *ready_queue;
  queue_t *suspend_queue;
  semaphore_t semaphore;
  mutex_t mutex;
  task_t disk_task;
} disk_t;


extern disk_t disk;

// inicializacao do gerente de disco
// retorna -1 em erro ou 0 em sucesso
// numBlocks: tamanho do disco, em blocos
// blockSize: tamanho de cada bloco do disco, em bytes
int disk_mgr_init(int *numBlocks, int *blockSize);

// leitura de um bloco, do disco para o buffer
int disk_block_read(int block, void *buffer);

// escrita de um bloco, do buffer para o disco
int disk_block_write(int block, void *buffer);

request_t *request_create(int operation, int block, void *buffer, task_t *task);

request_t *disk_scheduler();

request_t *fcfs();

request_t *sstf();

request_t *cscan();

void disk_queue_manager(void *arg);

#endif
