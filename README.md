# M5Stack Tab5 2048 - Classic Puzzle Game for Large Touch Display

A fully-featured 2048 puzzle game optimized for the M5Stack Tab5's large 10.1-inch touchscreen. Enjoy intuitive touch controls, achievement celebrations, and persistent best score tracking anywhere you go.

## Game Features

**Slide number tiles to reach 2048 in this classic puzzle game!**

- **Authentic 2048 Gameplay** - Classic rules that anyone can enjoy
- **Achievement Celebrations** - Victory message for 2048, special effects for 4096  
- **Large Touch Display** - Comfortable play on 10.1-inch screen
- **Best Score Saving** - Automatically saves your highest score

---

## Supported Device

| Device            | Input Method  | Features                          |
|------------------|---------------|-----------------------------------|
| **M5Stack Tab5** | Touch Screen  | Comfortable play on 10.1" display |

---

## Game Rules

### Basic Controls
- **Slide tiles**: Move in four directions (up, down, left, right)
- **Merge same numbers**: When tiles collide, they combine into the next number
- **Create larger numbers**: Strategically combine tiles
- **Reach 2048**: But you can aim even higher!

### Number Evolution
```
2 → 4 → 8 → 16 → 32 → 64 → 128 → 256 → 512 → 1024 → 2048 → 4096
```

### Victory, Game Over & Special Achievements
- **2048 Achievement**: Gold celebration message → Continue playing
- **4096 Achievement**: Legendary celebration with special effects → Tile resets to 2 and game continues
- **Game Over**: When the board fills up and no moves are possible

---

## How to Play

### 1. Game Start
A 4×4 board appears with two tiles (2 or 4)

### 2. Move Tiles
Touch the directional pad to slide all tiles in one direction

### 3. Merge Numbers
When tiles with the same number collide, they merge into the next number

### 4. New Tile Appears
After each valid move, a new tile (2 or 4) appears in a random empty space

### 5. Achievement Celebrations
- **2048 reached**: "Congratulations!" message (auto-clears after 3 seconds)
- **4096 reached**: "Incredible! Legendary Achievement!" message
- **4096 special handling**: The achieved tile automatically resets to 2

### 6. Continue Playing
Keep playing for higher scores even after achievements

---

## Controls

### Virtual D-Pad
```
    [ ↑ ]
[ ← ]   [ → ]
    [ ↓ ]
```

| Button | Action              |
|--------|---------------------|
| **↑**  | Slide all tiles up    |
| **→**  | Slide all tiles right |
| **↓**  | Slide all tiles down  |
| **←**  | Slide all tiles left  |

### Other Buttons
| Button       | Action           |
|-------------|------------------|
| **NEW GAME** | Start a new game |

---

## Scoring System

### Score Calculation
- The final number of merged tiles becomes your points
- Multiple merges in one move are all added together
- Achievement bonuses included for 2048 and 4096

### Examples
- 2+2=4 → **4 points earned**
- 4+4=8 → **8 points earned**
- 64+64=128 → **128 points earned**

---

## Setup Instructions

### Requirements
- Arduino IDE
- M5Stack Tab5

### Easy Installation

1. **Configure Arduino IDE**
   - File > Preferences > Additional Board Manager URLs:
   ```
   https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/arduino/package_m5stack_index.json
   ```

2. **Install M5Stack Boards**
   - Tools > Board > Board Manager
   - Search for "M5Stack" and install

3. **Install Library**
   - Tools > Manage Libraries
   - Search for "M5Unified" and install

4. **Upload**
   - Board setting: M5Stack-Tab5
   - Open `M5stack_tab5_2048.ino` and upload

---

## Strategy Tips

### For Beginners
- **Keep largest tile in corner**: Place big numbers in corners
- **Build in one direction**: Try to arrange numbers in the same direction
- **Ignore small merges**: Prioritize larger combinations

### Advanced Techniques
- **Corner strategy**: Keep the largest tile in a corner and don't move it
- **Linear arrangement**: Arrange tiles in ascending order along edges
- **Reserve space**: Always keep one row or column available

### 4096 Achievement Strategy
- **Continue after 2048**: Don't stop at 2048, keep playing
- **Plan 4096 placement**: Think ahead about where 4096 will appear  
- **Use resets strategically**: Incorporate the 4096→2 reset into your strategy

### Common Mistakes
- **Random moves**: Don't move without planning
- **Center play**: Use the edges instead of scattering tiles
- **Rushing**: Take time to think about the optimal move

---

## Understanding the Game Screen

### Main Display
- **2048 title**: Game name in the top left
- **SCORE**: Current score display
- **BEST**: Past highest score
- **4×4 board**: Main game area
- **D-pad**: Control buttons at the bottom of the screen
- **NEW GAME**: Restart button

### Celebration Messages
- **2048 achievement**: Gold-bordered congratulations message
- **4096 achievement**: Magenta and gold-bordered special message  
- **Auto-clear**: Both disappear automatically after 3 seconds

---

## Frequently Asked Questions

### Q: What happens after reaching 2048?
A: A celebration message appears, but you can continue playing. Aim for even higher scores!

### Q: What happens when I create a 4096 tile?
A: A special legendary achievement message appears, and that 4096 tile automatically resets to 2. You can continue playing.

### Q: Is my best score saved?
A: Yes, it's automatically saved even when you turn off the device.

### Q: What if I make a wrong move?
A: There's currently no UNDO function. Please move carefully.

### Q: What if the game becomes slow?
A: Try resetting with the NEW GAME button.

---

## Ways to Enjoy

### Casual Play
- Time-killing during commute
- Refreshing during breaks
- Relaxing before bedtime

### Challenge Play
- Aim for 2048 achievement
- Reach 4096 legendary status
- Beat your best score
- Achieve 2048 with minimum moves

### With Family & Friends
- Score competitions
- Take turns playing
- Discuss strategies together

---

## License

Provided under MIT License. Free for personal, educational, and commercial use.

---

## Credits

Original 2048 game by Gabriele Cirulli  
M5Stack Tab5 optimized version with large touch controls, achievement system, and celebration effects
