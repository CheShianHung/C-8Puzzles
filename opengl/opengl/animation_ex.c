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

struct node;
typedef struct node {
	int childrenSize;
	int values[9];
	int g;
	int h;
	int zeroPos;
	int lastAction; //1: up, 2: down, 3: left, 4: right
	struct node *parent;
	struct node *children;
} Node;

struct list;
typedef struct list {
	Node *n;
	struct list *next;
} List;

struct solutionList;
typedef struct solutionList {
	int action;
	struct solutionList *next;
} SolutionList;

//@@***********************************************************************************@@
// Global Variables
MyPoint bottomLeftPt;
MyPoint boxCordinates[8];
SolutionList *h1Solution = NULL;
SolutionList *h2Solution = NULL;

int initValues[9];
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
int h1Mode = 0; // -1: off, 0: before run, 1: running, 2: ready, 3: runningAnimation
int h2Mode = 0; // -1: off, 0: before run, 1: running, 2: ready, 3: runningAnimation
int hMethod = 0;


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
void switchPosition(int *ary, int a, int b);
int heuristic(int *ary);
void aStar();
void destroyTree(Node *n);
void expendTree(Node *n);
void displayTree(Node *n);
List* addToList(List *h, Node *n);
List* removeFront(List *h);
void displayList(List *h);
int isGoal(int *ary);
SolutionList* addToSolutionList(SolutionList * h, int action);
void displaySolutionList(SolutionList *h);
int solvable(int *ary);


//int moveCount(int *ary);

//@@***********************************************************************************@@
int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	init_setup(WINDOW_XS, WINDOW_YS, WINDOW_NAME);
	setupPuzzle();
	
	//printf("current position: %d\n\n", currentPosition);
	//printf("h1: %d\nh2: %d\n", heuristic1(values), heuristic2(values));

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
	glColor3f(0.2, 0.2, 0.2);				// setting pen color 
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
	
	//h1Mode 
	// -1: off, 0: before run, 1: running, 2: ready, 3: runningAnimation
	//h2Mode
	// -1: off, 0: before run, 1: running, 2: ready, 3: runningAnimation

	//Manual mode
	if (h1Mode == -1 && h2Mode == -1) {
		output(10, 346, 2, "Manual mode");
		//output(10, 325, 2, "k: h2");
		//output(10, 308, 2, "s: manual");
	}
	//Finding solution
	else if (h1Mode == 1 || h2Mode == 1) {
		printf("finding solution...\n");
		output(10, 320, 2, "Finding solution...");
	}
	//Running animation
	else if (h1Mode == 3 || h2Mode == 3) {
		printf("running solution...\n");
		output(10, 320, 2, "Running solution...");
	}
	else {
		if (h1Mode == 0) {
			output(10, 346, 2, "j: h1");
		}
		else if (h1Mode == 2) {
			output(10, 346, 2, "n: run h1");
		}
		if (h2Mode == 0) {
			output(10, 325, 2, "k: h2");
		}
		else if (h2Mode == 2) {
			output(10, 325, 2, "m: run h2");
		}
		output(10, 308, 2, "s: manual");
	}

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
		// -1: off, 0: before run, 1: running, 2: ready, 3: runningAnimation
	case 'j':
		if (h1Mode == 0 && h2Mode != 1 && h2Mode != 3) {
			hMethod = 1;
			h1Mode = 1;
			glutPostRedisplay();
			aStar();
		}
		break;
	case 'k':
		if (h2Mode == 0 && h1Mode != 1 && h1Mode != 3) {
			hMethod = 2;
			h2Mode = 1;
			glutPostRedisplay();
			aStar();
		}
		break;
	case 'Q':
	case 'q':
		//Destroy the list
		while (h1Solution) {
			List *temp = h1Solution->next;
			h1Solution = h1Solution->next;
			temp = NULL;
			free(temp);
		}
		while (h2Solution) {
			List *temp = h2Solution->next;
			h2Solution = h2Solution->next;
			temp = NULL;
			free(temp);
		}

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
			//printf("direction: %d\n", d);
			//printf("values: %d\n", selectedValue);
			doAnimation = d;
			switch (d) {
			case 1: 
				switchPosition(values, currentPosition, currentPosition + 3);
				currentPosition += 3;
				break;
			case 2:
				switchPosition(values, currentPosition, currentPosition - 3);
				currentPosition -= 3;
				break;
			case 3:
				switchPosition(values, currentPosition, currentPosition + 1);
				currentPosition++;
				break;
			case 4:
				switchPosition(values, currentPosition, currentPosition - 1);
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

	int seed = (int)time(NULL);
	int usedValues[9];

	srand(seed);
	do {
		for (int i = 0; i < 9; i++) {
			usedValues[i] = i;
		}
		for (int i = 0; i < 9; i++) {
			int r = rand() % (9 - i);
			values[i] = usedValues[r];
			usedValues[r] = usedValues[8 - i];
			usedValues[8 - i] = values[i];

			//Testing
			/*
			if (i == 0) values[i] = 3;
			else if (i == 1) values[i] = 2;
			else if (i == 2) values[i] = 5;
			else if (i == 3) values[i] = 6;
			else if (i == 4) values[i] = 4;
			else if (i == 5) values[i] = 8;
			else if (i == 6) values[i] = 0;
			else if (i == 7) values[i] = 1;
			else if (i == 8) values[i] = 7;
			else values[i] = i;
			*/
		}
		//printf("problem check: %d\n",solvable(values));
	} while (solvable(values) == 0);

	for(int i = 0; i < 9; i++){
		initValues[i] = values[i];
		if (values[i] != 0) {
			GLfloat xPos, yPos;
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

void switchPosition(int *ary, int a, int b) {
	int temp = ary[a];
	ary[a] = ary[b];
	ary[b] = temp;
}

int heuristic(int *ary) {
	int count = 0;
	if (hMethod == 1) {
		for (int i = 0; i < 9; i++) {
			if (ary[i] != i) {
				count++;
			}
		}
	}
	else if (hMethod == 2) {
		for (int i = 0; i < 9; i++) {
			if (ary[i] != i) {
				count += abs(ary[i] / 3 - i / 3) + abs(ary[i] % 3 - i % 3);
				//printf("%d: %d\n", ary[i], abs(ary[i] / 3 - i / 3) + abs(ary[i] % 3 - i % 3));
			}
		}
	}
	
	/*
	if (count > 50) {
		printf("heuristic\n");
		for (int i = 0; i < 9; i++) {
			printf("%d  ", ary[i]);
		}
		printf("\ncount: %d\n", count);
	}
	*/

	return count;
}

void aStar() {
	//Create root
	Node *r = (Node*)malloc(sizeof(Node));
	for (int i = 0; i < 9; i++) {
		r->values[i] = initValues[i];
	}
	r->childrenSize = 0;
	r->g = 0;
	r->h = heuristic(values);
	r->zeroPos = currentPosition;
	r->parent = NULL;
	Node *root = r;

	//Create exploring list
	List *hList = NULL;
	hList = addToList(hList, root);
	displayList(hList);
	
	int testCount = 0;
	while (hList->n->h != 0) {
	//while(testCount < 5){
		Node *fn = hList->n;
		hList = removeFront(hList);
		expendTree(fn);
		for (int i = 0; i < fn->childrenSize; i++) {
			hList = addToList(hList, &fn->children[i]);
			//displayList(hList);
		}
		printf("[%d, %d]\n", hList->n->g, hList->n->h);
		//displayList(hList);
		//testCount++;
	}
	
	//Input solution
	Node *t = hList->n;
	while (t->parent) {
		if (hMethod == 1) {
			h1Solution = addToSolutionList(h1Solution, t->lastAction);
		}
		else if (hMethod == 2) {
			h2Solution = addToSolutionList(h2Solution, t->lastAction);
		}
		t = t->parent;
	}
	

	//Display solutions
	if (hMethod == 1) {
		displaySolutionList(h1Solution);
	}
	else if (hMethod == 2) {
		displaySolutionList(h2Solution);
	}

	//Destroy the tree
	destroyTree(root);

	//Destroy the list
	while (hList) {
		List *temp = hList->next;
		hList = hList->next;
		temp = NULL;
		free(temp);
	}

	if (hMethod == 1) h1Mode = 2;
	else h2Mode = 2;

	glutPostRedisplay();
}

void destroyTree(Node *n) {
	if (n != NULL) {
		//if (n->parent) {
			//printf("parent[%d, %d]\n", n->parent->g, n->parent->h);
		//}
		n->parent = NULL;
		//free(n->parent);
		//printf("current at [%d, %d, %d]\n", n->g, n->h, n->childrenSize);
		for (int i = 0; i < n->childrenSize; i++) {
			//printf("destroy children from [%d, %d]\n", n->g, n->h); 
			destroyTree(&n->children[i]);
		}
		n = NULL;
		free(n);
	}
}

void expendTree(Node *n) {
	int zeroPos = n->zeroPos;
	int actions[4];
	for (int i = 0; i < 4; i++) {
		actions[i] = 0;
	}

	if (zeroPos + 3 <= 8 && n->lastAction != 2) {
		n->childrenSize++;
		actions[0] = 1;
	}
	if (zeroPos - 3 >= 0 && n->lastAction != 1) {
		n->childrenSize++;
		actions[1] = 1;
	}
	if ((zeroPos + 1) % 3 != 0 && n->lastAction != 4) {
		n->childrenSize++;
		actions[2] = 1;
	}
	if ((zeroPos - 1) % 3 != 2 && zeroPos != 0 && n-> lastAction != 3) {
		n->childrenSize++;
		actions[3] = 1;
	}

	/*
	printf("expend from [g, h, childrenSize, lastAction, zeroPos] => [%d, %d, %d, %d, %d]\n", n->g, n->h, n->childrenSize, n->lastAction, n->zeroPos);
	for (int i = 0; i < 9; i++) {
		printf("%d  ", n->values[i]);
	}
	printf("\n");
	*/

	n->children = (Node*)calloc(n->childrenSize, sizeof(Node));
	
	int counter = 0;
	for (int i = 0; i < 4; i++) {
		if (actions[i] == 1) {		
			for (int i = 0; i < 9; i++) {
				n->children[counter].values[i] = n->values[i];
			}
			switch (i) {
			case 0:
				n->children[counter].zeroPos = zeroPos + 3;
				switchPosition(n->children[counter].values, zeroPos, zeroPos + 3);
				break;
			case 1:
				n->children[counter].zeroPos = zeroPos - 3;
				switchPosition(n->children[counter].values, zeroPos, zeroPos - 3);
				break;
			case 2:
				n->children[counter].zeroPos = zeroPos + 1;
				switchPosition(n->children[counter].values, zeroPos, zeroPos + 1);
				break;
			case 3:
				n->children[counter].zeroPos = zeroPos - 1;
				switchPosition(n->children[counter].values, zeroPos, zeroPos - 1);
				break;
			}	
			n->children[counter].lastAction = i + 1;
			n->children[counter].h = heuristic(n->children[counter].values);
			n->children[counter].childrenSize = 0;
			n->children[counter].g = n->g + 1;
			n->children[counter].parent = n;
			n->children[counter].children = NULL;

			counter++;
			//printf("expend children [%d, %d, %d]\n", n->children[counter - 1].g, n->children[counter - 1].h, n->children[counter - 1].childrenSize);
		}
	}
}

void displayTree(Node* h) {
	
}

List* addToList(List *h, Node *n) {
	List *newNode = (List*)malloc(sizeof(List));
	newNode->n = n;
	newNode->next = NULL;
	if (!h) {
		h = newNode;
	}
	else {
		List *l = h;
		List *pre = NULL;
		while (l && n->g + n->h > l->n->g + l->n->h) {
			pre = l;
			l = l->next;
		}
		if (!pre) {
			newNode->next = h;
			h = newNode;
		}
		else if (!l) {
			pre->next = newNode;
		}
		else {
			pre->next = newNode;
			newNode->next = l;
		}
	}

	return h;
}

List* removeFront(List *h) {
	List* temp = h;
	h = h->next;
	free(temp);
	return h;
}

void displayList(List* h) {
	if (h) {
		printf("[g, h]: ");
		List *l = h;
		while (l) {
			printf("[%d, %d]  ", l->n->g, l->n->h);
			l = l->next;
		}
		printf("\n");
	}
	else {
		printf("empty list\n");		
	}
}

int isGoal(int *ary) {
	int same = 1;
	for (int i = 0; i < 9; i++) {
		if (ary[i] != i) same = 0;
	}
	return same;
}

SolutionList* addToSolutionList(SolutionList * h, int action) {
	SolutionList *newSolution = (SolutionList*)malloc(sizeof(SolutionList));
	newSolution->action = action;
	newSolution->next = h;
	h = newSolution;

	return h;
}

void displaySolutionList(SolutionList *h) {
	if (h) {
		printf("actions: ");
		SolutionList *sl = h;
		while (sl) {
			switch (sl->action) {
			case 1:
				printf("up  ");
				break;
			case 2:
				printf("down  ");
				break;
			case 3:
				printf("left  ");
				break;
			case 4:
				printf("right  ");
				break;
			}
			sl = sl->next;
		}
		printf("\n");
	}
	else {
		printf("empty list\n");
	}
}

int solvable(int *ary) {
	/*
	int counter = 0;
	for (int i = 0; i < 9; i++) {
		if (ary[i] != 0) {
			int count = ary[i] - 1;
			for (int j = 0; j < i; j++) {
				if (ary[j] <= ary[i] && count > 0) count--;
			}
			counter += count;
			//printf("%d: %d   ", ary[i], count);
		}
	}
	//printf("\nsolvable count: %d\n", counter);
	if (counter % 2 == 0) return 1;
	else return 0;
	*/
	int counter = 0;
	for (int i = 0; i < 9 - 1; i++) {
		for (int j = i + 1; j < 9; j++) {
			if (ary[i] != 0 && ary[j] != 0 && ary[i] > ary[j])
				counter++;
		}
	}
	if (counter % 2 == 0) return 1;
	else return 0;
}

/*
struct node;
typedef struct node {
	int childrenSize;
	int values[9];
	int g;
	int h;
	int zeroPos;
	int lastAction; //1: up, 2: down, 3: left, 4: right
	struct node *parent;
	struct node *children;
} Node;
*/