#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include "utility.h"
#include "files.h"

void randString(char* str, int len){
	static char set[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	srand(time(NULL));
	int i;
	str[0]='_';
	for(i=1;i<len;++i)
		str[i] = set[(int)(((double)rand()/(RAND_MAX+1u))*36)];
}

void getfiles(int peer, int wait_time){

    {
    	int val = 0;
   		ioctl(peer, FIONBIO, &val);  // enabling BLOCKING mode I/O
   	}

   /*	struct timeval timeout;      
    timeout.tv_sec = wait_time;
    timeout.tv_usec = 0;
    
    int rt = setsockopt(peer, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
    if(rt < 0) exit_on_error("recv_broadcast: setsockopt");
*/
	while(1){
		char msg[BUFSIZ];
		int rt;
		rt = read(peer, msg, BUFSIZ);
		if(rt < 0) exit_on_error("getfiles: read_msg");
		if(rt == 0) exit_err("getfiles: peer closed connection_msg");
		
		if(strcasecmp(msg, "GG")==0) { puts("Rc: GG"); close(peer); break; }
		char* ptr;
		long fname_len = strtol(msg, &ptr, 10);
		char fname[fname_len+1];
		strncpy(fname, ptr+1, fname_len);  // +1 to skip the ':'
		fname[fname_len] = '\0';
		long long fsize = strtol(ptr+fname_len+2, &ptr, 10);
		if((*ptr)!='\0') exit_err("getfiles: endpoint mismatch_msg");
		printf("Rc: %ld:%s:%lld\n", fname_len, fname, fsize);

		char name[fname_len+6];  // +5 incase we have to add a random string at the end and +1 for '\0'
		strcpy(name, fname);
		
		// check for enough space is left out for now
		int out = 0;
		
		while(1){
			static int turns = 5;
			if(!turns) exit_err("getfiles: Could not obtain a new file_out");
			out = open(name, O_RDWR | O_CREAT | O_EXCL, 0666);
			if(out < 0){
				if(errno == EEXIST){
					strcpy(name, fname);
					char append[5];
					randString(append, 4);
					append[4] = '\0';
					strcat(name, append);
					--turns;
					continue;
				}
				else exit_on_error("getfiles: open_out");
			}
			break;
		}
		
		char msg[fname_len+14] = "\"Receiving: ";
		notify(strcat(strcat(msg, *file),"\"");

		long long nbytes = 0;
		while(nbytes!=fsize){
			char buf[BUFSIZ]; 
			rt = read(peer, buf, BUFSIZ);
			if(rt < 0) exit_on_error("getfiles: read_out");
			if(rt == 0) exit_err("getfiles: peer closed connection_out");
			nbytes += rt;
			rt = write(out, buf, rt);
			if(rt < 0) exit_on_error("getfiles: write_out");
		}
		
		rt = write(peer, "GG", 3);
		puts("Sn: GG");
		if(rt < 0) exit_on_error("getfiles: write_GG");
		if(rt == 0) exit_err("getfiles: peer closed connection_GG");

		close(out);

	}
}

