#include <Windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>
#include <list>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#define Reset 0
#define Solution 1
#include <thread>
#include <dos.h >

using namespace std;

list<int> Peg1;
list<int> Peg2;
list<int> Peg3;
int loop;


void myInit(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}

void ring(void) {			// The bottom ring (the largest)
	glBegin(GL_LINE_STRIP);
	glVertex2i(36, 0);
	glVertex2i(36, 10);
	glVertex2i(174, 10);
	glVertex2i(174, 0);
	glEnd();
	glFlush();
}
int RINGS;
void user() {
	cout << "How many rings do you want?: ";
	cin >> RINGS;
	int x = 0;
	for (int i = 0; i < RINGS; i++) {
		Peg1.push_back(x);	//creates a list of values used to create a ring
		x += 2;		//each iteration our value increases by two
	}
}

void post(void);
void RESET(void) {
	Peg1.clear();
	Peg2.clear();
	Peg3.clear();
	int x = 0;
	for (int i = 0; i < RINGS; i++) {
		Peg1.push_back(x);
		x += 2;
	}
	post();
}

void DrawPeg1() {		//Draw multiple rings with viewports on the first peg
	int y = 0;
	for (int n : Peg1) {
		glViewport(n, y, 640 - (12 * n / 2), 480);	//using the values that are in the list we can create a ring based off that value. It starts at the bottom and works its way up
		ring();
		y += 10;
	}
}

void DrawPeg2() {
	int y = 0;
	for (int n : Peg2) {
		glViewport(n+200, y, 640 - (12 * n / 2), 480);
		ring();
		y += 10;
	}
}

void DrawPeg3() {
	int y = 0;
	for (int n : Peg3) {
		glViewport(n + 400, y, 640 - (12 * n / 2), 480);
		ring();
		y += 10;
	}

}

void post(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0, 0, 0);
	DrawPeg1();
	DrawPeg2();
	DrawPeg3();

	glViewport(0, 0, 640, 480);
	glBegin(GL_LINE_STRIP);		//first peg
	glVertex2i(100, 0);
	glVertex2i(100, 350);
	glVertex2i(110, 350);
	glVertex2i(110, 0);
	glEnd();

	glBegin(GL_LINE_STRIP);		//second peg
	glVertex2i(300, 0);
	glVertex2i(300, 350);
	glVertex2i(310, 350);
	glVertex2i(310, 0);
	glEnd();

	glBegin(GL_LINE_STRIP);		//third peg
	glVertex2i(500, 0);
	glVertex2i(500, 350);
	glVertex2i(510, 350);
	glVertex2i(510, 0);

	glEnd();
	glFlush();
}

void SOLUTION(list<int>& first, list<int>& third, list<int>& second, int rings) { //Recursion that solves the problem automatically for the user.
	if (rings == 1) {
		if (first.empty() == false) {
			third.push_back(first.back());
			first.pop_back();
			post();
			std::this_thread::sleep_for(.5s); //Slows down the function so the user can see it in action.
		}
	}
	else {
		SOLUTION(first, second, third, rings - 1);
		SOLUTION(first, third, second, 1);
		SOLUTION(second, third, first, rings - 1);
	}
}

int getter;
bool placement = false;	//This keeps track on whether or not we are getting a ring or placing a ring
void myMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		y = 480 - y;

		if (x >= 100 && x <= 110) {	
			if (placement == false) {			//gets the ring on top
				getter = Peg1.back();	//top ring is at the back of the list and put it in a variable
				Peg1.pop_back();	//deletes that ring since we are moving it
				placement=true;		//increments placement, next time a peg is clicked, it will set getter to the back of that pegs list
			}

			else if(placement){
				if (Peg1.empty() == true) {		//need an "if" statement to check if the peg is empty, without this the program crashes
					Peg1.push_back(getter);	
					placement=false;
				}
				else if (getter < Peg1.back()) {	//checks if the ring is bigger than the bottom ring
					cout << "You can't do that\n";
				}
				else if (getter > Peg1.back()) {	//places ring on that peg if its smaller than the one below it.
					Peg1.push_back(getter);
					placement=false;
				}


			}

		}

		if (x >= 300 && x <= 310) {		//same process as before, except its dont with the second peg
			if (placement == false && !Peg2.empty()) {
				getter = Peg2.back();
				Peg2.pop_back();
				placement=true;
			}
			else if(placement) {
				if (Peg2.empty() == true) {
					Peg2.push_back(getter);
					placement=false;
				}
				else if (getter < Peg2.back()) {
					cout << "You can't do that\n";
				}
				else if (getter > Peg2.back()) {
					Peg2.push_back(getter);
					placement=false;
				}


			}
		}

		if (x >= 500 && x <= 510) {	//Third peg
			if (placement == false && !Peg3.empty()) {
				getter = Peg3.back();
				Peg3.pop_back();
				placement=true;
			}
			else if(placement){
				if (Peg3.empty() == true) {
					Peg3.push_back(getter);
					placement=false;
				}

				else if (getter < Peg3.back()) {
					cout << "You can't do that\n";
				}

				else if (getter > Peg3.back()) {
					Peg3.push_back(getter);
					placement=false;
				}


			}

		}
	}
}
void processMenuEvents(int option)
{
	switch (option)
	{
	case Reset:
		RESET();
		break;
	case Solution:
		RESET();
		SOLUTION(Peg1, Peg3, Peg2, RINGS);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Program");
	user();
	myInit();
	glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Reset", Reset);
	glutAddMenuEntry("Solution", Solution);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutDisplayFunc(post);
	glutMouseFunc(myMouse);
	glutMainLoop();
}