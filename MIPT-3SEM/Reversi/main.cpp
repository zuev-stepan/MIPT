#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>

using namespace std;

enum {
    BLACK = 1 << 0,
    WHITE = 1 << 1,
    CAN_TURN_BLACK = 1 << 2,
    CAN_TURN_WHITE = 1 << 3
};

struct pt {
    int x, y, fit;
    pt(){}
    pt(int a, int b, int c) : x(a), y(b), fit(c) {}
};

inline bool isEmpty(int x) {
    return (x & 3) == 0;
}

inline bool inside(int x, int y) {
    return (x <= 8 && x >= 1 && y <= 8 && y >= 1);
}

inline int getFlag(int x) {
    if (x == WHITE)
        return CAN_TURN_WHITE;
    return CAN_TURN_BLACK;
}

inline int getOtherColor(int x) {
    if (x == 1)
        return 2;
    return 1;
}
float w[9][9] = {
    {0,  0,  0,  0,  0,  0,  0,  0,  0},
    {0, 40, -5,  1,  1,  1,  1, -5, 40},
    {0, -5, -5,  1,  1,  1,  1, -5, -5},
    {0,  1,  1,  1,  1,  1,  1,  1,  1},
    {0,  1,  1,  1,  1,  1,  1,  1,  1},
    {0,  1,  1,  1,  1,  1,  1,  1,  1},
    {0,  1,  1,  1,  1,  1,  1,  1,  1},
    {0, -5, -5,  1,  1,  1,  1, -5, -5},
    {0, 40, -5,  1,  1,  1,  1, -5, 40}
};

struct board {
    int a[10][10];
    int size;
    inline bool checkTurn(int x, int y, int dx, int dy, int myColor, int otherColor) {
        if (!isEmpty(a[x][y]))
            return false;
        int i = x + dx, j = y + dy;
        bool s = false;
        while (inside(i, j) && (a[i][j] & otherColor)) {
            i += dx, j += dy;
            s = true;
        }
        return s && ((a[i][j] & myColor) != 0);
    }

    inline bool doTurn(int x, int y, int dx, int dy, int myColor, int otherColor) {
        if (!checkTurn(x, y, dx, dy, myColor, otherColor))
            return false;
        int i = x + dx, j = y + dy;
        while (i <= 8 && i >= 1 && j <= 8 && j >= 1 && ((a[i][j] & myColor) == 0)) {
            a[i][j] ^= otherColor | myColor;
            i += dx;
            j += dy;
        }
        return true;
    }

    void recalc(int x, int y) {
        a[x][y] &= 3;
        if (!isEmpty(a[x][y]))
            return;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) {
                    continue;
                }
                if (checkTurn(x, y, dx, dy, BLACK, WHITE)) {
                    a[x][y] |= CAN_TURN_BLACK;
                } else if (checkTurn(x, y, dx, dy, WHITE, BLACK)) {
                    a[x][y] |= CAN_TURN_WHITE;
                }
            }
        }
    }

    bool move(int x, int y, int myColor, int otherColor) {
        bool f = false;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) {
                    continue;
                }
                f |= doTurn(x, y, dx, dy, myColor, otherColor);
            }
        }
        if (f) {
            a[x][y] |= myColor;
            size++;
        }
        return f;
    }

    int fitness(int myColor) {
        int ans = 0;
        if (size < 50) {
            for (int i = 1; i <= 8; i++) {
                for (int j = 1; j <= 8; j++) {
                    ans += ((a[i][j] & BLACK) != 0) * w[i][j];
                    ans -= ((a[i][j] & WHITE) != 0) * w[i][j];
                }
            }
        } else {
            for (int i = 1; i <= 8; i++) {
                for (int j = 1; j <= 8; j++) {
                    ans += ((a[i][j] & BLACK) != 0);
                    ans -= ((a[i][j] & WHITE) != 0);
                }
            }
        }

        if (myColor == WHITE) {
            return -ans;
        }
        return ans;
    }

    void print() {
        printf("#abcdefgh\n");
        for (int j = 1; j <= 8; j++) {
            printf("%d", j);
            for (int i = 1; i <= 8; i++) {
                recalc(i, j);
                if (a[i][j] & BLACK) {
                    printf("B");
                    continue;
                }
                if (a[i][j] & WHITE) {
                    printf("W");
                    continue;
                }
                if ((a[i][j] & (CAN_TURN_WHITE | CAN_TURN_BLACK)) == (CAN_TURN_WHITE | CAN_TURN_BLACK)) {
                    printf("e");
                    continue;
                }
                if (a[i][j] & CAN_TURN_WHITE) {
                    printf("w");
                    continue;
                }
                if (a[i][j] & CAN_TURN_BLACK) {
                    printf("b");
                    continue;
                }
                printf(".");
            }
            printf("\n");
        }
    }
};

board b;
int myColor = WHITE, otherColor = BLACK;
int sign = -1;
int bestResult[10];

pt dfs(board& b, int depth, int currColor, int otherColor) {
    if (depth == 0) {
        return pt(0, 0, b.fitness(currColor));
    }
    board b1 = b;
    int ans = 1000000000, x, y;
    for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 8; j++) {
            if (b1.move(i, j, currColor, otherColor)) {
                pt t = dfs(b1, depth - 1, otherColor, currColor);
                if (ans > t.fit) {
                    ans = t.fit, x = i, y = j;
                }
                b1 = b;
            }
        }
    }
    if (ans == 1000000000) {
        return pt(0, 0, -dfs(b, depth - 1, otherColor, currColor).fit);
    }
    return pt(x, y, -ans);
}

int main()
{
    //freopen("l.txt", "rt", stdin);
    char s[10];
    b.a[4][4] = b.a[5][5] = WHITE;
    b.a[4][5] = b.a[5][4] = BLACK;
    b.size = 4;
    //ofstream log;
    //log.open("l.txt");
    char c;
    int x, y;
    scanf("%s%s", s, s);
    if (s[0] == 'b') {
        myColor = BLACK;
        otherColor = WHITE;
        sign = 1;
    }
    while(1) {
        scanf("%s", s);
        if (s[0] == 't') {
            //log << "turn\n";
            pt t = dfs(b, 4, myColor, otherColor);
            b.move(t.x, t.y, myColor, otherColor);
            printf("move %c %d\n", t.x + 'a' - 1, t.y);
            fflush(stdout);
        } else if (s[0] == 'm') {
            scanf(" %c%d", &c, &y);
            //log << "move " << c << y << '\n';
            x = c - 'a' + 1;
            b.move(x, y, otherColor, myColor);
        } else if (s[0] == 's') {
            b.print();
        } else {
            return 0;
        }
        //b.print();
        //log.flush();
    }
    return 0;
}
