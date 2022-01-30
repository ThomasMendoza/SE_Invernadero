
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>
#include <stdlib.h>

void serial_begin();
unsigned char serial_read_char();
void serial_print_char(unsigned char caracter);
void serial_print_str(char* cadena);
void serial_println_str(char *cadena);
char* get_RX_buffer();
bool is_data_ready();
static volatile char float_str[45] = {0};
char* flot2str(float h);

