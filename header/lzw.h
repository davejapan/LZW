#define DEBUG

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<errno.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdbool.h>
#include<sys/sendfile.h>

//encoder function definition
int lzw_encoder(char *buffer, size_t size, FILE *fp);
//decoder function definition
int lzw_decoder(char *buffer, size_t size, FILE *fp);
