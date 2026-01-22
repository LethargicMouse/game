COMPILE = g++ -lsfml-system -lsfml-window -lsfml-graphics -Wall -Wextra -std=c++20

BUILD = build
NAME = game
SRC = src
GAME = $(BUILD)/game

# freaking magic
CPPS = $(wildcard $(SRC)/*.cpp)
OBJECTS = $(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$(CPPS))

$(BUILD)/game: $(OBJECTS)
	$(COMPILE) -o $(GAME) $(OBJECTS)

# freakier magic
$(BUILD)/%.o: $(SRC)/%.cpp $(SRC)/%.h
	$(COMPILE) -c $< -o $@

run: $(GAME)
	./$(GAME)

clean:
	rm -rf $(BUILD)/*
