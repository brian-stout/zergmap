#ifndef BINARY_H
#define BINARY_H

#include <arpa/inet.h>
#include <stdio.h>
#include <stdint.h>

uint64_t
ntohll(uint64_t);

uint32_t
ntoh24(uint32_t);

float
bin_to_float(uint32_t);

double
bin_to_doub(uint64_t);

uint64_t
doub_to_bin(double);

uint32_t
float_to_bin(float);

uint64_t
htonll(uint64_t);

uint32_t
hton24(uint32_t i);

#endif
