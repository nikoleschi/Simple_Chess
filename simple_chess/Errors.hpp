#pragma once

enum class Errors {
	WRONG_FIGURE_TYPE,
	SPACE_BETWEEN_FIGURE_COORDS,
	FIRST_COORD_NOT_LETTER,
	FIRST_COORD_EXCEEDS_SIZE,
	SECOND_COORD_NOT_NUMBER,
	SECOND_COORD_EXCEEDS_SIZE,
	INPUT_EXCEED_MAX_SIZE,
	FIGURE_ON_THE_CELL,
	FIGURE_NOT_ELIGIBLE_TO_MOVE,
	FIGURE_ON_DESIRED_PATH,
	FIGURE_HAS_BEEN_TAKEN,
	ALLIED_KING_TO_CLOSE_TO_ENEMY
};