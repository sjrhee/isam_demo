# =============================================================================
# Makefile for CISAM Project
# C-ISAM (Indexed Sequential Access Method) Example Programs
# =============================================================================

# Compiler and flags
CC = gcc
CFLAGS = -I./include -w -g
LDFLAGS = -L./lib -lisam -ldl

# Directory structure
DEMO_DIR = demo
SRC_DIR = $(DEMO_DIR)/src
BUILD_DIR = $(DEMO_DIR)/build

# Source files and build targets
SOURCES = $(addprefix $(SRC_DIR)/, add_indx.c add_rcrd.c bld_file.c chaining.c sqntl_rd.c transctn.c upd_file.c)
OBJECTS = $(SOURCES:.c=.o)
TARGETS = $(addprefix $(BUILD_DIR)/, add_indx add_rcrd bld_file chaining sqntl_rd transctn upd_file)

# Default target
all: $(TARGETS)
	@echo "✓ Build complete! Executables in: $(BUILD_DIR)/"
	@ls -lh $(BUILD_DIR)/*

# Build rules for executables
$(BUILD_DIR)/add_indx: $(SRC_DIR)/add_indx.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(BUILD_DIR)/add_rcrd: $(SRC_DIR)/add_rcrd.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(BUILD_DIR)/bld_file: $(SRC_DIR)/bld_file.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(BUILD_DIR)/chaining: $(SRC_DIR)/chaining.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(BUILD_DIR)/sqntl_rd: $(SRC_DIR)/sqntl_rd.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(BUILD_DIR)/transctn: $(SRC_DIR)/transctn.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(BUILD_DIR)/upd_file: $(SRC_DIR)/upd_file.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Compile source files to object files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Ensure build directory exists before building
$(TARGETS): | $(BUILD_DIR)

# Clean up object files and executables
clean:
	rm -f $(OBJECTS) $(TARGETS)
	rm -rf $(BUILD_DIR)

# Clean up only object files
clean-obj:
	rm -f $(OBJECTS)

# Rebuild everything
rebuild: clean all

# Help target
help:
	@echo "========================================="
	@echo "  CISAM Project - Makefile Targets"
	@echo "========================================="
	@echo "  make              - Build all programs"
	@echo "  make clean        - Remove all build artifacts"
	@echo "  make clean-obj    - Remove only object files"
	@echo "  make rebuild      - Clean and rebuild"
	@echo "  make docker       - Build Docker image"
	@echo "  make docker-run   - Run Docker container"
	@echo "  make help         - Show this help message"
	@echo ""
	@echo "Project Structure:"
	@echo "  demo/src/         - Source code (.c files)"
	@echo "  demo/build/       - Compiled executables"
	@echo "  include/          - Header files (isam.h)"
	@echo "  lib/              - Libraries (llib-lisam)"
	@echo "========================================="

# Docker targets
docker:
	@./docker/build.sh

docker-run:
	@./docker/run.sh

.PHONY: all clean clean-obj rebuild help docker docker-run
