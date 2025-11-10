# Cache Lab Makefile
# Author: Nick Waddoups
# Date: Oct 22, 2025
#
BUILD_DIR ?= build
SUBMISSION_TAR ?= submission-${USER}.tar.gz
WORKING_TAR ?= cachelab_in_progress.tar.gz

# Default target: build the project
all: build

# Build the project in the build/ directory
build:
	@echo "Building cache lab in '${BUILD_DIR}/'..."
	@mkdir -p ${BUILD_DIR}
	@cd ${BUILD_DIR} && cmake -DCMAKE_COMPILE_WARNING_AS_ERROR=OFF .. && make
	@echo "Creating symlinks..."
	@ln -sf ${BUILD_DIR}/csim csim
	@ln -sf ${BUILD_DIR}/test-trans test-trans
	@ln -sf ${BUILD_DIR}/tracegen tracegen

# Clean the build directory
clean:
	@echo "Removing '${BUILD_DIR}/' and 'submission-$(USER).tar.gz'..."
	@rm -rf ${BUILD_DIR}
	@rm -f ${SUBMISSION_TAR}
	@rm -f ${WORKING_TAR}
	@echo "Removing symlinks..."
	@rm -f csim test-trans tracegen

# Create a submission archive
submit: clean
	@echo "Rebuilding with warnings as errors enabled..."
	@mkdir -p ${BUILD_DIR}
	@cd ${BUILD_DIR} && cmake -DCMAKE_COMPILE_WARNING_AS_ERROR=ON .. && make
	@echo "Creating submission archive..."
	@tar -czvf ${SUBMISSION_TAR} csim.c trans.c

# Zip all the work together to transfer it to another computer
zip_all: clean
	@echo "Zipping together all project files (for easy transfer)..."
	@tar --exclude='${WORKING_TAR}' -czvf ${WORKING_TAR} . 
	@echo "Files are all zipped up in ${WORKING_TAR}."

.PHONY: all build clean submit 
