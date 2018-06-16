#include "message.h"

Header* parse_header(char* line) {
	Header* h = malloc(sizeof(Header));

	char lex[256];
	int lexi = 0;
	int i = 0;
	int section = 1;

	resetLex:
	memset(lex, '\0', 256);
	lexi = 0;

	while (line[i] != ' ' && line[i] != '\0') {
		lex[lexi++] = line[i++];
	}
	lex[lexi] = '\0';

	if (section == 1) {
		h->type = strdup(lex);
	} else if (section == 2) {
		h->file = strdup(lex);
	} else if (section == 3) {
		h->version = strdup(lex);
	}
	section++;

	if (section == 4) {
		return h;
	}

	i++;
	goto resetLex;
}

Message* parse_message(char* buf) {
	Message* msg = malloc(sizeof(Message));
	
	char lex[256];
	int lexi = 0;
	int i = 0;

	// Get header
	while (buf[i] != '\n') {
		lex[lexi++] = buf[i++];
	}
	lex[lexi] = '\0';
	Header* h = parse_header(lex);
	msg->header = h;

	return msg;
}

void print_message_header(Message* m) {
	Header* h = m->header;
	printf("%s %s %s\n", h->type, h->file, h->version);
}

void delete_header(Header* h) {
	free(h->type);
	free(h->file);
	free(h->version);
	free(h);
}