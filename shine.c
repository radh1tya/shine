#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>

/* tcp stuff */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

char nama_server[100];

void
connect_domain(void)
{
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(6667);

    printf("Masukkan nama server:\n");
    fgets(nama_server, sizeof(nama_server), stdin);

    nama_server[strcspn(nama_server, "\n")] = 0;

    struct hostent *server = gethostbyname(nama_server);
    if (server == NULL) {
        printf("Gagal resolve domain %s\n", nama_server);
        exit(1);
    }

    address.sin_addr.s_addr = *(unsigned long *)server->h_addr;

    if (connect(client_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        printf("Gagal konek ke server %s\n", nama_server);
        exit(1);
    }

    printf("Berhasil konek ke %s\n", nama_server);

    close(client_socket);
}

int
main(void)
{
    connect_domain();
    return 0;
}
