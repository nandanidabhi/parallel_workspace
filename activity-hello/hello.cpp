#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main () {

  char hostName[256];
  char *IPAddress;
  struct hostent *host_entry;
  int hostname;
  hostname = gethostname(hostName, sizeof(hostName));

  host_entry = gethostbyname(hostName);

  IPAddress = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));

  printf("Hostname: %s\n", hostName);
  printf("Host IP: %s\n", IPAddress);

  return 0;
}


