#include "SimpleChess.hpp"

bool SimpleChess::isOnBoard(int x, int y)
{
	return x >= 0 && x < sizeX && y >= 0 && y < sizeY;
}

bool SimpleChess::checkDirections(bool* dir)
{
    return dir[Directions::LEFT] || dir[Directions::DOWN] || dir[Directions::RIGHT] || dir[Directions::UP];
}

set<pair<int, int>> SimpleChess::generateCheckPositions(Figure& f, set<pair<int, int>> pos)
{
    bool directions[] = { true, true, true, true };
    int dist = 1;

    while (checkDirections(directions))
    {
        for (int i = 0; i < sizeof(directions); ++i)
            if (directions[i])
            {
                int newX = f.x + moveX4[i] * dist;
                int newY = f.y + moveY4[i] * dist;
                if (!isOnBoard(newY, newX) || checkAlliedKing(board[convertX(newX)][convertY(newY)])) directions[i] = false;
                else pos.insert({ newX, newY });
            }
        ++dist;
    }

    return pos;
}

set<pair<int, int>> SimpleChess::checkPositions()
{
    set<pair<int, int>> pos;
    
    pos = generateCheckPositions(alliedRook1, pos);
    pos = generateCheckPositions(alliedRook2, pos);

    return pos;
}

bool SimpleChess::isInCheck(int x, int y, set<pair<int,int>> pos)
{
    if (checkTooCloseToAlliedKing(x, y)) return false;
  
    return !pos.count(make_pair(x, y));
}

bool SimpleChess::checkValidFigure(char ch)
{
	return ch == ALLIED_KING || ch == FIRST_ROOK || ch == SECOND_ROOK;
}

bool SimpleChess::checkAlliedKing(char ch)
{
    return ch == ALLIED_KING;
}

bool SimpleChess::checkBlank(char ch)
{
	return ch == char(Board_Pieces::WHITESPACE);
}

bool SimpleChess::checkLowercase(char ch)
{
	return ch >= 'a' && ch <= 'z';
}

bool SimpleChess::checkUppercase(char ch)
{
	return ch >= 'A' && ch <= 'Z';
}

bool SimpleChess::checkNumber(char ch)
{
	return ch >= '0' && ch <= '9';
}

bool SimpleChess::checkSizeExceeded(int a, int b)
{
	return a > b;
}

bool SimpleChess::checkRookTaken(Figure f)
{
	return f.taken;
}

void SimpleChess::rookValidation(int x, int y, Figure f)
{
    if (checkRookTaken(f)) throw Errors::FIGURE_HAS_BEEN_TAKEN;

    if (x == f.x)
    {
        int yIncrement = (y - f.y) / abs(y - f.y);
        for (int i = f.y + yIncrement;i != y;i += yIncrement)
            if (!checkBlank(board[convertX(f.x)][convertY(i)])) throw Errors::FIGURE_ON_DESIRED_PATH;
    }
    else if (y == f.y)
    {
        int xIncrement = (x - f.x) / abs(x - f.x);
        for (int i = f.x + xIncrement;i != x;i += xIncrement)
            if (!checkBlank(board[convertX(i)][convertY(f.y)])) throw Errors::FIGURE_ON_DESIRED_PATH;
    }
    else if (x != f.x && y != f.y) throw Errors::FIGURE_NOT_ELIGIBLE_TO_MOVE;
}

bool SimpleChess::checkTooCloseToAlliedKing(int x, int y)
{
    return !(abs(x - alliedKing.x) > 1 || abs(y - alliedKing.y) > 1);
}

bool SimpleChess::checkTooCloseToEnemyKing(int x, int y)
{
    return !(abs(x - enemyKing.x) > 1 || abs(y - enemyKing.y) > 1);
}

void SimpleChess::kingValidation(int x, int y)
{
    if (!( (abs(x - alliedKing.x) == 1 && abs(y - alliedKing.y) <= 1) 
        || (abs(x - alliedKing.x) <= 1 && abs(y - alliedKing.y) == 1) )) throw Errors::FIGURE_NOT_ELIGIBLE_TO_MOVE;
    if (checkTooCloseToEnemyKing(x, y)) throw Errors::ALLIED_KING_TO_CLOSE_TO_ENEMY;
}

Move SimpleChess::checkInput(string input)
{
    int inputSize = input.length(), size = 0;
    int coordX, coordY;

    if (!checkValidFigure(input[Input_Indexes::FIGURE_INDEX])) throw Errors::WRONG_FIGURE_TYPE;
    if (!checkBlank(input[Input_Indexes::WHITESPACE_INDEX])) throw Errors::SPACE_BETWEEN_FIGURE_COORDS;

    if (checkLowercase(input[Input_Indexes::LETTER_COORD_INDEX])) input[Input_Indexes::LETTER_COORD_INDEX] -= 'a' - 'A';
    if (!checkUppercase(input[Input_Indexes::LETTER_COORD_INDEX])) throw Errors::FIRST_COORD_NOT_LETTER;
    if (checkSizeExceeded(int(input[Input_Indexes::LETTER_COORD_INDEX]), 'A' + sizeX - 1)) throw Errors::FIRST_COORD_EXCEEDS_SIZE;

    coordY = input[Input_Indexes::LETTER_COORD_INDEX] - 'A';

    if (inputSize > 5) throw Errors::INPUT_EXCEED_MAX_SIZE;
    else if (inputSize == 4)
    {
        if (!checkNumber(input[Input_Indexes::NUMBER_COORD_INDEX_1])) throw Errors::SECOND_COORD_NOT_NUMBER;
        size = input[Input_Indexes::NUMBER_COORD_INDEX_1] - '0';
    }
    else
    {
        if (!checkNumber(input[Input_Indexes::NUMBER_COORD_INDEX_1]) || !checkNumber(input[Input_Indexes::NUMBER_COORD_INDEX_2])) 
            throw Errors::SECOND_COORD_NOT_NUMBER;
        size = (input[Input_Indexes::NUMBER_COORD_INDEX_1] - '0') * 10 + input[Input_Indexes::NUMBER_COORD_INDEX_2] - '0';
    }

    if (checkSizeExceeded(size, sizeY)) throw Errors::SECOND_COORD_EXCEEDS_SIZE;
    coordX = size - 1;

    if (!checkBlank(board[convertX(coordX)][convertY(coordY)])) throw Errors::FIGURE_ON_THE_CELL;

    return Move(input[Input_Indexes::FIGURE_INDEX], coordX, coordY);
}

int SimpleChess::randomNumber(int dimension)
{
    return rand() % dimension;
}

void SimpleChess::randomizeArray(int arr[], int size)
{
    for (int i = size - 1; i > 0; i--)
    {
        int j = randomNumber(i + 1);
        swap(arr[i], arr[j]);
    }
}

bool SimpleChess::enemyKingMove(set<pair<int, int>> pos)
{
    int seeds[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    pair<int, int> newCoords;
    bool found = false;

    randomizeArray(seeds, sizeof(seeds) / sizeof(int));

    for (int i = 0;i < sizeof(seeds) / sizeof(int);++i)
    {
        int newX = enemyKing.x + moveX8[seeds[i]];
        int newY = enemyKing.y + moveY8[seeds[i]];

        if (isOnBoard(newY, newX) && isInCheck(newX, newY, pos))
        {
            if (board[convertX(newX)][convertY(newY)] == FIRST_ROOK || board[convertX(newX)][convertY(newY)] == SECOND_ROOK)
            {
                if (board[convertX(newX)][convertY(newY)] == FIRST_ROOK) takeRook(alliedRook1);
                else takeRook(alliedRook2);

                moveFigure(enemyKing, newX, newY);
                return false;
            }

            if (!found)
            {
                newCoords = { newX, newY };
                found = true;
            }
        }
    }

    if (found)
    {
        moveFigure(enemyKing, newCoords.first, newCoords.second);
        return false;
    }

    return true;
}

void SimpleChess::generateBoard()
{
    vector<char> row;

    /* First row */
    row.push_back(char(Board_Pieces::TOP_LEFT_CORNER));
    for (int i = 1;i < 4 * sizeX;++i)
        if (i % 4 == 0) row.push_back(char(Board_Pieces::HORIZONTAL_DOWN_CONNECTOR));
        else row.push_back(char(Board_Pieces::HORIZONTAL_PIPE));
    
    row.push_back(char(Board_Pieces::TOP_RIGHT_CORNER));
    board.push_back(row);
    
    /* Middle rows */
    for (int i = 1; i < 2 * sizeY;++i)
    {
        row.clear();
        if (i % 2 == 1)
        {
            for (int i = 0;i <= 4 * sizeX;++i)
            {
                if (i % 4 == 0) row.push_back(char(Board_Pieces::VERTICAL_PIPE));
                else row.push_back(char(Board_Pieces::WHITESPACE));
            }
        }
        else
        {
            row.push_back(char(Board_Pieces::VERTICAL_RIGHT_CONNECTOR));
            for (int i = 1;i < 4 * sizeX;++i)
            {
                if (i % 4 == 0) row.push_back(char(Board_Pieces::FOUR_WAY_CONNECTOR));
                else row.push_back(char(Board_Pieces::HORIZONTAL_PIPE));
            }
            row.push_back(char(Board_Pieces::VERTICAL_LEFT_CONNECTOR));
        }
        board.push_back(row);
    }

    /* Last row*/
    row.clear();
    row.push_back(char(Board_Pieces::BOT_LEFT_CORNER));
    for (int i = 1;i < 4 * sizeX;++i)
        if (i % 4 == 0) row.push_back(char(Board_Pieces::HORIZONTAL_UP_CONNECTOR));
        else row.push_back(char(Board_Pieces::HORIZONTAL_PIPE));
    
    row.push_back(char(Board_Pieces::BOT_RIGHT_CORNER));
    board.push_back(row);
}

void SimpleChess::generateFigures()
{
    //Enemy King
    enemyKing.x = randomNumber(sizeY);
    enemyKing.y = randomNumber(sizeX);
    enemyKing.sign = ENEMY_KING;
    board[convertX(enemyKing.x)][convertY(enemyKing.y)] = ENEMY_KING;

    //Allied King
    do
    {
        alliedKing.x = randomNumber(sizeY);
        alliedKing.y = randomNumber(sizeX);
    } while (checkTooCloseToEnemyKing(alliedKing.x, alliedKing.y));
    alliedKing.sign = ALLIED_KING;
    board[convertX(alliedKing.x)][convertY(alliedKing.y)] = ALLIED_KING;

    //Allied Rook 1
    do alliedRook1.x = randomNumber(sizeY);
    while (alliedRook1.x == enemyKing.x);
    do alliedRook1.y = randomNumber(sizeX);
    while (alliedRook1.y == enemyKing.y || !checkBlank(board[convertX(alliedRook1.x)][convertY(alliedRook1.y)]));
    alliedRook1.sign = FIRST_ROOK;
    board[convertX(alliedRook1.x)][convertY(alliedRook1.y)] = FIRST_ROOK;

    //Allied Rook 2
    do alliedRook2.x = randomNumber(sizeY);
    while (alliedRook2.x == enemyKing.x);
    do alliedRook2.y = randomNumber(sizeX);
    while (alliedRook2.y == enemyKing.y || !checkBlank(board[convertX(alliedRook2.x)][convertY(alliedRook2.y)]));
    alliedRook2.sign = SECOND_ROOK;
    board[convertX(alliedRook2.x)][convertY(alliedRook2.y)] = SECOND_ROOK;
}

int SimpleChess::convertX(int x)
{
    return x * 2 + 1;
}

int SimpleChess::convertY(int y)
{
    return y * 4 + 2;
}

void SimpleChess::printBoard()
{
    cout << "  A";
    for (int i = 1;i < sizeX;++i) cout << setw(4) << char('A' + i);
    cout << setw(15) << "Legend:\n";
    for (int i = 0;i <= sizeY * 2;++i)
    {
        for (int j = 0;j <= sizeX * 4;++j) cout << board[i][j];
        if (i % 2 == 1) cout << " " << i / 2 + 1;
        switch (i)
        {
        case 0: cout << setw(19) << "P - enemy King"; break;
        case 1: cout << setw(18) << "K - allied King"; break;
        case 2: cout << setw(22) << "R - allied Rook 1"; break;
        case 3: cout << setw(20) << "r - allied Rook 2"; break;
        }
        cout << endl;
    }
}

void SimpleChess::printTurnsTaken(const char* option)
{
    printf("Turns taken%s: %d\n", option, turns);
}

void SimpleChess::printInstructions()
{
    cout << "Input should be \"x sN\", where\nx - figure you want to move(K, R, r)\ns - letter of column(Can be either lower or upper case)\nN - number of row\n";
    cout << "menu -> return to the menu\nexit -> exit the program\n";
}

void SimpleChess::takeRook(Figure& f)
{
    f.taken = true;
    f.x = TAKEN;
    f.y = TAKEN;
}

void SimpleChess::moveFigure(Figure& f, int x, int y)
{
    board[convertX(f.x)][convertY(f.y)] = Board_Pieces::WHITESPACE;
    board[convertX(x)][convertY(y)] = f.sign;
    f.x = x;
    f.y = y;
}

bool SimpleChess::checkEndGame(set<pair<int, int>> pos)
{
    system("cls");
    if (enemyKingMove(pos))
    {
        printBoard();
        if (!isInCheck(enemyKing.x, enemyKing.y, pos))
        {
            cout << "Congratulations! You won!\n";
            printTurnsTaken(WIN);
            return true;
        }
        else
        {
            cout << "Stalemate! (Draw)\n";
            printTurnsTaken(DRAW);
            return true;
        }
    }
    else if (alliedRook1.taken && alliedRook2.taken)
    {
        printBoard();
        cout << "Draw!\n";
        printTurnsTaken(DRAW);
        return true;
    }
    
    return false;
}

SimpleChess::SimpleChess(const int sizeX, const int sizeY)
{
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    turns = 0;

    generateBoard();
    generateFigures();

}

bool SimpleChess::startGame()
{
    string userInput;
    string errMsg = EMPTY_STRING;
    do
    {
        system("cls");
        printBoard();
        printTurnsTaken(EMPTY_STRING);
        printInstructions();
        if (!errMsg.empty()) cout << "Invalid input! " << errMsg << "\n";
        cout << ">>";
        getline(cin, userInput);
        if (userInput == MENU) return true;
        else if (userInput == EXIT) return false;
        else
        {
            try
            {
                Move moveObject = checkInput(userInput);
                errMsg.clear();
                switch (moveObject.figureSign)
                {
                case FIRST_ROOK:
                {
                    rookValidation(moveObject.x, moveObject.y, alliedRook1);
                    moveFigure(alliedRook1, moveObject.x, moveObject.y);
                } break;
                case SECOND_ROOK:
                {
                    rookValidation(moveObject.x, moveObject.y, alliedRook2);
                    moveFigure(alliedRook2, moveObject.x, moveObject.y);
                } break;
                case ALLIED_KING:
                {
                    kingValidation(moveObject.x, moveObject.y);
                    moveFigure(alliedKing, moveObject.x, moveObject.y);
                } break;
                }

                ++turns;

                set <pair<int, int>> checkPos = checkPositions();
                if (checkEndGame(checkPos))
                {
                    system("pause");
                    break;
                }
            }
            catch (Errors error) {
                switch (error)
                {
                case Errors::WRONG_FIGURE_TYPE: errMsg = "Figure must be K, R, r!"; break;
                case Errors::SPACE_BETWEEN_FIGURE_COORDS: errMsg = "There must be 1 space between figure and coordinates!"; break;
                case Errors::FIRST_COORD_NOT_LETTER: errMsg = "First coordinate is not a letter!"; break;
                case Errors::FIRST_COORD_EXCEEDS_SIZE: errMsg = "First coordinate exceeds the size of the board!"; break;
                case Errors::SECOND_COORD_NOT_NUMBER: errMsg = "Second coordinate is not a number!"; break;
                case Errors::SECOND_COORD_EXCEEDS_SIZE: errMsg = "Second coordinate exceeds the size of the board!"; break;
                case Errors::INPUT_EXCEED_MAX_SIZE: errMsg = "Input exceeds the maximum size!"; break;
                case Errors::FIGURE_ON_THE_CELL: errMsg = "There is a figure on desired cell!"; break;
                case Errors::FIGURE_NOT_ELIGIBLE_TO_MOVE: errMsg = "Figure is not eligible to move to desired coordinates!"; break;
                case Errors::FIGURE_ON_DESIRED_PATH: errMsg = "There is a figure on the path of the figure you want to move!"; break;
                case Errors::FIGURE_HAS_BEEN_TAKEN: errMsg = "Figure has been taken and cannot move!"; break;
                case Errors::ALLIED_KING_TO_CLOSE_TO_ENEMY: errMsg = "Allied King is not eligible to moove to desired coordinates because they are too close to Enemy King!"; break;
                }
            }
        }
    } while (true);

    return true;
}
