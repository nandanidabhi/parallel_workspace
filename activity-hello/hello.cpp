#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void checkHostName(int hostname){
  if(hostname == -1){
    perror("gethostname");
    exit(1);}
}

void checkIPbuffer(char *IPbuffer){
  if(NULL == IPbuffer){
    perror("inet_ntoa");
    exit(1);
  }}
void checkHostEntry(struct hostent * hostentry){
  if(hostentry == NULL){
    perror("gethostbyname");
    exit(1);}}

int main () {

  char hostbuffer[256];
  char *IPbuffer;
  struct hostent *host_entry;
  int hostname;
  hostname = gethostname(hostbuffer, sizeof(hostbuffer));
  checkHostName(hostname);

  host_entry = gethostbyname(hostbuffer);
  checkHostEntry(host_entry);

  IPbuffer = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));

  printf("Hostname: %s\n", hostbuffer);
  printf("Host IP: %s", IPbuffer);

  return 0;
}


