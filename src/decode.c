#include "lzw.h"
#include "dictionary.h"

#define WRITE_CACHE_SIZE 1024*1024*4

//LZW encoder function
int lzw_decoder(char *buffer, size_t size, FILE *out_fp)
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
  size_t write_cache_ptr = 0;

  char *str = NULL;
  char *curr_str = NULL;
  char *next_str = NULL;
  size_t curr_size = 0;
  size_t next_size = 0;

  bool flag = 1;

  // variables to hold current and next codes
  size_t curr_code;
  size_t next_code;

  // start with current code
  curr_code = (unsigned char)buffer[i];
  curr_code |= (((unsigned char)buffer[i+1] & 0xF)<<8);
  i= i+2;

  while( i< size)
  {
    next_code = 0;
    if(flag == 0)
    {
      next_code = (unsigned char)buffer[i];
      next_code |= (((unsigned char)buffer[i+1] & 0xF)<<8);
      i+=2;
    }
    else
    {
      next_code |= (((unsigned char)buffer[i-1] & 0xF0)>>4);
      next_code |= (((unsigned char)buffer[i])<<4);
      i++;
    }

    // decode the current code from dictionary
    get_string_from_dict(dictionary, &curr_str, &curr_size, curr_code);
    // allocate memory for an entry to be added to dictionary
    str = (char*) malloc(sizeof(char) * (curr_size+1));

    // copy decoded string one character at a time
    size_t j = 0;
    while(j < curr_size)
    {
      str[j] = curr_str[j];
      j++;
    }

    // lookup and append first character of next decoded string to generate new dictionary entry
    if(get_string_from_dict(dictionary, &next_str, &next_size, next_code) == -2)
    {
      str[curr_size] = curr_str[0];
    }
    else
    {
      str[curr_size] = next_str[0];
    }

    // check if newly generated entry exists in dictionary, if not add into it
    int pos = -1;
    if(lookup_into_dict(dictionary, str, curr_size+1, &pos))
      add_into_dict(dictionary, str, curr_size+1);

    // malloc clean up
    free(str);
    // print_dictionary(dictionary);

    //  write decoded string into file
    fwrite(curr_str, sizeof(char), curr_size, out_fp);

    //  assign 'current next' code to current code for next iteration
    curr_code = next_code;
    //  flip the flag at every iteration
    flag = !flag;
  }

  // Decode and write the last code ending the file
  get_string_from_dict(dictionary, &curr_str, &curr_size, curr_code);
  fwrite(curr_str, sizeof(char), curr_size, out_fp);

  //dictionary data structure cleanup
  ret = destruct_dictionary(dictionary);
  if(ret != 0)
  {
    return ret;
  }

  // return on success
  return 0;
}
