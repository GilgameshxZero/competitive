SHELL := /bin/bash

# Directories relative paths.
ROOT_DIR := ../
OBJ_DIR := $(ROOT_DIR)obj/
BIN_DIR := $(ROOT_DIR)bin/

PROJECT_NAME := $(PROJECT)
PROJECT_SRC_DIR := $(ROOT_DIR)$(PROJECT_NAME)/

PROJECT_SRCS := $(wildcard $(PROJECT_SRC_DIR)$(FILES).cpp)
PROJECT_SRC_NAMES := $(basename $(notdir $(PROJECT_SRCS)))
PROJECT_OBJS := $(addprefix $(OBJ_DIR)$(PROJECT_NAME)/, \
	$(addsuffix .o, $(PROJECT_SRC_NAMES)))
PROJECT_BIN := $(BIN_DIR)$(PROJECT_NAME)

# Flags for build. Debug by default.
CXX ?= clang++
CXXFLAGS := -std=c++17 -m64 -pthread -Wall -Wextra -D_CONSOLE
CXXFLAGS_DEBUG := -D _DEBUG -g -O0
CXXFLAGS_RELEASE := -D NDEBUG -Ofast
CXXFLAGS_INSTRUMENT :=
LDLIBS := -pthread -lresolv
LDLIBS_INSTRUMENT := -fsanitize=address,undefined,leak

# Default to DEBUG builds. Check if RELEASE=1 is set.
ifeq ($(RELEASE), 1)
	CXXFLAGS := $(CXXFLAGS) $(CXXFLAGS_RELEASE)
else
	CXXFLAGS := $(CXXFLAGS) $(CXXFLAGS_DEBUG)
endif

ifeq ($(INSTRUMENT), 1)
	CXXFLAGS := $(CXXFLAGS) $(CXXFLAGS_INSTRUMENT)
	LDLIBS := $(LDLIBS) $(LDLIBS_INSTRUMENT)
endif

# -lstdc++fs is included by default on MacOS and cannot be specified on the \
	command line.
UNAME_S := $(shell uname -s)
ifneq ($(UNAME_S), Darwin)
	LDLIBS := $(LDLIBS) -lstdc++fs
endif

# Keep intermediate and output files.
.SECONDARY: $(PROJECT_OBJS) $(PROJECT_BIN)

# Mark .PHONY for rules to avoid compiling files named the same as rules.

# Rules for single tests just redirect to test binaries.
# Enable second expansion for some fancy pattern matching later.
.SECONDEXPANSION:
$(PROJECT_NAME): $(BIN_DIR)$$@

# Run a single project whose name is specified by PROJECT.
.PHONY: run
run: $(PROJECT_NAME)
	@cd $(BIN_DIR) && ./$(PROJECT_NAME) $(ARGS)

# Delete intermediate files.
.PHONY: clean
clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR)

# Static rules to nest one directory deep inside ../obj.
# Link rule.
$(PROJECT_BIN): $(BIN_DIR)% : $(PROJECT_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(PROJECT_OBJS) $(LDLIBS) -o $@

# Compile rule. Stem % matchines source file name.
$(PROJECT_OBJS): $(OBJ_DIR)%.o : $(ROOT_DIR)$$*.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@
