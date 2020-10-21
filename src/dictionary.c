#include "lzw.h"
#include "dictionary.h"

int initialize_dictionary(Dictionary *dict, size_t size)
{
  //sanity check
  if(NULL == dict || size <= 0xFF)
  {
    return -1;
  }

  dict->total_size = size;
  dict->current_pos = 0;
  dict->ptr = (Dictionary_entry*) malloc(sizeof(Dictionary_entry) * size);
  if(dict->ptr == NULL)
  {
    return -2;
  }

  while(dict->current_pos <= 0xFF)
  {
    dict->ptr[dict->current_pos].str = (char*) malloc(sizeof(char));
    if(dict->ptr[dict->current_pos].str == NULL)
    {
      return dict->current_pos;
    }
    dict->ptr[dict->current_pos].str[0] = (unsigned char)dict->current_pos;
    dict-> ptr[dict->current_pos].size = 1;

    dict->current_pos++;
  }

  return 0;
}

int get_string_from_dict(Dictionary *dict, char **str, size_t *size, size_t pos)
{
  //sanity check
  if(NULL == dict || NULL == str)
  {
    return -1;
  }

  if(pos >= dict->current_pos)
  {
    // printf("dggrf");
    // fflush(stdout);

    return -2;
  }
  // printf("INSIDE");
  *str = dict->ptr[pos].str;
  *size = dict->ptr[pos].size;

  return 0;
}

int lookup_into_dict(Dictionary *dict, char *str, size_t size, int *pos)
{
  //initialize pos pointer for ENTRY NOT FOUND value
  *pos = -1;

  //sanity check
  if(NULL == dict || NULL == str || 0 == size || NULL == pos)
  {
    return -1;
  }

  //initialize runner to 256- position in dictionary from where encoder created entries start
  size_t pointer = 0;

  //iterate until end of valid entries
  while(pointer < dict->current_pos)
  {
    //first compare size for faster lookup
    if(size == dict->ptr[pointer].size)
    {
      size_t i = 0;
      //match dictionary entry with string
      while(i < size)
      {
        if(dict->ptr[pointer].str[i] != str[i])
        {
          break;
        }
        i++;
      }

      if(i==size)
      {
        *pos = pointer;
        return 0;
      }
    }
    pointer++;
  }

  return 1;
}

int add_into_dict(Dictionary *dict, char *str, size_t str_size)
{
  //sanity check
  if(NULL == dict || NULL == str || 0 == str_size)
  {
    return -1;
  }

  if(dict->current_pos == dict->total_size)
  {
      return -2;
  }

  dict->ptr[dict->current_pos].str = (char*) malloc(sizeof(char)*str_size);
  if(dict->ptr[dict->current_pos].str == NULL)
  {
    return dict->current_pos;
  }

  dict-> ptr[dict->current_pos].size = str_size;
  while(str_size-- > 0)
  {
    dict->ptr[dict->current_pos].str[str_size] = str[str_size];
    // printf("%x ", (unsigned char)dict->ptr[dict->current_pos].str[str_size]);
  }
  dict->current_pos++;

  return 0;
}

int destruct_dictionary(Dictionary *dict)
{
  //  sanity check
  if(NULL == dict)
  {
    return -1;
  }

  while(dict->total_size-- >0)
  {
      free(dict->ptr[dict->total_size].str);
  }
  free(dict->ptr);

  return 0;
}

#ifdef DEBUG
int print_dictionary(Dictionary *dict)
{
  if(NULL == dict)
  {
    return -1;
  }

  size_t i = 0;

  printf("\n\nDictionary Size- %ld \nCurrent Position- %ld", dict->total_size, dict->current_pos);
  printf("\n\n==========================================================================================================\n");
  while(i< dict->current_pos)
  {
    printf("| %ld\t= ", i);
    size_t j = 0;
    while(j < dict->ptr[i].size)
    {
      printf("%c", dict->ptr[i].str[j]);
      j++;
    }
    printf("\t%ld\t", dict-> ptr[i].size);
    i++;
    if(i%8==0)
    {
      printf("|\n");
    }
  }
  printf("========================================================================================================\n");

  return 0;
}
#endif
