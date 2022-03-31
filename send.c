// Bartosz Janikowski, 315489


/* send.c
Majority of the code is from the lecture page.
*/


#include "send.h"

int send_packets(int sockfd, uint16_t icd_id, int icd_seq, int ttl, const char *ip)
{
    struct sockaddr_in recipient;
    bzero(&recipient, sizeof(recipient));
    recipient.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &recipient.sin_addr);

    struct icmp header;
    header.icmp_type = ICMP_ECHO;
    header.icmp_code = 0;
    header.icmp_hun.ih_idseq.icd_id = icd_id;
    header.icmp_hun.ih_idseq.icd_seq = icd_seq;
    header.icmp_cksum = 0;
    header.icmp_cksum = compute_icmp_checksum(
        (u_int16_t *)&header, sizeof(header));

    int sockopt = setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));
    if (sockopt < 0)
    {
        fprintf(stderr, "setsockopt error: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    ssize_t bytes_sent = sendto(
        sockfd,
        &header,
        sizeof(header),
        0,
        (struct sockaddr *)&recipient,
        sizeof(recipient));
    if (bytes_sent < 0)
    {
        fprintf(stderr, "sendto error: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}