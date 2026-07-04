LIB_NAME := aeco

CC := gcc
AS := fasm
AR := ar

SRC_DIR := src
BUILD_DIR := build
INC_DIR := include
LIB_DIR := lib
TEST_DIR := test

CPPFLAGS := -I$(INC_DIR)
CFLAGS := -Wall -Wextra -g -MMD -MP

C_SRC := $(wildcard $(SRC_DIR)/*.c)
ASM_SRC := $(wildcard $(SRC_DIR)/*.asm)
TEST_SRC := $(wildcard $(TEST_DIR)/*.c)

STATIC_C_OBJ := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/static/%.o,$(C_SRC))
STATIC_ASM_OBJ := $(patsubst $(SRC_DIR)/%.asm,$(BUILD_DIR)/static/%.o,$(ASM_SRC))

SHARED_C_OBJ := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/shared/%.o,$(C_SRC))
SHARED_ASM_OBJ := $(patsubst $(SRC_DIR)/%.asm,$(BUILD_DIR)/shared/%.o,$(ASM_SRC))

TEST_OBJ := $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/test/%.o,$(TEST_SRC))
TEST_BIN := $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/test/%,$(TEST_SRC))

STATIC_LIB := $(LIB_DIR)/lib$(LIB_NAME).a
SHARED_LIB := $(LIB_DIR)/lib$(LIB_NAME).so

DEPS := \
	$(STATIC_C_OBJ:.o=.d) \
	$(SHARED_C_OBJ:.o=.d) \
	$(TEST_OBJ:.o=.d)

.PHONY: all
all: static shared test

.PHONY: static
static: $(STATIC_LIB)

.PHONY: shared
shared: $(SHARED_LIB)

.PHONY: test
test: $(TEST_BIN)

.PHONY: run-tests
run-tests: test
	@set -e; \
	for t in $(TEST_BIN); do \
		echo "---- $$t ----"; \
		./$$t; \
	done

$(STATIC_LIB): $(STATIC_C_OBJ) $(STATIC_ASM_OBJ)
	@mkdir -p $(LIB_DIR)
	$(AR) rcs $@ $^

$(SHARED_LIB): $(SHARED_C_OBJ) $(SHARED_ASM_OBJ)
	@mkdir -p $(LIB_DIR)
	$(CC) -shared -o $@ $^

$(BUILD_DIR)/static/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/shared/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -c $< -o $@

$(BUILD_DIR)/static/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(AS) $< $@

$(BUILD_DIR)/shared/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(AS) $< $@

$(BUILD_DIR)/test/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/test/%: $(BUILD_DIR)/test/%.o $(STATIC_LIB)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $< $(STATIC_LIB)

-include $(DEPS)

.PHONY: lsp
lsp:
	@rm -f compile_commands.json
	bear -- $(MAKE) clean all

.PHONY: debug
debug:
	@echo "CC             = $(CC)"
	@echo "AS             = $(AS)"
	@echo "AR             = $(AR)"
	@echo "SRC_DIR        = $(SRC_DIR)"
	@echo "BUILD_DIR      = $(BUILD_DIR)"
	@echo "INC_DIR        = $(INC_DIR)"
	@echo "LIB_DIR        = $(LIB_DIR)"
	@echo "TEST_DIR       = $(TEST_DIR)"
	@echo "C_SRC          = $(C_SRC)"
	@echo "ASM_SRC        = $(ASM_SRC)"
	@echo "TEST_SRC       = $(TEST_SRC)"
	@echo "STATIC_C_OBJ   = $(STATIC_C_OBJ)"
	@echo "STATIC_ASM_OBJ = $(STATIC_ASM_OBJ)"
	@echo "SHARED_C_OBJ   = $(SHARED_C_OBJ)"
	@echo "SHARED_ASM_OBJ = $(SHARED_ASM_OBJ)"
	@echo "TEST_OBJ       = $(TEST_OBJ)"
	@echo "TEST_BIN       = $(TEST_BIN)"
	@echo "DEPS           = $(DEPS)"

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR)
