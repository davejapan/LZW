#define DICTIONARY_SIZE 4096

typedef struct dict_entry{
  char *str;
  size_t size;
}Dictionary_entry;

typedef struct dictionary{
  Dictionary_entry *ptr;
  size_t current_pos;
  size_t total_size;
} Dictionary;

int initialize_dictionary(Dictionary*, size_t);
int lookup_into_dict(Dictionary*, char*, size_t, int*);
int add_into_dict(Dictionary*, char*, size_t);
int get_string_from_dict(Dictionary *dict, char **str, size_t *size, size_t pos);
int destruct_dictionary(Dictionary*);

#ifdef DEBUG
int print_dictionary(Dictionary*);
#endif
