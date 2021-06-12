#define MODES 6
#define IP_ADDRSTRLEN 15
#define MAX_FILE_SIZE 10240

typedef struct historical{
    char ip[IP_ADDRSTRLEN];
    int port;
    int count;
    struct historical *next;
} historical;

char *remove_spaces(char *str);
int * clean(char *str);

void output();
int report();

void packetHandler(unsigned char *userData, const struct pcap_pkthdr* pkthdr, const unsigned char* packet);