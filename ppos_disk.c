#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "disk.h"
#include "ppos_disk.h"
#include "queue.h"

// Fila pras requisicoes de disco
static queue_t *request_queue = NULL;

// Disk manager task
void disk_mgr_task() {
    // Implementation of the disk manager task
}

// Initialize the disk manager
int disk_mgr_init(int *num_blocks, int *block_size) {
    // Implementar
    return disk_init(num_blocks, block_size);
}

// Le um bloco do disco
int disk_block_read(int block, void *buffer) {
    // Implementacao
}

// Escreve um bloco no disco
int disk_block_write(int block, void *buffer) {
    // Implementacao
}

// Signal handler do SIGUSR1
void disk_signal_handler(int signum) {
    // Implementacao
}
