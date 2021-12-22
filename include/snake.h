#include <graphics.h>
#include <conio.h>

#define WIDTH 40// 40 Blocks in every rows
#define HEIGHT 30
#define SIZE 20


class snake {
private:

    int Blocks[WIDTH][HEIGHT] = { 0 };
    char moveDirection, oldMoveDirection;
    int foodX, foodY;
    bool isFailure = false;

public:
    snake() {};

    void initGame();
    void show();//Redraw the image of snake and food every time it runs
    void updateWithInput();//Updates related to input
    void updateWithoutInput();
    void moveSnake();
};

