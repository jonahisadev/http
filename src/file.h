#ifndef HTTP_FILE_H
#define HTTP_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* read_file(char* request_path, int* len);

#endif // HTTP_FILE_H