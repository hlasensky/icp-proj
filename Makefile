# Define project variables
PROJECT_NAME = MyProject

# Define directories
SRC_DIR = src
BUILD_DIR = build
DOC_DIR = doc

QMAKE_PRO = icp_v4.pro
# Include paths for Qt libraries (adjust based on your setup)
# QMAKE_INCLUDE_PATH = /usr/include/qt5

# Qmake target
qmake:
	qmake $(SRC_DIR)/$(QMAKE_PRO)

# Doxygen target
doxygen:
	$(MAKE) -C $(DOC_DIR)

# Clean target
clean:
	rm -rf $(BUILD_DIR)
	$(MAKE) -C $(DOC_DIR) clean

# Build target (depends on qmake)
build: qmake
	make -C $(BUILD_DIR)

# Help target
help:
	@echo "Available targets:"
	@echo "  qmake      - Run qmake to generate build files"
	@echo "  doxygen     - Generate documentation using Doxygen"
	@echo "  clean       - Remove build directory and Doxygen output"
	@echo "  build       - Build the project (requires qmake first)"
	@echo "  help        - Show this help message"

# Include Doxygen configuration file (optional)
ifeq ($(strip $(DOXYGEN_CONFIG)), )
	DOXYGEN_CONFIG = Doxyfile
endif
$(DOC_DIR)/%.pdf: $(DOC_DIR)/%.dox $(DOXYGEN_CONFIG)
	doxygen $(DOXYGEN_CONFIG)


