CC := cc
TARGET := blashphemous

SRC_DIR := src
INC_DIR := include
BUILD_DIR := build

rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

SRCS := $(call rwildcard,$(SRC_DIR)/,*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

CFLAGS := -std=c99 -Wall -Wextra -I$(INC_DIR) $(shell pkg-config --cflags raylib)
LDFLAGS := $(shell pkg-config --libs raylib)

.PHONY: all run clean debug release

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

debug: CFLAGS += -g -O0 -DDEBUG
debug: all

release: CFLAGS += -O2
release: all

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
