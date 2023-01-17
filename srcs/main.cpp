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

int check_port(std::string port_str) {

    int port_nb;

    if (port_str.find_first_not_of("0123456789") != std::string::npos)
        return (-1);
    
    port_nb = atoi(port_str.c_str());

    if (port_nb < 1 || port_nb > 65535)
        return (-1);

    return (port_nb);
}

int main(int argc, char **argv) {

    SA                  serv_addr, cli_addr;
    int                 serv_sock, serv_port;
    int                 epoll_fd;
    int                 events_ready;
    struct epoll_event  event, cli_event, events[MAX_EVENTS];
    char                buff_recv[MAX_BUFFER], buff_send[MAX_BUFFER];

    // BASIC ERROR CHECKING

    if (argc != 3) {
        std::cerr << "Wrong number of arguments." << std::endl;
        std::cerr << "Usage: ./ircserv <port> <passwd>" << std::endl;
        return (-1);
    }

    serv_port = check_port(argv[1]);
    if (serv_port == -1) {
        std::cerr << "Invalid port number." << std::endl;
        return (-2);
    }

    // SERVER INITIAL SETUP

    if ((serv_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Error during listening socket creation." << std::endl;
        return (-3);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(serv_port);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Error while binding socket to server address." << std::endl;
        return (-4);
    }

    if (listen(serv_sock, 10) < 0) {
        std::cerr << "Error while listening on server address." << std::endl;
        return (-5);
    }

    // EPOLL INITIAL SETUP

    if ((epoll_fd = epoll_create1(0)) < 0) {
        std::cerr << "Error while creating epoll file descriptor." << std::endl;
        return (-6);
    }

    event.events = EPOLLIN;
    event.data.fd = serv_sock;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serv_sock, &event) < 0) {
        std::cerr << "Error while adding server socket to epoll instance." << std::endl;
        return (-7);
    } 

    // SERVER LISTENING LOOP

    while (true) {

        // Waiting for new EPOLLIN events from sockets in epoll instance

        events_ready = epoll_wait(epoll_fd, events, MAX_EVENTS, 0);

        // Processing sockets ready for reading returned by epoll instance

        for (int i = 0; i < events_ready; i++) {

            if (events[i].data.fd == serv_sock) {

                // If event is on the server socket, creating new client socket and adding it to epoll instance
                
                socklen_t   cli_addr_size = sizeof(cli_addr);

                memset(&cli_addr, 0, cli_addr_size);

                int cli_sock = accept(serv_sock, (struct sockaddr *)&cli_addr, &cli_addr_size);
                if (cli_sock < 0) {
                    std::cerr << "Error while accepting new connection to server." << std::endl;
                    return (-8);
                }

                cli_event.data.fd = cli_sock;
                cli_event.events = EPOLLIN;

                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, cli_sock, &cli_event) < 0) {
                    std::cerr << "Error while adding new client socket to epoll instance." << std::endl;
                    return (-9);
                }
            }
            else {

                // If event is on a client socket, read input data sent by client

                memset(&buff_recv, 0, MAX_BUFFER);

                if (recv(events[i].data.fd, buff_recv, MAX_BUFFER, 0) == 0) {

                    // If connection has been closed by client, remove fd from epoll instance and close it

                    std::cout << "Connection with client " << events[i].data.fd << " has terminated, closing socket now..." << std::endl;
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                    close(events[i].data.fd);
                }
                else {

                    // If data has been received from client, process it accordingly

                    std::cout << "Client says: " << buff_recv << std::endl;

                    // Sending data to client

                    memset(&buff_send, 0, MAX_BUFFER);
                    strcpy(buff_send, "Hi this is the server\n");
                    if (send(events[i].data.fd, buff_send, MAX_BUFFER, 0) < 0) {
                        std::cerr << "Error while sending data to client." << std::endl;
                        return (-10);
                    }

                    // CLIENT MESSAGE PARSING TO IMPLEMENT HERE
                }
            }
        }
    }

    close(serv_sock);

    return (0);
}
