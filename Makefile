##
## EPITECH PROJECT, 2025
## R-Type
## File description:
## Makefile
##

ifeq ($(OS), Windows_NT)
CONAN			:= $(shell where conan)
else
CONAN			:= $(shell command -v conan)
endif

CONAN_PRESET	:= conan-release

BUILD_DIR		:= build

BUILD_TYPE		:= Release

.PHONY:	all
all:	build

.PHONY:	debug
debug:
	@$(MAKE) -s BUILD_TYPE=Debug CONAN_PRESET=conan-debug

.PHONY:	install
install:	check_conan
	@conan profile detect -e
	@conan install . --output-folder=$(BUILD_DIR) --build=missing --settings=build_type=${BUILD_TYPE} -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True

.PHONY:	configure
configure:
ifeq ($(OS), Windows_NT)
	@cmake --preset conan-default -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
else
	@cmake --preset $(CONAN_PRESET) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
endif

.PHONY:	build
build:	install configure
	@cmake --build $(BUILD_DIR) --preset $(CONAN_PRESET) --config $(BUILD_TYPE)

.PHONY:	tests_run
tests_run:
	@ctest --build-config $(BUILD_TYPE)

.PHONY:	clean
clean:
ifeq ($(OS), Windows_NT)
	@if exist build (rmdir /s /q build)
else
	@rm -rf $(BUILD_DIR)
endif

.PHONY:	re
re:	fclean all

.PHONY:	fclean
fclean:	clean
ifeq ($(OS), Windows_NT)
	@if exist CMakeUserPresets.json (del /q CMakeUserPresets.json)
	@if exist conan_provider.cmake (del /q conan_provider.cmake)
	@if exist Testing (rmdir /s /q Testing)
else
	@rm -f CMakeUserPresets.json
	@rm -f conan_provider.cmake
	@rm -rf Testing
endif

.PHONY:	help
help:
	@echo "Available targets:"
	@echo "    all             Install dependencies, configure, and build."
	@echo "    debug           Same as 'all', but with a 'Debug' build type. 'BUILT_TYPE=<Type>' and 'CONAN_PRESET=conan-<type>' can also be append to any target."
	@echo "    install         Install dependencies using Conan."
	@echo "    configure       Configure the project using CMake."
	@echo "    build           Build the project using CMake."
	@echo "    tests_run       Run the tests of the project."
	@echo "    clean           Clean the build directory."
	@echo "    re              Clean the build directory, install dependencies, configure, and build."
	@echo "    fclean          Force clean (remove all generated files)."
	@echo "    help            Display this help and exit."

check_conan:
ifndef CONAN
	@echo "Conan not found, trying to install it..."
	@pip install conan || (echo "Could not install Conan, please install it manually: https://conan.io/downloads." && exit 1)
endif
