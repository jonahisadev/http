#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "message.h"
#include "file.h"

#define CONNECTIONS 10

void sendData(int handle, char* payload) {
	send(handle, payload, strlen(payload), 0);
	close(handle);
}

int main(int argc, char** argv) {
	// Variables
	unsigned short port;
	char buf[2048];
	struct sockaddr_in client;
	struct sockaddr_in server;
	int s;
	int ns;
	int c_len;

	// Get port
	port = (unsigned short) atoi(argv[1]);

	// Main Server
	s = socket(AF_INET, SOCK_STREAM, 0);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;
	bind(s, (struct sockaddr*)&server, sizeof(server));

	printf("Listening...\n");
	while (1) {
		// Listen
		listen(s, CONNECTIONS);

		// Accept connection
		c_len = sizeof(client);
		ns = accept(s, (struct sockaddr*)&client, (socklen_t*)&c_len);

		// Get info from client
		recv(ns, buf, sizeof(buf), 0);
		
		// Parse message into struct
		Message* msg = parse_message(buf);
		printf("Client request: "); print_message_header(msg);

		// This currently only supports GET requests
		// Send a 501 if anything else
		if (strcmp(msg->header->type, "GET")) {
			sendData(ns, "HTTP/1.1 501 Not Implemented\n\n501 Not Implemented");
			printf("Sent 501\n");
			goto end;
		}

		// Send data back
		int content_length = 0;
		char* contents;
		if (!strcmp(msg->header->file, "/")) {
			contents = read_file("index.html", &content_length);
		} else {
			contents = read_file(msg->header->file, &content_length);
		}

		// Send not found if there's no file
		if (contents == NULL) {
			sendData(ns, "HTTP/1.1 404 Not Found\n\n404 Not found!");
			printf("Sent 404\n");
			goto end;
		}

		// Otherwise, generate a response
		char* response = malloc(256 + content_length);
		sprintf(response, "HTTP/1.1 200 OK\nServer: JonaHTTP/0.1\nContent-Length: %d\n\n%s", 
			content_length, contents);
		printf("Sent 200\n");
		sendData(ns, response);
		free(response);

		end:
		delete_header(msg->header);
		free(msg);
	}

	close(s);

	printf("Server worked\n");

	return 0;
}