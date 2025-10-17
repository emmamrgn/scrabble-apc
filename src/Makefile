CXX      = g++

.DEFAULT_GOAL := all

# Directories
BIN_DIR = ../bin
OBJ_DIR = ../obj
ELEMENTS_OBJ_DIR = $(OBJ_DIR)/elements
GUI_OBJ_DIR = $(OBJ_DIR)/gui

# Create directories if they don't exist
$(shell mkdir -p $(BIN_DIR) $(ELEMENTS_OBJ_DIR) $(GUI_OBJ_DIR) $(OBJ_DIR))

# Source files
SOURCES = 
SOURCES += elements/bonus.cpp
SOURCES += elements/spot.cpp
SOURCES += elements/player.cpp
SOURCES += elements/letter.cpp
SOURCES += elements/bag.cpp
SOURCES += elements/board.cpp
SOURCES += elements/NoeudGaddag.cpp
SOURCES += elements/Gaddag.cpp

# Generate object file paths
OBJECTS = $(SOURCES:%.cpp=$(OBJ_DIR)/%.o)

CXXFLAGS  += -g -Wall -std=c++11 -pedantic -O3
LDFLAGS +=

# Pattern rule for object files
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -MMD $(CXXFLAGS) -c $< -o $@

CLEAN_OBJECTS = $(OBJECTS)
TARGETS = 

########## test_board ##########

BOARD_SOURCES = test_board.cpp
BOARD_OBJECTS = $(BOARD_SOURCES:%.cpp=$(OBJ_DIR)/%.o)
BOARD_TARGET = $(BIN_DIR)/test_board

test_board: $(BOARD_OBJECTS) $(OBJECTS)
	$(CXX) $(BOARD_OBJECTS) $(OBJECTS) -o $(BOARD_TARGET) $(LDFLAGS)
    
$(BOARD_OBJECTS): $(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -MMD $(CXXFLAGS) -c $< -o $@

TARGETS += $(BOARD_TARGET)
CLEAN_OBJECTS += $(BOARD_OBJECTS)

########## gui_board ##########

GUI_DIR = gui
GUI_SOURCES = $(GUI_DIR)/main_gui.cpp $(GUI_DIR)/gui.cpp $(GUI_DIR)/board_gui.cpp
GUI_OBJECTS = $(GUI_SOURCES:%.cpp=$(OBJ_DIR)/%.o)
GUI_TARGET = $(BIN_DIR)/scrabble_app
GUI_LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

gui_board: $(OBJECTS) $(GUI_OBJECTS)
	$(CXX) $(GUI_OBJECTS) $(OBJECTS) -o $(GUI_TARGET) $(LDFLAGS) $(GUI_LDFLAGS)

$(GUI_OBJECTS): $(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -MMD $(CXXFLAGS) -c $< -o $@

TARGETS += $(GUI_TARGET)
CLEAN_OBJECTS += $(GUI_OBJECTS)

########## all and clean ##########

all: test_board gui_board

DEPS = $(CLEAN_OBJECTS:.o=.d)

clean:
	@rm -f $(DEPS) $(CLEAN_OBJECTS)
	@rm -f $(TARGETS)
	@echo "Cleaned all generated files"

-include $(DEPS)