/*****
CTIS164 - Lab03
Interactivity: Animating Shapes
*****
ver06: complete
additions:
- all shapes animates
- stop / start animation
- enlarge / shrink
*****/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 400

#define TIMER_PERIOD 16   //period for the timer
#define TIMER_ON      1   //0:disable timer, 1:enable timer

#define D2R 0.0174532

//shapes
#define RECTANGLE 0
#define TRIANGLE  1
#define BALLOON   2

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; //current Window width and height

                          // global variables
bool activeTimer = true;

int mode = BALLOON; // initial shape is BALLOON
int r = 180, g = 172, b = 50; // initial color

char shapes[3][20] = { "RECTANGLE", "TRIANGLE", "BALLOON" };

//RECTANGLE
int xR = 0, yR = 130; //rectangle's initial position
int widthR = 200, heightR = 100; //rectangle's initial dimensions

//TRIANGLE
int xT = 0, yT = 130; //triangle's initial position
int heightT = 100; //triangle's initial height

//BALLOON
int xB = 0, yB = -55; //BALLOON's initial position
int radiusB = 75; //BALLOON's initial radius

                  /*****
                  to draw circle, center at (x,y) radius r
                  DAIRE
                  *****/
void circle(int x, int y, int r) {
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++) {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

/*****
CEMBER
*****/
void circle_wire(int x, int y, int r) {
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++) {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font) {
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++) {
        glutBitmapCharacter(font, string[i]);
    }
}


/*****
to display text with variables
vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
*****/
void vprint(int x, int y, void* font, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++) {
        glutBitmapCharacter(font, str[i]);
    }
}

void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

// FUNCTIONS TO DISPLAY SHAPES

void drawRectangle() {
    glRectf(xR - widthR / 2, yR - heightR / 2, xR + widthR / 2, yR + heightR / 2);
}

void drawTriangle() {
    glBegin(GL_TRIANGLES);
    glVertex2f(xT - heightT, yT - heightT / 2);
    glVertex2f(xT + heightT, yT - heightT / 2);
    glVertex2f(xT, yT + heightT / 2);
    glEnd();
}

void drawBalloon() {
    // Part 1 : string/rope 
    glLineWidth(3);
    glColor3f(1, 1, 0); // yellow
    glBegin(GL_LINES);
    glVertex2f(xB, yB - radiusB);
    glVertex2f(xB, yB - radiusB - radiusB * 0.8);
    glEnd();
    glLineWidth(1);

    // Part 2 : Body
    glColor3ub(r, g, b);
    circle(xB, yB, radiusB);

    // Part 3 : Tip of the body
    glBegin(GL_TRIANGLES);
    glVertex2f(xB, yB - (radiusB * 0.66));
    glVertex2f(xB - (radiusB / 5), yB - (radiusB * 1.2));
    glVertex2f(xB + (radiusB / 5), yB - (radiusB * 1.2));
    glEnd();

    // Part 4 : Light Reflection
    glColor3f(0.9, 0.9, 0.9);
    circle(xB + (radiusB / 3), yB + (radiusB / 3), (radiusB / 3));
}


/*****
to display onto window using OpenGL commands
*****/
void display() {
    //clear window to black
    glClearColor(0.25, 0.25, 0.25, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3ub(r, g, b);

    switch (mode) {
    case RECTANGLE:
        drawRectangle();
        break;

    case TRIANGLE:
        drawTriangle();
        break;

    case BALLOON:
        drawBalloon();
        break;
    }

    glColor3f(1, 1, 0); // white
    vprint(-(winWidth / 2) + 20, -(winHeight / 2) + 40, GLUT_BITMAP_8_BY_13, "R=%d G=%d B=%d", r, g, b);

    glColor3f(1, 1, 1); // yellow
    vprint(-(winWidth / 2) + 20, -(winHeight / 2) + 20, GLUT_BITMAP_8_BY_13, "Shape = %s", shapes[mode]);

    glColor3f(0, 1, 1); // blue
    vprint((winWidth / 2) - 290, -(winHeight / 2) + 80, GLUT_BITMAP_8_BY_13, "<-, ->     : WOOOO");
    vprint((winWidth / 2) - 290, -(winHeight / 2) + 65, GLUT_BITMAP_8_BY_13, "left click : change color");
    vprint((winWidth / 2) - 290, -(winHeight / 2) + 50, GLUT_BITMAP_8_BY_13, "right click: change location");
    vprint((winWidth / 2) - 290, -(winHeight / 2) + 35, GLUT_BITMAP_8_BY_13, "e,E / s,S  : enlarge / shrink");
    vprint((winWidth / 2) - 290, -(winHeight / 2) + 20, GLUT_BITMAP_8_BY_13, "space bar  : stop / start animation");

    glutSwapBuffers();
}

/*****
key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
*****/
void onKeyDown(unsigned char key, int x, int y) {
    //exit when ESC is pressed.
    if (key == 27)
        exit(0);

    if (key == 'e' || key == 'E') {
        switch (mode) {
        case RECTANGLE:
            if (widthR < winWidth / 2) {
                widthR += 10;
                heightR += 5;
            }
            break;

        case TRIANGLE:
            if (heightT < winHeight / 2) {
                heightT += 10;
            }
            break;

        case BALLOON:
            if (radiusB < winHeight / 3) {
                radiusB += 10;
            }
            break;
        }
    }

    if (key == 's' || key == 'S') {
        switch (mode) {
        case RECTANGLE:
            if (widthR > winWidth / 10) {
                widthR -= 10;
                heightR -= 5;
            }
            break;

        case TRIANGLE:
            if (heightT > winHeight / 10) {
                heightT -= 10;
            }
            break;

        case BALLOON:
            if (radiusB > winHeight / 10) {
                radiusB -= 10;
            }
            break;
        }
    }

    if (key == ' ')
        activeTimer = !activeTimer;

    //to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y) {
    //exit when ESC is pressed.
    if (key == 27)
        exit(0);

    //to refresh the window it calls display() function
    glutPostRedisplay();
}

/*****
special Key like GLUT_KEY_F1, F2, F3,...
arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
*****/
void onSpecialKeyDown(int key, int x, int y) {
    //write your codes here.

    if (key == GLUT_KEY_LEFT) {
        if (mode == RECTANGLE)
            mode = BALLOON;
        else
            mode--;
    }

    if (key == GLUT_KEY_RIGHT) {
        if (mode == BALLOON)
            mode = RECTANGLE;
        else
            mode++;
    }

    //to refresh the window it calls display() function
    glutPostRedisplay();
}


/*****
special Key like GLUT_KEY_F1, F2, F3,...
arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
*****/
void onSpecialKeyUp(int key, int x, int y) {
    //write your codes here

    switch (key) {

    case GLUT_KEY_UP:
        up = false;
        break;

    case GLUT_KEY_DOWN:
        down = false;
        break;

    case GLUT_KEY_LEFT:
        left = false;
        break;

    case GLUT_KEY_RIGHT:
        right = false;
        break;
    }

    //to refresh the window it calls display() function
    glutPostRedisplay();
}


/*****
When a click occurs in the window,
It provides which button
buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
states  : GLUT_UP , GLUT_DOWN
x, y is the coordinate of the point that mouse clicked
*****/
void onClick(int button, int stat, int x, int y) {

    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {
        r = rand() % 256;
        g = rand() % 256;
        b = rand() % 256;
    }

    if (button == GLUT_RIGHT_BUTTON && stat == GLUT_DOWN) {
        switch (mode) {
        case RECTANGLE:
            xR = x - winWidth / 2;
            yR = winHeight / 2 - y;
            break;

        case TRIANGLE:
            xT = x - winWidth / 2;
            yT = winHeight / 2 - y;
            break;

        case BALLOON:
            xB = x - winWidth / 2;
            yB = winHeight / 2 - y;
            break;
        }

    }

    //to refresh the window it calls display() function
    glutPostRedisplay();
}

/*****
This function is called when the window size changes.
w : is the new width of the window in pixels
h : is the new height of the window in pixels
*****/
void onResize(int w, int h) {
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); //refresh window
}

void onMoveDown(int x, int y) {
    //write your codes here.



    //to refresh the window it calls display() function	
    glutPostRedisplay();
}

/*****
GLUT to OpenGL coordinate conversion
x2 = x1 - winWidth / 2
y2 = winHeight / 2 - y1
*****/
void onMove(int x, int y) {
    //write your codes here.



    //to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);

    if (activeTimer) {
        // animate the current shape until it touches the edge of the window.
        switch (mode) {
        case RECTANGLE:
            if (-yR + heightR / 2 < winHeight / 2)
                yR--;
            break;

        case TRIANGLE:
            if (-yT + heightT / 2 < winHeight / 2)
                yT--;
            break;

        case BALLOON:
            if (yB + radiusB < winHeight / 2)
                yB++;
            break;
        }
    }

    //to refresh the window it calls display() function
    glutPostRedisplay(); //display()

}
#endif

void init() {
    //smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void main(int argc, char* argv[]) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(1800, 200);
    glutCreateWindow("Lab03(800x400) Interactivity: Animating Shapes");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //keyboard registration
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //mouse registration
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    init();

    //initialize random generator
    srand(time(0));

    glutMainLoop();

}
