#include "lzw.h"
#include "dictionary.h"

#define WRITE_CACHE_SIZE 1020

//LZW encoder function
int lzw_encoder(char *buffer, size_t size, FILE *out_fp)
{
  int ret = 0;
  char write_cache[WRITE_CACHE_SIZE] = {};

  //sanity check
  if(buffer == NULL || size == 0 || out_fp == NULL)
  {
    ret = -1;
  }

  //dictionary instance to be used for encoding
  Dictionary *dictionary = (Dictionary*) malloc(sizeof(Dictionary));
  if(dictionary == NULL)
  {
    return -2;
  }

  //initialize dictionary data structure and initial values
  ret = initialize_dictionary(dictionary, DICTIONARY_SIZE);
  if(ret != 0)
  {
    return ret;
  }

  //iterate over input data
  size_t i = 0;
  size_t write_cache_val_count = 0;

  while( i< size)
  {
    //Find longest available substring starting from current pointer in dictionary
    char *str = &buffer[i];

    size_t str_size = 1;
    int pos = -1;
    int last_pos = -1;

    while(!lookup_into_dict(dictionary, str, str_size, &pos))
    {
      last_pos = pos;
      if(i+str_size == size)
      {
        str_size++;
        break;
      }
      else
      {
        str_size++;
      }
    }

    //Add substring + next char into disctionary, encode substring, move onto next character
    if(i+str_size != size)
    {
      ret = add_into_dict(dictionary, str, str_size);
    }
    str_size--;
    i+=str_size;

    if(write_cache_val_count%2 == 0)
    {
      write_cache[(write_cache_val_count * 3)/2] = (unsigned char)(last_pos & 0xFF);
      write_cache[((write_cache_val_count * 3)/2)+1] = (unsigned char)((last_pos & 0xF00)>>8);
    }
    else
    {
      write_cache[(write_cache_val_count * 3)/2] |= (unsigned char)((last_pos & 0xF)<<4);
      write_cache[((write_cache_val_count * 3)/2)+1] = (unsigned char)((last_pos & 0xFF0)>>4);
    }

    if(write_cache_val_count++ == ((WRITE_CACHE_SIZE*2)/3)- 1)
    {
      fwrite(write_cache, sizeof(char), WRITE_CACHE_SIZE, out_fp);
      write_cache_val_count = 0;
    }
  }
  fwrite(write_cache, sizeof(char), ((write_cache_val_count*3)/2)+(write_cache_val_count%2), out_fp);

  //dictionary data structure cleanup
  ret = destruct_dictionary(dictionary);
  if(ret != 0)
  {
    return ret;
  }

  return 0;
}
