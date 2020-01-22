typedef struct cache_line {

  long int accessed;//used for LRU
  long int tag;
  int is_valid; //is_valid bit: 1 or 0

}
cache_line;

cache_line ** make_cache(int num_sets);
void clear_cache(int num_sets, cache_line ** cache_main);
void simple_counting(int is_read);
void zero_out_globals();

FILE * open_file_clean(char * trace_file_str);
void process_no_prefetch(FILE* trace_file, int block_bits, int set_bits, unsigned long mask);
void process_with_prefetch(FILE* trace_file, int block_bits, int set_bits, unsigned long mask, int block_size);

void normal_read(long int curr_set, long int curr_tag);
void normal_write(long int curr_set, long int curr_tag);

void prefetch_read_helper(long int curr_set, long int curr_tag);
void prefetch_write_helper(long int curr_set, long int curr_tag);

void prefetch_read(long int curr_set, long int curr_tag, long int new_set, long int new_tag);
void prefetch_write(long int curr_set, long int curr_tag, long int new_set, long int new_tag);
