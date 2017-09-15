/*
*** FILE NAME   : animation_ex.c
*** DESCRIPTION : This program is an animation example program using OpenGL
*** DATE        : Sep. 2017
*** WRITTEN By  : JKL, Che Shian Hung
*/

#include <stdio.h>               // standard C libraries
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <GL/glut.h>             // GLUT library


//@@***********************************************************************************@@
// Constants
#define WINDOW_XS 300							// Window size
#define WINDOW_YS 400
#define WINDOW_NAME "8 Puzzle"			// Window name

#define ANI_MSEC 5						// gap between frames

//@@***********************************************************************************@@
// Structures
typedef struct pt
{
	GLfloat x, y;
}MyPoint;

//@@***********************************************************************************@@
// Global Variables
MyPoint bottomLeftPt;
MyPoint boxCordinates[8];
int values[9];

int recLength = 90;		// size of puzzle
int recHeight = 90;

int offSetX = 5;
int offSetY = 5;

int doAnimation = 0;	// =0 no animation, 1= animation
int gap = 1;			// step for animation

int currentPosition;
int selectedValue = -1;
int animationCounter;


//@@***********************************************************************************@@
// Function prototypes
void reshape_handler(int width, int height);
void init_setup(int width, int height, char *windowName);
void display_func(void);
void keyboard_func(unsigned char c, int x, int y);
void mouse_func(int button, int state, int x, int y);
void animation_func(int val);
void output(int x, int y, int mode, char *string);
void drawTile(char *val, float x, float y);
int getPosition(int x, int y);
int getDirection(int p);
void setupPuzzle();
void drawAllTiles();
void switchPosition(int a, int b);

//@@***********************************************************************************@@
int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	init_setup(WINDOW_XS, WINDOW_YS, WINDOW_NAME);

	setupPuzzle();
	printf("current position: %d\n\n", currentPosition);

	bottomLeftPt.x = 0;
	bottomLeftPt.y = 0;

	glutDisplayFunc(display_func);
	glutKeyboardFunc(keyboard_func);
	glutMouseFunc(mouse_func);
	glutTimerFunc(ANI_MSEC, animation_func, 0);

	glutMainLoop();

	return 1;
}	// end of main()

	//@@***********************************************************************************@@
void reshape_handler(int width, int height)
{
	glViewport(0, 0, width, height);							// sets the viewport
	glMatrixMode(GL_PROJECTION);								// projection matrix
	glLoadIdentity();											// loads identity matrix
	gluOrtho2D(0.0, (GLdouble)width, 0.0, (GLdouble)height);	// 2D orthographic projection
}	// end of reshape_handler()

	//@@***********************************************************************************@@
void init_setup(int width, int height, char *windowName)
{
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  // single buffer, rgb color
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  // double buffer, rgb color
	glutInitWindowSize(width, height);			  // init. window size
	glutInitWindowPosition(5, 5);				  // init. window position
	glutCreateWindow(windowName);				  // window name
	glutReshapeFunc(reshape_handler);		      // sets the reshape call back
}	// end of init_setup()

	//@@***********************************************************************************@@
void display_func(void)
{
	int i;

	glClearColor(0.0, 0.0, 0.0, 1.0);   // background color (yellow)
	glClear(GL_COLOR_BUFFER_BIT);         // clearing the buffer not to keep the color

	/*
										  // draw top of the puzzle board
	glColor3f(1.0, 1.0, 1.0);				// setting pen color 
	glBegin(GL_POLYGON);
	glVertex2i(0, 300);
	glVertex2i(300, 300);
	glVertex2i(300, 400);
	glVertex2i(0, 400);
	glEnd();

	*/

	// banner
	glColor3f(1.0, 1.0, 1.0);					// setting pen color
	output(70, 360, 1, "CS4200/5200: 8 Puzzle");	// puzzle number
	output(150, 320, 2, "Che Shian Hung");	// puzzle number

											// draw the grids for the puzzle board
	glColor3f(1.0, 1.0, 1.0);			// setting pen color
	glBegin(GL_LINES);
	for (i = 0; i < WINDOW_XS; i += 100)
	{
		glVertex2i(i, 0);		// vertical grid lines
		glVertex2i(i, 300);

		glVertex2i(0, i);		// horizontal grid lines
		glVertex2i(300, i);
	}
	glVertex2i(0, 300);
	glVertex2i(300, 300);
	glEnd();

	drawAllTiles();

	glFlush();

	glutSwapBuffers();	// double buffering

}	// end of display_func()


	//@@***********************************************************************************@@
void keyboard_func(unsigned char c, int x, int y)
{
	switch (c)
	{
	case 'Q':
	case 'q':
		printf("Good Bye !\n");
		exit(0);				 // terminates the program
	}  // end of switch
}	// end of keyboard_func()

	//@@***********************************************************************************@@
void mouse_func(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && doAnimation == 0) {
		int p = getPosition(x, WINDOW_YS - y);
		//printf("position: %d\n", p);
		
		if (p != -1 && p != currentPosition){
			selectedValue = values[p];
			int d = getDirection(p);
			int temp;

			//printf("direction: %d\n", d);
			printf("values: %d\n", selectedValue);
			doAnimation = d;
			switch (d) {
			case 1: 
				switchPosition(currentPosition, currentPosition + 3);
				currentPosition += 3;
				break;
			case 2:
				switchPosition(currentPosition, currentPosition - 3);
				currentPosition -= 3;
				break;
			case 3:
				switchPosition(currentPosition, currentPosition + 1);
				currentPosition++;
				break;
			case 4:
				switchPosition(currentPosition, currentPosition - 1);
				currentPosition--;
				break;
			}
			animationCounter = 0;
			glutPostRedisplay();
		}
	}
}

//@@***********************************************************************************@@
void animation_func(int val)
{
	if (doAnimation == 1) {
		boxCordinates[selectedValue - 1].y += gap;
		animationCounter++;
	}
	else if (doAnimation == 2) {
		boxCordinates[selectedValue - 1].y -= gap;
		animationCounter++;
	}
	else if (doAnimation == 3) {
		boxCordinates[selectedValue - 1].x -= gap;
		animationCounter++;
	}
	else if (doAnimation == 4) {
		boxCordinates[selectedValue - 1].x += gap;
		animationCounter++;
	}
	if (animationCounter >= 100) {
		doAnimation = 0;
	}

	/*
	// moving for animation
	if (doAnimation == 1)
	{
		bottomLeftPt.x += gap;

		if (bottomLeftPt.x > 200)
		{
			gap *= (-1);
		}
		else if (bottomLeftPt.x < 0)
		{
			bottomLeftPt.x = 0;
			gap *= (-1);
			doAnimation = 0;
		}
	}
	if (doAnimation == 2) {
		bottomLeftPt.y += gap;
		if (bottomLeftPt.y > 200)
			gap *= (-1);
		else if (bottomLeftPt.y < 0) {
			bottomLeftPt.y = 0;
			gap *= (-1);
			doAnimation = 0;
		}
	}
	*/

	glutPostRedisplay();

	glutTimerFunc(ANI_MSEC, animation_func, 0);

}	//end animation_func

	//@@***********************************************************************************@@
void output(int x, int y, int mode, char *string)
{
	int len, i;
	glRasterPos2i(x, y);
	len = (int)strlen(string);
	if (mode == 1)
	{
		for (i = 0; i < len; i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
		}
	}
	else if (mode == 2)
	{
		for (i = 0; i < len; i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
		}
	}
}

void drawTile(char *val, float x, float y) {
	// draw a puzzle piece
	int x2 = (int)x;
	int y2 = (int)y;
	glColor3f(1.0, 1.0, 1.0);			// setting pen color
	glBegin(GL_POLYGON);
	glVertex2i(x2 + offSetX, y2 + offSetY);
	glVertex2i(x2 + offSetX + recLength, y2 + offSetY);
	glVertex2i(x2 + offSetX + recLength, y2 + offSetY + recHeight);
	glVertex2i(x2 + offSetX, y2 + offSetY + recHeight);
	glEnd();
	glColor3f(0.0, 0.0, 0.0);			// setting pen color
	output(x2 + offSetX + 39, y2 + offSetY + 37, 1, val);	// puzzle number
}

int getPosition(int x, int y) {
	if (x < 95 && x > 5) {
		if (y < 95 && y > 5) return 6;
		else if (y < 195 && y > 105) return 3;
		else if (y < 295 && y > 205) return 0;
	}
	else if (x < 195 && x > 105) {
		if (y < 95 && y > 5) return 7;
		else if (y < 195 && y > 105) return 4;
		else if (y < 295 && y > 205) return 1;
	}
	else if (x < 295 && x > 205) {
		if (y < 95 && y > 5) return 8;
		else if (y < 195 && y > 105) return 5;
		else if (y < 295 && y > 205) return 2;
	}

	return -1;
}

int getDirection(int p) {
	//up
	if (p == currentPosition + 3) return 1;
	//down 
	else if (p == currentPosition - 3) return 2;
	//left
	else if (p == currentPosition + 1 && p % 3 != 0) return 3;
	//right
	else if (p == currentPosition - 1 && p % 3 != 2) return 4;
	else return 0;
}

void setupPuzzle() {

	int seed = time(NULL);
	int usedValues[9];
	for (int i = 0; i < 9; i++) {
		usedValues[i] = i;
	}

	srand(seed);
	for (int i = 0; i < 9; i++) {
		int r = rand() % (9 - i);
		values[i] = usedValues[r];
		usedValues[r] = usedValues[8 - i];
		usedValues[8 - i] = values[i];

		if (values[i] != 0) {
			int xPos, yPos;
			switch (i) {
			case 0:
				xPos = 0;
				yPos = 200;
				break;
			case 1:
				xPos = 100;
				yPos = 200;
				break;
			case 2:
				xPos = 200;
				yPos = 200;
				break;
			case 3:
				xPos = 0;
				yPos = 100;
				break;
			case 4:
				xPos = 100;
				yPos = 100;
				break;
			case 5:
				xPos = 200;
				yPos = 100;
				break;
			case 6:
				xPos = 0;
				yPos = 0;
				break;
			case 7:
				xPos = 100;
				yPos = 0;
				break;
			case 8:
				xPos = 200;
				yPos = 0;
				break;
			}
			boxCordinates[values[i] - 1].x = xPos;
			boxCordinates[values[i] - 1].y = yPos;
		}
		else
			currentPosition = i;
	}

}

void drawAllTiles() {
	char str[20];
	for (int i = 0; i < 9; i++) {
		if (values[i] != 0) {
			int num = values[i];
			snprintf(str, 20, "%d", num);
			drawTile(str, boxCordinates[num - 1].x, boxCordinates[num- 1].y);
		}
	}
}

void switchPosition(int a, int b) {
	int temp = values[a];
	values[a] = values[b];
	values[b] = temp;
}