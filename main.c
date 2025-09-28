#ifndef CMATRIX_H
#define CMATRIX_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

//Basic Definitions
#define MIN_DROP_L 5
#define MAX_DROP_L 25
#define MIN_V 1
#define MAX_V 3
#define FRAME_DELAY 100000 // == 100000 us or 100 ms

//Character set
#define CHAR_SET "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()_+-=[]{}|;:,.<>?"

//Structure Setup
typedef struct {
		char *characters; // Array of char in column
		int length;       // Length of the current drop
		int y;			  // Current position in Y-Axis
		int velocity;	  // Speed of chars falling (rows/frame)
		int active;       // Activity of column (1/0)
		int max_length;	  // Max length drop can reach
} column_x;

//Global Vars
extern int screen_height;
extern int screen_width;
extern column_x *columns;

//Function (Placeholders)
void init_matrix(void); //initialize matrix
void clear_matrix(void); //clear matrix
void init_cols(void); //initialize a column



#endif
