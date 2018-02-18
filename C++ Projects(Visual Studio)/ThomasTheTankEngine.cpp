// ********** Sent as a .txt due to being unable to find .css in Xcode files
#include <iostream>
#include <math.h>

#include <gl/glut.h>
using namespace std;

BOOLEAN leftFace = false;	
BOOLEAN rightFace = false;
BOOLEAN forwardFace = true;
BOOLEAN rearFace = false;

float lightPos[] = { 0.0, 3.0, 0.0, 1.0 }; // Spotlight position.
static float spotAngle = 10.0; // Spotlight cone half-angle.
float spotDirection[] = { 0.0, -1.0, 0.0 }; // Spotlight direction.
static float spotExponent = 20.0; // Spotlight exponent = attenuation factor.
static float xMove = -.5, zMove = 3.5; // Movement components.

class Point3 {
public:
	float x, y, z;
	void set(float dx, float dy, float dz) { x = dx; y = dy; z = dz; }
	void set(Point3& p) { x = p.x; y = p.y; z = p.z; }
	Point3(float xx, float yy, float zz) { x = xx; y = yy; z = zz; }
	Point3() { x = y = z = 0; }
	void build4tuple(float v[])
	{// load 4-tuple with this color: v[3] = 1 for homogeneous
		v[0] = x; v[1] = y; v[2] = z; v[3] = 1.0f;
	}
};
//@@@@@@@@@@@@@@@@@@ Vector3 class @@@@@@@@@@@@@@@@
class Vector3 {
public:
	float x, y, z;
	void set(float dx, float dy, float dz) { x = dx; y = dy; z = dz; }
	void set(Vector3& v) { x = v.x; y = v.y; z = v.z; }
	void flip() { x = -x; y = -y; z = -z; } // reverse this vector
	void setDiff(Point3& a, Point3& b)//set to difference a - b
	{
		x = a.x - b.x; y = a.y - b.y; z = a.z - b.z;
	}
	void normalize()//adjust this vector to unit length
	{
		double sizeSq = x * x + y * y + z * z;
		if (sizeSq < 0.0000001)
		{
			cerr << "\nnormalize() sees vector (0,0,0)!";
			return; // does nothing to zero vectors;
		}
		float scaleFactor = 1.0 / (float)sqrt(sizeSq);
		x *= scaleFactor; y *= scaleFactor; z *= scaleFactor;
	}
	Vector3(float xx, float yy, float zz) { x = xx; y = yy; z = zz; }
	Vector3(Vector3& v) { x = v.x; y = v.y; z = v.z; }
	Vector3() { x = y = z = 0; } //default constructor
	Vector3 cross(Vector3 b) //return this cross b
	{
		Vector3 c(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);
		return c;
	}
	float dot(Vector3 b) // return this dotted with b
	{
		return x * b.x + y * b.y + z * b.z;
	}
};

class Camera {
private:
	Point3 eye;
	Vector3 u, v, n;
	double viewAngle, aspect, nearDist, farDist; // view volume shape
	void setModelViewMatrix(); // tell OpenGL where the camera is

public:
	Camera(); // default constructor
	void set(Point3 eye, Point3 look, Vector3 up); // like gluLookAt()
	void roll(float angle); // roll it
	void pitch(float angle); // increase pitch
	void yaw(float angle); // yaw it
	void slide(float delU, float delV, float delN); // Z slide it
	void sideSlide(float delU, float delV, float delN); // X Slide
	void jump(float angle); // Jumps camera to an orientation
	void vertSlide(float delU, float delV, float delN); // Y Slide
	void setShape(float vAng, float asp, float nearD, float farD)

	{
		viewAngle = vAng;
		aspect = asp;
		nearDist = nearD;
		farDist = farD;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(viewAngle, aspect, nearDist, farDist);
		setModelViewMatrix();
	}
};

Camera::Camera()
{
}


void Camera::setModelViewMatrix(void)
{ // load modelview matrix with existing camera values
	float m[16];
	Vector3 eVec(eye.x, eye.y, eye.z); // a vector version of eye
	m[0] = u.x; m[4] = u.y; m[8] = u.z;  m[12] = -eVec.dot(u);
	m[1] = v.x; m[5] = v.y; m[9] = v.z;  m[13] = -eVec.dot(v);
	m[2] = n.x; m[6] = n.y; m[10] = n.z;  m[14] = -eVec.dot(n);
	m[3] = 0;   m[7] = 0;   m[11] = 0;    m[15] = 1.0;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(m); // load OpenGL�s modelview matrix
}
void Camera::set(Point3 Eye, Point3 look, Vector3 up)
{	// create a modelview matrix and send it to OpenGL
	eye.set(Eye); // store the given eye position
	n.set(eye.x - look.x, eye.y - look.y, eye.z - look.z);// make n
	u.set(up.cross(n).x, up.cross(n).y, up.cross(n).z); // make u = up X n
	n.normalize(); u.normalize(); // make them unit length
	v.set(n.cross(u).x, n.cross(u).y, n.cross(u).z);  // make v =  n X u  // make v =  n X u
	setModelViewMatrix(); // tell OpenGL
}

// Z axis Slide
void Camera::slide(float delU, float delV, float delN)
{
	//eye.x += delU * u.x + delV * v.x + delN * n.x;
	//eye.y += delU * u.y + delV * v.y + delN * n.y;
	eye.z += delU * u.z + delV * v.z + delN * n.z;
	setModelViewMatrix();
}

// X axis Slide
void Camera::sideSlide(float delU, float delV, float delN)
{
	eye.x += delU * u.x + delV * v.x + delN * n.x;
	//eye.y += delU * u.y + delV * v.y + delN * n.y;
	//eye.z += delU * u.z + delV * v.z + delN * n.z;
	setModelViewMatrix();
}

// Y axis Slide
void Camera::vertSlide(float delU, float delV, float delN)
{
	//eye.x += delU * u.x + delV * v.x + delN * n.x;
	eye.y += delU * u.y + delV * v.y + delN * n.y;
	//eye.z += delU * u.z + delV * v.z + delN * n.z;
	setModelViewMatrix();
}

// Z axis Rotation
void Camera::roll(float angle)
{ // roll the camera through angle degrees
	float cs = cos(3.14159265 / 180 * angle);
	float sn = sin(3.14159265 / 180 * angle);
	Vector3 t(u); // remember old u
	u.set(cs*t.x + sn*v.x, cs*t.y + sn*v.y, cs*t.z + sn*v.z);
	v.set(-sn*t.x + cs*v.x, -sn*t.y + cs*v.y, -sn*t.z + cs*v.z);
	setModelViewMatrix();
}

// X axis Rotation
void Camera::pitch(float angle)
{ // roll the camera through angle degrees
	float cs = cos(3.14159265 / 90 * angle);
	float sn = sin(3.14159265 / 90 * angle);
	Vector3 t(n); // remember old u
	n.set(cs*t.x + sn*v.x, cs*t.y + sn*v.y, cs*t.z + sn*v.z);
	v.set(-sn*t.x + cs*v.x, -sn*t.y + cs*v.y, -sn*t.z + cs*v.z);
	setModelViewMatrix();
}

// Y axis Rotation
void Camera::yaw(float angle)
{
	float cs = cos(3.14159265 / 180 * angle);
	float sn = sin(3.14159265 / 180 * angle);
	Vector3 t(u); // remember old u
	u.set(cs*t.x + sn*n.x, cs*t.y + sn*n.y, cs*t.z + sn*n.z);
	n.set(-sn*t.x + cs*n.x, -sn*t.y + cs*n.y, -sn*t.z + cs*n.z);
	setModelViewMatrix();
}

// Jump camera to orientation
void Camera::jump(float angle)
{
	float cs = cos(3.14159265 / 180 * angle);
	float sn = sin(3.14159265 / 180 * angle);
	Vector3 t(u); // remember old u
	u.set(cs*t.x + sn*v.x, cs*t.y + sn*v.y, cs*t.z + sn*v.z);
	v.set(-sn*t.x + cs*v.x, -sn*t.y + cs*v.y, -sn*t.z + cs*v.z);
	setModelViewMatrix();

	cs = cos(3.14159265 / 90 * angle);
	sn = sin(3.14159265 / 90 * angle);
	Vector3 b(n); // remember old u
	n.set(cs*b.x + sn*v.x, cs*b.y + sn*v.y, cs*b.z + sn*v.z);
	v.set(-sn*b.x + cs*v.x, -sn*b.y + cs*v.y, -sn*b.z + cs*v.z);
	setModelViewMatrix();
}


Camera cam; // global camera object

float changex = 12 - 23;
float changey = 0;
float changez = 12 - 67.5;

void setup() {
	//glClearColor(.53f, .80f, .94f, 1.0f);
	glClearColor(0.03, 0.19, 0.23, 1.0);
	glEnable(GL_DEPTH_TEST); // Enable depth testing.

							 // Turn on OpenGL lighting.
	glEnable(GL_LIGHTING);

	// Light property vectors.
	float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
	float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float globAmb[] = { 0.15, 0.15, 0.15, 1 };

	// Light properties.
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);

	glEnable(GL_LIGHT0); // Enable particular light source.
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.

														 // Material property vectors.
	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float matShine[] = { 50.0 };

	// Material properties shared by all the spheres.
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

	// Cull back faces.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Enable color material mode:
	// The ambient and diffuse color of the front faces will track the color set by glColor().
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void drawFloor()
{
	glColor3f(0, 1, 0);
	glBegin(GL_POLYGON);
	glVertex3f(-100, 0, -100);
	glVertex3f(-100, 0, 100);
	glVertex3f(100, 0, 100);
	glVertex3f(100, 0, -100);
	glEnd();

}


void rail()
{

	glColor3f(.47, .49, .47);
	glPushMatrix();
	glTranslated(0, 0, 0);
	glScaled(.15, .25, 4);
	glutSolidCube(1.0);
	glPopMatrix();
	glEnd();

	glPushMatrix();
	glTranslated(1, 0, 0);
	glScaled(.15, .25, 4);
	glutSolidCube(1.0);
	glPopMatrix();
	glEnd();

	for (int i = -2; i < 3; i++) {
		glPushMatrix();
		glTranslated(.5, 0, i);
		glRotated(90, 1, 90, 1);
		glScaled(.15, .25, 1);
		glutSolidCube(1.0);
		glPopMatrix();
		glEnd();
	}
	

}


//move
//rotate
//size

#define PI 3.1415927
void draw_cylinder(GLfloat radius, GLfloat height, GLfloat color, GLfloat color2, GLfloat color3) {
	GLfloat x = 0.0;
	GLfloat y = 0.0;
	GLfloat angle = 0.0;
	GLfloat angle_stepsize = 0.1;


	glColor3f(0, 0, 0);
	glBegin(GL_QUAD_STRIP);
	angle = 0.0;
	while (angle < 2 * PI) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		glVertex3f(x, y, height);
		glVertex3f(x, y, 0.0);
		angle = angle + angle_stepsize;
	}
	glVertex3f(radius, 0.0, height);
	glVertex3f(radius, 0.0, 0.0);
	glEnd();


	glColor3f(color, color2, color3);
	glBegin(GL_POLYGON);
	angle = 0.0;
	while (angle < 2 * PI) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		glVertex3f(x, y, height);
		angle = angle + angle_stepsize;
	}
	glVertex3f(radius, 0.0, height);
	glEnd();


}

void traindecal() {
	glColor3f(1, 1, 0);
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, 0);
	glVertex3f(0, .25, 0);
	glVertex3f(1, .25, 0);
	glVertex3f(1, 0, 0);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(.40, .25, 0);
	glVertex3f(.40, 1.5, 0);
	glVertex3f(.60, 1.5, 0);
	glVertex3f(.60, .25, 0);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(.40, 1.25, 0);
	glVertex3f(0, 1.25, 0);
	glVertex3f(0, 1.40, 0);
	glVertex3f(.4, 1.5, 0);
	glEnd();

}

void wheels() {
	glPushMatrix();
	glTranslated(0, 0, 0);
	glRotated(-90, 0, 1, 0);
	draw_cylinder(.15, .15, 0, 0,1);
	glPopMatrix();
	glEnd();

	glPushMatrix();
	glTranslated(0, 0, -.60);
	glRotated(-90, 0, 1, 0);
	draw_cylinder(.15, .15, 0, 0,1);
	glPopMatrix();
	glEnd();

	glPushMatrix();
	glTranslated(0, 0, -1.15);
	glRotated(-90, 0, 1, 0);
	draw_cylinder(.15, .15, 0, 0,1);
	glPopMatrix();
	glEnd();
}



void Thomas() {

	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslated(0, 0, -.65);
	glScaled(1.5, 1.5, 3.5);
	glutSolidCube(.5);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, .23);
	draw_cylinder(.3, .3,.75,.76,.75);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, .5, .4);
	glRotated(90, 50, 1, 1);
	draw_cylinder(.05, .3 , 1.0, 1.0, 1.0);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(0, -.4, -.65);
	glScaled(1.15, .1, 2);
	glutSolidCube(1.0);
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(-.39, 0, -.8);
	glScaled(.15, .15, .15);
	glRotated(270, 0, 1, 0);
	traindecal();
	glPopMatrix();

	glPushMatrix();
	glTranslated(.39, 0, -.8);
	glScaled(.15, .15, .15);
	glRotated(90, 0, 1, 0);
	traindecal();
	glPopMatrix();
	glPushMatrix();
	glTranslated(-.45, -.55, 0);
	wheels();
	glPopMatrix();

	glPushMatrix();
	glTranslated(.45, -.55, -1.15);
	glRotated(180, 0, 1, 0);
	wheels();
	glPopMatrix();
	glPushMatrix();

	glTranslatef(xMove, -.25, zMove); // Move the spotlight.
									 // Draw the spotlight cone in wireframe after disabling lighting
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glRotatef(180.0, 0.0, 1.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidCone(3.0 * tan(spotAngle / 180.0 * PI), 3.0, 20, 20);
	glEnable(GL_LIGHTING);
	glPopMatrix();

	// Spotlight properties including position.
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spotAngle);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirection);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spotExponent);

	glPopMatrix();

	glEnd();
}

GLfloat chocho = .5;
GLfloat chochoX = .5;
GLint rotation = 0;
//<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>>>>>>>>>>
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0, 0, 0);
	drawFloor();
	glPushMatrix();
	glTranslated(.5, -2, 11);
	glScaled(.5, .5, .5);
	glPopMatrix();
	
	for (int i = -50; i < 50; i += 4) {
		glPushMatrix();
		glTranslated(-50, 0, i);
		rail();
		glPopMatrix();

		glPushMatrix();
		glTranslated(46, 0, i);
		rail();
		glPopMatrix();

		glPushMatrix();
		glTranslated(0, 0, i);
		rail();
		glPopMatrix();

		glPushMatrix();
		glTranslated(i, 0, -50);
		glRotated(90, 0, 1, 0);
		rail();
		glPopMatrix();

		glPushMatrix();
		glTranslated(i, 0, 50);
		glRotated(90, 0, 1, 0);
		rail();
		glPopMatrix();

		glPushMatrix();
		glTranslated(i, 0, 10);
		glRotated(90, 0, 1, 0);
		rail();
		glPopMatrix();

	}
	glPushMatrix();
	glTranslated(chochoX, .85, chocho);
	glRotated(rotation, 0, 1, 0);
	Thomas();
	glPopMatrix();
	//cout << chochoX << "\n"<< chocho << "\n";
	glFlush();
	glutSwapBuffers();
}


//<<<<<<<<<<<<<<<<<<<<<<<< myKeyboard >>>>>>>>>>>>>>>>>>>>>>
void myKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		
	case 's':

		if (chocho < -50 || forwardFace == true)
			break;
		else if(chochoX==46.5 || chochoX == -49.5 || chochoX ==.5){
			rotation = 180;
			chocho -= .5;
			cam.slide(0, 0, 0.5);
			rightFace = false;
			leftFace = false;
			rearFace = true;
		}
		break;
	
	case 'a':
		
		if (chochoX > 46 || rightFace ==true)
			break;
		else if (chocho == 49.5 || chocho == -50.5 || chocho == 9.5){
			chochoX += .5;
			rotation = 90;
			cam.sideSlide(-0.5, 0, 0);
			rearFace = false;
			forwardFace = false;
			leftFace = true;
		}
		break;
		
	case 'd':
		if (chochoX < -49 || leftFace==true)
			break;
		else if (chocho==49.5 || chocho ==-50.5 || chocho == 9.5){
			chochoX -= .5;
			rotation = 270;
			cam.sideSlide(0.5, 0, 0);
			rightFace = true;
			forwardFace = false;
			rearFace = false;
		}
		break;

	case 'w':
		if (chocho > 49 || rearFace==true)
			break;
		else if(chochoX==46.5 || chochoX==-49.5 || chochoX==.5){
			rotation = 0;
			chocho += .5;
			cam.slide(0, 0, -.5);
			forwardFace = true;
			leftFace = false;
			rightFace = false;
		}
		break;

	case 'R':      cam.roll(-1.0); break;
	case 'r': cam.roll(1.0); break;

		// Pitch
	case 'P':       cam.pitch(-0.5); break;
	case 'p':    cam.pitch(0.5); break;

		// Yaw
	case 'Y':       cam.yaw(-1.0); break;
	case 'y':    cam.yaw(1.0); break;

		// Jump
	case 'J':       cam.jump(-10.0); break;
	case 'j':    cam.jump(10.0); break;
	case 'm':	cam.sideSlide(.5, 0, 0); break;
	case 'M': cam.slide(0, 0, -.5); break;

	default:
		break;

	}
	glutPostRedisplay();
}


//<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

void myMotion(int x, int y)
{
	static bool firstpt = true;
	static int lastx;
	static int lasty;
	float slidex, slidey;
	if (firstpt)
	{
		lastx = x;
		lasty = 480 - y;
		firstpt = false;
	}
	else
	{
		y = 480 - y;
		slidex = x - lastx;
		slidey = y - lasty;
		cam.slide(slidex / 480.0, slidey / 480, 0);
		lastx = x;
		lasty = y;
		glutPostRedisplay();
	}
}

int main(int argc, char **argv)
{
	Point3 e;
	Point3 l;
	Vector3 u;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // double buffering
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Project");
	glutKeyboardFunc(myKeyboard);
	glutDisplayFunc(myDisplay);
	glutMotionFunc(myMotion);
	glClearColor(.53f, .80f, .94f, 1.0f);  
	glColor3f(0.0f, 0.0f, 0.0f);
	glViewport(0, 0, 640, 480);
	e.set(1, 2, -15);
	l.set(1, 2, 10);
	u.set(0, 1, 0);
	cam.set(e, l, u); // make the initial camera
	cam.setShape(30.0f, 64.0f / 48.0f, 0.5f, 500.0f);
	//glEnable(GL_DEPTH_TEST);
	setup();
	glutMainLoop();

	return 0;
}
