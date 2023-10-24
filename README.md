# MineSweeper_QT_6_5_2
## Introduction

Personal Qt Project by [CCMagical542](https://github.com/CCMagical542)

Download: [Releases · CCMagical542/MineSweeper_QT_6_5_2 (github.com)](https://github.com/CCMagical542/MineSweeper_QT_6_5_2/releases)

Qt version: Qt 6.5.2 MinGW 64-bit

Compiler: MinGW 11.2.0 64-bit

## **Feature**

1. All of the components is in a **scroll area**. No need to be concerned if you shrink the main widget.

2. Customize Difficulty: you can set the number of the boxes on the horizontal direction and the vertical direction (as "**Width**" and "**Height**"). Both of them must be greater than 2 and less than 30. The number of **Mines** should be greater than 0 and less than "**Width * Height**"

3. Default Difficulties (Buttons on the top bar): 
   1) Apprentice: 9*9 with 10 mines
   2) Adept: 16*16 with 40 mines
   3) Expert: 30*16 with 99 mines

4. Support setting flags
5. "Highlight" the surrounding undetected  boxes when you click on a detected box.
6. In the detecting process, if there is a box who is going to show blank (no mines place around), program will continue to automatically detect its surrounding undetected boxes.

7. The mines are only deployed when you click a box first after restarting.
8. click the **Star** button to restart. Its icon will be changed when you win or fail.
