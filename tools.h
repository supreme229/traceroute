#ifndef TOOLS_H
#define TOOLS_H

#include <stdbool.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

enum packet_type
{
    ECHO_REPLY,    // delivered
    TIME_EXCEEDED, // ttl need to be increased
    NOTHING        // none of the above:)
};

/*
We hold here different status of single process of collecting packets.
*/
enum packet_status
{
    GATHERING_PACKETS,     // still gathering
    COLLECTED_ALL_PACKETS, // got 3 correct packets
    TIMEOUT,               // 1 second passed
    NO_RESPONSE            // didn't collect any packets
};

/*
Checksum function from lecture.
*/
u_int16_t compute_icmp_checksum(const void *buff, int length);

/*
Function to validate program arguments, so validating if we have IP and if it is correct.
*/
bool argument_validation(char *argv[], int argc);

void status_print(int status, long int time, int packet_counter);

#endif
