# M5Stack Tab5 2048 - Classic Puzzle Game with Large Touch Display

A fully-featured implementation of the popular 2048 puzzle game optimized for the M5Stack Tab5's large 10.1-inch touchscreen. Features authentic gameplay mechanics, persistent best score tracking, intuitive touch controls, and a polished visual interface that takes full advantage of the Tab5's expansive display.

## Key Features

### Classic 2048 Gameplay  
- **Authentic mechanics**: Slide tiles to combine matching numbers  
- **4x4 grid**: Traditional board layout with smooth animations
- **Win condition**: Reach the coveted 2048 tile to achieve victory
- **Progressive difficulty**: Strategic gameplay that becomes increasingly challenging

### Advanced Scoring System  
- **Real-time score tracking**: Live score updates during gameplay  
- **Persistent best score**: Automatically saves high scores to flash memory
- **Visual score display**: Clear, prominent score and best score indicators

### Intuitive Touch Interface  
- **Virtual D-Pad**: Large, responsive directional controls  
- **Touch feedback**: Visual confirmation for all touch interactions
- **Single-tap controls**: Effortless tile movement in four directions
- **New Game button**: Quick restart functionality

### Optimized for Large Display  
- **10.1-inch touchscreen**: Spacious gameplay area with large, easy-to-see tiles
- **High-resolution graphics**: Crisp visuals that take advantage of the Tab5's display
- **Comfortable touch targets**: Large virtual D-Pad buttons perfect for extended play
- **Immersive experience**: Room for detailed score displays and clear game information

---

## Supported Devices

| Device            | Input Method       | Special Features                          |
|------------------|--------------------|-------------------------------------------|
| **M5Stack Tab5**  | Touchscreen       | Large 10.1" display with precise touch controls |

---

## Game Mechanics

### Basic Rules
- **Slide tiles** in any of four directions (up, down, left, right)
- **Combine matching numbers** when they collide during sliding
- **Create higher numbers** through strategic tile combinations
- **Reach 2048** to win the game, but continue playing for higher scores

### Tile Progression
```
2 → 4 → 8 → 16 → 32 → 64 → 128 → 256 → 512 → 1024 → 2048
```

### Win/Lose Conditions
- **Victory**: Successfully create a 2048 tile
- **Game Over**: Board fills completely with no possible moves
- **Continue**: Keep playing after reaching 2048 for maximum scores

---

## Complete User Journey

### 1. Game Start
- Clean 4x4 grid with two random tiles (2 or 4)
- Score starts at zero
- Best score displays previous high score

### 2. Active Gameplay
- Touch directional controls to slide tiles
- Watch tiles combine and scores increase
- New random tile appears after each valid move

### 3. Strategic Play
- Plan moves to create larger tiles
- Manage board space efficiently
- Build toward the 2048 goal

### 4. Game Completion
- Achieve victory by reaching 2048
- Continue for higher scores
- Start new game with improved strategy

---

## Touch Controls

### Virtual D-Pad Layout

```
    [ ↑ ]
[ ← ]   [ → ]
    [ ↓ ]
```

| Control Direction | Function          | Visual Feedback    |
|------------------|-------------------|--------------------|
| **Up Touch**     | Slide tiles up    | Button highlight   |
| **Right Touch**  | Slide tiles right | Button highlight   |
| **Down Touch**   | Slide tiles down  | Button highlight   |
| **Left Touch**   | Slide tiles left  | Button highlight   |

### Additional Controls

| Touch Area        | Function          | Visual Feedback    |
|------------------|-------------------|--------------------|
| **NEW GAME**     | Restart game      | Button animation   |

> All touch controls provide immediate visual feedback with color changes and brief animations.

---

## Scoring System

### Score Calculation
- **Tile value** determines points awarded
- **Combined tiles** add their final value to score
- **Multiple combinations** in single move accumulate
- **Best score** automatically saves to flash memory

### Score Display Elements
- **Current Score**: Large, prominent display during gameplay
- **Best Score**: Persistent high score tracking across sessions
- **Real-time Updates**: Immediate score reflection after each move

**Example Scoring:**
- Combine two 2s → +4 points
- Combine two 4s → +8 points  
- Combine two 64s → +128 points

---

## Setup Guide

### Requirements
- Arduino IDE 1.8.19+
- M5Stack board definitions
- M5Unified library

### Installation

1. **Configure Arduino IDE**
```
File > Preferences > Additional Board Manager URLs:
https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/arduino/package_m5stack_index.json
```

2. **Install Board Support**
```
Tools > Board > Board Manager
Search: "M5Stack" → Install
```

3. **Install Required Library**
```
Tools > Manage Libraries
Search: "M5Unified" → Install latest version
```

4. **Select Your Board**
- **Tab5**: Tools > Board > M5Stack-Tab5

5. **Upload the Program**
- Open `M5stack_tab5_2048.ino`
- Connect device via USB-C
- Select correct COM port (Tools > Port)
- Upload and enjoy!

---

## Visual Design

### Color Palette

| Tile Value | Background Color | Text Color | Hex Code   |
|------------|------------------|------------|------------|
| **Empty**  | Light Gray      | -          | #cdc1b4    |
| **2**      | Light Beige     | Dark Gray  | #eee4da    |
| **4**      | Light Orange    | Dark Gray  | #ede0c8    |
| **8**      | Orange          | White      | #f2b179    |
| **16+**    | Progressive     | White      | Various    |

### Layout Elements
- **Title**: Large "2048" branding
- **Score Boxes**: Rounded rectangles with clean typography
- **Game Board**: Central 4x4 grid with proper spacing
- **Controls**: Large touch targets for easy interaction

---

## Game Strategy Tips

### Beginner Strategy
- **Keep largest tile in corner**: Prevents fragmentation
- **Build in one direction**: Maintain organized tile arrangement
- **Don't chase small merges**: Focus on larger combinations

### Advanced Techniques  
- **Corner strategy**: Lock highest tile in corner position
- **Linear building**: Create ascending sequences along edges
- **Space management**: Always maintain at least one empty row/column

### Common Mistakes
- **Random swiping**: Plan each move carefully
- **Ignoring corners**: Center play leads to fragmentation
- **Impatient merging**: Wait for optimal combination opportunities

---

## Technical Specifications

| Metric             | Value                    |
| ------------------ | ------------------------ |
| **Grid Size**      | 4×4 (16 tiles)          |
| **Touch Response** | < 100ms                  |
| **Memory Usage**   | < 30% of available RAM   |
| **Display Update** | 60 FPS smooth rendering  |
| **Save System**    | Flash memory persistence |
| **Display**        | 10.1" touchscreen optimized |

---

## Performance Optimizations

### Memory Management
- **Static allocation**: Pre-calculated positions and layouts
- **Efficient arrays**: Minimal dynamic memory usage
- **Smart rendering**: Only redraw changed elements

### Display Efficiency  
- **Partial updates**: Redraw only modified panels
- **Optimized fonts**: Balanced readability and performance
- **Color caching**: Pre-defined RGB565 color constants

### Input Responsiveness
- **Touch debouncing**: Prevents accidental double-taps
- **Visual feedback**: Immediate response confirmation
- **Smooth animations**: 20ms update cycle for fluid experience

---

## Advanced Features

### Persistent Storage
- **Best score tracking**: Survives power cycles and resets
- **Flash memory usage**: Efficient non-volatile storage
- **Data integrity**: Safe read/write operations

### Error Handling
- **Move validation**: Prevents invalid tile movements
- **Game state management**: Clean transitions between states
- **Touch boundary checking**: Accurate input detection

### Code Architecture
- **Object-oriented design**: Clean GameBoard class structure
- **Separation of concerns**: Logic, display, and input handling
- **Extensible framework**: Easy to modify and enhance

---

## Customization Options

### Visual Modifications
```cpp
// Color theme adjustments
#define BGCOLOR     0xFFBD  // Background color
#define BOARDCOLOR  0xBDF3  // Board color
#define TITLE_COLOR 0x938E  // Title color

// Layout fine-tuning
const int PANEL_SIZE = 160;     // Tile size
const int BORDER_SIZE = 10;     // Spacing between tiles
const int DPAD_SIZE = 100;      // Control button size
```

### Gameplay Tweaks
```cpp
// Tile spawn probability
panels[randomIndex] = (random(100) < 10) ? 2 : 1; // 10% for 4, 90% for 2

// Win condition
if (panels[i] > 10) {  // Tile value 2048 (2^11)
  return i + GAME_WIN;
}
```

---

## Troubleshooting

### Common Issues

| Problem              | Solution                              |
|---------------------|---------------------------------------|
| **Touch not working** | Check M5.Touch.getDetail() in loop  |
| **Score not saving**  | Verify flash memory write permissions |
| **Display corruption** | Ensure proper M5.Display.begin()    |
| **Slow performance**   | Check for memory leaks in game loop |

### Debug Output
Enable serial debugging for development:
```cpp
Serial.begin(115200);
Serial.println("Touch detected: " + String(touch.x) + "," + String(touch.y));
```

---

## Roadmap

### Planned Features
- **Undo functionality**: Take back the last move
- **Auto-save during gameplay**: Resume interrupted games
- **Statistics tracking**: Games played, win percentage
- **Custom tile themes**: Alternative visual styles

### Hardware Expansion
- **M5Stack Basic**: Button-based control adaptation
- **External displays**: Larger screen support
- **Multiplayer modes**: Turn-based gameplay

---

## License

Released under the MIT License.  
Free for personal, educational, and commercial use.

---

## Credits

Based on the original 2048 game by Gabriele Cirulli.  
Specifically optimized for M5Stack Tab5 hardware with enhanced touch controls and large display utilization.
