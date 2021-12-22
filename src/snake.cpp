#include <graphics.h>
#include <conio.h>
#include "snake.h"

void snake::initGame() {
    initgraph(WIDTH * SIZE, HEIGHT * SIZE);
    setbkcolor(LIGHTGRAY);
    BeginBatchDraw();
    cleardevice();
    setlinecolor(WHITE);
    for (int column_line = SIZE; column_line< WIDTH * SIZE; column_line += SIZE)
        line(column_line, 0, column_line, HEIGHT * SIZE);
    for (int row_line = SIZE; row_line < HEIGHT * SIZE; row_line += SIZE)
        line(0, row_line, WIDTH * SIZE, row_line);
    moveDirection = oldMoveDirection = 'd';
    Blocks[WIDTH / 2][HEIGHT / 2] = 1;
    for (int snake_head = 1; snake_head < 4; snake_head++)
        Blocks[WIDTH / 2 - snake_head][HEIGHT / 2] = snake_head + 1;
    foodX = rand() % (WIDTH - 2) + 1;//1to38
    foodY = rand() % (HEIGHT - 2) + 1;
}

void snake::show() {
    for (int column = 0; column < WIDTH; column++) {
        for (int row = 0; row < HEIGHT; row++) {
            if (Blocks[column][row] != 0)
                setfillcolor(HSVtoRGB(Blocks[column][row] * 10, 0.9, 1));
            else
                setfillcolor(LIGHTGRAY);
            fillrectangle(columan * SIZE, row * SIZE, (column + 1) * SIZE, (row + 1) * SIZE);
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
    for (int column = 0; column < WIDTH; column++)
        for (int row = 0; row < HEIGHT; row++)
            if (Blocks[column][row] != 0)
                Blocks[column][row]++;
    //Find the x,y coordinates of the old snake head and old snake tail
    int oldHeadX, oldHeadY, oldTailX, oldTailY;
    int tailBlocks = 0;
    for (int column = 0; column < WIDTH; column++) {
        for (int row = 0; row < HEIGHT; row++) {
            if (tailBlocks < Blocks[column][row]) {
                tailBlocks = Blocks[column][row];
                oldTailX = column;
                oldTailY = row;
            }
            if (Blocks[column][row] == 2) {
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
