/*
 * @author 	Alexander Rüedlinger <a.rueedlinger@gmail.com>
 * @date 	08.03.2015
 *
 * A C driver for the sensor MCP9808.
 *  
 */

#ifndef __MCP9808__
#define __MCP9808__
#include <stdint.h>
#include "mcp9808.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <linux/i2c-dev.h>
#include <time.h>
#include <math.h>
#endif


typedef struct {
	/* file descriptor */
	int file;
	
	/* i2c device address */
	int address;
	
	/* i2c device file path */
	char *i2c_device;	
} mcp9808_t;



#define TO_S(x)	(mcp9808_t*) x

#define __MCP9808__DEBUG__
#ifdef __MCP9808__DEBUG__				
#define DEBUG(...)	printf(__VA_ARGS__)
#else
#define DEBUG(...)
#endif



/*
 * Prototypes for helper functions
 */

int mcp9808_set_addr(void *_s);


/*
 * Implemetation of the helper functions
 */
 
 
/*
 * Sets the address for the i2c device file.
 * 
 * @param mcp9808 sensor
 */
int mcp9808_set_addr(void *_s) {
	mcp9808_t* s = TO_S(_s);
	int error;

	if((error = ioctl(s->file, I2C_SLAVE, s->address)) < 0)
		DEBUG("error: ioctl() failed\n");

	return error;
} 



/*
 * Implementation of the interface functions.
 */
 

/**
 * Creates a mcp9808 sensor object.
 *
 * @param i2c device address
 * @param i2c device file path
 * @return mcp9808 sensor
 */
void *mcp9808_init(int address, const char* i2c_device_filepath) {
	DEBUG("device: init using address %#x and i2cbus %s\n", address, i2c_device_filepath);
	
	void *_s = malloc(sizeof(mcp9808_t));
	if(_s == NULL)  {
		DEBUG("error: malloc returns NULL pointer\n");
		return NULL;
	}

	mcp9808_t *s = TO_S(_s);
	s->address = address;

	s->i2c_device = (char*) malloc(strlen(i2c_device_filepath) * sizeof(char));
	if(s->i2c_device == NULL) {
		DEBUG("error: malloc returns NULL pointer!\n");
		return NULL;
	}

	// copy string
	strcpy(s->i2c_device, i2c_device_filepath);
	
	// open i2c device
	int file;
	if((file = open(s->i2c_device, O_RDWR)) < 0) {
		DEBUG("error: %s open() failed\n", s->i2c_device);
		return NULL;
	}
	s->file = file;
	if(mcp9808_set_addr(s) < 0)
		return NULL;

	DEBUG("device: open ok\n");
	return _s;
}



/**
 * Closes a mcp8909 object.
 * 
 * @param mcp8909 sensor
 */
void mcp9808_close(void *_s) {
	DEBUG("close device\n");
	mcp9808_t *s = TO_S(_s);
	
	if(close(s->file) < 0)
		DEBUG("error: %s close() failed\n", s->i2c_device);
	
	free(s->i2c_device); // free string
	s->i2c_device = NULL;
	free(s); // free structure
	_s = NULL;
}

float mcp9808_temperature(void *_s) {
	mcp9808_t *s = TO_S(_s);
	return 0;
}
