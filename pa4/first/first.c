#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include"first.h"


int mem_reads;
int mem_writes;
int cache_hit;
int cache_miss;
int associativity; //not sure if this is best being global
long int counter;
cache_line ** cache;


int main(int argc, char ** argv) {

  //not sure if this should be globa
  int num_sets;

  int cache_size = atoi(argv[1]); //total size of cache passed from args
  char * assoc_str = argv[2];
  int block_size = atoi(argv[4]); //block size passed from args
  char* trace_file_str = argv[5]; //trace file passed from args
  FILE* trace_file;

  //seting up associativity and num_sets based on what we read from the args
  if (assoc_str[0] == 'd') {

    associativity = 1;
    num_sets = cache_size / block_size;

  } else if (assoc_str[5] != ':') {

    num_sets = 1;
    associativity = cache_size / block_size;

  } else {
    int hold = 0;
    sscanf(argv[2], "assoc:%d", &hold);
    associativity = hold;
    num_sets = cache_size / block_size / hold;

  }

  int block_bits = log(block_size) / log(2);
  int set_bits = log(num_sets) / log(2);
  long int mask = ((1 << set_bits) - 1);


  trace_file = open_file_clean(trace_file_str);

  cache = make_cache(num_sets);

  process_no_prefetch(trace_file, block_bits, set_bits, mask);

  clear_cache(num_sets, cache);
  zero_out_globals();

  //have to open the file again due to issues that n am unaware of (if it ain't broke don't fix it)
  trace_file = open_file_clean(trace_file_str);

  process_with_prefetch(trace_file, block_bits, set_bits, mask, block_size);

  return 0;
}


cache_line ** make_cache(int num_sets) {

  cache = (cache_line **) malloc(num_sets * sizeof(cache_line *));
  for (int n = 0; n < num_sets; n++) {
    cache[n] = (cache_line * ) malloc((associativity) * sizeof(cache_line));
  }
  for (int n = 0; n < num_sets; n++) {
    for (int m = 0; m < associativity; m++) {
      cache[n][m].is_valid = 0;
    }
  }
  return cache;
}


void clear_cache(int num_sets, cache_line** cache_main) {
  for (int n = 0; n < num_sets; n++) {
    for (int m = 0; m < associativity; m++) {
      cache_main[n][m].accessed = 0;
      cache_main[n][m].tag = 0;
      cache_main[n][m].is_valid = 0;
    }
  }
}

void zero_out_globals()
{
  cache_miss = 0;
  cache_hit = 0;
  mem_reads = 0;
  mem_writes = 0;
  counter = 0;
}


FILE* open_file_clean(char * trace_file_str)
{
  FILE* trace_file = fopen(trace_file_str, "r");
  return trace_file;
}

void simple_counting(int is_read)
{
  if(is_read == 1)
  {
    cache_miss++;
    mem_reads++;
    counter++;
  }
  else if (is_read == 0)
  {
    cache_miss++;
    mem_writes++;
    mem_reads++;
    counter++;
  }
}


void process_no_prefetch(FILE* trace_file, int block_bits, int set_bits, unsigned long mask)
{
  char type;
  long int address;
  long int curr_tag;
  long int curr_set;

  while (fscanf(trace_file, "%*x: %c %lx", &type, &address) == 2) {

    curr_set = (address >> block_bits) & mask;
    curr_tag = address >> (block_bits + set_bits);

    if (type == 'R') {

      normal_read(curr_set, curr_tag);

    }

    if (type == 'W') {

      normal_write(curr_set, curr_tag);

    }

  }

  printf("%s", "no-prefetch");
  printf("%s", "\n");
  printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n", mem_reads, mem_writes, cache_hit, cache_miss);

  fclose(trace_file);

}


void process_with_prefetch(FILE* trace_file, int block_bits, int set_bits,  unsigned long mask, int block_size)
{
  char type;

  long int address;
  long int new_address;

  long int curr_tag;
  long int new_tag;

  long int curr_set;
  long int new_set;

  while (fscanf(trace_file, "%*x: %c %lx", &type, & address) == 2) {

    curr_set = (address >> block_bits) & mask;
    curr_tag = address >> (block_bits + set_bits);
    new_address = address + block_size;
    new_set = (new_address >> block_bits) & mask;
    new_tag = new_address >> (block_bits + set_bits);

    if (type == 'R') {

      prefetch_read(curr_set, curr_tag, new_set, new_tag);

    }
    if (type == 'W') {

      prefetch_write(curr_set, curr_tag, new_set, new_tag);

    }

  }

  printf("%s", "with-prefetch");
  printf("%s", "\n");
  printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n", mem_reads, mem_writes, cache_hit, cache_miss);

  fclose(trace_file);

}


void normal_read(long int curr_set, long int curr_tag) {

  for (int n = 0; n < associativity; n++) {
    if (cache[curr_set][n].is_valid == 0) {
      simple_counting(1);
      cache[curr_set][n].accessed = counter;
      cache[curr_set][n].tag = curr_tag;
      cache[curr_set][n].is_valid = 1;
      return;
    } else if (cache[curr_set][n].tag == curr_tag) {
      cache_hit++;
      counter++;
      cache[curr_set][n].accessed = counter;
      return;
    } else if (n == (associativity - 1)) {

        simple_counting(1);
        int hold_acc = 0;
        for (int m = 0; m < associativity; m++) {

          if (cache[curr_set][m].accessed <= cache[curr_set][hold_acc].accessed) {
            hold_acc = m;
          }
        }

        cache[curr_set][hold_acc].accessed = counter;
        cache[curr_set][hold_acc].tag = curr_tag;
        cache[curr_set][hold_acc].is_valid = 1;
        return;
      }
   }
  return;
}


void normal_write(long int curr_set, long int curr_tag) {


  for (int n = 0; n < associativity; n++) {
    if (cache[curr_set][n].is_valid == 0) {
      simple_counting(0);
      cache[curr_set][n].accessed = counter;
      cache[curr_set][n].tag = curr_tag;
      cache[curr_set][n].is_valid = 1;

      return;
    } else if(cache[curr_set][n].tag == curr_tag) {
        cache_hit++;
        mem_writes++;
        counter++;
        cache[curr_set][n].accessed = counter;
        return;
    } else if (n == (associativity - 1)) {

        simple_counting(0);
        int hold_acc = 0;
        for (int m = 0; m < associativity; m++) {

          if (cache[curr_set][m].accessed <= cache[curr_set][hold_acc].accessed) {
            hold_acc = m;
          }
        }
        cache[curr_set][hold_acc].accessed = counter;
        cache[curr_set][hold_acc].tag = curr_tag;
        cache[curr_set][hold_acc].is_valid = 1;
        return;
      }
  }
  return;
}


void prefetch_read_helper(long int curr_set, long int curr_tag) {

  for (int n = 0; n < associativity; n++) {
    if (cache[curr_set][n].is_valid == 0) {

      mem_reads++;
      counter++;
      cache[curr_set][n].accessed = counter;
      cache[curr_set][n].tag = curr_tag;
      cache[curr_set][n].is_valid = 1;

      return;
    } else if (cache[curr_set][n].tag == curr_tag) {

      return;
    } else if (n == (associativity - 1)) {

      mem_reads++;

      int hold_acc = 0;
      for (int m = 0; m < associativity; m++) {

        if (cache[curr_set][m].accessed <= cache[curr_set][hold_acc].accessed) {
          hold_acc = m;
        }
      }
      counter++;
      cache[curr_set][hold_acc].accessed = counter;
      cache[curr_set][hold_acc].tag = curr_tag;
      cache[curr_set][hold_acc].is_valid = 1;
      return;
    }

  }

}


void prefetch_write_helper(long int curr_set, long int curr_tag) {

  for (int n = 0; n < associativity; n++) {
    if (cache[curr_set][n].is_valid == 0) {

      mem_reads++;
      counter++;
      cache[curr_set][n].accessed = counter;
      cache[curr_set][n].tag = curr_tag;
      cache[curr_set][n].is_valid = 1;

      return;
    } else if (cache[curr_set][n].tag == curr_tag) {

        return;
    } else if (n == (associativity - 1)) {
      mem_reads++;
      int hold_acc = 0;
      for (int m = 0; m < associativity; m++) {

        if (cache[curr_set][m].accessed <= cache[curr_set][hold_acc].accessed) {
          hold_acc = m;
        }
      }
      counter++;
      cache[curr_set][hold_acc].accessed = counter;
      cache[curr_set][hold_acc].tag = curr_tag;
      cache[curr_set][hold_acc].is_valid = 1;
      return;
    }
  }
    return;
}


void prefetch_read(long int curr_set, long int curr_tag, long int new_set, long int new_tag) {

  for (int n = 0; n < associativity; n++) {
    if (cache[curr_set][n].is_valid == 0) {
      simple_counting(1);
      cache[curr_set][n].accessed = counter;
      cache[curr_set][n].tag = curr_tag;
      cache[curr_set][n].is_valid = 1;

      prefetch_read_helper(new_set, new_tag);
      return;

    } else if (cache[curr_set][n].tag == curr_tag) {
      cache_hit++;
      counter++;
      cache[curr_set][n].accessed = counter;
      return;
    } else if (n == (associativity - 1)) {

      simple_counting(1);
      int hold_acc = 0;
      for (int m = 0; m < associativity; m++) {

        if (cache[curr_set][m].accessed <= cache[curr_set][hold_acc].accessed) {
          hold_acc = m;
        }
      }
      cache[curr_set][hold_acc].accessed = counter;
      cache[curr_set][hold_acc].tag = curr_tag;
      cache[curr_set][hold_acc].is_valid = 1;
      prefetch_read_helper(new_set, new_tag);
      return;
    }

  }
  return;
}


void prefetch_write(long int curr_set, long int curr_tag, long int new_set, long int new_tag) {

  for (int n = 0; n < associativity; n++) {
    if (cache[curr_set][n].is_valid == 0) {
      simple_counting(0);
      cache[curr_set][n].accessed = counter;
      cache[curr_set][n].tag = curr_tag;
      cache[curr_set][n].is_valid = 1;
      prefetch_write_helper(new_set, new_tag);
      return;

    } else if (cache[curr_set][n].tag == curr_tag) {
      cache_hit++;
      mem_writes++;
      counter++;
      cache[curr_set][n].accessed = counter;
      return;
    } else if (n == (associativity - 1)) {

        simple_counting(0);
        int hold_acc = 0;
        for (int m = 0; m < associativity; m++) {

          if (cache[curr_set][m].accessed <= cache[curr_set][hold_acc].accessed) {
            hold_acc = m;
          }
        }
        cache[curr_set][hold_acc].accessed = counter;
        cache[curr_set][hold_acc].tag = curr_tag;
        cache[curr_set][hold_acc].is_valid = 1;
        prefetch_write_helper(new_set, new_tag);
        return;
      }
    }
  return;
}
