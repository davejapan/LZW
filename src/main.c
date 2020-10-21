#include "lzw.h"

int main(int argc, char *argv[])
{
  if(argc< 4)
  {
    printf("Format : lzw <encode/decode> <in_filename> <out_filename>\n");
    return -1;
  }

  if(strcmp(argv[1], "encode") != 0 && strcmp(argv[1], "decode") != 0)
  {
    printf("Format : lzw <encode/decode> <in_filename> <out_filename>\n");
    return -2;
  }

  FILE *in_fp = NULL;
  if((in_fp= fopen(argv[2], "r")) == NULL)
  {
    return -3;
  }

  FILE *out_fp = NULL;
  if((out_fp= fopen(argv[3], "w+")) == NULL)
  {
    return -4;
  }

  long lSize;
  char *buffer;
  size_t result;

  // obtain file size:
  fseek (in_fp , 0 , SEEK_END);
  lSize = ftell (in_fp);
  rewind(in_fp);

  // allocate memory to contain the whole file:
  if((buffer = (char*) malloc (sizeof(char)*lSize)) == NULL)
  {
    return -4;
  }

  // copy the file into the buffer:
  result = fread (buffer, 1, lSize, in_fp);
  if(result != lSize)
  {
    return -5;
  }

  int ret = 0;
  if(strcmp(argv[1], "encode") == 0)
  {
    ret = lzw_encoder(buffer, lSize, out_fp);
  }
  else
  {
    ret = lzw_decoder(buffer, lSize, out_fp);
  }

  //Input, outout file and buffer cleanup
  free(buffer);
  fclose(in_fp);
  fclose(out_fp);

  return ret;
}
