// Bartosz Janikowski, 315489

/* main.c
Just main, he runs the business.
*/

#include "receive.h"
#include "send.h"
#include "tools.h"

#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define MAX_TTL 30
#define PACKETS_SEND 3

int main(int argc, char *argv[])
{
    if (!argument_validation(argv, argc))
    {
        fprintf(stderr, "argument error!\n");
        return EXIT_FAILURE;
    }

    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0)
    {
        fprintf(stderr, "socket error: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    bool delivered = false;
    long int time = 0, start = 1000;
    uint16_t process_pid = getpid(), seq_id = 0;
    int packet_counter = 0;
    enum packet_status status;
    enum packet_type type;

    for (int ttl = 1; ttl <= MAX_TTL && !delivered; ttl++)
    {
        for (int i = 0; i < PACKETS_SEND; i++)
        {
            seq_id = (ttl - 1) * 3 + i;
            if (send_packets(sockfd, process_pid, (ttl - 1) * 3 + i, ttl, argv[1]) == EXIT_FAILURE)
            {
                return EXIT_FAILURE;
            }
        }

        printf("%d. ", ttl);

        char sender_ip_str[3][20];

        fd_set descriptors;
        FD_ZERO(&descriptors);
        FD_SET(sockfd, &descriptors);
        struct timeval tv;

        tv.tv_sec = 1;
        tv.tv_usec = 0;
        time = 0;

        status = GATHERING_PACKETS;

        packet_counter = 0;

        // Gathering our 3 packets...
        while (status == GATHERING_PACKETS)
        {
            int ready = select(sockfd + 1, &descriptors, NULL, NULL, &tv);

            // If we got packet to receive
            if (ready)
            {
                type = NOTHING;

                // Let's receive it!
                if (receive_packet(sockfd, &type, process_pid, seq_id, sender_ip_str[packet_counter]) == EXIT_FAILURE)
                {
                    return EXIT_FAILURE;
                }

                // We need to check if it's the packet we're looking for
                if (type != NOTHING)
                {
                    // Job done!
                    if (type == ECHO_REPLY)
                    {
                        delivered = true;
                    }

                    bool to_print = true;

                    // Printing ip addresses
                    for (int i = 0; i < packet_counter; i++)
                    {
                        if (strcmp(sender_ip_str[i], sender_ip_str[packet_counter]) == 0)
                        {
                            to_print = false;
                            break;
                        }
                    }

                    if (to_print)
                    {
                        printf("%s ", sender_ip_str[packet_counter]);
                    }

                    time += start - ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
                    packet_counter++;
                }
            }

            // Nothing else to receive
            if (ready == 0)
            {
                status = TIMEOUT;
                if (packet_counter == 0)
                {
                    status = NO_RESPONSE;
                }
                break;
            }

            // Oops...
            if (ready < 0)
            {
                fprintf(stderr, "select error: %s\n", strerror(errno));
                return EXIT_FAILURE;
            }

            // Job done, got all packets
            if (packet_counter == 3)
            {
                status = COLLECTED_ALL_PACKETS;
            }
        }
        // Printing time etc.
        status_print(status, time, packet_counter);
    }

    // All done
    return EXIT_SUCCESS;
}