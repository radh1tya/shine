#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/select.h>

/* tcp stuff */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>


#define h_addr h_addr_list[0]

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
    nickname[strcspn(nickname, "\n")] = '\0';
    snprintf(irc_nickname, 50, "NICK %s\r\n", nickname);

    char username[15];
    char irc_username[50];

    printf("username:\n"); 
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    char realname[15];
    printf("realname:\n");
    fgets(realname, sizeof(realname), stdin);
    realname[strcspn(realname, "\n")] = '\0';

    snprintf(irc_username, 50, "USER %s 0 * :%s\r\n", username, realname);

    /* send nickname & username */
    write(client_socket, irc_nickname, strlen(irc_nickname));
    write(client_socket, irc_username, strlen(irc_username));
}


void
motd(void)
{
    printf("welcome to shine irc\n");
    printf("*** this is a testing setup ***\n");
}

void
handle_connection(void) {
    char buffer[512];
    char shell[128];
    fd_set readfds;
    int max_fd;

    while (1) {
        FD_ZERO(&readfds);
        /* lihatin socket server */
        FD_SET(client_socket, &readfds);
        /* lihatin input pengguna */
        FD_SET(STDIN_FILENO, &readfds);
        max_fd = client_socket > STDIN_FILENO ? client_socket : STDIN_FILENO;

        printf("shine> ");
        fflush(stdout);
        if (select(max_fd + 1, &readfds, NULL, NULL, NULL) < 0) {
            perror("select gagal");
            return;
        }

        if (FD_ISSET(client_socket, &readfds)) {
            int bytes = read(client_socket, buffer, sizeof(buffer) - 1);
            if (bytes <= 0) {
                printf("koneksi terputus\n");
                return;
            }
            buffer[bytes] = '\0';
            printf("%s", buffer);

            /* void ping_pong(void) */

            if (strncmp(buffer, "PING ", 5) == 0) {
                char pong[512];
                snprintf(pong, sizeof(pong), "PONG %s", buffer + 5);
                write(client_socket, pong, strlen(pong));
            }
        }

        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            fgets(shell, sizeof(shell), stdin);
            shell[strcspn(shell, "\n")] = '\0';
            printf("debug: %s\n", shell);

            if(strncmp(shell, "/join ", 6) == 0) {
                char command[512];
                snprintf(command, sizeof(command), "JOIN %s\r\n", shell+6);
                write(client_socket, command, strlen(command));
            } else if (strcmp(shell, "/quit") == 0) {
                return;
            }
        }
    }
}
int
main(void)
{
    motd();

    char shell[128];

    while(1) {
        printf("shine> ");
        fgets(shell, sizeof(shell), stdin);
        shell[strcspn(shell, "\n")] = '\0';

        if(strcmp(shell, "c") == 0) {
            connect_domain();
            identity();
            handle_connection();
        }
    }

    return 0;
}
