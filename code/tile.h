#include <iostream>
using namespace std;

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