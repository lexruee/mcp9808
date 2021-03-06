# MCP9808

Raspberry Pi C driver and Python bindings for the sensor MCP9808.


## Example Usage

### C language

```c
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

```

## Python language

```python
import time
from tentacle_pi.MCP9808 import MCP9808
mcp = MCP9808(0x18,"/dev/i2c-2")

for x in range(0,10):
        temperature = mcp.temperature()
        print "temperature: %0.2f" % temperature
        time.sleep(2)
```


## Dependencies

* i2c-tools 
* libi2c-dev
* python-dev

