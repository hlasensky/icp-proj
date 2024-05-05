# Tomáš Hlásenský (xhlase01)
# author Michael Babušík (xbabus01)

DOC := doc
ZIPFILE := xhlase01-xbabus01.zip
ZIPFILES := src/* doc/images/* build/ Makefile README.txt Doxyfile 
BUILD_DIR := build
TARGET := icp_v4 
# Targets
.PHONY: all clean run doc
all:$(TARGET)

$(TARGET):
	mkdir -p build
	cd build && qmake ../src/icp_v4.pro && make
clean:
	rm -rf $(BUILD_DIR) $(DOC) $(ZIPFILE)

run: $(TARGET)
	./$(BUILD_DIR)/$(TARGET)

doxygen:
	doxygen Doxyfile

pack:
	zip $(ZIPFILE) $(ZIPFILES)