#ifndef INPUT_H
#define INPUT_H

#include <arpa/inet.h>
#include <stdio.h>
#include <stdint.h>
#include <sysexits.h>
#include <stdlib.h>

#include "binary.h"

typedef struct zerg
{
    int zergID;
    long int HP;
    long int maxHP;
    double longitude;
    double latitude;
    float altitude;
}   zerg;

enum
{
    GLOBAL_HEADER_SZ = 24
};

enum
{
    PCAP_PACKET_SZ = 16
};

enum
{
    ETHERNET_HEADER_SZ = 14
};

enum
{
    IPV4_HEADER_SZ = 20
};

enum
{
    IPV6_HEADER_SZ = 40
};

enum
{
    UDP_HEADER_SZ = 8
};

enum
{
    ZERG_HEADER_SZ = 12
};

zerg * get_status(zerg * zerg_unit, FILE * fp);
zerg * get_gps(zerg * zerg_unit, FILE * fp);

zerg * init_zerg_unit(void);
zerg * read_pcap_packet(FILE * fp);


#endif
