#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include<time.h>

#define WIDTH 60
#define HEIGHT 20

int x, y;
int fruitX, fruitY;
int score;
int gameOver;
int tailX[100], tailY[100];
int nTail;

enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum eDirecton dir;

// ---------------------------------------------------------
// 模块1：屏幕控制
void Gotoxy(int x, int y) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    
    COORD pos; //定义一个坐标结构体
    pos.X = x;
    pos.Y = y;
    
    // 把光标瞬间移动到指定坐标
    SetConsoleCursorPosition(handle, pos);
}

void HideCursor() {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(handle, &CursorInfo);
    
    CursorInfo.bVisible = 0; // 设置为不可见
    SetConsoleCursorInfo(handle, &CursorInfo);
}

// ---------------------------------------------------------
// 模块2：数据初始化 (涉及 随机数与时间)
void Setup() {
    gameOver = 0;
    dir = STOP;
    x = WIDTH / 2;
    y = HEIGHT / 2;
    
    // 用当前时间作为种子，初始化随机数生成器
    srand(time(NULL));
    
    // 生成随机坐标
    fruitX = rand() % (WIDTH - 2) + 1;
    fruitY = rand() % (HEIGHT - 2) + 1;
    
    score = 0;
    nTail = 0;
    HideCursor();
}

void Draw() {
    Gotoxy(0, 0); // 回到左上角重绘，防止闪烁

    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\n");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0) printf("#");
            
            if (i == y && j == x)
                printf("O");
            else if (i == fruitY && j == fruitX)
                printf("0");
            else {
                int print = 0;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        printf("o");
                        print = 1;
                    }
                }
                if (!print) printf(" ");
            }

            if (j == WIDTH - 1) printf("#");
        }
        printf("\n");
    }

    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\n");
    printf("Score: %d", score);
}

// ---------------------------------------------------------
// 模块3：输入控制 (涉及 键盘监听)

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a': 
            if(dir!=RIGHT)
            dir = LEFT; break;
            case 'd':
            if(dir!=LEFT)
             dir = RIGHT; break;
            case 'w':
            if(dir!=DOWN)
            dir = UP; break;
            case 's':
            if(dir!=UP)
            dir = DOWN; break;
            case 'x': gameOver = 1; break;
        }
    }
}

// ---------------------------------------------------------
// 模块4：游戏逻辑 (涉及 随机生成)
void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    
    switch (dir) {
            case LEFT:  x--; break;
            case RIGHT: x++; break;
            case UP:    y--; break;
            case DOWN:  y++; break;
            default: break;
    }

    if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0)
        gameOver = 1;

    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = 1;

    if (x == fruitX && y == fruitY) {
        score += 10;
        nTail++;
        // 吃到食物后，随机生成新位置
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
    }
}

// ---------------------------------------------------------
// 主函数 (涉及 暂停与时间控制)
int main() {
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(50);
    }
    
    printf("\n\nGame Over!\n");
    system("pause");
    return 0;
}