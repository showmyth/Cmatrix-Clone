#include "cmatrix.h"

int screen_height;
int screen_width;
column_x *columns;
int color_mode = COLOR_MODE_GREEN; // Default to green

//returning a random character set for a matrix
char return_random_char(void) {
		return CHAR_SET[rand() % strlen(CHAR_SET)];
}

//returning a color pair number based on color mode
int get_random_color(void) {
		switch (color_mode) {
				case COLOR_MODE_GREEN:
						return PAIR_GREEN;
				
				case COLOR_MODE_WHITE:
						return PAIR_WHITE;
				
				case COLOR_MODE_RAINBOW:
				default: {
						int colors[] = {
								PAIR_RED, PAIR_GREEN, PAIR_YELLOW,
								PAIR_BLUE, PAIR_PURPLE, PAIR_CYAN, PAIR_WHITE
						};
						return colors[rand() % 7]; // 7 colors (excluding black)
				}
		}
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
		
		//initialize colors
		if (has_colors()) {
				start_color();
				init_pair(PAIR_BLACK, COLOR_BLACK, COLOR_BLACK);
				init_pair(PAIR_RED, COLOR_RED, COLOR_BLACK);
				init_pair(PAIR_GREEN, COLOR_GREEN, COLOR_BLACK);
				init_pair(PAIR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
				init_pair(PAIR_BLUE, COLOR_BLUE, COLOR_BLACK);
				init_pair(PAIR_PURPLE, COLOR_MAGENTA, COLOR_BLACK);
				init_pair(PAIR_CYAN, COLOR_CYAN, COLOR_BLACK);
				init_pair(PAIR_WHITE, COLOR_WHITE, COLOR_BLACK);
		}
		
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
								// Get a random color for this character
								int color_pair = get_random_color();
								
								// Apply color and print character
								attron(COLOR_PAIR(color_pair));
								mvaddch(y, i, columns[i].characters[j]);
								attroff(COLOR_PAIR(color_pair));
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
int main(int argc, char *argv[]) {
		// Parse command line arguments
		if (argc > 1) {
				int flag = atoi(argv[1]);
				if (flag >= COLOR_MODE_GREEN && flag <= COLOR_MODE_RAINBOW) {
						color_mode = flag;
				} else {
						printf("Usage: %s [color_mode]\n", argv[0]);
						printf("  0 - Green (default)\n");
						printf("  1 - White\n");
						printf("  2 - Rainbow\n");
						return 1;
				}
		}

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
