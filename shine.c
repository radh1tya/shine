#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>

/* tcp stuff */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

int client_socket;

void
connect_domain(void)
{
    char nama_server[50];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

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
}

void
identity(void)
{
    char nickname[15];
    char irc_nickname[50];

    printf("nickname:\n");
    fgets(nickname, sizeof(nickname), stdin);
    snprintf(irc_nickname, 50, "NICK %s\r\n", nickname);

    char username[15];
    char irc_username[50];

    printf("username:\n");
    
    fgets(username, sizeof(username), stdin);
    snprintf(irc_username, 50, "USER %s 0 * :linux\r\n", username);

    /* send nickname & username */
    write(client_socket, irc_nickname, strlen(irc_nickname));
    write(client_socket, irc_username, strlen(irc_username));
}

int
main(void)
{
    connect_domain();
    identity();
    return 0;
}
