#include "mcp9808.h"
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv){
	char *i2c_device = "/dev/i2c-2";
	int address = 0x18;
	
	void *s = mcp9808_init(address, i2c_device);
	
	if(s != NULL){
		int i;
		for(i = 0; i < 10; i++) {
			float t = mcp9808_temperature(s);
			printf("temperature: %0.2f\n", t);
			usleep(2 * 1000 * 1000);
		}
	
		mcp9808_close(s);
	}
	
	return 0;
}
