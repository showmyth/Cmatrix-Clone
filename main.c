#include "cmatrix.h"

int screen_height;
int screen_width;
column_x *columns;


//returning a random character set for a matrix
char return_random_char(void) {
		return CHAR_SET[rand() % strlen(CHAR_SET)];
}

//setting random range
int return_random_range(int min, int max) {
		return min + rand() % (min + max - 1);
}

//initializing ncurses lib
void init_matrix(void) {
		initscr();
		cbreak(); //disables buffers
		noecho(); //prevents echo of input
		nodelay(stdscr, TRUE); //removes delays
		curs_set(0); //hides cursor
		
		//setting up screen matrix
		getmaxyx(stdscr, screen_height, screen_width);
		srand(time(NULL)); //time based random number generation
}

void init_cols(void) {
		columns = malloc(screen_width * sizeof(column_x));

		for (int i = 0 ; i < screen_width ; i++) {
				columns[i].max_length = return_random_range(MIN_DROP_L, MAX_DROP_L);
				columns[i].characters = malloc(columns[i].max_length * sizeof(char));
				columns[i].length = 0;
				columns[i].y = -return_random_range(0, screen_height);
				columns[i].velocity = return_random_range(MIN_V, MAX_V);
				columns[i].active = rand() % 2 == 0;

				//initializing columns with random set of chars		
				for (int j = 0 ; j < columns[i].max_length ; j++) {
						
				}
		}

}

