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
		return min + rand() % (max - min + 1);
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
					columns[i].characters[j] = return_random_char();	
				}
		}

}

void update_columns(void) {
		for (int i = 0 ; i < screen_width ; i++) {
				if (!columns[i].active) {
						//activate any inactive column randomly
						if (rand() % 100 < 2) { //equivalent to a 2%ish chance for it activate
								columns[i].active = 1;
								columns[i].y = -columns[i].max_length;
								columns[i].length = 0;
						} continue;
				}

				//moving columns downward according to velocity
				columns[i].y += columns[i].velocity;
				
				//grow till we reach max length of column
				if (columns[i].length < columns[i].max_length) {
						columns[i].length ++;
				}

				//Randomly change characters in column
				for (int j=0 ; j < columns[i].length ; j++) {
						if (rand() % 10 == 0) {	
								columns[i].characters[j] = return_random_char();
						}
				}
				
				//if column leaves screen => make it inactive
				if (columns[i].y - columns[i].length > screen_height) {
						columns[i].active = 0;
				}
		}		
}

//Drawing the matrix
void draw_matrix(void) {
		clear(); //clears screen

		for (int i = 0 ; i < screen_width ; i++) {
				if (!columns[i].active){
						continue;
				}

				//drawing each char in column
				for (int j = 0 ; j < columns[i].length ; j++) { 
						int y = columns[i].y - j;

						//check if char on screen
						if (y>=0 &&  y < screen_height) {
								mvaddch(y, i, columns[i].characters[j]);
						}

				}

		}
		refresh();
}

void clear_matrix(void) {
		for (int i = 0 ; i < screen_width ; i++) {
				free(columns[i].characters);
		}
		free(columns);
		endwin(); //restore terminal
}

//Main Function
int main() {
		init_matrix();
		init_cols();
		//Main Loop
		while(1) {
				//check for exit key == 'Q' or 'q'
				int ch = getch();
				if (ch == 'q' || ch == 'Q'){
						break;
				}

				update_columns();
				draw_matrix();
				usleep(FRAME_DELAY);
		}

		clear_matrix();
		return 0;

}
