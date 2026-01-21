COMPILE = g++ -lsfml-system -lsfml-window -lsfml-graphics -Wall -Wextra
BUILD = build
GAME = $(BUILD)/game

$(GAME): game.cpp
	$(COMPILE) -o $(GAME) game.cpp

run: $(GAME)
	./$(GAME)

clean:
	rm -rf $(BUILD)/*
