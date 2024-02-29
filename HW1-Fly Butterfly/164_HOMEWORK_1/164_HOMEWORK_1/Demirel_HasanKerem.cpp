/*********
   CTIS164 - Template Source Program
----------
STUDENT : Hasan Kerem Demirel
SECTION : 003
HOMEWORK: Homework I
----------
PROBLEMS: -
----------
ADDITIONAL FEATURES: 1) If you left click on the right window the right curtain will be closed (/ opened if it is closed)
                        If you left click on the left window the left curtain will be closed (/ opened if it is closed)
                     2) You can see the flowers flowing with the beatiful wind of spring in the air
                     3) If you press the up arrow key the butterfly can move upwards and stops when it comes to the top of the window
                        If you press the down arrow key the butterfly can move downwards and stops when it comes to the bottom of the window
                     4) If you press the left arrow key (!while the butterfly is moving!) the butterfly can move backwards and
                        stops when it comes to the start of the window
                     5) If you right click in the window (!while the butterfly is moving!) the butterfly can teleport wherever you clicked on
                     (in the limits of the window)(only horizontal)
                     6) If you press 'esc' you will exit the program

***********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 650

#define TIMER_PERIOD  10 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define OPEN 1          //Left side curtain's condition : open
#define OPEN2 1         //Right side curtain's condition : open
#define CLOSED -1       //Left side curtain's condition : closed
#define CLOSED2 -1      //Right side curtain's condition : closed

/* Global Variables*/
bool activeTimer = true;
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height


//Curtain
int mode1 = OPEN;       //Left curtain's condition
int mode2 = OPEN2;      //Right curtain's condition

int xCC = -147, yCC = -23, xCircle = -225, yCircle = -179;      //circles' (in the bottom of the curtain) initial positions
int widthCC = 294, heightCC = 320;                              //rectangle's initial dimensions (open curtain)
int rCircle = 15;                                               //circles' (in the bottom of the curtain) radius

int xC2 = 252, yC2 = -25;                                       //rectangle's initial position (right curtain)
int widthC2 = 85, heightC2 = 320;                               //rectangle's initial dimensions (right curtain)

int xC = -252, yC = -25;                                         //circles' (in the bottom of the curtain) initial positions
int widthC = 85, heightC = 320;                                  //rectangle's initial dimensions (open curtain)

//Butterfly
int xBH = -120, yBH = -10;          //location of head of the butterfly
int xSWH = -153, ySWH = -16;        //location of the small wing of the butterfly
int xBWH = -186, yBWH = -45;        //location of the big wing of the butterfly
int radius = 10;                    //radius of head of the butterfly
int rSWH = 22;                      //radius of the small wing of the butterfly
int rBWH = 31;                      //radius of the big wing of the butterfly

//int xT = xCG + rCG - 5, yT = yCG + rCG - 5;
//int hT = 10;

//RECTANGLE
int xR = 0, yR = -50;               //rectangle's initial position (window)
int widthR = 585, heightR = 355;    //rectangle's initial dimensionsions (window)
int yL = yR + 250;                  //rectangle's initital position (label)


void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));         //angle /= 2 -> half circle
    }
    glEnd();
}

// display text with variables.
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
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
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}



//view through the window
void drawRectangle() {
    glRectf(xR - widthR / 2, yR - heightR / 2, xR + widthR / 2, yR + heightR / 2);
}

//outer window
void drawRectangle2(void)
{

    int xW = 0, yW = -50;
    int widthW = 625, heightW = 395;

    glColor3f(120 / 225., 21 / 225., 21 / 225.);
    glRectf(xW - widthW / 2, yW - heightW / 2, xW + widthW / 2, yW + heightW / 2);



    glLineWidth(3);
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(310, 148);
    glVertex2i(-310, 148);
    glVertex2i(-310, -248);
    glVertex2i(310, -248);
    glEnd();
}


//right inner window
void drawWindow1() {

    glLineWidth(11);
    glColor3f(165 / 225., 21 / 225., 21 / 225.);
    glBegin(GL_LINE_LOOP);
    glVertex2i(288, 128);
    glVertex2i(0, 128);
    glVertex2i(0, -228);
    glVertex2i(288, -228);
    glEnd();

    glLineWidth(2);
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(283, 124);
    glVertex2i(6, 124);
    glVertex2i(6, -223);
    glVertex2i(283, -223);
    glEnd();

    glLineWidth(3);
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(295, 135);
    glVertex2i(-295, 135);
    glVertex2i(-295, -235);
    glVertex2i(295, -235);
    glEnd();

}

//left inner window
void drawWindow2() {

    glLineWidth(11);
    glColor3f(165 / 225., 21 / 225., 21 / 225.);
    glBegin(GL_LINE_LOOP);
    glVertex2i(0, 128);
    glVertex2i(-288, 128);
    glVertex2i(-288, -228);
    glVertex2i(0, -228);
    glEnd();

    glLineWidth(2);
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(-6, 124);
    glVertex2i(-283, 124);
    glVertex2i(-283, -223);
    glVertex2i(-6, -223);
    glEnd();
}

//black edges of windows
void edge() {

    glLineWidth(2);
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(0, -235);
    glVertex2i(0, 135);
    glEnd();
}

//the grass ground
void drawGrassG() {

    int xG = 0, yG = -50; //rectangle's initial position
    int widthG = 585, heightG = 140; //rectangle's initial dimensions

    glColor3f(0, 1, 0);
    glRectf(xG - widthR / 2, yG - heightR / 2, xG + widthG / 2, yG + heightG / 2);

}


//right curtain is open
void drawOpenCurtain() {

    glColor3f(62 / 225., 12 / 225., 94 / 225.);

    glRectf(xC - widthC / 2, yC - heightC / 2, xC + widthC / 2, yC + heightC / 2);

    circle(-225, -179, 15);

    circle(-255, -179, 15);

    circle(-285, -179, 15);
}

//left curtain is open
void drawOpenCurtain2() {

    glColor3f(62 / 225., 12 / 225., 94 / 225.);

    glRectf(xC2 - widthC2 / 2, yC2 - heightC2 / 2, xC2 + widthC2 / 2, yC2 + heightC2 / 2);

    circle(225, -179, 15);

    circle(255, -179, 15);

    circle(285, -179, 15);
}

//right curtain is closed
void drawClosedCurtain()
{

    glColor3f(62 / 225., 12 / 225., 94 / 225.);

    glRectf(xCC - widthCC / 2, yCC - heightCC / 2, xCC + widthCC / 2, yCC + heightCC / 2);

    circle(xCircle + 210, yCircle, rCircle);

    circle(xCircle + 180, yCircle, rCircle);

    circle(xCircle + 150, yCircle, rCircle);

    circle(xCircle + 120, yCircle, rCircle);

    circle(xCircle + 90, yCircle, rCircle);

    circle(xCircle + 60, yCircle, rCircle);

    circle(xCircle + 30, yCircle, rCircle);

    circle(xCircle, yCircle, rCircle);

    circle(xCircle - 30, yCircle, rCircle);

    circle(xCircle - 60, yCircle, rCircle);

    //

    glColor3f(62 / 225., 12 / 225., 94 / 225.);

    glRectf(xC2 - widthC2 / 2, yC2 - heightC2 / 2, xC2 + widthC2 / 2, yC2 + heightC2 / 2);

    circle(225, -179, 15);

    circle(255, -179, 15);

    circle(285, -179, 15);
}

void drawClosedCurtain2()
{
    int xCircle2 = xCircle * -1,
        xCC2 = xCC * -1;

    glColor3f(62 / 225., 12 / 225., 94 / 225.);

    glRectf(xCC2 - widthCC / 2, yCC - heightCC / 2, xCC2 + widthCC / 2, yCC + heightCC / 2);

    circle(xCircle2 - 210, yCircle, rCircle);

    circle(xCircle2 - 180, yCircle, rCircle);

    circle(xCircle2 - 150, yCircle, rCircle);

    circle(xCircle2 - 120, yCircle, rCircle);

    circle(xCircle2 - 90, yCircle, rCircle);

    circle(xCircle2 - 60, yCircle, rCircle);

    circle(xCircle2 - 30, yCircle, rCircle);

    circle(xCircle2, yCircle, rCircle);

    circle(xCircle2 + 30, yCircle, rCircle);

    circle(xCircle2 + 60, yCircle, rCircle);

    //

    glColor3f(62 / 225., 12 / 225., 94 / 225.);

    glRectf(xC - widthC / 2, yC - heightC / 2, xC + widthC / 2, yC + heightC / 2);

    circle(-225, -179, 15);

    circle(-255, -179, 15);

    circle(-285, -179, 15);

}

void drawWings()
{
    //small wings

    glColor3f(1, 20 / 225., 147 / 225.);
    circle(xSWH, ySWH, rSWH);
    glColor3f(31 / 225., 81 / 225., 1);
    circle(xSWH, ySWH, rSWH - 8);
    glColor3f(1, 20 / 225., 147 / 225.);
    circle(xSWH, ySWH, rSWH - 16);

    //big wings
    
    glColor3f(1, 20 / 225., 147 / 225.);
    circle(xBWH, yBWH, rBWH);
    glColor3f(31 / 225., 81 / 225., 1);
    circle(xBWH, yBWH, rBWH - 11);
    glColor3f(1, 20 / 225., 147 / 225.);
    circle(xBWH, yBWH, rBWH - 20);
    glColor3f(31 / 225., 81 / 225., 1);
    circle(xBWH, yBWH, rBWH - 28);

}

void drawFlowers() {

    int f_rad = 10;
    int num1, num2;

    for (int i = 1; i <= 15; i++)
    {
        num1 = rand() % 540 - 270,
            num2 = rand() % 218 - 149;

        int x = num1,
            y = num2;

        //leaf
        glColor3f(1, 1, 1);
        circle(x, y + 8, f_rad);
        glColor3f(1, 1, 1);
        circle(x + 8, y, f_rad);
        glColor3f(1, 1, 1);
        circle(x, y - 8, f_rad);
        glColor3f(1, 1, 1);
        circle(x - 8, y, f_rad);

        //body
        glColor3f(1, 1, 0);
        circle(x, y, f_rad * 2.5 / 3);
    }


}

void drawButterfly()
{

    /*HEAD*/
    glColor3ub(0, 0, 0);
    circle(xBH, yBH, radius);

    //eye
    glColor3f(1, 1, 1);
    circle(xBH + 2, yBH + 2, 3);

    //eyeball
    glColor3f(0, 100 / 225., 1);
    circle(xBH + 3, yBH + 3, 2);

    //antennas
    //#1
    glLineWidth(2);
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(xBH - 2, yBH + 10);        //-122,0
    glVertex2f(xBH - 35, yBH + 40);       //-155,30
    glEnd();
    //#2
    glLineWidth(2);
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(xBH - 2, yBH + 10);        //-122,0
    glVertex2f(xBH - 25, yBH + 45);       //-145,35
    glEnd();

    //ARMS
    glLineWidth(1);
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(xBH - 10, yBH - 10);       //-130,-20
    glVertex2f(xBH - 10, yBH - 40);       //-130,-50
    glEnd();

    glLineWidth(1);
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(xBH - 10, yBH - 10);       //-130,-20
    glVertex2f(xBH + 10, yBH - 10);       //-110,-30
    glEnd();


    //WINGS
    drawWings();

    //BODY
    glLineWidth(5);
    glColor3f(0, 0, 0); //black
    glBegin(GL_LINES);
    glVertex2f(xBH, yBH);
    glVertex2f(xBH - 60, yBH - 80);       //-180,-90
    glEnd();
}


void drawLabel()        //widthR = 585, heightR = 355
{
    glColor3f(120 / 225., 51 / 225., 21 / 225.);
    glRectf(xR - (widthR - 30) / 2, yL - (heightR / 5) / 2, xR + (widthR - 30) / 2, yL + (heightR / 5) / 2);

    glColor3f(1, 1, 1);
    glRectf(xR - (widthR - 50) / 2, yL - (heightR / 5 - 20) / 2, xR + (widthR - 50) / 2, yL + (heightR / 5 - 20) / 2);

    glColor3f(128 / 225., 128 / 225., 128 / 225.);
    circle(xR + 272, yL + 29, 6);

    glColor3f(158 / 225., 158 / 225., 158 / 225.);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(xR + 275, yL + 32);
    glVertex2f(xR + 269, yL + 24);
    glEnd();

    glColor3f(128 / 225., 128 / 225., 128 / 225.);
    circle(xR - 272, yL + 29, 6);

    glColor3f(158 / 225., 158 / 225., 158 / 225.);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(xR - 275, yL + 32);
    glVertex2f(xR - 269, yL + 24);
    glEnd();

    glColor3f(128 / 225., 128 / 225., 128 / 225.);
    circle(xR + 272, yL - 29, 6);

    glColor3f(158 / 225., 158 / 225., 158 / 225.);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(xR + 275, yL - 32);
    glVertex2f(xR + 269, yL - 24);
    glEnd();

    glColor3f(128 / 225., 128 / 225., 128 / 225.);
    circle(xR - 272, yL - 29, 6);

    glColor3f(158 / 225., 158 / 225., 158 / 225.);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(xR - 275, yL - 32);
    glVertex2f(xR - 269, yL - 24);
    glEnd();
}

//displaying
void display() {
   
    //
    // clear the wall to camel color
    //
    glClearColor(193 / 225., 154 / 225., 107 / 225., 0);
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i <= 1000; i += 100)        //line patterns on the wall as decoration
    {
        glLineWidth(8);
        glColor3f(220 / 225., 194 / 225., 157 / 225.);
        glBegin(GL_LINES);
        glVertex2f(-500 + i, 1000);
        glVertex2f(-500 + i, -1000);
        glEnd();
    }

    drawRectangle2();                            //outer window

    glColor3f(135 / 225., 206 / 225., 225 / 225.);
    drawRectangle();                            //sky view thorugh the window

    drawLabel();                                //label at the top

    glColor3f(1, 0, 0);
    vprint(xR - 120, yL - 2, GLUT_BITMAP_8_BY_13, "Spring has come by Kerem Demirel");      //writing in the label

    drawGrassG();                               //grass as ground

    srand(time(NULL));                          //for random number generating for the location of the flowers

    drawFlowers();                              //flowers' flow

    drawButterfly();                            //butterfly

    drawWindow1();                              //inner left window

    drawWindow2();                              //inner right window

    edge();                                     //edge/side  of the windows

    switch (mode1) {                            //left curtain open/closed
    case OPEN:
        drawOpenCurtain();
        break;

    case CLOSED:
        drawClosedCurtain();
        break;
    }

    switch (mode2) {                             //right curtain open/closed
    case OPEN2:
        drawOpenCurtain2();
        break;

    case CLOSED2:
        drawClosedCurtain2();
        break;
    }

    glutSwapBuffers();
}

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

void onMoveDown(int x, int y) {}

void onMove(int x, int y) {}


// key function for ASCII charachters
void onKeyDown(unsigned char key, int x, int y)
{
    
    if (key == 27)                       // exit when ESC is pressed.
        exit(0);

    if (key == ' ')
        activeTimer = !activeTimer;      //stop / start the action of the butterfly flying (horizontal)

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y){}


// key function for special charachters 
void onSpecialKeyDown(int key, int x, int y)
{
  
    switch (key) {
    case GLUT_KEY_UP:
        up = true;
        break;
    case GLUT_KEY_DOWN:
        down = true;
        break;
    case GLUT_KEY_LEFT:
        left = true;
        break;
    case GLUT_KEY_RIGHT:
        right = true;
        break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
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

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onClick(int button, int stat, int x, int y)
{
  
    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)         //to close/open the curtains
    {
        if (x < 500 && x > 0 && y < 550 && y > 190)              //to close/open the left curtain
        {
            mode1 *= -1;
        }

        if (x < 1010 && x > 510 && y < 550 && y>190)             //to close/open the right curtain
        {
            mode2 *= -1;
        }
    }


    if (button == GLUT_RIGHT_BUTTON && stat == GLUT_DOWN)       //to change the butterfly's direction backwards while it is moving
    {

        int diff = xBH - xSWH,
            diff2 = xBH - xBWH;

        if (0 < x && x < 1010 && 190 < y && y < 550)
        {
            xBH = x;
            xSWH = x - diff;
            xBWH = x - diff2;
        }
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);


    if (activeTimer)                    //to change the butterfly's direction backwards while it is moving
    {
        if (left)
        {
            if (xBH > -200)
            {
                xBH--;
                xSWH--;
                xBWH--;
            }
        }

        else                            //moves the butterfly (horizontal)
        { 
            if (xBH < 275)
            {
                xBH++;
                xSWH++;
                xBWH++;
                
            }
            else                       //butterfly returns to the start of the window when it reaches to the end
            {
                xBH -= 470;
                xSWH -= 470;
                xBWH -= 470;
                
            }
        }
    }

    if (up)                             //butterfly flies upwards
    {
        if (yBH < 80)
        {
            yBH++;
            ySWH++;
            yBWH++;
        }
    }

    else if (down)                      //butterfly flies downwards
    {
        if (yBH > -140)
        {
            yBH--;
            ySWH--;
            yBWH--;
        }
    }

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void main(int argc, char* argv[]) {


    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(450, 75);
    glutCreateWindow("Spring Has Come");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    //glutMotionFunc(onMoveDown);
    //glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    glutMainLoop();
}
