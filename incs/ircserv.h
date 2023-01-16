/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pblagoje <pblagoje@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 14:11:27 by pblagoje          #+#    #+#             */
/*   Updated: 2023/01/16 18:41:23 by pblagoje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_H
# define IRCSERV_H

// MACROS

#define SA          struct sockaddr_in

#define MAX_BUFFER  4096

#define SERV_PWD    "irc123"
#define SERV_HOST   "127.0.0.1"

// C++ LIBRARIES

#include <cstring>
#include <cstdlib>
#include <iostream>

// SOCKET NETWORKING LIBRARIES

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#endif
