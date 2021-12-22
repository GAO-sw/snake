#include <graphics.h>
#include <conio.h>
#include "snake.h"

void snake::initGame() {
    initgraph(WIDTH * SIZE, HEIGHT * SIZE);
    setbkcolor(LIGHTGRAY);
    BeginBatchDraw();
    cleardevice();
    setlinecolor(WHITE);
    for (int i = SIZE; i < WIDTH * SIZE; i += SIZE)
        line(i, 0, i, HEIGHT * SIZE);
    for (int i = SIZE; i < HEIGHT * SIZE; i += SIZE)
        line(0, i, WIDTH * SIZE, i);
    moveDirection = oldMoveDirection = 'd';
    Blocks[WIDTH / 2][HEIGHT / 2] = 1;
    for (int i = 1; i < 4; i++)
        Blocks[WIDTH / 2 - i][HEIGHT / 2] = i + 1;
    foodX = rand() % (WIDTH - 2) + 1;//1到38
    foodY = rand() % (HEIGHT - 2) + 1;
}

void snake::show() {
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            if (Blocks[i][j] != 0)
                setfillcolor(HSVtoRGB(Blocks[i][j] * 10, 0.9, 1));
            else
                setfillcolor(LIGHTGRAY);
            fillrectangle(i * SIZE, j * SIZE, (i + 1) * SIZE, (j + 1) * SIZE);
        }
    }
    setfillcolor(LIGHTGREEN);
    fillrectangle(foodX * SIZE, foodY * SIZE, (foodX + 1) * SIZE, (foodY + 1) * SIZE);

    if (isFailure) {
        settextcolor(LIGHTRED);
        settextstyle(80, 0, _T("宋体"));
        setbkmode(TRANSPARENT);
        outtextxy(240, 220, _T("GAME OVER"));
    }

    FlushBatchDraw();
}

void snake::updateWithInput() {
    if (_kbhit() && isFailure == 0) {
        char input = _getch();
        if (input == 'a' || input == 's' || input == 'd' || input == 'w') {
            moveDirection = input;
            if (moveDirection == 'a' && oldMoveDirection == 'd')
                moveDirection = 'd';
            else if (moveDirection == 's' && oldMoveDirection == 'w')
                moveDirection = 'w';
            else if (moveDirection == 'd' && oldMoveDirection == 'a')
                moveDirection = 'a';
            else if (moveDirection == 'w' && oldMoveDirection == 's')
                moveDirection = 's';
            else oldMoveDirection = input;
            moveSnake();
        }
    }
}

void snake::updateWithoutInput() {
    if (isFailure)
        return;
    static int waitIndex = 1;
    waitIndex++;
    if (waitIndex == 20) {
        moveSnake();
        waitIndex = 1;
    }
}

void snake::moveSnake() {
    //update the snake body
    for (int i = 0; i < WIDTH; i++)
        for (int j = 0; j < HEIGHT; j++)
            if (Blocks[i][j] != 0)
                Blocks[i][j]++;
    //Find the x,y coordinates of the old snake head and old snake tail
    int oldHeadX, oldHeadY, oldTailX, oldTailY;
    int tailBlocks = 0;
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            if (tailBlocks < Blocks[i][j]) {
                tailBlocks = Blocks[i][j];
                oldTailX = i;
                oldTailY = j;
            }
            if (Blocks[i][j] == 2) {
                oldHeadX = i;
                oldHeadY = j;
            }
        }
    }
    //According to the moving direction of the snake to assign the xy coordinates of the new snake head
    int newHeadX = oldHeadX, newHeadY = oldHeadY;
    switch (moveDirection) {
    case'a':
        newHeadX -= 1;
        break;
    case 's':
        newHeadY += 1;
        break;
    case 'd':
        newHeadX += 1;
        break;
    case 'w':
        newHeadY -= 1;
        break;
    }
    //Determine if the game has failed
    if (newHeadX >= WIDTH || newHeadX < 0 || newHeadY >= HEIGHT || newHeadY < 0 || Blocks[newHeadX][newHeadY] != 0) {
        isFailure = 1;
        return;
    }
    //Treatment of snake head
    Blocks[newHeadX][newHeadY] = 1;
    //Handling of eaten food and snake tail
    if (newHeadX == foodX && newHeadY == foodY) {
        foodX = rand() % (WIDTH - 2) + 1;
        foodY = rand() % (HEIGHT - 2) + 1;
    }
    else
        Blocks[oldTailX][oldTailY] = 0;
}
