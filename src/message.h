#ifndef HTTP_MESSAGE_H
#define HTTP_MESSAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Header {
	char* type;
	char* file;
	char* version;
} Header;

typedef struct Message {
	Header* header;
	// Add more
} Message;

Header* parse_header(char* line);
Message* parse_message(char* buf);
void print_message_header(Message* m);

void delete_header(Header* h);

#endif // HTTP_MESSAGE_H