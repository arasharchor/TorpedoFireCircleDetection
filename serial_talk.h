#include<iostream>
#include<fstream>
#include<cstdlib>
#include <string.h>

using namespace std;

int send_via_port(void *message, char const *type, int size);
void * read_from_port(void *message);
void port_initialize(char const *portname, char const *baud);
void port_destruct();
extern int my_finish_flag;	// true after a sensor reading is completely read
extern int my_start_flag;	// true after a sensor read thread is created
//char my_serial_port[20];
//char my_baud[20];


