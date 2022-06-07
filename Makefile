INCLUDES=$(wildcard Source/*.hpp) SimplePluginHost/JuceLibraryCode/JuceHeader.h
EXPHEADERS=export/include/

LIBEXPORT=export/lib/libSimplePluginHost.a
LIB=SimplePluginHost/Builds/LinuxMakefile/build/libSimplePluginHost.a
SOURCES=$(wildcard Source/*)

all: prepare

.SILENT:

prepare: $(LIBEXPORT) $(EXPHEADERS)
	echo "Copying headers in export/include"
	cp $(INCLUDES) $(EXPHEADERS)

$(LIB): $(SOURCES)
	echo "Compiling lib"
	make -C SimplePluginHost/Builds/LinuxMakefile
	echo "Library compiled"


$(LIBEXPORT): $(LIB)
	echo "Copying library in export/lib"
	cp $(LIB) $(LIBEXPORT)

compile: $(LIB)

clean:
	rm -rf export/include export/lib
	mkdir export/include export/lib

clean-all: clean
	make clean -C SimplePluginHost/Builds/LinuxMakefile