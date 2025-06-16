LIB_NAME = retro-crypto

SOURCE_DIR = src/

LIB_DIR = retro/

LIB_SRC = $(SOURCE_DIR)$(LIB_DIR)

CRYPTO_DIR = crypto/

CRYPTO_SRC = $(SOURCE_DIR)$(CRYPTO_DIR)

CLI_DIR = cli/

CLI_SRC = $(SOURCE_DIR)$(CLI_DIR)

BUILD_DIR = build/

BINARY_DIR = bin/

INCLUDE += -I$(LIB_SRC)
INCLUDE += -I$(LIB_SRC)interfaces
INCLUDE += -I$(CRYPTO_SRC)

CXXFLAGS = $(INCLUDE) -DUSE_MONERO=1 -DUSE_KECCAK=1
CCFLAGS = $(INCLUDE) -DUSE_MONERO=1 -DUSE_KECCAK=1

CLI_TARGET = $(findstring $(MAKECMDGOALS), cli)
ifeq ($(CLI_TARGET), cli)
	CXXFLAGS += -std=c++11 -pthread
endif

LIB_SOURCES = $(shell find $(LIB_SRC) -type f -iname '*.cpp')
LIB_OBJECTS = $(foreach x, $(basename $(LIB_SOURCES)), $(BUILD_DIR)$(patsubst $(SOURCE_DIR)%,%,$(x).o))

CRYPTO_SOURCES = $(shell find $(CRYPTO_SRC) -type f -iname '*.c')
CRYPTO_OBJECTS = $(foreach x, $(basename $(CRYPTO_SOURCES)), $(BUILD_DIR)$(patsubst $(SOURCE_DIR)%,%,$(x).o))

CLI_SOURCES = $(shell find $(CLI_SRC) -type f -iname '*.cpp')
CLI_OBJECTS = $(foreach x, $(basename $(CLI_SOURCES)), $(BUILD_DIR)$(patsubst $(SOURCE_DIR)%,%,$(x).o))

$(BUILD_DIR)$(LIB_DIR)%.o: $(LIB_SRC)%.cpp
	@mkdir -p "$(dir $@)"
	@$(CXX) $(CXXFLAGS) -c $^ -o $@

$(BUILD_DIR)$(CRYPTO_DIR)%.o: $(CRYPTO_SRC)%.c
	@mkdir -p "$(dir $@)"
	@$(CC) $(CCFLAGS) -c $^ -o $@

$(BUILD_DIR)$(CLI_DIR)%.o: $(CLI_SRC)%.cpp
	@mkdir -p "$(dir $@)"
	@$(CXX) $(CXXFLAGS) -c $^ -o $@

build-retro-lib: $(CRYPTO_OBJECTS) $(LIB_OBJECTS)

cli: clean $(CRYPTO_OBJECTS) $(LIB_OBJECTS) $(CLI_OBJECTS)
	@mkdir -p "$(BINARY_DIR)"
	$(CXX) $(CXXFLAGS) $(CRYPTO_OBJECTS) $(LIB_OBJECTS) $(CLI_OBJECTS) -o $(BINARY_DIR)$(LIB_NAME)

clean:
	rm -fR $(BUILD_DIR) $(BINARY_DIR)
