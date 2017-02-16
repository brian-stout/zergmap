#include "binary.h"

uint64_t
htonll( uint64_t i )
{
    uint32_t a;
    uint32_t b;
    uint64_t r = 0;

    a = i >> 32;
    b = i;
    r = r | htonl(b);
    r = r << 32;
    r = r | htonl(a);
    return r;
}

uint32_t
hton24( uint32_t i )
{
    uint32_t a = i & 0xffff;

    a = htons(a);
    a <<= 8;
    i >>= 16;
    i = i | a;

    return i;
}

uint64_t
ntohll( uint64_t i )
{
    uint32_t a;
    uint32_t b;
    uint64_t r = 0;

    a = i >> 32;
    b = i;
    r = r | ntohl(b);
    r = r << 32;
    r = r | ntohl(a);
    return r;
}

uint32_t
ntoh24( uint32_t i )
{
    uint32_t a = i & 0xffff;

    a = ntohs(a);
    a <<= 8;
    i >>= 16;
    i = i | a;

    return i;
}

//Solution adapted from http://stackoverflow.com/a/28884902
//Credit to user Antoine L 
float
bin_to_float( uint32_t a )
{
    union
    {
        uint32_t b;
        float f;
    } u;

    u.b = a;
    return u.f;
}

double
bin_to_doub( uint64_t a )
{
    union
    {
        uint64_t b;
        double d;
    } u;

    u.b = a;
    return u.d;
}

uint32_t
float_to_bin( float a )
{
    union
    {
        float b;
        uint32_t uint;
    } u;

    u.b = a;
    return u.uint;
}

uint64_t
doub_to_bin( double a )
{
    union
    {
        double b;
        uint64_t uint;
    } u;

    u.b = a;
    return u.uint;
}
