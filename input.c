#include "input.h"

zerg * init_zerg_unit(void)
{
    zerg * zerg_unit = (zerg *)malloc(sizeof(zerg));
    zerg_unit->zergID = 0;
    zerg_unit->HP = 0;
    zerg_unit->maxHP = 0;
    zerg_unit->longitude = 0;
    zerg_unit->latitude = 0;
    zerg_unit->altitude = 0;
    return zerg_unit;
}



zerg * get_gps(zerg * zerg_unit, FILE * fp)
{
    uint64_t longitude;
    fread(&longitude, sizeof(longitude), 1, fp);
    uint64_t latitude;
    fread(&latitude, sizeof(latitude), 1, fp);
    uint32_t altitude;
    fread(&altitude, sizeof(altitude), 1, fp);
    //Skip over the rest of the GPS packet
    fseek(fp, 12, SEEK_CUR);

    zerg_unit->longitude = bin_to_doub(ntohll(longitude));
    zerg_unit->latitude = bin_to_doub(ntohll(latitude));
    zerg_unit->altitude = bin_to_float(ntohl(altitude));

    return zerg_unit;
}

zerg * get_status(zerg * zerg_unit, FILE * fp)
{
    uint32_t HP;
    fread(&HP, 3, 1, fp);
    //Skipping over armor
    fseek(fp, 1, SEEK_CUR);
    uint32_t maxHP;
    fread(&maxHP, 3, 1, fp);
    //Skipping over the zerg type
    fseek(fp, 1, SEEK_CUR);
    //Skipping over the speed data
    fseek(fp, 4, SEEK_CUR); 

    zerg_unit->HP = ntoh24(HP);
    zerg_unit->maxHP = ntoh24(maxHP);

    return zerg_unit;  
}

zerg * read_pcap_packet(FILE * fp)
{
    //Go to size of file value in PCAP header
    //4 bytes to skip the time keeping portion of the PCAP
    //Unix Epoch was read outside of test file to error check
    fseek(fp, 4, SEEK_CUR);

    uint32_t bytesLeft;
    fread(&bytesLeft, sizeof(bytesLeft), 1, fp);

    //Go to the IP version
    //4 + because that's how many bytes remain in the pcap_packet to be read
    fseek(fp, 4 + ETHERNET_HEADER_SZ, SEEK_CUR);

    bytesLeft -= ETHERNET_HEADER_SZ;

    uint8_t versionIHL;

    fread(&versionIHL, sizeof(versionIHL), 1, fp);
    bytesLeft -= sizeof(versionIHL);


    int IPType = versionIHL >> 4;

    //Go to the Zerg packet
    //Skip the rest of the ipv4 header + udp header - 1 for read byte
    if(IPType == 4)
    {
        fseek(fp, IPV4_HEADER_SZ + UDP_HEADER_SZ - 1, SEEK_CUR);
        bytesLeft -= (IPV4_HEADER_SZ + (UDP_HEADER_SZ - 1));
    }
    //Skip the rest of the ipv6 header + udp header - 1 for read byte
    else if(IPType == 6)
    {
        fseek(fp, IPV6_HEADER_SZ + UDP_HEADER_SZ - 1, SEEK_CUR);
        bytesLeft -= (IPV6_HEADER_SZ + (UDP_HEADER_SZ - 1));
    }
    //Oh no
    else
    {
        printf("Corrupt packet!\n");
    }

    //Read the 8 bytes containing the version and type of the zerg packet
    uint8_t versionType;
    fread(&versionType, sizeof(versionType), 1, fp);
    bytesLeft -= sizeof(versionType);
    
    //Grabbing the first half for the version
    int type = versionType & 0xf;

    //If it isn't a GPS packet or a status packet, we don't care
    if(type != 1 && type != 3)
    {
        fseek(fp, bytesLeft, SEEK_CUR);
        return NULL;
    }

    //Shifting four bits over and dropping the version off the type
    int version = versionType >> 4;

    //If it isn't version 1, we can't read the packet
    if(version != 1)
    {
        fseek(fp, bytesLeft, SEEK_CUR);
        return NULL;
    }

    //Jumps past the zerg length portion (3 bytes)
    // ID portion (2 bytes) and the source ID
    fseek(fp, 3 + 2, SEEK_CUR);
    bytesLeft -= (3 + 2);

    //Initializing the zerg_unit
    zerg * zerg_unit = init_zerg_unit();

    //Reading in the ID for the zerg struct being created
    fread(&zerg_unit->zergID, 2, 1, fp);
    zerg_unit->zergID = ntohs(zerg_unit->zergID);
    bytesLeft -= 2;

    //Skipping rest of the zerg packet, the sequence ID 

    fseek(fp, 4, SEEK_CUR);
    bytesLeft -= 4;
    
    //Doing different things for the different zerg packets
    if(type == 1)
    {
        //TODO: Put this in a function, read_status(zerg * zerg_unit, FILE * fp)
        //      Gotta combine BytesLeft since it wont return it
        zerg_unit = get_status(zerg_unit, fp);
        //Size of a zerg_status packet
        bytesLeft -= 12;
    }
    else if(type == 3)
    {
        zerg_unit = get_gps(zerg_unit, fp);
        //Size of a zerg_gps packet
        bytesLeft -= 32;               
    }

    if (bytesLeft > 0)
    {
        fseek(fp, bytesLeft, SEEK_CUR);
    }
    return zerg_unit;
} 
