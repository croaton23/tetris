COMPILER = g++
LIBS = -lncurses -pthread
DEPENDENCIES = tetris_math.h
OUTPUT_DIR = obj

_OBJ = tetris.o
OBJ = $(patsubst %,$(OUTPUT_DIR)/%,$(_OBJ))

$(OUTPUT_DIR)/%.o: %.cpp $(DEPENDENCIES)
	$(COMPILER) -c -o $@ $<

tetris: $(OBJ)
	$(COMPILER) -o $@ $^ $(LIBS)