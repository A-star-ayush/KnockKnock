#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <strings.h>
#include <errno.h>
#include "utility.h"
#include "peerConnect.h"

int establishConnection(int tcp_fd, struct sockaddr_in* x){

	struct sockaddr_in fadd;
	bzero(&fadd, sizeof(struct sockaddr_in));
	fadd.sin_family = AF_INET;
	inet_aton("127.0.0.1", &fadd.sin_addr);
	fadd.sin_port = htons(20000);

	int rt = connect(tcp_fd, aCast(&fadd), sizeof(struct sockaddr_in));
	if(rt < 0){
		perror("connect");
	  return 0;	
	}
	return 1;

}
