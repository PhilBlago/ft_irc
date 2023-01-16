/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pblagoje <pblagoje@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 14:13:24 by pblagoje          #+#    #+#             */
/*   Updated: 2023/01/16 18:54:32 by pblagoje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ircserv.h"

// TO DO :
// - ADD CHECKS ON PORT AND PASSWD ARGUMENTS

int main(int argc, char **argv) {

    SA          serv_addr, cli_addr;
    int         serv_sock, cli_sock;
    int         serv_port;
    int         serv_passwd;
    socklen_t   cli_addr_size;
    char        buff_recv[MAX_BUFFER], buff_send[MAX_BUFFER];

    // BASIC ERROR CHECKING

    if (argc != 3) {
        std::cerr << "Wrong number of arguments." << std::endl;
        std::cerr << "Usage: ./ircserv <port> <passwd>" << std::endl;
        return (1);
    }

    serv_port = atoi(argv[1]);
    serv_passwd = atoi(argv[2]);

    // SERVER SETUP

    if ((serv_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Error during socket creation." << std::endl;
        return (2);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(serv_port);
    serv_addr.sin_addr.s_addr = inet_addr(SERV_HOST);

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Error while binding socket to server address." << std::endl;
        return (3);
    }

    if (listen(serv_sock, 10) < 0) {
        std::cerr << "Error while listening on server address." << std::endl;
        return (4);
    }

    // SERVER LISTENING LOOP

    while (1) {

        // Accepting new client connection and storing client address information

        cli_addr_size = sizeof(cli_addr);
        memset(&cli_addr, 0, cli_addr_size);
        
        if ((cli_sock = accept(serv_sock, (struct sockaddr *)&cli_addr, &cli_addr_size)) < 0) {
            std::cerr << "Error while accepting new connection to server." << std::endl;
            return (5);
        }

        // Reading data received from client

        memset(&buff_recv, 0, MAX_BUFFER);
        if (recv(cli_sock, buff_recv, MAX_BUFFER, 0) < 0) {
            std::cerr << "Error while receiving data from client." << std::endl;
            return (6);
        }
        
        std::cout << "Client says: " << buff_recv << std::endl;

        // Sending data to client

        memset(&buff_send, 0, MAX_BUFFER);
        strcpy(buff_send, "Hi this is the server\n");
        if (send(cli_sock, buff_send, MAX_BUFFER, 0) < 0) {
            std::cerr << "Error while send data to client." << std::endl;
            return (7);
        }

        // Closing current client socket (forbidden function?)

        close(cli_sock);
    }

    return (0);
}
