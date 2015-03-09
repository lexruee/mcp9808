import time
from tentacle_pi.MCP9808 import MCP9808
mcp = MCP9808(0x18,"/dev/i2c-2")

for x in range(0,100):
        temperature = mcp.temperature()
        print "temperature: %0.2f" % temperature
        time.sleep(2)







