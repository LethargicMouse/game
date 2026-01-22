COMPILE = g++ -lsfml-system -lsfml-window -lsfml-graphics -Wall -Wextra
BUILD = build
NAME = game
GAME = $(BUILD)/$(NAME)
SRC = src
SOURCES = $(SRC)/main.cpp $(SRC)/game.cpp $(SRC)/player.cpp

$(GAME): $(SOURCES)
	$(COMPILE) -o $(GAME) $(SOURCES)

run: $(GAME)
	./$(GAME)

clean:
	rm -rf $(BUILD)/*
