#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>

int main (int argc, char *argv[]) {
	int					g;
	char				ip[INET6_ADDRSTRLEN];
	struct addrinfo		hints, *ai, *ai0;

	if (argc < 2) {
		printf("Usage: %s <hostname> ...\n", argv[0]);
		return 1;
	}

	for (int i = 1; i < argc; i++) {
		memset(&hints, 0, sizeof(hints));
		hints.ai_family		= PF_UNSPEC;
		hints.ai_socktype	= SOCK_STREAM;

		if ((g = getaddrinfo(argv[i], "80", &hints, &ai0)) != 0) {
			printf("Unable to look up %s: %s\n", argv[i], gai_strerror(g));
		} else {
			for (ai = ai0; ai != NULL; ai = ai->ai_next) {
				switch (ai->ai_family) {
					case AF_INET:
						inet_ntop(AF_INET, &(((struct sockaddr_in *)ai->ai_addr)->sin_addr), ip, INET_ADDRSTRLEN);
						printf("%s IPV4 %s\n", argv[i], ip);
						break;

					case AF_INET6:
						inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)ai->ai_addr)->sin6_addr), ip, INET6_ADDRSTRLEN);
						printf("%s IPV6 %s\n", argv[i], ip);
						break;

					default:
						printf("Unknown AF for %s\n", argv[i]);
				}
			}
			freeaddrinfo(ai0);
		}
	}
	return 0;
}
