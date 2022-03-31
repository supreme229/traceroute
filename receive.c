// Bartosz Janikowski, 315489

/* receive.c
Here we receive the packets and verify if they are the correct ones.
*/

#include "receive.h"

int receive_packet(int sockfd, enum packet_type *value, int pid, int id_seq, char sender_ip[20])
{

	struct sockaddr_in sender;
	socklen_t sender_len = sizeof(sender);
	u_int8_t buffer[IP_MAXPACKET];
	u_int8_t *exc_buffer = buffer;

	ssize_t packet_len = recvfrom(sockfd, buffer, IP_MAXPACKET, 0, (struct sockaddr *)&sender, &sender_len);
	if (packet_len < 0)
	{
		fprintf(stderr, "recvfrom error: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	char sender_ip_str[20];
	inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str, sizeof(sender_ip_str));

	// Getting the ICMP_header
	struct ip *ip_header = (struct ip *)buffer;
	ssize_t ip_header_len = 4 * ip_header->ip_hl;
	struct icmphdr *icmp_header = (struct icmphdr *)(buffer + ip_header_len);

	// Below we figure out type of received packet
	switch (icmp_header->type)
	{
	case ICMP_TIME_EXCEEDED:;
		exc_buffer += 8;
		exc_buffer += 4 * ip_header->ip_hl;
		struct ip *ip_header_exc = (struct ip *)exc_buffer;
		exc_buffer += ip_header_exc->ip_hl * 4;
		struct icmp *icmp_exc = (struct icmp *)exc_buffer;

		// Realise if it's response to OUR paccket
		if (icmp_exc->icmp_id == pid && (icmp_exc->icmp_seq == id_seq || icmp_exc->icmp_seq == id_seq - 1 || icmp_exc->icmp_seq == id_seq - 2))
		{
			*value = TIME_EXCEEDED;
			strcpy(sender_ip, sender_ip_str);
		}
		break;

	case ICMP_ECHOREPLY:;
		// Realise if it's response to OUR paccket
		if (icmp_header->un.echo.id == pid && (icmp_header->un.echo.sequence == id_seq || icmp_header->un.echo.sequence == id_seq - 1 || icmp_header->un.echo.sequence == id_seq - 2))
		{
			*value = ECHO_REPLY;
			strcpy(sender_ip, sender_ip_str);
		}
		break;
	}

	return EXIT_SUCCESS;
}