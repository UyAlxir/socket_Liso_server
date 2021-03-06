#include "parse.h"
#include "log.h"

/**
* Given a char buffer returns the parsed request headers
*/

#define HEADER_LINES 19

Request * parse(char *buffer, int size, int socketFd,int *cur) {
  //Differant states in the state machine
	enum {
		STATE_START = 0, STATE_CR, STATE_CRLF, STATE_CRLFCR, STATE_CRLFCRLF
	};

	int i = 0, state;
	size_t offset = 0;
	char ch;
	char buf[BUF_SIZE];
	memset(buf, 0, BUF_SIZE);

	state = STATE_START;
	while (state != STATE_CRLFCRLF) {
		char expected = 0;

		if (i == size||i==BUF_SIZE)
			break;

		ch = buffer[i++];
		buf[offset++] = ch;

		switch (state) {
		case STATE_START:
		case STATE_CRLF:
			expected = '\r';
			break;
		case STATE_CR:
		case STATE_CRLFCR:
			expected = '\n';
			break;
		default:
			state = STATE_START;
			continue;
		}

		if (ch == expected)
			state++;
		else
			state = STATE_START;

	}
	cur[0]+=i;
    //Valid End State
	if (state == STATE_CRLFCRLF) {
		Request *request = (Request *) malloc(sizeof(Request));
        request->header_count=0;
        //TODO You will need to handle resizing this in parser.y
        request->headers = (Request_header *) malloc(sizeof(Request_header)*HEADER_LINES);
		set_parsing_options(buf, i, request);

		if (yyparse() == SUCCESS) {
            return request;
		}

	}

	// when it is falied to parse , clear the buffer of parser
	yylex_destroy();	
    //TODO Handle Malformed Requests
    // printf("Parsing Failed\n");
	Log_er("Parsing Failed");
	
	return NULL;
}

