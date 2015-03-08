/*
 * @author 	Alexander Rüedlinger <a.rueedlinger@gmail.com>
 * @date 	08.03.2015
 *
 * A C driver for the sensor MCP9808.
 *  
 */


void *mcp9808_init(int address, const char* i2c_device_filepath);

void mcp9808_close(void *_s);

float mcp9808_temperature(void *_s);


