#ifndef SEND_H
#define SEND_H

#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "tools.h"

/*
Function to send packets, based on lecture.
*/
int send_packets(int sockfd, uint16_t icd_id, int icd_seq, int ttl, const char *ip);

#endif