#ifndef GUI_HPP
#define GUI_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "../elements/player.hpp"
#include "../elements/bag.hpp"
#include "../elements/board.hpp"

class GUI {
public:
    GUI();
    bool init();
    void updateGrid(const std::vector<std::vector<char>> &grid);
    void run();
    bool isOpen() const { return mWindow.isOpen(); }
    void processEvents();
    void update();
    void render();
    sf::RenderWindow& getWindow() { return mWindow; }

    void insertLetter(int x, int y, char letter);
    void insertWord(int x, int y, const std::string& word, bool horizontal);
    void clearLetter(int x, int y);
    void updatePlayerHand(const player& currentPlayer);
    void updatePlayerHand(const std::vector<char>& hand); 
    void updateScore(int score);
    
    // New methods for game interaction
    void initGame();
    void playTurn();
    bool handleLetterPlacement(int x, int y);
    
private:
    sf::RenderWindow mWindow;
    sf::Font mFont;
    std::vector<std::vector<sf::RectangleShape>> mGrid;
    std::vector<std::vector<sf::Text>> mGridText;
    std::vector<sf::RectangleShape> mHandTiles;
    std::vector<sf::Text> mPlayerHandText;
    sf::RectangleShape mSeparator;
    sf::Text mScoreText;
    sf::Text mGameStatusText;
    int mPlayerScore = 0;
    int mSelectedLetterIndex = -1;
    
    // Game state variables
    Board mBoard;
    bag mBag;
    player mPlayer;
    bool mGameInitialized = false;
    bool mTurnActive = false;
    std::vector<std::pair<int, int>> mCurrentWordPositions;
    
    // Grid properties
    const int mGridSize = 15;
    const float mTileSize = 40.0f;
    const float mHandTileSize = 50.0f;
    const float mBoardWidth = 600.0f;
    const float mRightPanelWidth = 300.0f;
    const float mTotalWidth = mBoardWidth + mRightPanelWidth;
    const float mTotalHeight = 700.0f;
    const float mScoreYPos = 50.0f;
    const float mHandYPos = 150.0f;
    
    // Tile colors
    const sf::Color mColorNormal = sf::Color(240, 240, 240);
    const sf::Color mColorDL = sf::Color(173, 216, 230);
    const sf::Color mColorTL = sf::Color(0, 0, 255);
    const sf::Color mColorDW = sf::Color(255, 192, 203);
    const sf::Color mColorTW = sf::Color(255, 0, 0);
    const sf::Color mColorStart = sf::Color(255, 192, 203);

    void initializeGrid();
    void drawGrid();
    void drawPlayerHand();
    void drawScore();
    void drawGameStatus();
    void drawSeparator();
    void setupBonusTiles();
};

#endif // GUI_HPP