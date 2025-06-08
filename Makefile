LIB_NAME = retro-crypto

SOURCE_DIR = src/

LIB_DIR = retro/

LIB_SRC = $(SOURCE_DIR)$(LIB_DIR)

CLI_DIR = cli/

CLI_SRC = $(SOURCE_DIR)$(CLI_DIR)

BUILD_DIR = build/

BINARY_DIR = bin/

INCLUDE += -I$(LIB_SRC)
INCLUDE += -I$(LIB_SRC)interfaces

CXXFLAGS = $(INCLUDE)

LIB_SOURCES = $(shell find $(LIB_SRC) -type f -iname '*.cpp')
LIB_OBJECTS = $(foreach x, $(basename $(LIB_SOURCES)), $(BUILD_DIR)$(patsubst $(SOURCE_DIR)%,%,$(x).o))

CLI_SOURCES = $(shell find $(CLI_SRC) -type f -iname '*.cpp')
CLI_OBJECTS = $(foreach x, $(basename $(CLI_SOURCES)), $(BUILD_DIR)$(patsubst $(SOURCE_DIR)%,%,$(x).o))

$(BUILD_DIR)$(LIB_DIR)%.o: $(LIB_SRC)%.cpp
	@mkdir -p "$(dir $@)"
	@$(CXX) $(CXXFLAGS) -c $^ -o $@

$(BUILD_DIR)$(CLI_DIR)%.o: $(CLI_SRC)%.cpp
	@mkdir -p "$(dir $@)"
	@$(CXX) $(CXXFLAGS) -c $^ -o $@

build-retro-lib: $(LIB_OBJECTS)

cli: clean $(LIB_OBJECTS) $(CLI_OBJECTS)
	@mkdir -p "$(BINARY_DIR)"
	$(CXX) $(CXXFLAGS) $(LIB_OBJECTS) $(CLI_OBJECTS) -o $(BINARY_DIR)$(LIB_NAME)

clean:
	rm -fR $(BUILD_DIR) $(BINARY_DIR)
