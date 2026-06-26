# Compilador e flags
CC      = gcc
CFLAGS  = -Wall -Wextra -g -std=c11
LDLIBS  = -lm

SRC_DIR = src
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:.c=.o)
TARGET  = grid

# Alvo padrão: roda quando você digita só "make"
all: $(TARGET)

# Linka todos os .o no executável final
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

# Como transformar cada .c em .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Apaga os arquivos gerados
clean:
	rm -f $(OBJECTS) $(TARGET)

# Compila e já executa
run: all
	./$(TARGET)

.PHONY: all clean run