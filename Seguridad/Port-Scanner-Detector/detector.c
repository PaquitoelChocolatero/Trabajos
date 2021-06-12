#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>

#include <arpa/inet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>

#include "IP2Location-C-Library-master/libIP2Location/IP2Location.h"
#include "detector.h"

//Color definitions for printing
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_ORANGE "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[36m"

int selected[MODES]; //Bitmap with the possible modes

//Lists with ips registered in this mode
historical *tcp_syn_list;
historical *tcp_connect_list;
historical *tcp_fin_list;
historical *tcp_null_list;
historical *tcp_xmas_list;
historical *udp_list;

//Object to locate the ip
IP2Location *IP2LocationObj;
IP2LocationRecord *record;

int main(int argc, char **argv)
{
    //Check for correct usage and file type
    if (argc != 2)
    {
        printf("Usage: ./detector <pcap-file>\n");
        return -1;
    }
    char *ext = strrchr(argv[1], '.');
    if ((ext == NULL) || (strcmp(ext + 1, "pcap")))
    {
        printf("Error: File must be pcap\n");
        return -1;
    }

    //Detection mode
    fprintf(stdout, "%s%s%s%s", ANSI_COLOR_BLUE, ":: ", ANSI_COLOR_RESET, "6 Detection modes.\n 1  TCP SYN\n 2  TCP CONNECT\n 3  TCP FIN\n 4  TCP NULL\n 5  TCP XMAS\n 6  UDP\n");
    fprintf(stdout, "%s%s%s%s", ANSI_COLOR_BLUE, "==> ", ANSI_COLOR_RESET, "Desired mode: (eg: '1 6 2 4', '3-6', '^5')\n");
    fprintf(stdout, "%s%s%s", ANSI_COLOR_BLUE, "==> ", ANSI_COLOR_RESET);

    //Parsing selected modes
    char *input;
    char modes[20];
    fgets(modes, 20, stdin);
    strcpy(modes, remove_spaces(modes));

    if ((input = strrchr(modes, '^')) != NULL)
    {
        if (atoi(input + 1) > MODES)
        {
            printf("Error: %d is not a correct mode\n", atoi(input + 1));
            return -1;
        }
        for (int i = 0; i < atoi(input + 1); i++)
            selected[i] = 1;
    }
    else if ((input = strrchr(modes, '-')) != NULL)
    {
        if (atoi(input + 1) > MODES)
        {
            printf("Error: %d is not a correct mode\n", atoi(input + 1));
            return -1;
        }
        if (atoi(input + 1) < atoi(input - 1))
        {
            printf("Error: %d is bigger than %d\n", atoi(input - 1), atoi(input + 1));
            return -1;
        }
        for (int i = atoi(input - 1) - 1; i < atoi(input + 1); i++)
            selected[i] = 1;
    }
    else
    {
        int *p = clean(modes);
        for (int i = 0; i < MODES; i++)
        {
            if (*(p + i) > 0)
            {
                if (*(p + i) > MODES)
                {
                    printf("Error: %d is not a correct mode\n", *(p + i));
                    return -1;
                }
                selected[*(p + i) - 1] = 1;
            }
        }
    }

    //Open the traffic file
    char errorBuff[PCAP_ERRBUF_SIZE];
    pcap_t *file = pcap_open_offline(argv[1], errorBuff);

    if (file == NULL)
    {
        fprintf(stderr, "Error: %s\n", errorBuff);
        return -1;
    }

    if (pcap_loop(file, 0, packetHandler, NULL) < 0) {
        fprintf(stderr, "Error: %s\n", errorBuff);
        return -1;
    }

    //DISCLAIMER
    fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "\nDISCLAIMER\n" , ANSI_COLOR_RESET);
    printf("IPs are selected as potential scanners if more than 5 packets to different ports are found, more information in the report file.\n\n");

    output();

    IP2LocationObj = IP2Location_open("IP2Location-C-Library-master/data/IP-COUNTRY.BIN");
    report();

    return 0;
}

void packetHandler(unsigned char *userData, const struct pcap_pkthdr *pkthdr, const unsigned char *packet)
{
    const struct ether_header *ethernetHeader = (struct ether_header *)packet;
    const struct ip *ipHeader;
    const struct tcphdr *tcpHeader;
    const struct udphdr *udpHeader;
    char sourceIP[INET_ADDRSTRLEN];
    char destIP[INET_ADDRSTRLEN];
    unsigned int sourcePort;

    if (ntohs(ethernetHeader->ether_type) == ETHERTYPE_IP)
    {
        ipHeader = (struct ip *)(packet + sizeof(struct ether_header));
        inet_ntop(AF_INET, &(ipHeader->ip_src), sourceIP, INET_ADDRSTRLEN); //Atacker's IP
        inet_ntop(AF_INET, &(ipHeader->ip_dst), destIP, INET_ADDRSTRLEN); //Atacker's IP

        //If TCP
        if (ipHeader->ip_p == IPPROTO_TCP)
        {
            tcpHeader = (struct tcphdr *)(packet + sizeof(struct ether_header) + sizeof(struct ip));
            sourcePort = ntohs(tcpHeader->source);

            //If syn is activated and TCP SYN is selected
            if (tcpHeader->th_flags & TH_SYN && selected[0] == 1)
            {
                int found = 0;

                //If first element
                if(tcp_syn_list == NULL){
                    historical *first = (historical *) malloc(sizeof(historical));
                    strcpy(first->ip, sourceIP);
                    first->port = sourcePort;
                    first->count = 1;
                    first->next = NULL;

                    tcp_syn_list = first;
                }

                //Aux list to go through the list
                historical *aux = tcp_syn_list;

                //Go though the list searching for the ip
                while(aux != NULL)
                {
                    if (strcmp(aux->ip, sourceIP) == 0){
                        //If scanning ports
                        if(aux->port != sourcePort) aux->count++;
                        
                        found = 1;
                    }
                    aux = aux->next;
                }

                //If source not on list add it
                if(found == 0){
                    historical *next = (historical *) malloc(sizeof(historical));
                    strcpy(next->ip, sourceIP);
                    next->port = sourcePort;
                    next->count = 1;
                    next->next = tcp_syn_list;
                    tcp_syn_list = next;
                }
            }

            //TCP CONNECT
            if ((tcpHeader->th_flags & (TH_RST | TH_ACK)) == (TH_RST | TH_ACK) && selected[1] == 1)
            {
                int found = 0;

                //If first element
                if(tcp_syn_list == NULL){
                    historical *first = (historical *) malloc(sizeof(historical));
                    strcpy(first->ip, destIP);
                    first->port = sourcePort;
                    first->count = 1;
                    first->next = NULL;

                    tcp_connect_list = first;
                }

                //Aux list to go through the list
                historical *aux = tcp_connect_list;

                //Go though the list searching for the ip
                while(aux != NULL)
                {
                    if (strcmp(aux->ip, destIP) == 0){
                        //If scanning ports
                        if(aux->port != sourcePort) aux->count++;
                        
                        found = 1;
                    }
                    aux = aux->next;
                }

                //If source not on list add it
                if(found == 0){
                    historical *next = (historical *) malloc(sizeof(historical));
                    strcpy(next->ip, destIP);
                    next->port = sourcePort;
                    next->count = 1;
                    next->next = tcp_connect_list;
                    tcp_connect_list = next;
                }
            }

            //TCP XMAS🎅🎄
            if ((tcpHeader->th_flags & (TH_FIN | TH_PUSH | TH_URG)) == (TH_FIN | TH_PUSH | TH_URG) && selected[4] == 1)
            {
                int found = 0;

                //If first element
                if(tcp_xmas_list == NULL){
                    historical *first = (historical *) malloc(sizeof(historical));
                    strcpy(first->ip, sourceIP);
                    first->port = sourcePort;
                    first->count = 1;
                    first->next = NULL;

                    tcp_xmas_list = first;
                }

                //Aux list to go through the list
                historical *aux = tcp_xmas_list;

                //Go though the list searching for the ip
                while(aux != NULL)
                {
                    if (strcmp(aux->ip, sourceIP) == 0){
                        //If scanning ports
                        if(aux->port != sourcePort) aux->count++;
                        
                        found = 1;
                    }
                    aux = aux->next;
                }

                //If source not on list add it
                if(found == 0){
                    historical *next = (historical *) malloc(sizeof(historical));
                    strcpy(next->ip, sourceIP);
                    next->port = sourcePort;
                    next->count = 1;
                    next->next = tcp_xmas_list;
                    tcp_xmas_list = next;
                }
            }

            //TCP FIN
            if (tcpHeader->th_flags == TH_FIN && selected[2] == 1)
            {
                int found = 0;

                //If first element
                if(tcp_fin_list == NULL){
                    historical *first = (historical *) malloc(sizeof(historical));
                    strcpy(first->ip, sourceIP);
                    first->port = sourcePort;
                    first->count = 1;
                    first->next = NULL;

                    tcp_fin_list = first;
                }

                //Aux list to go through the list
                historical *aux = tcp_fin_list;

                //Go though the list searching for the ip
                while(aux != NULL)
                {
                    if (strcmp(aux->ip, sourceIP) == 0){
                        //If scanning ports
                        if(aux->port != sourcePort) aux->count++;
                        
                        found = 1;
                    }
                    aux = aux->next;
                }

                //If source not on list add it
                if(found == 0){
                    historical *next = (historical *) malloc(sizeof(historical));
                    strcpy(next->ip, sourceIP);
                    next->port = sourcePort;
                    next->count = 1;
                    next->next = tcp_fin_list;
                    tcp_fin_list = next;
                }
            }

            //TCP NULL
            if (tcpHeader->th_flags == 0 && selected[3] == 1)
            {
                int found = 0;

                //If first element
                if(tcp_null_list == NULL){
                    historical *first = (historical *) malloc(sizeof(historical));
                    strcpy(first->ip, sourceIP);
                    first->port = sourcePort;
                    first->count = 1;
                    first->next = NULL;

                    tcp_null_list = first;
                }

                //Aux list to go through the list
                historical *aux = tcp_null_list;

                //Go though the list searching for the ip
                while(aux != NULL)
                {
                    if (strcmp(aux->ip, sourceIP) == 0){
                        //If scanning ports
                        if(aux->port != sourcePort) aux->count++;
                        
                        found = 1;
                    }
                    aux = aux->next;
                }

                //If source not on list add it
                if(found == 0){
                    historical *next = (historical *) malloc(sizeof(historical));
                    strcpy(next->ip, sourceIP);
                    next->port = sourcePort;
                    next->count = 1;
                    next->next = tcp_null_list;
                    tcp_null_list = next;
                }
            }
        }

        //If UDP
        else if (ipHeader->ip_p == IPPROTO_UDP && selected[5] == 1)
        {
            udpHeader = (struct udphdr *)(packet + sizeof(struct ether_header) + sizeof(struct ip));
            sourcePort = ntohs(udpHeader->source);

            int found = 0;

            //If first element
            if(udp_list == NULL){
                historical *first = (historical *) malloc(sizeof(historical));
                strcpy(first->ip, sourceIP);
                first->port = sourcePort;
                first->count = 1;
                first->next = NULL;

                udp_list = first;
            }

            //Aux list to go through the list
            historical *aux = udp_list;

            //Go though the list searching for the ip
            while(aux != NULL)
            {
                if (strcmp(aux->ip, sourceIP) == 0){
                    //If scanning ports
                    if(aux->port != sourcePort) aux->count++;
                    
                    found = 1;
                }
                aux = aux->next;
            }

            //If source not on list add it
            if(found == 0){
                historical *next = (historical *) malloc(sizeof(historical));
                strcpy(next->ip, sourceIP);
                next->port = sourcePort;
                next->count = 1;
                next->next = udp_list;
                udp_list = next;
            }
        }
    }
}

//Function to print the detected IPs
void output(){

    /*                                          TCP SYN                                                 */
    //IP List
    historical *aux = tcp_syn_list;
    int found = 0;
    
    //If mode is selected
    if(selected[0] == 1){
        //No IP
        if(aux == NULL){
            fprintf(stdout, "%s%s%s%s%s%s%s", ANSI_COLOR_RED, "[" , ANSI_COLOR_RESET, "-", ANSI_COLOR_RED,"] TCP SYN\n", ANSI_COLOR_RESET);
            found = 1;
        }

        //IPs
        while(aux != NULL)
        {
            //Valid IP
            if(aux->count > 5){
                if(found == 0){
                    fprintf(stdout, "%s%s%s%s%s%s%s", ANSI_COLOR_GREEN, "[" , ANSI_COLOR_RESET, "+", ANSI_COLOR_GREEN,"] TCP SYN\n", ANSI_COLOR_RESET);
                    found = 1;
                }
                // if(aux->next != NULL) printf(" ├── %s\n", aux->ip);
                if(aux->next != NULL) printf(" └── %s\n", aux->ip);
                else printf(" └── %s\n", aux->ip);
            }
            aux = aux->next;
        }
        //Warning, might be a valid ip
        if(found == 0) fprintf(stdout, "%s%s%s%s%s%s%s", ANSI_COLOR_ORANGE, "[" , ANSI_COLOR_RESET, "!", ANSI_COLOR_ORANGE,"] TCP SYN\n", ANSI_COLOR_RESET);
    }



    /*                                          TCP CONNECT                                                 */
    //Reset variables
    aux = tcp_connect_list;
    found = 0;

    //If mode is selected
    if(selected[1] == 1){
        //No IP
        if(aux == NULL){
            fprintf(stdout, "%s%s%s%s%s%s%s", ANSI_COLOR_RED, "[" , ANSI_COLOR_RESET, "-", ANSI_COLOR_RED,"] TCP CONNECT\n", ANSI_COLOR_RESET);
            found = 1;
        }    
        //IPs
        while(aux != NULL)
        {
            if(aux->count > 5){
                if(found == 0){
                    fprintf(stdout, "%s%s%s%s%s%s%s", ANSI_COLOR_GREEN, "[" , ANSI_COLOR_RESET, "+", ANSI_COLOR_GREEN,"] TCP CONNECT\n", ANSI_COLOR_RESET);
                    found = 1;
                }
                // if(aux->next != NULL) printf(" ├── %s\n", aux->ip);
                if(aux->next != NULL) printf(" └── %s\n", aux->ip);
                else printf(" └── %s\n", aux->ip);
            }
            aux = aux->next;
        }
        //Warning, might be a valid ip
        if(found == 0) fprintf(stdout, "%s%s%s%s%s%s%s", ANSI_COLOR_ORANGE, "[" , ANSI_COLOR_RESET, "!", ANSI_COLOR_ORANGE,"] TCP CONNECT\n", ANSI_COLOR_RESET);
    }



    /*                                          TCP FIN                                                 */
    //Reset variables
    aux = tcp_fin_list;
    found = 0;

    //If mode is selected
    if(selected[2] == 1){
        //No IP
        if(aux == NULL){
            fprintf(stdout, "%s%s%s%s%s%s%s", ANSI_COLOR_RED, "[" , ANSI_COLOR_RESET, "-", ANSI_COLOR_RED,"] TCP FIN\n", ANSI_COLOR_RESET);
            found = 1;
        }
        //IPs
        while(aux != NULL)
        {
            if(aux->count > 5){
                if(found == 0){
                    fprintf(stdout, "%s%s%s%s%s%s%s", ANSI_COLOR_GREEN, "[" , ANSI_COLOR_RESET, "+", ANSI_COLOR_GREEN,"] TCP FIN\n", ANSI_COLOR_RESET);
                    found = 1;
                }
                // if(aux->next != NULL) printf(" ├── %s\n", aux->ip);
                if(aux->next != NULL) printf(" └── %s\n", aux->ip);
                else printf(" └── %s\n", aux->ip);
            }
            aux = aux->next;
        }
        //Warning, might be a valid ip
        if(found == 0) fprintf(stdout, "%s%s%s%s%s%s%s", ANSI_COLOR_ORANGE, "[" , ANSI_COLOR_RESET, "!", ANSI_COLOR_ORANGE,"] TCP FIN\n", ANSI_COLOR_RESET);
    }



    /*                                          TCP NULL                                                 */
    //Reset variables
    aux = tcp_null_list;
    found = 0;

    //If mode is selected
    if(selected[3] == 1){
        //No IP
        if(aux == NULL){
            fprintf(stdout, "%s%s%s%s%s%s%s", ANSI_COLOR_RED, "[" , ANSI_COLOR_RESET, "-", ANSI_COLOR_RED,"] TCP NULL\n", ANSI_COLOR_RESET);
            found = 1;
        }
        //IPs
        while(aux != NULL)
        {
            if(aux->count > 5){
                if(found == 0){
                    fprintf(stdout, "%s%s%s%s%s%s%s", ANSI_COLOR_GREEN, "[" , ANSI_COLOR_RESET, "+", ANSI_COLOR_GREEN,"] TCP NULL\n", ANSI_COLOR_RESET);
                    found = 1;
                }
                // if(aux->next != NULL) printf(" ├── %s\n", aux->ip);
                if(aux->next != NULL) printf(" └── %s\n", aux->ip);
                else printf(" └── %s\n", aux->ip);
            }
            aux = aux->next;
        }
        //Warning, might be a valid ip
        if(found == 0) fprintf(stdout, "%s%s%s%s%s%s%s", ANSI_COLOR_ORANGE, "[" , ANSI_COLOR_RESET, "!", ANSI_COLOR_ORANGE,"] TCP NULL\n", ANSI_COLOR_RESET);
    }



    /*                                          TCP XMAS                                                 */
    //Reset variables
    aux = tcp_xmas_list;
    found = 0;

    //If mode is selected
    if(selected[4] == 1){
        //No IP
        if(aux == NULL){
            fprintf(stdout, "%s%s%s%s%s%s%s", ANSI_COLOR_RED, "[" , ANSI_COLOR_RESET, "-", ANSI_COLOR_RED,"] TCP XMAS 🎅🎄\n", ANSI_COLOR_RESET);
            found = 1;
        }
        //IPs
        while(aux != NULL)
        {
            if(aux->count > 5){
                if(found == 0){
                    fprintf(stdout, "%s%s%s%s%s%s%s", ANSI_COLOR_GREEN, "[" , ANSI_COLOR_RESET, "+", ANSI_COLOR_GREEN,"] TCP XMAS 🎅🎄\n", ANSI_COLOR_RESET);
                    found = 1;
                }
                // if(aux->next != NULL) printf(" ├── %s\n", aux->ip);
                if(aux->next != NULL) printf(" └── %s\n", aux->ip);
                else printf(" └── %s\n", aux->ip);
            }
            aux = aux->next;
        }
        //Warning, might be a valid ip
        if(found == 0) fprintf(stdout, "%s%s%s%s%s%s%s", ANSI_COLOR_ORANGE, "[" , ANSI_COLOR_RESET, "!", ANSI_COLOR_ORANGE,"] TCP XMAS 🎅🎄\n", ANSI_COLOR_RESET);
    }



    /*                                          UDP                                                 */
    //Reset variables
    aux = udp_list;
    found = 0;

    //If mode is selected
    if(selected[5] == 1){
        //No IP
        if(aux == NULL){
            fprintf(stdout, "%s%s%s%s%s%s%s", ANSI_COLOR_RED, "[" , ANSI_COLOR_RESET, "-", ANSI_COLOR_RED,"] UDP\n", ANSI_COLOR_RESET);
            found = 1;
        }
        //IPs
        while(aux != NULL)
        {
            if(aux->count > 5){
                if(found == 0){
                    fprintf(stdout, "%s%s%s%s%s%s%s", ANSI_COLOR_GREEN, "[" , ANSI_COLOR_RESET, "+", ANSI_COLOR_GREEN,"] UDP\n", ANSI_COLOR_RESET);
                    found = 1;
                }
                // if(aux->next != NULL) printf(" ├── %s\n", aux->ip);
                if(aux->next != NULL) printf(" └── %s\n", aux->ip);
                else printf(" └── %s\n", aux->ip);
            }
            aux = aux->next;
        }
        //Warning, might be a valid ip
        if(found == 0) fprintf(stdout, "%s%s%s%s%s%s%s", ANSI_COLOR_ORANGE, "[" , ANSI_COLOR_RESET, "!", ANSI_COLOR_ORANGE,"] UDP\n", ANSI_COLOR_RESET);
    }
}


//Function to generate report file
int report()
{
    //Generate a new report file each time
    int fd;
    if((fd = creat("report.txt", 0777)) < 0){
        fprintf(stderr, "ERROR: Unable to create report\n");
        return -1;
    } 
    if((fd = open("report.txt", O_RDWR, 0777)) < 0){
        fprintf(stderr, "ERROR: Unable to open report\n");
        return -1;
    }

    char buffer[500];
    strcpy(buffer, "************************\n* PORT SCANNING REPORT *\n************************\n\n");
    write(fd, buffer, strlen(buffer));

    strcpy(buffer, "DISCLAIMER\nIPs are selected as potential scanners if more than 5 packets to different ports are found, those with less packets found are marked with '*' in case you want to make sure by yourself.\nIPs are tracked nationaly thanks to IP2Location's library, although the IP found might not be the real attackers IP.\n\n");
    write(fd, buffer, strlen(buffer));

    //IP List
    historical *aux = tcp_syn_list;
    char count[100];
    
    //If mode is selected
    if(selected[0] == 1){
        strcpy(buffer, "TCP SYN:\n--------\n");
        write(fd, buffer, strlen(buffer));
        
        //No IP
        if(aux == NULL){
            strcpy(buffer, "No IPs found\n");
            write(fd, buffer, strlen(buffer));
        }
        //IPs
        while(aux != NULL)
        {
            //Check count
            if(aux->count > 5) strcpy(buffer, "");
            else strcpy(buffer, "  *");

            //Format ip report
            strcat(buffer, "IP ");
            strcat(buffer, aux->ip);
            strcat(buffer, " with ");
            sprintf(count, "%d", aux->count);
            strcat(buffer, count);
            strcat(buffer, " ports scanned from");

            //Geolocate ip
            if(strstr(aux->ip, "192.168.") != NULL) strcat(buffer, " your local network");
            else{
                record = IP2Location_get_all(IP2LocationObj, aux->ip);
                strcat(buffer, " ");
                strcat(buffer, record->country_long);
            }
            strcat(buffer, "\n");

            write(fd, buffer, strlen(buffer));
            aux = aux->next;
        }
        strcpy(buffer, "\n");
        write(fd, buffer, strlen(buffer));
    } 



    /*                                          TCP CONNECT                                                 */
    //Reset variables
    aux = tcp_connect_list;

    //If mode is selected
    if(selected[1] == 1){
        strcpy(buffer, "TCP CONNECT:\n------------\n");
        write(fd, buffer, strlen(buffer));
        
        //No IP
        if(aux == NULL){
            strcpy(buffer, "No IPs found\n");
            write(fd, buffer, strlen(buffer));
        }
        //IPs
        while(aux != NULL)
        {
            //Check count
            if(aux->count > 5) strcpy(buffer, "");
            else strcpy(buffer, "  *");

            //Format ip report
            strcat(buffer, "IP ");
            strcat(buffer, aux->ip);
            strcat(buffer, " with ");
            sprintf(count, "%d", aux->count);
            strcat(buffer, count);
            strcat(buffer, " ports scanned from");

            //Geolocate ip
            if(strstr(aux->ip, "192.168.") != NULL) strcat(buffer, " your local network");
            else{
                record = IP2Location_get_all(IP2LocationObj, aux->ip);
                strcat(buffer, " ");
                strcat(buffer, record->country_long);
            }
            strcat(buffer, "\n");

            write(fd, buffer, strlen(buffer));
            aux = aux->next;
        }
        strcpy(buffer, "\n");
        write(fd, buffer, strlen(buffer));
    }



    /*                                          TCP FIN                                                 */
    //Reset variables
    aux = tcp_fin_list;

    //If mode is selected
    if(selected[2] == 1){
        strcpy(buffer, "TCP FIN:\n--------\n");
        write(fd, buffer, strlen(buffer));
        
        //No IP
        if(aux == NULL){
            strcpy(buffer, "No IPs found\n");
            write(fd, buffer, strlen(buffer));
        }
        //IPs
        while(aux != NULL)
        {
            //Check count
            if(aux->count > 5) strcpy(buffer, "");
            else strcpy(buffer, "  *");

            //Format ip report
            strcat(buffer, "IP ");
            strcat(buffer, aux->ip);
            strcat(buffer, " with ");
            sprintf(count, "%d", aux->count);
            strcat(buffer, count);
            strcat(buffer, " ports scanned from");

            //Geolocate ip
            if(strstr(aux->ip, "192.168.") != NULL) strcat(buffer, " your local network");
            else{
                record = IP2Location_get_all(IP2LocationObj, aux->ip);
                strcat(buffer, " ");
                strcat(buffer, record->country_long);
            }
            strcat(buffer, "\n");

            write(fd, buffer, strlen(buffer));
            aux = aux->next;
        }
        strcpy(buffer, "\n");
        write(fd, buffer, strlen(buffer));
    }



    /*                                          TCP NULL                                                 */
    //Reset variables
    aux = tcp_null_list;

    //If mode is selected
    if(selected[3] == 1){
        strcpy(buffer, "TCP NULL:\n---------\n");
        write(fd, buffer, strlen(buffer));
        
        //No IP
        if(aux == NULL){
            strcpy(buffer, "No IPs found\n");
            write(fd, buffer, strlen(buffer));
        }
        //IPs
        while(aux != NULL)
        {
            //Check count
            if(aux->count > 5) strcpy(buffer, "");
            else strcpy(buffer, "  *");

            //Format ip report
            strcat(buffer, "IP ");
            strcat(buffer, aux->ip);
            strcat(buffer, " with ");
            sprintf(count, "%d", aux->count);
            strcat(buffer, count);
            strcat(buffer, " ports scanned from");

            //Geolocate ip
            if(strstr(aux->ip, "192.168.") != NULL) strcat(buffer, " your local network");
            else{
                record = IP2Location_get_all(IP2LocationObj, aux->ip);
                strcat(buffer, " ");
                strcat(buffer, record->country_long);
            }
            strcat(buffer, "\n");

            write(fd, buffer, strlen(buffer));
            aux = aux->next;
        }
        strcpy(buffer, "\n");
        write(fd, buffer, strlen(buffer));
    }



    /*                                          TCP XMAS                                                 */
    //Reset variables
    aux = tcp_xmas_list;

    //If mode is selected
    if(selected[4] == 1){
        strcpy(buffer, "TCP XMAS🎅🎄:\n-------------\n");
        write(fd, buffer, strlen(buffer));
        
        //No IP
        if(aux == NULL){
            strcpy(buffer, "No IPs found\n");
            write(fd, buffer, strlen(buffer));
        }
        //IPs
        while(aux != NULL)
        {
            //Check count
            if(aux->count > 5) strcpy(buffer, "");
            else strcpy(buffer, "  *");

            //Format ip report
            strcat(buffer, "IP ");
            strcat(buffer, aux->ip);
            strcat(buffer, " with ");
            sprintf(count, "%d", aux->count);
            strcat(buffer, count);
            strcat(buffer, " ports scanned from");

            //Geolocate ip
            if(strstr(aux->ip, "192.168.") != NULL) strcat(buffer, " your local network");
            else{
                record = IP2Location_get_all(IP2LocationObj, aux->ip);
                strcat(buffer, " ");
                strcat(buffer, record->country_long);
            }
            strcat(buffer, "\n");

            write(fd, buffer, strlen(buffer));
            aux = aux->next;
        }
        strcpy(buffer, "\n");
        write(fd, buffer, strlen(buffer));
    }



    /*                                          UDP                                                 */
    //Reset variables
    aux = udp_list;

    //If mode is selected
    if(selected[5] == 1){
        strcpy(buffer, "UDP:\n----\n");
        write(fd, buffer, strlen(buffer));
        
        //No IP
        if(aux == NULL){
            strcpy(buffer, "No IPs found\n");
            write(fd, buffer, strlen(buffer));
        }
        //IPs
        while(aux != NULL)
        {
            //Check count
            if(aux->count > 5) strcpy(buffer, "");
            else strcpy(buffer, "  *");

            //Format ip report
            strcat(buffer, "IP ");
            strcat(buffer, aux->ip);
            strcat(buffer, " with ");
            sprintf(count, "%d", aux->count);
            strcat(buffer, count);
            strcat(buffer, " ports scanned from");

            //Geolocate ip
            if(strstr(aux->ip, "192.168.") != NULL) strcat(buffer, " your local network");
            else{
                record = IP2Location_get_all(IP2LocationObj, aux->ip);
                strcat(buffer, " ");
                strcat(buffer, record->country_long);
            }
            strcat(buffer, "\n");

            write(fd, buffer, strlen(buffer));
            aux = aux->next;
        }
    }

    int op;
    if((op = close(fd)) < 0){
        fprintf(stderr, "ERROR: Unable to close report\n");
        return -1;
    } 
    
    return 0;
}

char *remove_spaces(char *str)
{
    int i, j = 0;
    char *res;
    res = malloc(20);
    for (i = 0; i < strlen(str) - 1; i++)
    {
        if (str[i] != ' ')
        {
            res[j] = str[i];
            j++;
        }
    }
    return res;
}

int *clean(char *str)
{
    int i, j = 0;
    static int res[MODES];
    for (i = 0; i < strlen(str); i++)
    {
        if (isdigit(str[i]))
        {
            res[j] = str[i] - '0';
            j++;
        }
    }
    return res;
}