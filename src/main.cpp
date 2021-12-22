#include "snake_header.h"

int main() {
    snake sn;
    sn.initGame();
    while (1) {
        sn.show();
        sn.updateWithoutInput();
        sn.updateWithInput();
    }
    return 0;
}

