# Copyright (C) 2024  KA Wright

# Makefile - Build script

BUILD_DIR		:=	.build
OBJ_DIR			:=	$(BUILD_DIR)/obj
BIN_DIR			:=	$(BUILD_DIR)/bin
ASSET_DIR		:=	asset
SRC_DIR			:=	src
INSTALL_DIR		:=	/usr/local/bin

OBJS			:=	$(OBJ_DIR)/argv.o		\
					$(OBJ_DIR)/asset.o		\
					$(OBJ_DIR)/dasm.o		\
					$(OBJ_DIR)/err.o		\
					$(OBJ_DIR)/about.txt.o	\
					$(OBJ_DIR)/help.txt.o

FLAGS			:=	-Isrc/include			\
					-Wall					\
					-g

$(BIN_DIR)/k8d: $(SRC_DIR)/main.c $(OBJS)
	@echo 'BUILDING BINARY      [$@]'
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)
	@gcc $(FLAGS) $(OBJS) $(SRC_DIR)/main.c -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo 'BUILDING OBJECT      [$@]'
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(OBJ_DIR)
	@gcc $(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(ASSET_DIR)/%
	@echo 'LINKING ASSET        [$@]'
	@ld -r -b binary -o $@ $<

.PHONY: install
install: $(BIN_DIR)/k8d
	@cp $< $(INSTALL_DIR)/k8d
	@echo DONE!

.PHONY: uninstall
uninstall:
	@rm -f $(INSTALL_DIR)/k8d

.PHONY: all
all: $(BIN_DIR)/k8d
	@echo DONE!

.PHONY: clean
clean:
	@echo CLEANING BUILD ARTIFACTS
	@rm -rf $(BUILD_DIR)
	@echo DONE!
