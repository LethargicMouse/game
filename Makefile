COMPILE = g++ -lsfml-system -lsfml-window -lsfml-graphics -Wall -Wextra -std=c++20 -g
BUILD = build
NAME = game
GAME = $(BUILD)/$(NAME)
SRC = src
SOURCES = $(SRC)/*
CPP_FILES = $(SRC)/*.cpp

$(GAME): $(SOURCES)
	$(COMPILE) -o $(GAME) $(CPP_FILES)

run: $(GAME)
	./$(GAME)

clean:
	rm -rf $(BUILD)/*
