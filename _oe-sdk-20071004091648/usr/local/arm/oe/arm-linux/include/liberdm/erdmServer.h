#ifndef ERDMSERVER_H
#define ERDMSERVER_H
#include "erdm.h"
#include "erdminternal.h"

int dmGetServerPort(int *port);
int initServer(int *sockfd, int local);
int dmMessageParser(char *szMessage, uDmCommand *dmCommand);

#endif
