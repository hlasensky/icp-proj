# Define project variables
PROJECT_NAME = icp_v4

# Define directories
SRC_DIR = src
BUILD_DIR = build
DOC_DIR = doc
EXAMPLES_DIR = examples

# Qmake target
build:
	mkdir $(BUILD_DIR)
	qmake $(SRC_DIR)/$(PROJECT_NAME).pro 
	make -f $(BUILD_DIR)/Makefile qmake_all

# Doxygen target
doxygen:
	doxygen $(DOC_DIR)/Doxygen

# Clean target
clean:
	rm -rf $(BUILD_DIR)
	make -C $(DOC_DIR) clean

# Run target (for testing)
run: build
	./$(BUILD_DIR)/$(PROJECT_NAME)

# Pack target (for submission)
pack: clean build doxygen
	mkdir -p $(PROJECT_NAME)
	cp -r $(SRC_DIR) $(EXAMPLES_DIR) $(README.txt) $(MAKEFILE) $(PROJECT_NAME)
	zip -r $(PROJECT_NAME).zip $(PROJECT_NAME)

# Help target
help:
	@echo "Available targets:"
	@echo "  qmake      - Run qmake to generate build files"
	@echo "  build       - Build the project (requires qmake first)"
	@echo "  doxygen     - Generate documentation using Doxygen"
	@echo "  clean       - Remove build directory and Doxygen output"
	@echo "  run         - Run the simulator (after build)"
	@echo "  pack        - Package the project for submission"
	@echo "  help        - Show this help message"
