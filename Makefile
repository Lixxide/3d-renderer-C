# Compiler
CC = x86_64-w64-mingw32-gcc

# Project folders
SRC = .
OUT = dist
DATA = data

# Executable name
TARGET = $(OUT)/app.exe

SDL_PATH = SDL2/x86_64-w64-mingw32
# SDLimg_PATH = SDL2_image
# SDLmix_PATH = SDL2_mixer/x86_64-w64-mingw32

# Compiler flags
# 
# SDL -L$(SDL_PATH)/lib -lSDL3  -L$(SDLimg_PATH)/lib -lSDL3_image -mwindows

CFLAGS =  -O3 -I$(SDL_PATH)/include/
LDFLAGS =  -lmingw32 -L$(SDL_PATH)/lib/ -mwindows -lSDL2

# Source files and object files
SRCS = $(wildcard $(SRC)/*.c)
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link final executable
$(TARGET): $(OBJS)
	@mkdir -p $(OUT)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compile .c to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(SRC)/*.o $(TARGET)

# Run the program (optional)
run: $(TARGET)
	./$(TARGET)
