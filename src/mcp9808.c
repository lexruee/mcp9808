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

#define MCP9808_REG_MANUF_ID	0x06
#define MCP9808_REG_DEVICE_ID	0x07

#define MCP9808_REG_TMP_MSB		0x02
#define MCP9808_REG_TMP_LSB		0x03
#define MCP9808_REG_TMP 		0x05

typedef struct {
	/* file descriptor */
	int file;
	
	/* i2c device address */
	int address;
	
	/* i2c device file path */
	char *i2c_device;
	
	/* sensor specific data */
	int manuf_id;
	int device_id;
} mcp9808_t;



#define TO_S(x)	(mcp9808_t*) x

//#define __MCP9808__DEBUG__
#ifdef __MCP9808__DEBUG__				
#define DEBUG(...)	printf(__VA_ARGS__)
#else
#define DEBUG(...)
#endif



/*
 * Prototypes for helper functions
 */

int mcp9808_set_addr(void *_s);
void mcp9808_init_error_cleanup(void *_s);


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

	if((error = ioctl(s->file, I2C_SLAVE, s->address)) < 0) {
		DEBUG("error: ioctl() failed\n");
	}

	return error;
} 



/*
 * Frees allocated memory in the init function.
 * 
 * @param mcp9808 sensor
 */
void mcp9808_init_error_cleanup(void *_s) {
	mcp9808_t* s = TO_S(_s);
	
	if(s->i2c_device != NULL) {
		free(s->i2c_device);
		s->i2c_device = NULL;
	}
	
	free(s);
	s = NULL;
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
	if(_s == NULL) {
		DEBUG("error: malloc returns NULL pointer\n");
		return NULL;
	}

	mcp9808_t *s = TO_S(_s);
	s->address = address;

	s->i2c_device = (char*) malloc(strlen(i2c_device_filepath) * sizeof(char));
	if(s->i2c_device == NULL) {
		DEBUG("error: malloc returns NULL pointer!\n");
		mcp9808_init_error_cleanup(s);
		return NULL;
	}

	// copy string
	strcpy(s->i2c_device, i2c_device_filepath);
	
	// open i2c device
	int file;
	if((file = open(s->i2c_device, O_RDWR)) < 0) {
		DEBUG("error: %s open() failed\n", s->i2c_device);
		mcp9808_init_error_cleanup(s);
		return NULL;
	}
	s->file = file;
	
	if(mcp9808_set_addr(s) < 0) {
		mcp9808_init_error_cleanup(s); 
		return NULL;
	}
		
	uint16_t manuf_id = (uint16_t) i2c_smbus_read_word_data(s->file, MCP9808_REG_MANUF_ID);
	uint16_t device_id = (uint16_t) i2c_smbus_read_word_data(s->file, MCP9808_REG_DEVICE_ID);
	s->manuf_id = ((manuf_id & 0x00FF)<<8) | ((manuf_id & 0xFF00)>>8);
	s->device_id = ((device_id & 0x00FF)<<8) | ((device_id & 0xFF00)>>8);
	
	DEBUG("device: manuf_id: 0x%04x, device_id: 0x%04x\n", s->manuf_id, s->device_id);

	DEBUG("device: open ok\n");
	
	return _s;
}



/**
 * Closes a mcp8909 object.
 * 
 * @param mcp8909 sensor
 */
void mcp9808_close(void *_s) {
	if(_s == NULL) {
		return;
	}
	
	DEBUG("close device\n");
	mcp9808_t *s = TO_S(_s);
	
	if(close(s->file) < 0)
		DEBUG("error: %s close() failed\n", s->i2c_device);
	
	free(s->i2c_device); // free string
	s->i2c_device = NULL;
	free(s); // free structure
	_s = NULL;
}



/**
 * Returns the measured temperature in celsius.
 * 
 * @param mcp8909 sensor
 * @return temperature
 */
float mcp9808_temperature(void *_s) {
	mcp9808_t *s = TO_S(_s);
	
	// swap order of the msb and lsb bytes
	uint16_t temperature_word = i2c_smbus_read_word_data(s->file, MCP9808_REG_TMP);
	uint16_t raw_temperature = ((temperature_word & 0x00FF)<<8) | ((temperature_word & 0xFF00)>>8);
	
	float temperature = raw_temperature & 0x0FFF; // extract the first three bytes
	temperature /= 16.0;
	
	if(raw_temperature & 0x1000) { // check sign bit
		temperature -= 256.0;
	}
	DEBUG("temperature_word: %#x\n",temperature_word);
	DEBUG("temperature: %0.2f\n",temperature);
	
	return temperature;
}

