# Compiler and flags
NVCC = nvcc
GCC = gcc

FLAGS=-lSDL2 -lm

# Object files
CU_OBJS = julia_set_cuda.o
C_OBJS = perf_man.o complex.o graphics_utilities.o

# Target executable
TARGET = julia_set_cuda

# All object files
OBJS = $(CU_OBJS) $(C_OBJS)

# Default target
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(NVCC) $(FLAGS) -o $@ $^

# Rule to compile CUDA source files
%.o: %.cu
	$(NVCC) -c -o $@ $<

# Rule to compile C source files
%.o: %.c
	$(GCC) -c -o $@ $<

# Rule to clean up the build directory
clean:
	rm -f $(OBJS) $(TARGET) performance*.log

.PHONY: all clean
