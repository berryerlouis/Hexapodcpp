
# Compiler and linker settings
CXX = avr-g++
OBJCOPY = avr-objcopy
MCU = atmega1284p
F_CPU = 20000000
CXXFLAGS = -Wall -Wextra -std=gnu++17 -fno-exceptions -ffunction-sections -fdata-sections -fno-rtti -fno-threadsafe-statics -mmcu=$(MCU) -DF_CPU=$(F_CPU)UL -O1 -g -ftest-coverage
SRC_DIR = src
BUILD_DIR = build
TARGET = $(BUILD_DIR)/main
# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
SRC_FILES += $(wildcard $(SRC_DIR)/*/*.cpp)

# Object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Rules
all: $(TARGET).hex

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

$(TARGET).elf: $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

flash: $(HEXFILE)
	bin/stack/avrdude -c arduino -P COM4 -b 500000 -p m1284p -U flash:w:$(TARGET).elf

clean:
	rm -rf $(BUILD_DIR)/*

.PHONY: all clean