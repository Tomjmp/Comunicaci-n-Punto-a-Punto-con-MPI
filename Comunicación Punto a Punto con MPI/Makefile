# =============================================================================
# Makefile - Comunicacion Punto a Punto con MPI
# Compilador: mpicc (wrapper de gcc/clang con soporte MPI)
# =============================================================================

CC      = mpicc
CFLAGS  = -Wall -Wextra -O2
TARGET  = comunicacion
SRC     = src/comunicacion_punto_a_punto.c
NP      = 2

.PHONY: all run clean

# Regla por defecto: compila el programa
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Ejecuta el programa con 2 procesos MPI
run: $(TARGET)
	mpirun -np $(NP) ./$(TARGET)

# Elimina el binario compilado
clean:
	rm -f $(TARGET)
