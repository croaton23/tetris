COMPILER = g++
LIBS = -lncursesw
DEPENDENCIES = 	tetris.h	 	\
				tetris_math.h	\
				figure.h	 	\
				field.h
OUTPUT_DIR = obj

_OBJ = tetris.o main.o 
OBJ = $(patsubst %,$(OUTPUT_DIR)/%,$(_OBJ))

$(OUTPUT_DIR)/%.o: %.cpp $(DEPENDENCIES)
	$(COMPILER) -g -c -o $@ $<

tetris: $(OBJ)
	$(COMPILER) -g -o $@ $^ $(LIBS)

clean:
	rm -f $(OUTPUT_DIR)/*.o ./tetris 