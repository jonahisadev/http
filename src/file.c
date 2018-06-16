#include "file.h"

char* read_file(char* request_path, int* len) {
	char path[256];
	sprintf(path, "web%s", request_path);

	FILE* file = fopen(path, "r");
	if (!file)
		return NULL;

	fseek(file, 0, SEEK_END);
	*len = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* buf = malloc(*len + 1);
	fread(buf, 1, *len, file);
	fclose(file);
	buf[*len] = '\0';

	return buf;
}