#ifndef RECEIVE_H
#define RECEIVE_H

#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>

#include "tools.h"

/*
Function to receive packets. Returns EXIT_SUCCESS or EXIT_FAILURE, simple as that. 
Important information given through args reference.
*/
int receive_packet(int sockfd, enum packet_type *value, int pid, int seq_id, char sender_ip[20]);

#endif
