INCLUDES=$(wildcard Source/*.hpp) SimplePluginHost/JuceLibraryCode/JuceHeader.h
EXPHEADERS=export/include/
EXPLIBS=export/lib/
EXPDIRS=$(EXPHEADERS) $(EXPLIBS)

LIBEXPORT=export/lib/libSimplePluginHost.a
LIB=SimplePluginHost/Builds/LinuxMakefile/build/libSimplePluginHost.a
SOURCES=$(wildcard Source/*)

all: prepare

.SILENT:

prepare: $(EXPDIRS) $(LIBEXPORT)
	echo "Copying headers in $(EXPHEADERS)"
	cp $(INCLUDES) $(EXPHEADERS)

$(LIB): $(SOURCES)
	echo "Compiling lib"
	make -C SimplePluginHost/Builds/LinuxMakefile
	echo "Library compiled"

$(LIBEXPORT): $(LIB)
	echo "Copying library in $(EXPLIBS)"
	cp $(LIB) $(LIBEXPORT)

compile: $(LIB)

$(EXPDIRS):
	mkdir -p $(EXPDIRS)

clean:
	rm -rf $(EXPDIRS)

clean-all: clean
	make clean -C SimplePluginHost/Builds/LinuxMakefile