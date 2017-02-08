#include <arpa/inet.h>
#include <stdio.h>
#include <stdint.h>
#include <sysexits.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "binary.h"

typedef struct zerg
{
    uint16_t zergID;
    uint32_t minHP;
    uint32_t maxHP;
    uint64_t longitude;
    uint64_t latitude;
    uint32_t altitude;
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

int
main( int argc, char *argv[] )
{
    if (argc != 2)
    {
        printf("ERROR, USAGE: [zergmap] [~/<filename>.pcap]\n");
        return EX_USAGE;
    }

    FILE *fp = fopen(argv[1], "rb");

    if (!fp)
    {
        printf("Unable to open file!\n");
        return EX_USAGE;
    }

    //Skip the global header
    fseek(fp, GLOBAL_HEADER_SZ, SEEK_SET);
    
    //Go to the IP version

    fseek(fp, PCAP_PACKET_SZ + ETHERNET_HEADER_SZ, SEEK_CUR);

    uint8_t versionIHL;
    fread(&versionIHL, sizeof(versionIHL), 1, fp);
    int IPType = versionIHL >> 4;

    //Go to the Zerg packet
    //Skip the rest of the ipv4 header + udp header - 1 for read byte
    if(IPType == 4)
    {
        fseek(fp, IPV4_HEADER_SZ + UDP_HEADER_SZ - 1, SEEK_CUR);
    }
    //Skip the rest of the ipv6 header + udp header - 1 for read byte
    else if(IPType == 6)
    {
        fseek(fp, IPV6_HEADER_SZ + UDP_HEADER_SZ - 1, SEEK_CUR);
    }
    //Oh no
    else
    {
        printf("Corrupt packet!\n");
    }

    //Read the 8 bytes containing the version and type of the zerg packet
    uint8_t versionType;
    fread(&versionType, sizeof(versionType), 1, fp);
    
    //Grabbing the first half for the version
    int type = versionType & 0xf;

    //Shifting four bits over and dropping the version off the type
    int version = versionType >> 4;

    uint32_t bytesLeft;
    fread(&bytesLeft, 3, 1, fp);

    //Flipping to network bit order
    bytesLeft = hton24(bytesLeft);

    //Subtracting 1 because we read 1 byte of data already from the zerg packet
    bytesLeft -= 1;

    printf("len %d\n", bytesLeft);


    printf("IP version: %d\n", IPType);
    printf("type: %d, version: %d\n", type, version);
}
