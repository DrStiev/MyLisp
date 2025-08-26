# Makefile per compilare lispy.c e le sue dipendenze

# Variabili
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = lispy
SRC_DIR = src
BUILD_DIR = build

# Trova tutti i file .c ricorsivamente nella cartella src
SOURCES = $(shell find $(SRC_DIR) -name "*.c")
# Crea i nomi degli oggetti corrispondenti nella cartella build
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Target principale
all: $(TARGET)

# Regola per creare l'eseguibile
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ -ledit -lm

# Regola per compilare i file .c in .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Pulizia
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)

# Ricompilazione completa
rebuild: clean all

# Mostra le variabili (utile per debug)
debug:
	@echo "Sources: $(SOURCES)"
	@echo "Objects: $(OBJECTS)"
	@echo "Target: $(TARGET)"

# I target che non sono file
.PHONY: all clean rebuild debug