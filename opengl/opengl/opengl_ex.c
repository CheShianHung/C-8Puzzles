/*
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <GL/glut.h>


//Constants
#define WINDOW_XS 300
#define WINDOW_YS 400
#define WINDOW_NAME "OpenGL Example"

//Structures
typedef struct pt {
	GLfloat x, y;
}MyPoint;

//Global Variables
MyPoint bottomLeftPt;
MyPoint bottomLeftCo[9];
MyPoint boxOffset;

int recLength = 75;
int recHeight = 75;
int currentPosition;

//Function prototypes
void reshape_handler(int width, int height);
void init_setup(int width, int height, char *windowName);
void display_func(void);
void keyboard_func(unsigned char c, int x, int y);
void special_keyboard_func(int c, int x, int y);
void mouse_func(int botton, int state, int x, int y);


int main(int argc, char **argv) {
	glutInit(&argc, argv);

	init_setup(WINDOW_XS, WINDOW_YS, WINDOW_NAME);

	//inital position of bottom left corner of rectangle
	bottomLeftPt.x = 50;
	bottomLeftPt.y = 100;

	//initial offset
	boxOffset.x = 12;
	boxOffset.y = 12;

	//initial coordinates
	int tempIndex = 0;
	for(int i=0; i < 3; i++){
		for (int j = 0; j < 3; j++) {
			bottomLeftCo[tempIndex].x = j * WINDOW_XS / 3;
			bottomLeftCo[tempIndex].y = (2 - i) * WINDOW_XS / 3;
			tempIndex++;
		}
	}

	currentPosition = 0;

	glutDisplayFunc(display_func);
	glutKeyboardFunc(keyboard_func);
	glutSpecialFunc(special_keyboard_func);
	glutMouseFunc(mouse_func);
	glutMainLoop();

	return 1;
}

void reshape_handler(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)width, 0.0, (GLdouble)height);
}

void init_setup(int width, int height, char *windowName) {
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(5, 5);
	glutCreateWindow(windowName);
	glutReshapeFunc(reshape_handler);
}

void display_func(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	//draw lines
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	for (int i = 0; i < 3; i++) {
		glVertex2i(WINDOW_XS / 3 * i, 0);
		glVertex2i(WINDOW_XS / 3 * i, 300);

		glVertex2i(0, WINDOW_XS / 3 * i);
		glVertex2i(300, WINDOW_XS / 3 * i);
	}

	glVertex2i(0, 300);
	glVertex2i(300, 300);
	
	glEnd();

	//draw a rectagle
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2i(bottomLeftCo[currentPosition].x + boxOffset.x, bottomLeftCo[currentPosition].y + boxOffset.y);
	glVertex2i(bottomLeftCo[currentPosition].x + recLength + boxOffset.x, bottomLeftCo[currentPosition].y + boxOffset.y);
	glVertex2i(bottomLeftCo[currentPosition].x + recLength + boxOffset.x, bottomLeftCo[currentPosition].y + recHeight + boxOffset.y);
	glVertex2i(bottomLeftCo[currentPosition].x + boxOffset.x, bottomLeftCo[currentPosition].y + recHeight + boxOffset.y);
	glEnd();

	//draw the outlone of rectangle
	/*
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2i(bottomLeftPt.x, bottomLeftPt.y);
	glVertex2i(bottomLeftPt.x+recLength, bottomLeftPt.y);

	glVertex2i(bottomLeftPt.x+recLength, bottomLeftPt.y);
	glVertex2i(bottomLeftPt.x+recLength, bottomLeftPt.y+recHeight);

	glVertex2i(bottomLeftPt.x+recLength, bottomLeftPt.y+recHeight);
	glVertex2i(bottomLeftPt.x, bottomLeftPt.y+recHeight);

	glVertex2i(bottomLeftPt.x, bottomLeftPt.y+recHeight);
	glVertex2i(bottomLeftPt.x, bottomLeftPt.y);
	glEnd();
	*/
/*
	glFlush();
}

void keyboard_func(unsigned char c, int x, int y) {
	//cout << c;
	switch (c) {
	case 'w':
		if (currentPosition > 2)
			currentPosition -= 3;
		glutPostRedisplay();
		break;
	case 's':
		if (currentPosition < 6)
			currentPosition += 3;
		glutPostRedisplay();
		break;
	case 'a':
		if (currentPosition % 3 != 0)
			currentPosition--;
		glutPostRedisplay();
		break;
	case 'd':
		if (currentPosition % 3 != 2)
			currentPosition++;
		glutPostRedisplay();
		break;
	case 'Q':
	case 'q':
		printf("Good Bye !\n");
		exit(0);
	}
}

void special_keyboard_func(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		if (currentPosition > 2)
			currentPosition -= 3;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		if (currentPosition < 6)
			currentPosition += 3;
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		if (currentPosition % 3 != 0)
			currentPosition--;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		if (currentPosition % 3 != 2)
			currentPosition++;
		glutPostRedisplay();
		break;
	}
}

void mouse_func(int button, int state, int x, int y) {
	/*
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		recHeight += 10;

		if ((bottomLeftPt.y + recHeight) >= WINDOW_YS)
			recHeight -= 10;

		glutPostRedisplay();
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		recHeight -= 10;

		if (recHeight < 30)
			recHeight = 30;

		glutPostRedisplay();
	}
	*/
/*
}
*/