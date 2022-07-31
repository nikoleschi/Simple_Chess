#include "SimpleChess.hpp"
using namespace std;

const string ERROR_MESSAGE = "Invalid option!\n";

int main()
{
	srand((unsigned int)time(NULL));
	int sizeBoardX = DEFAULT_BOARD_SIZE, sizeBoardY = DEFAULT_BOARD_SIZE;
	char option;
	string errMsg;
	while (true)
	{
		system("cls");
		cout << errMsg;
		cout << "1. New game\n2. Change board size\n3. Exit\nChoose option: ";
		cin >> option;
		cin.ignore();
		errMsg.clear();
		switch (option)
		{
		case Options::NEW_GAME: 
		{
			SimpleChess game(sizeBoardX, sizeBoardY);
			if(!game.startGame()) return 0;
		} break;
		case Options::CHANGE_BOARD_SIZE:
		{
			system("cls");
			int newX, newY;
			do
			{
				cout << "Currently the size of the board is " << sizeBoardX << "x" << sizeBoardY;
				cout << ".\nThe minimum value for the size is 4 and the maximum value is 26.\n";
				cout << "Columns: ";
				cin >> newX;
				cout << "Rows: ";
				cin >> newY;
			} while (newX < MIN_BOARD_SIZE || newX > MAX_BOARD_SIZE || newY < MIN_BOARD_SIZE || newY > MAX_BOARD_SIZE);
			sizeBoardX = newX;
			sizeBoardY = newY;
		} break;
		case Options::EXIT_GAME: return 0;
		default: errMsg = ERROR_MESSAGE;
		}
	}

	return 0;
}