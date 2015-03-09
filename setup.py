from distutils.core import setup, Extension
setup(
	name="tentacle_pi.MCP9808", 
	version="1.0",
	packages = ["tentacle_pi"],
	ext_modules = [
		Extension("tentacle_pi.MCP9808", 
			sources = ["src/mcp9808.c", "src/mcp9808_ext.c"])
	]
)
