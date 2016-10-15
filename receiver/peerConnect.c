#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <strings.h>
#include <errno.h>
#include "utility.h"
#include "peerConnect.h"

int establishConnection(int tcp_fd, struct sockaddr_in* x){
	
	int rt = connect(tcp_fd, aCast(x), sizeof(struct sockaddr_in));
	if(rt < 0){
		perror("connect");
	  return 0;	
	}
	return 1;

}
