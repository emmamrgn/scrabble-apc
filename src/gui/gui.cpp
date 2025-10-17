#include "gui.hpp"
#include <iostream>
#include <sstream>

GUI::GUI() {}

bool GUI::init() {
    // Create the window
    mWindow.create(sf::VideoMode(mTotalWidth, mTotalHeight), "Scrabble App");

    // Load font - use absolute path or check current directory
    if (!mFont.loadFromFile("../../data/Roboto-Bold.ttf"))
    {
        std::cerr << "Failed to load Roboto-Bold.ttf. Trying default font...\n";
        if (!mFont.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"))
        {
            std::cerr << "Error loading any font. Cannot continue.\n";
            return false;
        }
    }

    // Initialize game status text
    mGameStatusText.setFont(mFont);
    mGameStatusText.setCharacterSize(18);
    mGameStatusText.setFillColor(sf::Color::Black);
    mGameStatusText.setPosition(mBoardWidth + 20, 250);
    mGameStatusText.setString("Game not started");
    
    // Initialize score text
    mScoreText.setFont(mFont);
    mScoreText.setCharacterSize(24);
    mScoreText.setFillColor(sf::Color::Black);
    mScoreText.setPosition(mBoardWidth + 20, mScoreYPos);
    mScoreText.setString("Score: 0");
    
    // Initialize separator
    mSeparator.setSize(sf::Vector2f(2, mTotalHeight));
    mSeparator.setPosition(mBoardWidth, 0);
    mSeparator.setFillColor(sf::Color::Black);
    
    // Initialize grid
    initializeGrid();
    
    // Setup bonus tiles
    setupBonusTiles();
    
    return true;
}

void GUI::initGame() {
    // Initialize bag
    mBag = bag();
    mBag.initialize_scrabble_bag();
    mBag.shuffle_bag();
    
    // Initialize player
    mPlayer = player(mBag);
    mPlayer.reload();
    
    // Initialize board
    mBoard = Board();
    
    // Update UI
    updatePlayerHand(mPlayer);
    updateScore(0);
    mGameStatusText.setString("Game started. Your turn!");
    
    mGameInitialized = true;
    mTurnActive = true;
}

void GUI::initializeGrid() {
    // Same as existing implementation
    mGrid.resize(mGridSize, std::vector<sf::RectangleShape>(mGridSize));
    mGridText.resize(mGridSize, std::vector<sf::Text>(mGridSize));
    
    float cellSize = mBoardWidth / mGridSize;
    
    for (int i = 0; i < mGridSize; ++i) {
        for (int j = 0; j < mGridSize; ++j) {
            mGrid[i][j].setSize(sf::Vector2f(cellSize, cellSize));
            mGrid[i][j].setPosition(i * cellSize, j * cellSize);
            mGrid[i][j].setFillColor(mColorNormal);
            mGrid[i][j].setOutlineColor(sf::Color::Black);
            mGrid[i][j].setOutlineThickness(1.0f);

            mGridText[i][j].setFont(mFont);
            mGridText[i][j].setCharacterSize(static_cast<unsigned int>(cellSize * 0.6f));
            mGridText[i][j].setFillColor(sf::Color::Black);
            mGridText[i][j].setPosition(
                i * cellSize + cellSize * 0.25f,
                j * cellSize + cellSize * 0.15f
            );
        }
    }
}

void GUI::setupBonusTiles() {
    // Set up bonus tiles (DL, TL, DW, TW) according to Scrabble rules
    // Double letter tiles
    std::vector<std::pair<int, int>> dlSpots = {
        {3, 0}, {11, 0}, {6, 2}, {8, 2}, {0, 3}, {7, 3}, {14, 3},
        {2, 6}, {6, 6}, {8, 6}, {12, 6}, {3, 7}, {11, 7},
        {2, 8}, {6, 8}, {8, 8}, {12, 8}, {0, 11}, {7, 11}, {14, 11},
        {6, 12}, {8, 12}, {3, 14}, {11, 14}
    };
    
    for (const auto& spot : dlSpots) {
        mGrid[spot.first][spot.second].setFillColor(mColorDL);
    }
    
    // Triple letter tiles
    std::vector<std::pair<int, int>> tlSpots = {
        {5, 1}, {9, 1}, {1, 5}, {5, 5}, {9, 5}, {13, 5},
        {1, 9}, {5, 9}, {9, 9}, {13, 9}, {5, 13}, {9, 13}
    };
    
    for (const auto& spot : tlSpots) {
        mGrid[spot.first][spot.second].setFillColor(mColorTL);
    }
    
    // Double word tiles
    std::vector<std::pair<int, int>> dwSpots = {
        {1, 1}, {2, 2}, {3, 3}, {4, 4}, {10, 10}, {11, 11}, {12, 12}, {13, 13},
        {13, 1}, {12, 2}, {11, 3}, {10, 4}, {4, 10}, {3, 11}, {2, 12}, {1, 13}
    };
    
    for (const auto& spot : dwSpots) {
        mGrid[spot.first][spot.second].setFillColor(mColorDW);
    }
    
    // Triple word tiles
    std::vector<std::pair<int, int>> twSpots = {
        {0, 0}, {7, 0}, {14, 0}, {0, 7}, {14, 7}, {0, 14}, {7, 14}, {14, 14}
    };
    
    for (const auto& spot : twSpots) {
        mGrid[spot.first][spot.second].setFillColor(mColorTW);
    }
    
    // Start tile (center)
    mGrid[7][7].setFillColor(mColorStart);
}

void GUI::run() {
    while (mWindow.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void GUI::processEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            mWindow.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            // Handle mouse clicks
            if (mGameInitialized && mTurnActive) {
                float cellSize = mBoardWidth / mGridSize;
                int x = static_cast<int>(event.mouseButton.x / cellSize);
                int y = static_cast<int>(event.mouseButton.y / cellSize);
                
                // Check if click is on board
                if (x >= 0 && x < mGridSize && y >= 0 && y < mGridSize) {
                    // If a letter is selected from hand, place it on board
                    if (mSelectedLetterIndex >= 0 && mSelectedLetterIndex < mPlayer.letters.size()) {
                        if (mGridText[x][y].getString().isEmpty()) {
                            // Get the letter from player's hand
                            char letter = mPlayer.letters[mSelectedLetterIndex].car;
                            insertLetter(x, y, letter);
                            
                            // Record this position as part of current word
                            mCurrentWordPositions.push_back({x, y});
                            
                            // Remove the letter from player's hand
                            mPlayer.letters.erase(mPlayer.letters.begin() + mSelectedLetterIndex);
                            
                            // Reset selection
                            mSelectedLetterIndex = -1;
                            
                            // Update hand display
                            updatePlayerHand(mPlayer);
                            
                            mGameStatusText.setString("Letter placed. Select another or press Enter to end turn.");
                        } else {
                            mGameStatusText.setString("This cell already has a letter!");
                        }
                    } else {
                        mGameStatusText.setString("Select a letter from your hand first.");
                    }
                }
                // Check if click is on player's hand
                else if (event.mouseButton.y >= mHandYPos && 
                         event.mouseButton.y <= mHandYPos + mHandTileSize &&
                         event.mouseButton.x >= mBoardWidth + 20 &&
                         event.mouseButton.x <= mBoardWidth + mRightPanelWidth - 20) {
                    
                    // Calculate which tile was clicked
                    float handStartX = mBoardWidth + 20;
                    float handWidth = mRightPanelWidth - 40;
                    int handSize = mPlayer.letters.size();
                    
                    if (handSize > 0) {
                        float tileWidth = handWidth / handSize;
                        
                        int tileIndex = static_cast<int>((event.mouseButton.x - handStartX) / tileWidth);
                        
                        if (tileIndex >= 0 && tileIndex < handSize) {
                            if (mSelectedLetterIndex == tileIndex) {
                                // Clicking the same letter again deselects it
                                mSelectedLetterIndex = -1;
                                mGameStatusText.setString("Letter deselected");
                                
                                // Reset tile colors
                                for (auto& tile : mHandTiles) {
                                    tile.setFillColor(sf::Color(245, 222, 179));
                                }
                            } else {
                                // Set selected letter index
                                mSelectedLetterIndex = tileIndex;
                                
                                // Highlight the selected tile
                                for (size_t i = 0; i < mHandTiles.size(); ++i) {
                                    if (i == mSelectedLetterIndex) {
                                        mHandTiles[i].setFillColor(sf::Color(255, 255, 150)); // Highlight yellow
                                    } else {
                                        mHandTiles[i].setFillColor(sf::Color(245, 222, 179)); // Normal color
                                    }
                                }
                                
                                mGameStatusText.setString("Selected letter: " + 
                                                         std::string(1, mPlayer.letters[tileIndex].car) + 
                                                         ". Now click on the board to place it.");
                            }
                        }
                    }
                }
                // Check if the player clicked the "End Turn" button (if implemented)
                else if (event.mouseButton.x > mBoardWidth + 20 && 
                         event.mouseButton.x < mTotalWidth - 20 &&
                         event.mouseButton.y > 400 && 
                         event.mouseButton.y < 440) {
                    // Only end turn if letters have been placed
                    if (!mCurrentWordPositions.empty()) {
                        playTurn();
                    } else {
                        mGameStatusText.setString("Place at least one letter before ending turn.");
                    }
                }
            } else if (!mGameInitialized) {
                // Check if click is on the "Start Game" area
                if (event.mouseButton.x > mBoardWidth + 20 && 
                    event.mouseButton.x < mTotalWidth - 20 &&
                    event.mouseButton.y > 300 && 
                    event.mouseButton.y < 340) {
                    initGame();
                }
            }
        }
        else if (event.type == sf::Event::KeyPressed) {
            // Handle key presses for game commands
            if (event.key.code == sf::Keyboard::Return && mGameInitialized && mTurnActive && !mCurrentWordPositions.empty()) {
                playTurn();
            }
            else if (event.key.code == sf::Keyboard::Escape && mGameInitialized && mTurnActive) {
                // Clear current word placement
                for (const auto& pos : mCurrentWordPositions) {
                    clearLetter(pos.first, pos.second);
                }
                mCurrentWordPositions.clear();
                
                // Reset selection
                mSelectedLetterIndex = -1;
                
                // Reset tile colors
                for (auto& tile : mHandTiles) {
                    tile.setFillColor(sf::Color(245, 222, 179));
                }
                
                mGameStatusText.setString("Cleared placed letters. Start again.");
            }
        }
    }
}

bool GUI::handleLetterPlacement(int x, int y) {
    // Only place letters if the cell is empty
    if (!mGridText[x][y].getString().isEmpty()) {
        return false;
    }
    
    // Check if player has selected a letter from their hand
    // This would be implemented with another click handler
    // For now, we'll show a message prompting the user
    mGameStatusText.setString("First select a letter from your hand, then click on the board");

    // In a complete implementation:
    // 1. The user would first click on a letter in their hand (track the selected letter index)
    // 2. Then click on the board to place it
    // 3. Move the letter from the player's hand to the board
    return false;
}

void GUI::playTurn() {
    // This is a simplified version of playing a turn
    // In a real implementation, you would:
    // 1. Validate the word placement
    // 2. Calculate the score
    // 3. Update the board state
    // 4. Refill the player's hand
    
    // For demonstration:
    mGameStatusText.setString("Turn completed!");
    
    // Refill player's hand
    mPlayer.reload();
    updatePlayerHand(mPlayer);
    
    // Update score (dummy value for demonstration)
    mPlayerScore += 10;
    updateScore(mPlayerScore);
    
    // Clear the current word positions
    mCurrentWordPositions.clear();
}

void GUI::update() {
    // Logic updates would go here
}

void GUI::render() {
    mWindow.clear(sf::Color::White);
    drawGrid();
    drawSeparator();
    drawScore();
    drawPlayerHand();
    drawGameStatus();
    mWindow.display();
}

void GUI::drawGrid() {
    float cellSize = mBoardWidth / static_cast<float>(mGridSize);
    for (int i = 0; i < mGridSize; ++i) {
        for (int j = 0; j < mGridSize; ++j) {
            mWindow.draw(mGrid[i][j]);
            mWindow.draw(mGridText[i][j]);
        }
    }
}

void GUI::drawPlayerHand() {
    for (size_t i = 0; i < mPlayerHandText.size(); ++i) {
        if (i < mHandTiles.size()) {
            mWindow.draw(mHandTiles[i]);
        }
        mWindow.draw(mPlayerHandText[i]);
    }
    
    // Draw "Start Game" button if game not initialized
    if (!mGameInitialized) {
        sf::RectangleShape startButton;
        startButton.setSize(sf::Vector2f(mRightPanelWidth - 40, 40));
        startButton.setPosition(mBoardWidth + 20, 300);
        startButton.setFillColor(sf::Color(100, 200, 100));
        
        sf::Text buttonText;
        buttonText.setFont(mFont);
        buttonText.setString("Start Game");
        buttonText.setCharacterSize(18);
        buttonText.setFillColor(sf::Color::Black);
        buttonText.setPosition(mBoardWidth + 80, 310);
        
        mWindow.draw(startButton);
        mWindow.draw(buttonText);
    }
}

void GUI::drawScore() {
    mWindow.draw(mScoreText);
}

void GUI::drawGameStatus() {
    mWindow.draw(mGameStatusText);
}

void GUI::drawSeparator() {
    mWindow.draw(mSeparator);
}

void GUI::updatePlayerHand(const player& currentPlayer) {
    mPlayerHandText.clear();
    mHandTiles.clear();
    
    float handStartX = mBoardWidth + 20;
    float handWidth = mRightPanelWidth - 40; // Margin of 20px on each side
    float tileWidth = handWidth / currentPlayer.letters.size();
    float tileHeight = mHandTileSize;
    
    for (size_t i = 0; i < currentPlayer.letters.size(); ++i) {
        // Create tile
        sf::RectangleShape tile;
        tile.setSize(sf::Vector2f(tileWidth - 4, tileHeight - 4)); // 2px margin between tiles
        tile.setPosition(handStartX + i * tileWidth + 2, mHandYPos + 2);
        tile.setFillColor(sf::Color(245, 222, 179)); // Light beige for tiles
        tile.setOutlineColor(sf::Color::Black);
        tile.setOutlineThickness(1.0f);
        mHandTiles.push_back(tile);
        
        // Create the letter text
        sf::Text letterText;
        letterText.setFont(mFont);
        letterText.setString(std::string(1, currentPlayer.letters[i].car));
        letterText.setCharacterSize(static_cast<unsigned int>(tileHeight * 0.6f));
        letterText.setFillColor(sf::Color::Black);
        
        // Center the text on the tile
        float xPos = handStartX + i * tileWidth + tileWidth * 0.35f;
        float yPos = mHandYPos + tileHeight * 0.2f;
        letterText.setPosition(xPos, yPos);
        
        mPlayerHandText.push_back(letterText);
    }
}

// Overload for using vector of chars instead of player object
void GUI::updatePlayerHand(const std::vector<char>& hand) {
    mPlayerHandText.clear();
    mHandTiles.clear();
    
    float handStartX = mBoardWidth + 20;
    float handWidth = mRightPanelWidth - 40;
    float tileWidth = handWidth / hand.size();
    float tileHeight = mHandTileSize;
    
    for (size_t i = 0; i < hand.size(); ++i) {
        sf::RectangleShape tile;
        tile.setSize(sf::Vector2f(tileWidth - 4, tileHeight - 4));
        tile.setPosition(handStartX + i * tileWidth + 2, mHandYPos + 2);
        tile.setFillColor(sf::Color(245, 222, 179));
        tile.setOutlineColor(sf::Color::Black);
        tile.setOutlineThickness(1.0f);
        mHandTiles.push_back(tile);
        
        sf::Text letterText;
        letterText.setFont(mFont);
        letterText.setString(std::string(1, hand[i]));
        letterText.setCharacterSize(static_cast<unsigned int>(tileHeight * 0.6f));
        letterText.setFillColor(sf::Color::Black);
        
        float xPos = handStartX + i * tileWidth + tileWidth * 0.35f;
        float yPos = mHandYPos + tileHeight * 0.2f;
        letterText.setPosition(xPos, yPos);
        
        mPlayerHandText.push_back(letterText);
    }
}

void GUI::updateScore(int score) {
    mPlayerScore = score;
    std::stringstream ss;
    ss << "Score: " << mPlayerScore;
    mScoreText.setString(ss.str());
}

void GUI::updateGrid(const std::vector<std::vector<char>>& board) {
    for (int i = 0; i < mGridSize && i < board.size(); ++i) {
        for (int j = 0; j < mGridSize && j < board[i].size(); ++j) {
            if (board[i][j] == '.') {
                mGridText[i][j].setString("");
            } else {
                mGridText[i][j].setString(std::string(1, board[i][j]));
            }
        }
    }
}

void GUI::insertLetter(int x, int y, char letter) {
    if (x >= 0 && x < mGridSize && y >= 0 && y < mGridSize) {
        mGridText[x][y].setString(std::string(1, letter));
    }
}

void GUI::insertWord(int x, int y, const std::string& word, bool horizontal) {
    if (horizontal) {
        for (size_t i = 0; i < word.length() && x + i < mGridSize; ++i) {
            insertLetter(x + i, y, word[i]);
        }
    } else {
        for (size_t i = 0; i < word.length() && y + i < mGridSize; ++i) {
            insertLetter(x, y + i, word[i]);
        }
    }
}

void GUI::clearLetter(int x, int y) {
    if (x >= 0 && x < mGridSize && y >= 0 && y < mGridSize) {
        mGridText[x][y].setString("");
    }
}