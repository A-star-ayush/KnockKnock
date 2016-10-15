#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/ioctl.h>
#include "files.h"
#include "utility.h"

void sendfiles(int peer, char** files, int wait_time){
	
	{
    	int val = 0;
   		ioctl(peer, FIONBIO, &val);  // enabling BLOCKING mode I/O
   	}
    int rt;
/*
   	struct timeval timeout;      
    timeout.tv_sec = wait_time;
    timeout.tv_usec = 0;


    rt = setsockopt(peer, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
    if(rt < 0) exit_on_error("recv_broadcast: setsockopt");
*/
	while(*files){
        struct stat sb;
        int len = strlen(*files);
		if(stat(*files, &sb) < 0) exit_on_error("sendfiles: stat");
		
		if((sb.st_mode & S_IFMT) != S_IFREG){
			char str[len + 30];
			strcpy(str, *files);
			strcat(str, " : not a regular file");
			notify(str);
			++files;
		}

		// can add capability to pause and resume transfers  .. we should transfer file size as well to check
		// for completeness on the other side (like whether sufficient space is there or not)

		else{
			char msg[BUFSIZ];
			int sz = snprintf(msg, BUFSIZ, "%d:%s:%lld", len, *files, (long long)sb.st_size);  // note: lld for file size
			if(sz >= BUFSIZ){
				char str[len + 30];
				strcpy(str, *files);
				strcat(str, " : to large an argument");
				notify(str);
				++files;
				continue;
			}
			else{
				int rt = write(peer, msg, sz+1);
				if(rt < 0) exit_on_error("sendfiles: write_msg");
				if(rt == 0) exit_err("sendfiles: peer closed connection_msg");
				printf("Sn: %s\n", msg);

				int in = open(*files, O_RDONLY);
				if(in < 0) exit_on_error("sendfiles: open_in");

				sleep(1);  // give the peer sometime to parse the information

				char buf[BUFSIZ]; 
				while(1){
					rt = read(in, buf, BUFSIZ);
					if(rt < 0) exit_on_error("getfiles: read_in");
					if(rt == 0) break;  // EOF has reached
					rt = write(peer, buf, rt);
					if(rt < 0) exit_on_error("getfiles: write_peer_in");
					if(rt == 0) exit_on_error("getfiles: peer closed connection_in");	
				}
				
				char rply[10];
				rt = read(peer, rply, 10);
				if(rt < 0) exit_on_error("sendfiles: read_rply");
				if(rt == 0) exit_err("sendfiles: peer closed connection_rply");
				if(strcasecmp(rply, "GG")==0) { puts("Rc: GG"); ++files; }
				else exit_err("sendfiles: connection problem_GG");

				close(in);

			}
		}
	}

	rt = write(peer, "GG", 3);
	if(rt < 0) exit_on_error("sendfiles: write_GG");
	if(rt == 0) exit_err("sendfiles: peer closed connection_GG");
	puts("Sn: GG");
	sleep(1);
}