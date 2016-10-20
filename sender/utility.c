#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "utility.h"

int createSocket(int type, int aFm, short pNum, int bAdd, int nListen){
	struct sockaddr_in lAdd;
	int fd, rt, sz = sizeof(struct sockaddr_in);
	{
		int M, socType;
		M = -(type==1);
		socType = (SOCK_STREAM & M) | (SOCK_DGRAM & ~M);
		fd = socket(PF_INET, socType, 0);
	}
	if(fd==-1) exit_on_error("createSocket:socket");
	
	long state = 1;  // this has to be done before binding
 	if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &state, sizeof(int)) < 0) exit_on_error("setsockopt");
 	
	bzero(&lAdd, sz);
	lAdd.sin_family = aFm;
	lAdd.sin_port = htons(pNum);
	lAdd.sin_addr.s_addr = bAdd;

	rt = bind(fd, aCast(&lAdd), sz);
	if(rt==-1) exit_on_error("createSocket:bind");
	if(type==1) { rt = listen(fd, nListen);
				  if(rt==-1) exit_on_error("createSocket:listen"); }
	
	return fd;
}

void notify(char* str){
	char arg[100] = { "notify-send " };
	int rt = system(strncat(arg, str, 88));
	if(rt < 0) exit_on_error("notify-send");
}
