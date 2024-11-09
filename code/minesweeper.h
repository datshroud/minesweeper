#include<bits/stdc++.h>
#include "tile.h"
#include "variables.h"

class Minesweeper{
private:
    int level = 1;
    vector<vector<Tile>> grid;
    vector<pair<int, int>> positiveMine, negativeMine;
    bool gameStatus = true;
    int tileLeft = 68;
    int blueFlagLeft = 9;
    int greenFlagLeft = 3;
    bool showMode = false;
public:
    Minesweeper(int level = 1);
    void setup();
    //khoi tao ma tran bom tuy theo muc do de den kho
    void InitGrid(int level = 1);
    //ham reset grid
    void resetGrid();
    //Ham mo cac o lan can khong co bom khi click chuot vao
    void revealTile(int x, int y);
    // tao render cho game
    void renderGrid(bool &running);
    void showMenu(bool &running);
    void endGame();
    void showBomb(int x, int y);
    //ham cam co
    void flagged(int i, int j);
    //ham chay game
    void run();
};