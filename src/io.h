#ifndef __MYPRINT_H__
#define __MYPRINT_H__
#include <stdint.h>

void write_to_port(uint8_t port, uint8_t value);

uint8_t read_from_port(uint8_t port);

#endif