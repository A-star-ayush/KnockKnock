#ifndef _BROADCAST_H
#define _BROADCAST_H

struct sockaddr_in* recv_broadcast(int sockfd, int wait_time);

#endif
