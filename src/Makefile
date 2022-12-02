# The compiler I will use to compile
COMPILER := gcc

# The file with main function
SRCS := main.c ../include/mathHelper.c ../include/helper.c

# The output application name
OUTPUT := game

# Header folder
HEADERS := -I../Include

# Links folder
LINKS := -L../lib

# Libraries to link
LINKFILES := -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

# The final command
all:
	$(COMPILER) $(HEADERS) $(LINKS) -o $(OUTPUT) $(SRCS) $(LINKFILES)