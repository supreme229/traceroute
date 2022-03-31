// Bartosz Janikowski, 315489

#include "tools.h"

/* tools.c
Important functions that i used in my program. Didn't know where to fit them so i decided to put them in seprate .c file.
*/

u_int16_t compute_icmp_checksum(const void *buff, int length)
{
	u_int32_t sum;
	const u_int16_t *ptr = buff;
	assert(length % 2 == 0);
	for (sum = 0; length > 0; length -= 2)
		sum += *ptr++;
	sum = (sum >> 16) + (sum & 0xffff);
	return (u_int16_t)(~(sum + (sum >> 16)));
}

void status_print(int status, long int time, int packet_counter)
{
	switch (status)
	{
	case NO_RESPONSE:
		printf("*");
		break;
	case COLLECTED_ALL_PACKETS:
		printf("%ldms", (time / packet_counter));
		break;
	case TIMEOUT:
		printf("???");
	default:
		break;
	}

	printf("\n");
}

bool argument_validation(char *argv[], int argc)
{
	if (argc != 2)
	{
		return false;
	}

	unsigned string_length = strlen(argv[1]);

	if (string_length < 7)
	{
		return false;
	}

	if (argv[1][0] < 48 || argv[1][0] > 57 || argv[1][string_length - 1] < 48 || argv[1][string_length - 1] > 57)
	{
		return false;
	}

	int number_counter = 1;
	int dot_counter = 0;

	for (unsigned i = 1; i < string_length; i++)
	{
		if ((argv[1][i] < 48 || argv[1][i] > 57) && argv[1][i] != '.')
		{
			return false;
		}

		if (argv[1][i] == '.')
		{
			dot_counter++;
			if (number_counter == 0 || number_counter > 3)
			{
				return false;
			}
			number_counter = 0;
		}
		else
		{
			number_counter++;
		}
		if (dot_counter > 3)
		{
			return false;
		}
	}

	return true;
}
