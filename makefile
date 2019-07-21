COMPILER = g++
LIBS = -lncurses
DEPENDENCIES = 	tetris.h	 	\
				tetris_math.h	\
				figure.h	 	\
				field.h
OUTPUT_DIR = obj

_OBJ = main.o tetris.o
OBJ = $(patsubst %,$(OUTPUT_DIR)/%,$(_OBJ))

$(OUTPUT_DIR)/%.o: %.cpp $(DEPENDENCIES)
	$(COMPILER) -c -o $@ $<

tetris: $(OBJ)
	$(COMPILER) -o $@ $^ $(LIBS)