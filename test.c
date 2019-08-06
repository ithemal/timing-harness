#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <assert.h>

#include "harness.h"

int main(int argc, char **argv) {
  char *code_file_name = argv[1];
  unsigned int unroll_factor = atoi(argv[2]);
  FILE *code_file = fopen(code_file_name, "rb");
  fseek(code_file, 0, SEEK_END);
  unsigned long code_size = ftell(code_file);
  fseek(code_file, 0, SEEK_SET);
  char *code_to_test = malloc(code_size);
  unsigned long bytes_read = fread(code_to_test, 1, code_size, code_file);
  assert(bytes_read == code_size);

  // allocate 3 pages, the first one for testing
  // the rest for writing down result
  int shm_fd = create_shm_fd("shm-path");

  // `measure` writes the result here
  int l1_read_supported, l1_write_supported, icache_supported;
  struct pmc_counters *counters = measure(
      code_to_test, code_size, unroll_factor,
      &l1_read_supported, &l1_write_supported, &icache_supported, 
      shm_fd);


  if (!counters) {
    fprintf(stderr, "failed to run test\n");
    return 1;
  }

  // print the result, ignore the first set of counters, which is garbage
  printf("Core_cyc\tL1_read_misses\tL1_write_misses\tiCache_misses\tContext_switches\n");
  int i;
  for (i = 1; i < HARNESS_ITERS; i++) {
    printf("%ld\t%ld\t%ld\t%ld\t%ld\n",
        counters[i].core_cyc,
        l1_read_supported ? counters[i].l1_read_misses : -1,
        l1_write_supported ? counters[i].l1_write_misses : -1,
        icache_supported ? counters[i].icache_misses : -1,
        counters[i].context_switches);
  }

  return 0;
}
