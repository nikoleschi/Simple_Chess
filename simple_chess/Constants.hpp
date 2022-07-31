#pragma once

const char ENEMY_KING = 'P';
const char ALLIED_KING = 'K';
const char FIRST_ROOK = 'R';
const char SECOND_ROOK = 'r';
const char EMPTY_STRING[] = "";
const char DRAW[] = " to draw";
const char WIN[] = " to win";
const char EXIT[] = "exit";
const char MENU[] = "menu";

const int moveX8[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
const int moveY8[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
const int moveX4[] = { -1, 0, 1, 0 };
const int moveY4[] = { 0, 1, 0, -1 };

const int MAX_SIZE_BOARD = 105;
const int DEFAULT_BOARD_SIZE = 8;
const int MIN_BOARD_SIZE = 4;
const int MAX_BOARD_SIZE = 26;
const int TAKEN = -1;