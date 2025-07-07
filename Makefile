LIB_NAME = retro-crypto

BASE_SOURCE_DIR = src/

LIB_DIR = retro/

LIB_SRC = $(BASE_SOURCE_DIR)$(LIB_DIR)

CRYPTO_DIR = crypto/

CRYPTO_SRC = $(BASE_SOURCE_DIR)$(CRYPTO_DIR)

CLI_DIR = cli/

CLI_SRC = $(BASE_SOURCE_DIR)$(CLI_DIR)

N64_DIR = n64

N64_SRC = $(BASE_SOURCE_DIR)$(N64_DIR)/

BASE_BUILD_DIR = build/

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
LIB_OBJECTS = $(foreach x, $(basename $(LIB_SOURCES)), $(BASE_BUILD_DIR)$(patsubst $(BASE_SOURCE_DIR)%,%,$(x).o))

CRYPTO_SOURCES = $(shell find $(CRYPTO_SRC) -type f -iname '*.c')
CRYPTO_OBJECTS = $(foreach x, $(basename $(CRYPTO_SOURCES)), $(BASE_BUILD_DIR)$(patsubst $(BASE_SOURCE_DIR)%,%,$(x).o))

CLI_SOURCES = $(shell find $(CLI_SRC) -type f -iname '*.cpp')
CLI_OBJECTS = $(foreach x, $(basename $(CLI_SOURCES)), $(BASE_BUILD_DIR)$(patsubst $(BASE_SOURCE_DIR)%,%,$(x).o))

N64_SOURCES = $(shell find $(N64_SRC) -type f -iname '*.cpp')
N64_OBJECTS = $(foreach x, $(basename $(N64_SOURCES)), $(BASE_BUILD_DIR)$(patsubst $(BASE_SOURCE_DIR)%,%,$(x).o))
N64_OBJECTS += $(LIB_OBJECTS)
N64_OBJECTS += $(CRYPTO_OBJECTS)

N64_TARGET = $(findstring $(MAKECMDGOALS), n64)
ifeq ($(N64_TARGET), n64)
	SOURCE_DIR = $(BASE_SOURCE_DIR)$(N64_DIR)
	BUILD_DIR = $(BASE_BUILD_DIR)$(N64_DIR)
	include $(N64_INST)/include/n64.mk
endif

$(BASE_BUILD_DIR)$(LIB_DIR)%.o: $(LIB_SRC)%.cpp
	@mkdir -p "$(dir $@)"
	@$(CXX) $(CXXFLAGS) -c $^ -o $@

$(BASE_BUILD_DIR)$(CRYPTO_DIR)%.o: $(CRYPTO_SRC)%.c
	@mkdir -p "$(dir $@)"
	@$(CC) $(CCFLAGS) -c $^ -o $@

$(BASE_BUILD_DIR)$(CLI_DIR)%.o: $(CLI_SRC)%.cpp
	@mkdir -p "$(dir $@)"
	@$(CXX) $(CXXFLAGS) -c $^ -o $@

build-retro-lib: $(CRYPTO_OBJECTS) $(LIB_OBJECTS)

cli: clean $(CRYPTO_OBJECTS) $(LIB_OBJECTS) $(CLI_OBJECTS)
	@mkdir -p "$(BINARY_DIR)"
	$(CXX) $(CXXFLAGS) $(CRYPTO_OBJECTS) $(LIB_OBJECTS) $(CLI_OBJECTS) -o $(BINARY_DIR)$(LIB_NAME)

n64: clean retro-crypto-n64.z64
	@mkdir -p "$(BINARY_DIR)"
	@mv retro-crypto-n64.z64 "$(BINARY_DIR)"

$(BUILD_DIR)/retro-crypto-n64.elf: $(N64_OBJECTS)

retro-crypto-n64.z64: N64_ROM_TITLE="Retro Crypto N64"

clean:
	rm -fR $(BASE_BUILD_DIR) $(BINARY_DIR)
	rm -fr $(N64_DIR) retro-crypto-n64.z64
