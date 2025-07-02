#include <M5Unified.h>
#include <Preferences.h>
#include <vector>

// =============================================================================
// Color Palette Definition (RGB565)
// =============================================================================
#define BGCOLOR     0xFFBD  // #faf8ef - Background color
#define BOARDCOLOR  0xBDF3  // #bbada0 - Board color
#define SCORE_TEXT  0x738E  // #776e65 - Score text color
#define TITLE_COLOR 0x938E  // #8f7a66 - Title color
#define WHITECOLOR  0xFFFF  // White
#define NUM_WHITE   0xFFFE  // #f9f6f2 - Number white

// Panel background colors
const uint16_t PANEL_COLORS[] = {
  0xDEFB, // Empty panel #cdc1b4
  0xFFF6, // 2          #eee4da
  0xF7B2, // 4          #ede0c8
  0xFD56, // 8          #f2b179
  0xFC2F, // 16         #f59563
  0xFB8F, // 32         #f67c5f
  0xF90B, // 64         #f65e3b
  0xF6F2, // 128        #edcf72
  0xF6A4, // 256        #edcc61
  0xF624, // 512        #edc850
  0xF5C3, // 1024       #edc53f
  0xF4C8  // 2048       #edc22e
};

// Number colors
const uint16_t NUMBER_COLORS[] = {
  0x738E, // 0: #776e65
  0x738E, // 2: #776e65
  0x738E, // 4: #776e65
  0xFFFE, // 8: #f9f6f2
  0xFFFE, // 16 and above are all white
  0xFFFE, 0xFFFE, 0xFFFE, 0xFFFE, 0xFFFE, 0xFFFE, 0xFFFE
};

// =============================================================================
// Game Constants
// =============================================================================
#define BOARD_SIZE       16     // 4x4 panel count
#define GAME_LOSE        -1     // Game over
#define GAME_WIN         100    // Game clear
#define NEW_GAME_BUTTON  10     // NEW GAME button ID

// Font size thresholds for different number lengths
namespace FontThresholds {
  const int SHORT_NUMBER_MAX_LENGTH = 3;   // 1-3 digits: use Font8
  const int MEDIUM_NUMBER_MAX_LENGTH = 4;  // 4 digits: use Font6
}

// Screen layout constants
namespace Layout {
  const int PANEL_SIZE = 160;
  const int BORDER_SIZE = 10;
  const int DPAD_SIZE = 100;
  const int DPAD_GAP = 15;
  const int TITLE_Y = 40;
  const int SCORE_Y = 40;
  const int SCORE_WIDTH = 160;
  const int SCORE_HEIGHT = 80;
  const int BUTTON_Y = 140;
  const int BUTTON_WIDTH = 340;
  const int BUTTON_HEIGHT = 60;
}

// =============================================================================
// Game Board Class
// =============================================================================
class GameBoard {
public:
  int panels[BOARD_SIZE];  // Game board (public for external access)

private:
  // Position information
  struct Position { int x, y; };
  Position panelPositions[BOARD_SIZE];
  Position dpadPositions[4];
  
  // Game state
  int score;
  int bestScore;
  Preferences preferences;  // For best score persistence
  
  // Pre-calculated layout coordinates
  int boardStartX, boardStartY;
  int dpadCenterX, dpadCenterY;

  // Helper methods for number display
  const char* getNumberText(int value) const;
  const lgfx::IFont* selectOptimalFont(const char* text) const;

public:
  GameBoard();
  
  // Game initialization and drawing
  void initializeGame();
  void drawInitialBoard();
  void drawPanel(int index);
  void drawScore();
  void drawBestScore();
  void drawNewGameButton();
  void drawVirtualDPad();
  
  // Game logic
  int addRandomTile();
  void slideLeft();
  void reverseBoard();
  void transposeBoard();
  int mergeAndSlide();
  void executeMove(int direction);
  
  // UI processing
  int checkTouchInput(int x, int y);
  void showGameOver();
  void updateBestScore();
};

// =============================================================================
// Constructor: Layout calculation
// =============================================================================
GameBoard::GameBoard() {
  randomSeed(analogRead(0));
  
  // Get screen size
  const int screenWidth = M5.Display.width();
  const int screenHeight = M5.Display.height();
  
  // Calculate board position
  const int boardTotalSize = 4 * Layout::PANEL_SIZE + 5 * Layout::BORDER_SIZE;
  boardStartX = (screenWidth - boardTotalSize) / 2;
  boardStartY = 220; // Below title and score display
  
  // Calculate panel positions
  for (int i = 0; i < BOARD_SIZE; i++) {
    const int col = i % 4;
    const int row = i / 4;
    panelPositions[i].x = boardStartX + Layout::BORDER_SIZE + 
                         (col * (Layout::PANEL_SIZE + Layout::BORDER_SIZE));
    panelPositions[i].y = boardStartY + Layout::BORDER_SIZE + 
                         (row * (Layout::PANEL_SIZE + Layout::BORDER_SIZE));
  }
  
  // Calculate D-Pad positions
  dpadCenterX = screenWidth / 2 - 40;
  dpadCenterY = boardStartY + boardTotalSize + Layout::DPAD_SIZE + 30;
  
  dpadPositions[0] = {dpadCenterX, dpadCenterY - Layout::DPAD_SIZE - Layout::DPAD_GAP}; // Up
  dpadPositions[1] = {dpadCenterX + Layout::DPAD_SIZE + Layout::DPAD_GAP, dpadCenterY}; // Right
  dpadPositions[2] = {dpadCenterX, dpadCenterY + Layout::DPAD_SIZE + Layout::DPAD_GAP}; // Down
  dpadPositions[3] = {dpadCenterX - Layout::DPAD_SIZE - Layout::DPAD_GAP, dpadCenterY}; // Left
}

// =============================================================================
// Helper methods for improved number display
// =============================================================================
const char* GameBoard::getNumberText(int value) const {
  static const char* numberTexts[] = {
    "", "2", "4", "8", "16", "32", "64", "128", "256", "512", "1024", "2048"
  };
  
  if (value >= 0 && value < static_cast<int>(sizeof(numberTexts) / sizeof(numberTexts[0]))) {
    return numberTexts[value];
  }
  
  // For values beyond our array, return empty string
  return "";
}

const lgfx::IFont* GameBoard::selectOptimalFont(const char* text) const {
  const int textLength = strlen(text);
  
  if (textLength <= FontThresholds::SHORT_NUMBER_MAX_LENGTH) {
    return &fonts::Font8;  // Large font for 1-3 digits
  } else {
    return &fonts::Font6;  // Medium font for 4 digits
  }
}

// =============================================================================
// Game initialization
// =============================================================================
void GameBoard::initializeGame() {
  score = 0;
  
  // Load best score from flash memory
  preferences.begin("game2048", false);
  bestScore = preferences.getInt("bestScore", 0);
  preferences.end();
  
  // Initialize board
  for (int i = 0; i < BOARD_SIZE; i++) {
    panels[i] = 0;
  }
  
  drawInitialBoard();
  addRandomTile();
  addRandomTile();
}

// =============================================================================
// Initial screen drawing
// =============================================================================
void GameBoard::drawInitialBoard() {
  M5.Display.fillScreen(BGCOLOR);
  M5.Display.setTextWrap(false);
  
  // Draw board background
  const int boardTotalSize = 4 * Layout::PANEL_SIZE + 5 * Layout::BORDER_SIZE;
  M5.Display.fillRoundRect(boardStartX, boardStartY, 
                          boardTotalSize, boardTotalSize, 8, BOARDCOLOR);
  
  // Draw all panels
  for (int i = 0; i < BOARD_SIZE; i++) {
    drawPanel(i);
  }
  
  // Draw title
  M5.Display.setTextColor(TITLE_COLOR);
  M5.Display.setTextDatum(top_left);
  M5.Display.setFont(&fonts::Font8);
  M5.Display.drawString("2048", 40, Layout::TITLE_Y);
  
  drawScore();
  drawBestScore();
  drawNewGameButton();
  drawVirtualDPad();
  
  // Clear game over message area
  M5.Display.fillRect(0, boardStartY - 90, M5.Display.width(), 90, BGCOLOR);
}

// =============================================================================
// Score display
// =============================================================================
void GameBoard::drawScore() {
  M5.Display.fillRoundRect(350, Layout::SCORE_Y, 
                          Layout::SCORE_WIDTH, Layout::SCORE_HEIGHT, 8, BOARDCOLOR);
  M5.Display.setTextColor(NUM_WHITE);
  M5.Display.setTextDatum(top_center);
  M5.Display.setFont(&fonts::Font4);
  M5.Display.drawString("SCORE", 430, Layout::SCORE_Y + 10);
  M5.Display.setFont(&fonts::Font6);
  M5.Display.drawString(String(score), 430, Layout::SCORE_Y + 35);
}

void GameBoard::drawBestScore() {
  M5.Display.fillRoundRect(530, Layout::SCORE_Y, 
                          Layout::SCORE_WIDTH, Layout::SCORE_HEIGHT, 8, BOARDCOLOR);
  M5.Display.setTextColor(NUM_WHITE);
  M5.Display.setTextDatum(top_center);
  M5.Display.setFont(&fonts::Font4);
  M5.Display.drawString("BEST", 610, Layout::SCORE_Y + 10);
  M5.Display.setFont(&fonts::Font6);
  M5.Display.drawString(String(bestScore), 610, Layout::SCORE_Y + 35);
}

// =============================================================================
// Button and UI drawing
// =============================================================================
void GameBoard::drawNewGameButton() {
  M5.Display.fillRoundRect(350, Layout::BUTTON_Y, 
                          Layout::BUTTON_WIDTH, Layout::BUTTON_HEIGHT, 8, TITLE_COLOR);
  M5.Display.setTextColor(WHITECOLOR);
  M5.Display.setTextDatum(middle_center);
  M5.Display.setFont(&fonts::Font4);
  M5.Display.drawString("NEW GAME", 520, Layout::BUTTON_Y + Layout::BUTTON_HEIGHT/2);
}

void GameBoard::drawVirtualDPad() {
  for (int i = 0; i < 4; i++) {
    M5.Display.fillRoundRect(dpadPositions[i].x, dpadPositions[i].y, 
                            Layout::DPAD_SIZE, Layout::DPAD_SIZE, 8, BOARDCOLOR);
  }
}

// =============================================================================
// Enhanced Panel drawing with adaptive font sizing
// =============================================================================
void GameBoard::drawPanel(int index) {
  const int value = panels[index];
  
  // Draw panel background
  M5.Display.fillRoundRect(panelPositions[index].x, panelPositions[index].y, 
                          Layout::PANEL_SIZE, Layout::PANEL_SIZE, 8, PANEL_COLORS[value]);
  
  // Draw number (only if value exists)
  if (value > 0) {
    const char* numberText = getNumberText(value);
    const lgfx::IFont* optimalFont = selectOptimalFont(numberText);
    
    M5.Display.setTextColor(NUMBER_COLORS[value]);
    M5.Display.setTextDatum(middle_center);
    M5.Display.setFont(optimalFont);
    
    const int centerX = panelPositions[index].x + Layout::PANEL_SIZE / 2;
    const int centerY = panelPositions[index].y + Layout::PANEL_SIZE / 2;
    M5.Display.drawString(numberText, centerX, centerY);
  }
}

// =============================================================================
// Best score update
// =============================================================================
void GameBoard::updateBestScore() {
  if (score > bestScore) {
    bestScore = score;
    
    // Save best score to flash memory
    preferences.begin("game2048", false);
    preferences.putInt("bestScore", bestScore);
    preferences.end();
    
    drawBestScore();
  }
}

// =============================================================================
// Touch input processing
// =============================================================================
int GameBoard::checkTouchInput(int x, int y) {
  // D-Pad touch detection
  for (int i = 0; i < 4; i++) {
    if (x >= dpadPositions[i].x && x <= dpadPositions[i].x + Layout::DPAD_SIZE &&
        y >= dpadPositions[i].y && y <= dpadPositions[i].y + Layout::DPAD_SIZE) {
      
      // Touch feedback
      M5.Display.fillRoundRect(dpadPositions[i].x, dpadPositions[i].y, 
                              Layout::DPAD_SIZE, Layout::DPAD_SIZE, 8, 0x7BEF);
      delay(100);
      drawVirtualDPad();
      return i;
    }
  }
  
  // NEW GAME button detection
  if (x >= 350 && x <= 690 && y >= Layout::BUTTON_Y && y <= Layout::BUTTON_Y + Layout::BUTTON_HEIGHT) {
    // Touch feedback
    M5.Display.fillRoundRect(350, Layout::BUTTON_Y, 
                            Layout::BUTTON_WIDTH, Layout::BUTTON_HEIGHT, 8, 0xDEFB);
    M5.Display.setTextColor(TITLE_COLOR);
    M5.Display.setTextDatum(middle_center);
    M5.Display.setFont(&fonts::Font6);
    M5.Display.drawString("NEW GAME", 520, Layout::BUTTON_Y + Layout::BUTTON_HEIGHT/2);
    delay(100);
    drawNewGameButton();
    return NEW_GAME_BUTTON;
  }
  
  return -1; // No touch
}

// =============================================================================
// Game logic: Left slide processing
// =============================================================================
void GameBoard::slideLeft() {
  for (int row = 0; row < 4; row++) {
    int tempRow[4] = {0};
    int writeIndex = 0;
    
    // Pack non-zero values to the left
    for (int col = 0; col < 4; col++) {
      const int value = panels[row * 4 + col];
      if (value != 0) {
        tempRow[writeIndex++] = value;
      }
    }
    
    // Copy result back to board
    for (int col = 0; col < 4; col++) {
      panels[row * 4 + col] = tempRow[col];
    }
  }
}

// =============================================================================
// Game logic: Board horizontal flip
// =============================================================================
void GameBoard::reverseBoard() {
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 2; col++) {
      std::swap(panels[row * 4 + col], panels[row * 4 + (3 - col)]);
    }
  }
}

// =============================================================================
// Game logic: Board transpose (swap rows and columns)
// =============================================================================
void GameBoard::transposeBoard() {
  int tempBoard[BOARD_SIZE];
  
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {
      tempBoard[col * 4 + row] = panels[row * 4 + col];
    }
  }
  
  for (int i = 0; i < BOARD_SIZE; i++) {
    panels[i] = tempBoard[i];
  }
}

// =============================================================================
// Game logic: Merge processing
// =============================================================================
int GameBoard::mergeAndSlide() {
  int scoreIncrease = 0;
  
  slideLeft();
  
  // Merge same values
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 3; col++) {
      const int currentIndex = row * 4 + col;
      const int nextIndex = currentIndex + 1;
      
      if (panels[currentIndex] != 0 && panels[currentIndex] == panels[nextIndex]) {
        panels[currentIndex]++;
        panels[nextIndex] = 0;
        
        const int value = 1 << panels[currentIndex];
        scoreIncrease += value;
        score += value;
      }
    }
  }
  
  slideLeft();
  return scoreIncrease;
}

// =============================================================================
// Game logic: Move execution
// =============================================================================
void GameBoard::executeMove(int direction) {
  switch (direction) {
    case 0: // Up
      transposeBoard();
      break;
    case 1: // Right
      reverseBoard();
      break;
    case 2: // Down
      transposeBoard();
      reverseBoard();
      break;
    case 3: // Left
      // No transformation needed
      break;
  }
  
  const int scoreIncrease = mergeAndSlide();
  
  // Restore original orientation
  switch (direction) {
    case 0: // Up
      transposeBoard();
      break;
    case 1: // Right
      reverseBoard();
      break;
    case 2: // Down
      reverseBoard();
      transposeBoard();
      break;
    case 3: // Left
      // No transformation needed
      break;
  }
  
  // Update screen
  for (int i = 0; i < BOARD_SIZE; i++) {
    drawPanel(i);
  }
  
  if (scoreIncrease > 0) {
    drawScore();
    updateBestScore();
  }
}

// =============================================================================
// Random tile addition
// =============================================================================
int GameBoard::addRandomTile() {
  std::vector<int> emptyPositions;
  
  // Win condition check
  for (int i = 0; i < BOARD_SIZE; i++) {
    if (panels[i] > 10) {
      return i + GAME_WIN;
    }
    if (panels[i] == 0) {
      emptyPositions.push_back(i);
    }
  }
  
  // Game over check
  if (emptyPositions.empty()) {
    return GAME_LOSE;
  }
  
  // Add new tile to random empty position
  const int randomIndex = emptyPositions[random(emptyPositions.size())];
  panels[randomIndex] = (random(100) < 10) ? 2 : 1; // 10% chance for 4, 90% for 2
  drawPanel(randomIndex);
  
  return 0;
}

// =============================================================================
// Game over display
// =============================================================================
void GameBoard::showGameOver() {
  updateBestScore();
  
  // Clear top message area
  M5.Display.fillRect(0, boardStartY - 90, M5.Display.width(), 90, BGCOLOR);
  
  // Game over message
  M5.Display.setTextColor(TITLE_COLOR);
  M5.Display.setTextDatum(top_center);
  M5.Display.setFont(&fonts::Font8);
  M5.Display.drawString("GAME OVER", M5.Display.width() / 2, boardStartY - 80);
  
  M5.Display.setFont(&fonts::Font4);
  M5.Display.setTextColor(SCORE_TEXT);
  drawNewGameButton();
}

// =============================================================================
// Utility functions
// =============================================================================
void copyBoard(int* destination, const int* source) {
  for (int i = 0; i < BOARD_SIZE; i++) {
    destination[i] = source[i];
  }
}

bool isBoardChanged(const int* boardA, const int* boardB) {
  for (int i = 0; i < BOARD_SIZE; i++) {
    if (boardA[i] != boardB[i]) {
      return true;
    }
  }
  return false;
}

bool isGameOver(const int* board) {
  // Game can continue if there are empty spaces
  for (int i = 0; i < BOARD_SIZE; i++) {
    if (board[i] == 0) {
      return false;
    }
  }
  
  // Check horizontal merge possibilities
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 3; col++) {
      if (board[row * 4 + col] == board[row * 4 + col + 1]) {
        return false;
      }
    }
  }
  
  // Check vertical merge possibilities
  for (int col = 0; col < 4; col++) {
    for (int row = 0; row < 3; row++) {
      if (board[row * 4 + col] == board[(row + 1) * 4 + col]) {
        return false;
      }
    }
  }
  
  return true; // No moves possible
}

// =============================================================================
// Game state management
// =============================================================================
enum class GameState {
  Playing,
  GameOver
};

// Global variables
GameBoard* gameBoard = nullptr;
bool needsRestart = false;
GameState currentGameState = GameState::Playing;

// =============================================================================
// Main processing
// =============================================================================
void setup() {
  auto config = M5.config();
  M5.begin(config);
  
  gameBoard = new GameBoard();
  gameBoard->initializeGame();
}

void loop() {
  // Game restart processing
  if (needsRestart) {
    gameBoard->initializeGame();
    currentGameState = GameState::Playing;
    needsRestart = false;
  }
  
  M5.update();
  auto touch = M5.Touch.getDetail();
  
  if (touch.wasReleased()) {
    const int touchResult = gameBoard->checkTouchInput(touch.x, touch.y);
    
    if (currentGameState == GameState::Playing) {
      // Direction key processing
      if (touchResult >= 0 && touchResult < 4) {
        int previousBoard[BOARD_SIZE];
        copyBoard(previousBoard, gameBoard->panels);
        
        gameBoard->executeMove(touchResult);
        
        // Add new tile only if board changed
        if (isBoardChanged(previousBoard, gameBoard->panels)) {
          gameBoard->addRandomTile();
          
          // Game over check
          if (isGameOver(gameBoard->panels)) {
            currentGameState = GameState::GameOver;
            gameBoard->showGameOver();
          }
        } else {
          // Game over even if board didn't change but no moves possible
          if (isGameOver(gameBoard->panels)) {
            currentGameState = GameState::GameOver;
            gameBoard->showGameOver();
          }
        }
      } 
      // NEW GAME button processing
      else if (touchResult == NEW_GAME_BUTTON) {
        needsRestart = true;
      }
    } 
    // NEW GAME button processing in game over state
    else if (currentGameState == GameState::GameOver) {
      if (touchResult == NEW_GAME_BUTTON) {
        needsRestart = true;
      }
    }
  }
  
  delay(20);
}
