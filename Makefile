# Tomáš Hlásenský (xhlase01)
# author Michael Babušík (xbabus01)

DOC_HTML := doc/html
DOC_LATEX := doc/latex
ZIPFILE := xhlase01-xbabus01.zip
ZIPFILES := src/* doc/images/* Makefile README.txt Doxyfile 
BUILD_DIR := build
TARGET := icp_v4 


.PHONY: all clean run doc
all:$(TARGET)

$(TARGET):
	mkdir -p build
	cd build && qmake ../src/icp_v4.pro && make
clean:
	rm -rf $(BUILD_DIR) $(DOC_HTML) $(DOC_LATEX) $(ZIPFILE)

run: $(TARGET)
	./$(BUILD_DIR)/$(TARGET)

doxygen:
	doxygen Doxyfile

pack:
	zip $(ZIPFILE) $(ZIPFILES)