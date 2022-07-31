#pragma once
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <string>
#include <set>
#include "Constants.hpp"
#include "Errors.hpp"
#include "Enums.hpp"
using namespace std;

struct Figure
{
    int x = -1;
    int y = -1;
    bool taken = false;
    char sign;
};

struct Move
{
    int x, y;
    char figureSign;

    Move(char f, int X, int Y)
    {
        figureSign = f;
        x = X;
        y = Y;
    }
};

class SimpleChess 
{
    Figure alliedKing;
    Figure alliedRook1;
    Figure alliedRook2;
    Figure enemyKing;

    int sizeX;
    int sizeY;
    int turns;

    vector<vector<char>> board;

    /* Validations */
    bool isOnBoard(int x, int y);
    bool checkDirections(bool* dir);
    set<pair<int,int>> generateCheckPositions(Figure& f, set<pair<int,int>> pos);
    set<pair<int, int>> checkPositions();
    bool isInCheck(int x, int y, set<pair<int,int>>);
    bool checkValidFigure(char ch);
    bool checkAlliedKing(char ch);
    bool checkBlank(char ch);
    bool checkLowercase(char ch);
    bool checkUppercase(char ch);
    bool checkNumber(char ch);
    bool checkSizeExceeded(int a, int b);
    bool checkRookTaken(Figure f);
    void rookValidation(int x, int y, Figure f);
    bool checkTooCloseToAlliedKing(int x, int y);
    bool checkTooCloseToEnemyKing(int x, int y);
    void kingValidation(int x, int y);
    Move checkInput(string input);
    
    /* AI */
    int randomNumber(int dimension);
    void randomizeArray(int arr[], int size);
    bool enemyKingMove(set<pair<int, int>> pos);

    /* Board generator */
    void generateBoard();
    void generateFigures();

    /* Converting X and Y coordinates to board coordinates */
    int convertX(int x);
    int convertY(int y);

    /* Printing board */
    void printBoard();
    void printTurnsTaken(const char* option);
    void printInstructions();

    /* Moving figures*/
    void takeRook(Figure& f);
    void moveFigure(Figure& f, int x, int y);

    /* End game */
    bool checkEndGame(set<pair<int, int>> pos);

public:
    SimpleChess(const int sizeX = 8, const int sizeY = 8);

    bool startGame();
};