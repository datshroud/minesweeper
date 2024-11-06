#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

using namespace std;

const int SCREEN_WIDTH_HARD = 24 * 40;
const int SCREEN_WIDTH_MEDIUM = 18 * 40;
const int SCREEN_WIDTH_EASY = 10 * 40;

const int SCREEN_HEIGHT_HARD = 20 * 40 + 60;
const int SCREEN_HEIGHT_MEDIUM = 14 * 40 + 60;
const int SCREEN_HEIGHT_EASY = 8 * 40 + 60;

const int GRID_WIDTH_HARD = 24;
const int GRID_HEIGHT_HARD = 20;
const int GRID_WIDTH_MEDIUM = 18;
const int GRID_HEIGHT_MEDIUM = 14;
const int GRID_WIDTH_EASY = 10;
const int GRID_HEIGHT_EASY = 8;

const int TILE_SIZE_HARD = 40;
const int TILE_SIZE_MEDIUM = 40;
const int TILE_SIZE_EASY = 40;

const int MINE_COUNT_HARD = 100;
const int MINE_COUNT_MEDIUM = 40;
const int MINE_COUNT_EASY = 12;

TTF_Font* gFont = NULL;
TTF_Font* bFont = NULL;
SDL_Texture* greenFlag;
SDL_Texture* blueFlag;
SDL_Texture* mine;
SDL_Texture* youLose;
SDL_Texture* youWin;
SDL_Texture* dropdown;
SDL_Window* window;
SDL_Renderer* renderer;

class Tile{
public:
    bool isPositiveMine = false;
    bool isNegativeMine = false;
    bool isRevealed = false;
    bool isPositiveFlagged = false;
    bool isNegativeFlagged = false;
    bool isMineNeared = false;
    int neighboringMine = 0;
};

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
    Minesweeper(int level = 1){
        this->level = level;
        InitGrid(level);
    }

    void setup(){
        if (SDL_Init(SDL_INIT_VIDEO)){
            cout << "SDL_Init Error: " << SDL_GetError() << endl;
            return;
        }
        if (TTF_Init() == -1){
            cout << "TTF_Init Error: " << TTF_GetError() << endl;
            return;
        }
        if (IMG_Init(IMG_INIT_PNG) == 0) {
            cout << "IMG_Init Error: " << IMG_GetError() << endl;
            return;
        }

        window = SDL_CreateWindow("Minesweeper", 
                                    SDL_WINDOWPOS_CENTERED, 
                                    SDL_WINDOWPOS_CENTERED, 
                                    SCREEN_WIDTH_EASY, 
                                    SCREEN_HEIGHT_EASY, 
                                    SDL_WINDOW_SHOWN);
        if (!window) {
            cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
            return;
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer){
            cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
            return;
        }
        gFont = TTF_OpenFont("res/font.ttf", 20);
        if (!gFont){
            cout << "TTF_OpenFont Error: " << TTF_GetError() << endl;
            return; 
        }
        bFont = TTF_OpenFont("res/font.ttf", 24);
        if (!bFont){
            cout << "TTF_OpenFont Error: " << TTF_GetError() << endl;
            return; 
        }

        greenFlag = IMG_LoadTexture(renderer, "res/img/green_flag.png");
        if (!greenFlag) {
            cout << "Can't load image: " << IMG_GetError() << endl;
            return;
        }
        blueFlag = IMG_LoadTexture(renderer, "res/img/blue_flag.png");
        if (!blueFlag){
            cout << "Can't load image: " << IMG_GetError() << endl;
            return;
        }
        mine = IMG_LoadTexture(renderer, "res/img/mine.png");
        if (!mine){
            cout << "Can't load image: " << IMG_GetError() << endl;
            return;
        }
        youLose = IMG_LoadTexture(renderer, "res/img/you_lose.png");
        if (!youLose){
            cout << "Can't load image yL: " << IMG_GetError() << endl;
            return;
        }
        youWin = IMG_LoadTexture(renderer, "res/img/you_win.png");
        if (!youWin){
            cout << "Can't load image yW: " << IMG_GetError() << endl;
            return;
        }
        dropdown = IMG_LoadTexture(renderer, "res/img/dropdown.png");
        if (!dropdown){
            cout << "Can't load image dropdown: " << IMG_GetError() << endl;
            return;
        }
    }

    //khoi tao ma tran bom tuy theo muc do de den kho
    void InitGrid(int level = 1){
        this->level = level;
        int height, width, mineCountPos, mineCountNeg, sheight, swidth;
        gameStatus = true;

        if (level == 1) {
            height = GRID_HEIGHT_EASY; width = GRID_WIDTH_EASY;
            mineCountPos = MINE_COUNT_EASY / 4; mineCountNeg = MINE_COUNT_EASY / 4;
            sheight = SCREEN_HEIGHT_EASY; swidth = SCREEN_WIDTH_EASY;
            tileLeft = 68;
        } else if (level == 2) {
            height = GRID_HEIGHT_MEDIUM; width = GRID_WIDTH_MEDIUM;
            mineCountPos = MINE_COUNT_MEDIUM / 4; mineCountNeg = MINE_COUNT_MEDIUM / 4;
            sheight = SCREEN_HEIGHT_MEDIUM; swidth = SCREEN_WIDTH_MEDIUM;
            tileLeft = height * width - 40;
        } else {
            height = GRID_HEIGHT_HARD; width = GRID_WIDTH_HARD;
            mineCountPos = MINE_COUNT_HARD / 4; mineCountNeg = MINE_COUNT_HARD / 4;
            sheight = SCREEN_HEIGHT_HARD; swidth = SCREEN_WIDTH_HARD;
            tileLeft = height * width - 100;
        }

        mineCountNeg *= 3;

        
        SDL_SetWindowSize(window, swidth, sheight);

        blueFlagLeft = mineCountNeg;
        greenFlagLeft = mineCountPos;

        showMode = false;

        grid.clear();
        grid.resize(height, vector<Tile>(width));
        resetGrid();

        // Dat min o vi tri ngau nhien
        //Min duong (+)
        srand(time(0));
        while (positiveMine.size() < mineCountPos){
            int x = rand() % height;
            int y = rand() % width;
            if (grid[x][y].isPositiveMine){}
            else {
                grid[x][y].isPositiveMine = true;
                positiveMine.push_back({x, y});
            }
        }
        //Min am (-)
        while (negativeMine.size() < mineCountNeg){
            int x = rand() % height;
            int y = rand() % width;
            if (grid[x][y].isPositiveMine || grid[x][y].isNegativeMine){}
            else {
                grid[x][y].isNegativeMine = true;
                negativeMine.push_back({x, y});
            }
        }

        // Tinh so min lan can o moi o
        for (int i = 0; i < height; i++){
            for (int j = 0; j < width; j++){
                grid[i][j].neighboringMine = 0;
                for (int x = -1; x <= 1; x++){
                    for (int y = -1; y <= 1; y++){
                        int dx = i + x;
                        int dy = j + y;
                        if (!(dx == i && dy == j) && min(dx, dy) >= 0 && dx < height && dy < width){
                            if (grid[dx][dy].isPositiveMine) {
                                grid[i][j].neighboringMine++;
                                grid[i][j].isMineNeared = true;
                            }
                            else if (grid[dx][dy].isNegativeMine) {
                                grid[i][j].neighboringMine--;
                                grid[i][j].isMineNeared = true;
                            }
                        } 
                    }
                }
            }
        }
    }
    //ham reset grid
    void resetGrid() {
        for (auto& row : grid) {
            for (auto& tile : row) {
                tile.isPositiveMine = false;
                tile.isNegativeMine = false;
                tile.isRevealed = false;
                tile.isPositiveFlagged = false;
                tile.isNegativeFlagged = false;
                tile.isMineNeared = false;
                tile.neighboringMine = 0;
            }
        }
        positiveMine.clear();
        negativeMine.clear();
        gameStatus = true; 
    }

    //Ham mo cac o lan can khong co bom khi click chuot vao
    void revealTile(int x, int y){
        if (x < 0 || y < 0 || x >= grid.size() || y >= grid[0].size() || grid[x][y].isRevealed) return;

        tileLeft--;
        grid[x][y].isRevealed = true;
        if (grid[x][y].isNegativeFlagged){
            grid[x][y].isNegativeFlagged = false;
            blueFlagLeft++;
        }
        if (grid[x][y].isPositiveFlagged){
            grid[x][y].isPositiveFlagged = false;
            greenFlagLeft++;
        }
        // de quy mo cac o lan can khong co bom
        if (!grid[x][y].isMineNeared){
            for (int i = -1; i <= 1; i++){
                for (int j = -1; j <= 1; j++){
                    int dx = i + x;
                    int dy = j + y;
                    if (!(dx == x && dy == y) && min(dx, dy) >= 0 && dx < grid.size() && dy < grid[0].size()) 
                        if (!grid[dx][dy].isRevealed) revealTile(dx, dy);                            
                }
            }
        }
    }

    //tao mau ngau nhien tu 0 den 7
    SDL_Color textColor0 = {(Uint8)(rand() % (64 + 128)), (Uint8)(rand() % (64 + 128)), (Uint8)(rand() % (64 + 128)), 255};
    SDL_Color textColor1 = {(Uint8)(rand() % (64 + 128)), (Uint8)(rand() % (64 + 128)), (Uint8)(rand() % (64 + 128)), 255};
    SDL_Color textColor2 = {(Uint8)(rand() % (64 + 128)), (Uint8)(rand() % (64 + 128)), (Uint8)(rand() % (64 + 128)), 255};
    SDL_Color textColor3 = {(Uint8)(rand() % (64 + 128)), (Uint8)(rand() % (64 + 128)), (Uint8)(rand() % (64 + 128)), 255};
    SDL_Color textColor4 = {(Uint8)(rand() % (64 + 128)), (Uint8)(rand() % (64 + 128)), (Uint8)(rand() % (64 + 128)), 255};
    SDL_Color textColor5 = {(Uint8)(rand() % (64 + 128)), (Uint8)(rand() % (64 + 128)), (Uint8)(rand() % (64 + 128)), 255};
    SDL_Color textColor6 = {(Uint8)(rand() % (64 + 128)), (Uint8)(rand() % (64 + 128)), (Uint8)(rand() % (64 + 128)), 255};
    SDL_Color textColor7 = {(Uint8)(rand() % (64 + 128)), (Uint8)(rand() % (64 + 128)), (Uint8)(rand() % (64 + 128)), 255};

    // tao render cho game
    void renderGrid(bool &running) {
        int width, height;

        if (level == 1) {
            height = SCREEN_HEIGHT_EASY;
            width = SCREEN_WIDTH_EASY;
        } else if (level == 2) {
            height = SCREEN_HEIGHT_MEDIUM;
            width = SCREEN_WIDTH_MEDIUM;
        } else {
            height = SCREEN_HEIGHT_HARD;
            width = SCREEN_WIDTH_HARD;
        }

        //title
        SDL_Rect titleRect = {0, 0, width, 60};
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(renderer, &titleRect);

        //mode box
        SDL_Rect modeRect = {20, 15, 120, 30};
        if (level == 1) SDL_SetRenderDrawColor(renderer, 0, 191, 255, 255);
        else if (level == 2) SDL_SetRenderDrawColor(renderer, 255, 223, 0, 255);
        else SDL_SetRenderDrawColor(renderer, 255, 69, 0, 255);
        SDL_RenderFillRect(renderer, &modeRect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &modeRect);

        //hien dong chu len modebox
        SDL_Color sModeColor = {0, 0, 0, 255};
        SDL_Surface* sModeSurface1 = TTF_RenderText_Solid(gFont, "Easy", sModeColor);
        SDL_Surface* sModeSurface2 = TTF_RenderText_Solid(gFont, "Medium", sModeColor);
        SDL_Surface* sModeSurface3 = TTF_RenderText_Solid(gFont, "Hard", sModeColor);
        SDL_Texture* sModeText = SDL_CreateTextureFromSurface(renderer, level == 1 ? sModeSurface1 : (level == 2 ? sModeSurface2 : sModeSurface3));
        int sModew, sModeh;
        SDL_QueryTexture(sModeText, NULL, NULL, &sModew, &sModeh);
        SDL_Rect sModeRect = {30, 20, sModew, sModeh};
        SDL_RenderCopy(renderer, sModeText, NULL, &sModeRect);

        // xem so luong co
        // hien so co xanh duong va xanh la
        SDL_Color bColor = {0, 0, 255, 255};
        SDL_Color gColor = {0, 255, 0, 255};
        SDL_Surface* bSurface = TTF_RenderText_Solid(gFont, to_string(blueFlagLeft).c_str(), bColor);
        SDL_Surface* gSurface = TTF_RenderText_Solid(gFont, to_string(greenFlagLeft).c_str(), gColor);
        SDL_Texture* bText = SDL_CreateTextureFromSurface(renderer, bSurface);
        SDL_Texture* gText = SDL_CreateTextureFromSurface(renderer, gSurface);
        SDL_FreeSurface(bSurface);
        SDL_FreeSurface(gSurface);
        int bWidth, bHeight, gWidth, gHeight;
        SDL_QueryTexture(bText, NULL, NULL, &bWidth, &bHeight);
        SDL_QueryTexture(gText, NULL, NULL, &gWidth, &gHeight);
        SDL_Rect bRect = {width - (bWidth + 20), 15, bWidth, 30};
        SDL_Rect bFlagRect = {width - (bWidth + 20 + 10 + 30), 15, 30, 30};
        SDL_Rect gRect = {width - (bWidth + 20 + 10 + 30 + 20 + gWidth), 15, gWidth, 30};
        SDL_Rect gFlagRect = {width - (bWidth + 20 + 10 + 30 + 20 + gWidth + 10 + 30), 15, 30, 30};
        SDL_RenderCopy(renderer, bText, NULL, &bRect);
        SDL_RenderCopy(renderer, blueFlag, NULL, &bFlagRect);
        SDL_RenderCopy(renderer, gText, NULL, &gRect);
        SDL_RenderCopy(renderer, greenFlag, NULL, &gFlagRect);

        for (int i = 0; i < grid.size(); i++) {
            for (int j = 0; j < grid[0].size(); j++) {
                int sz = 40;
                
                // xac dinh vi tri moi o
                SDL_Rect tileRect = { sz * j, sz * i + 60, sz, sz };

                // tao mau cho o khi o duoc mo
                if (grid[i][j].isRevealed) {
                    if (grid[i][j].isPositiveMine) {
                        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    } else if (grid[i][j].isNegativeMine) {
                        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                    } else {
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    }
                } else {
                    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                }
                SDL_RenderFillRect(renderer, &tileRect);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawRect(renderer, &tileRect);

                // hien thi so min lan can
                if (grid[i][j].isRevealed) {
                    if (!grid[i][j].isNegativeMine && !grid[i][j].isPositiveMine) {
                        if (grid[i][j].isMineNeared) {
                            SDL_Surface* surfaceMessage;
                            SDL_Color textColor;

                            int neighborMineCount = grid[i][j].neighboringMine;
                            switch (neighborMineCount) {
                                case 0: case -8: textColor = textColor0; break;
                                case 1: case -7: textColor = textColor1; break;
                                case 2: case -6: textColor = textColor2; break;
                                case 3: case -5: textColor = textColor3; break;
                                case 4: case -4: textColor = textColor4; break;
                                case 5: case -3: textColor = textColor5; break;
                                case 6: case -2: textColor = textColor6; break;
                                case 7: case -1: textColor = textColor7; break;
                                default: textColor = {0, 0, 0}; break;
                            }

                            surfaceMessage = TTF_RenderText_Solid(gFont, to_string(neighborMineCount).c_str(), textColor);
                            SDL_Texture *message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
                            SDL_Rect textRect;
                            if (grid[i][j].neighboringMine >= 0) textRect = { sz * j + 10, sz * i + 60 + 10, sz - 20, sz - 20 };
                            else textRect = { sz * j + 5, sz * i + 60 + 10, sz - 10, sz - 20 };
                            SDL_RenderCopy(renderer, message, NULL, &textRect);
                            SDL_DestroyTexture(message);

                            SDL_FreeSurface(surfaceMessage);
                        }
                    }
                }

                // ham cam co
                flagged(i, j);
                
                

                showBomb(i, j);
            }
        }
        //hien thi dropdown
        if (showMode){
            for (int i = 1; i <= 3; i++){
                SDL_Rect modeRecti = {20, 15 + 30 * i, 120, 30};
                if (i == 1) SDL_SetRenderDrawColor(renderer, 0, 191, 255, 255);
                else if (i == 2) SDL_SetRenderDrawColor(renderer, 255, 223, 0, 255);
                else SDL_SetRenderDrawColor(renderer, 255, 69, 0, 255);
                SDL_RenderFillRect(renderer, &modeRecti);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawRect(renderer, &modeRecti);

                SDL_Texture* sModeTexti = SDL_CreateTextureFromSurface(renderer, i == 1 ? sModeSurface1 : (i == 2 ? sModeSurface2 : sModeSurface3));
                int sModewi, sModehi;
                SDL_QueryTexture(sModeTexti, NULL, NULL, &sModewi, &sModehi);
                SDL_Rect sModeRecti = {30, 20 + 30 * i, sModewi, sModehi};
                SDL_RenderCopy(renderer, sModeTexti, NULL, &sModeRecti);
            }
        }
        SDL_FreeSurface(sModeSurface1);
        SDL_FreeSurface(sModeSurface2);
        SDL_FreeSurface(sModeSurface3);

        if (!gameStatus || !tileLeft) {
            SDL_RenderPresent(renderer);
            SDL_Delay(2000);
            showMenu(running);
        }
    }

    void showMenu(bool &running) {
        SDL_RenderClear(renderer);
        int width, height;
        bool replay = false;

        if (level == 1) {
            height = SCREEN_HEIGHT_EASY;
             width = SCREEN_WIDTH_EASY;
        } else if (level == 2) {
            height = SCREEN_HEIGHT_MEDIUM;
             width = SCREEN_WIDTH_MEDIUM;
        } else {
            height = SCREEN_HEIGHT_HARD;
            width = SCREEN_WIDTH_HARD;
        }

        SDL_Rect menuRect = {0, 0, width, height};  
        if (tileLeft) SDL_RenderCopy(renderer, youLose, NULL, &menuRect);
        else SDL_RenderCopy(renderer, youWin, NULL, &menuRect);
        SDL_Color textColor = {255, 255, 255};

        SDL_Surface* messageSurface = TTF_RenderText_Solid(bFont, "Play Again", textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);
        SDL_FreeSurface(messageSurface);

        //lay kich thuoc texture
        int textWidth, textHeight;
        SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);
        
        // tao 1 box cho play again
        SDL_Rect textRect = {(width - textWidth) / 2, height - (40 + textHeight), textWidth, textHeight};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_RenderPresent(renderer);

        SDL_Event e;
        bool waiting = true;
        while (waiting) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    waiting = false;
                    replay = false;
                }
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    int x = e.button.x;
                    int y = e.button.y;
                    if (x >= (width - textWidth) / 2 && x <= (width - textWidth) / 2 + textWidth &&
                        y >= height - (40 + textHeight) && y <= height - (40 + textHeight) + 40) {
                        replay = true;
                        waiting = false;
                    }
                }
            }
        }
        if (replay) {
            gameStatus = true;
            InitGrid(level);
            SDL_RenderClear(renderer);
            renderGrid(running);
            SDL_RenderPresent(renderer);
            SDL_Delay(16);
        }
        else running = false;
    }

    void endGame(){
        TTF_CloseFont(gFont);
        TTF_CloseFont(bFont);
        SDL_DestroyTexture(greenFlag);
        SDL_DestroyTexture(blueFlag);
        SDL_DestroyTexture(mine);
        SDL_DestroyTexture(youLose);
        SDL_DestroyTexture(youWin);
        SDL_DestroyTexture(dropdown);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }


    void showBomb(int x, int y){
        if (grid[x][y].isNegativeFlagged || grid[x][y].isPositiveFlagged) return;
        SDL_Rect mineRect = {40 * y + 5, 40 * x + 60 + 5, 30, 30};
        if (grid[x][y].isNegativeMine || grid[x][y].isPositiveMine){
            if (grid[x][y].isRevealed) {
                SDL_RenderCopy(renderer, mine, NULL, &mineRect);
            }
        }
    }

    //ham cam co
    void flagged(int i, int j){
        int sz = 40;
        SDL_Rect tileRect = { sz * j, sz * i + 60, sz, sz };

        SDL_Rect flagRect = {sz * j + 5, sz * i + 60 + 5, 30, 30};
        if (!grid[i][j].isPositiveFlagged && !grid[i][j].isNegativeFlagged){}
        else if (grid[i][j].isPositiveFlagged){
            SDL_RenderCopy(renderer, greenFlag, NULL, &flagRect);
        }
        else if (grid[i][j].isNegativeFlagged){
            SDL_RenderCopy(renderer, blueFlag, NULL, &flagRect);
        }
    }

    //ham chay game
    void run(){
        bool running = true;
        SDL_Event event;

        while (running){
            while(SDL_PollEvent(&event)){
                if (event.type == SDL_QUIT) running = false;
                else{
                    if (event.type == SDL_MOUSEBUTTONDOWN){
                        int mY, mX;
                        SDL_GetMouseState(&mX, &mY);

                        
                        if (showMode) {
                            if (mX >= 20 && mX <= 140 && mY >= 15 + 30 && mY < 45 + 30) {
                                showMode = false;
                                InitGrid(1);
                            }
                            else if (mX >= 20 && mX <= 140 && mY >= 15 + 60 && mY < 45 + 60) {
                                showMode = false;
                                InitGrid(2);
                            }
                            else if (mX >= 20 && mX <= 140 && mY >= 15 + 90 && mY <= 45 + 90) {
                                showMode = false;
                                InitGrid(3);
                            }
                            else showMode = false;
                        } else if (!showMode){
                            if (mX >= 20 && mX <= 140 && mY >= 15 && mY <= 45) showMode = true;
                            else if (mY >= 60) {
                                int x = (mY - 60) / 40;
                                int y = mX / 40;
                                if (!grid[x][y].isRevealed){
                                    if (event.button.button == SDL_BUTTON_LEFT){
                                        if (mX >= 20 && mX <= 140 && mY >= 15 && mY <= 45) showMode = true;
                                        else {
                                            if (grid[x][y].isNegativeFlagged || grid[x][y].isPositiveFlagged){}
                                            else if (grid[x][y].isPositiveMine || grid[x][y].isNegativeMine){
                                                for (auto [i, j] : positiveMine) grid[i][j].isRevealed = true;
                                                for (auto [i, j] : negativeMine) grid[i][j].isRevealed = true;
                                                gameStatus = false;
                                            }
                                            else revealTile(x, y);
                                        }
                                    }
                                    else if (event.button.button == SDL_BUTTON_RIGHT){
                                        if (!grid[x][y].isPositiveFlagged && !grid[x][y].isNegativeFlagged){
                                            grid[x][y].isPositiveFlagged = true;
                                            greenFlagLeft--;
                                        }
                                        else if (grid[x][y].isPositiveFlagged){
                                            grid[x][y].isPositiveFlagged = false;
                                            grid[x][y].isNegativeFlagged = true;
                                            greenFlagLeft++;
                                            blueFlagLeft--;
                                        }
                                        else if (grid[x][y].isNegativeFlagged){
                                            blueFlagLeft++;
                                            grid[x][y].isNegativeFlagged = false;
                                        }
                                    }
                                }
                            }
                        }
                        
                    }
                }
            }
            SDL_RenderClear(renderer);
            renderGrid(running);
            SDL_RenderPresent(renderer);
            SDL_Delay(16);
        }

    }
};

int main(int argc, char *argv[]){
    Minesweeper Game;
    Game.setup();
    Game.InitGrid();
    Game.run();
    Game.endGame();
    return 0;
}
