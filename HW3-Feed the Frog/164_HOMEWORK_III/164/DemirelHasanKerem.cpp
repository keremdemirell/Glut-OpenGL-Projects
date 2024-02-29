/*********
   CTIS164 - Template Source Program
----------
STUDENT : Hasan Kerem Demirel
SECTION : 003
HOMEWORK: 3
----------
PROBLEMS: There is a problem with rotation of the fly which i was not able to understand. (the rotation is only implied on the body of the fly because i could not figure out why
          was a problem and i could not solve it. Thus,  i did not use rotation to the other parts of the fly). Also, The body of the fly is created on the location where mouse
          is clicked without the rotation.
----------
ADDITIONAL FEATURES: 1-) There is a timer (20 seconds)
                     2-) When the time ends there is an end game screen.
                     3-) The end game screen can be 2 different screens depending on how many flies the frog has eaten.
                     -If the frog has eaten less than 20 flies, because the frog is going to live, in the game over screen, the amount of the flies eaten is shown.
                     -Otherwise, because the frog is not going to live, there is a memorial speech for out frog 'frodo'.
                     4-) The frog gets bigger till it reaches to limits of getting bigger everytime it eats a fly.
                     5-) The frog becomes red when it eats a certain amount of flies.
                     6-) When the frog eates 20 flies, it explodes and dissappears and gives a message to the user.
                     -Also thwn the frog eates 20 flies, the remaining time becomes 3 seconds so that the player would not wait till the time ends.
                     7-) There are different error messages at the different places of the screen according to the amount of the flies eaten.
                     8-) Also, according to the amount of the flies eaten, the purpose of the game title (feed the frog!!!) changes.
                     9-) It exits the program when pressed 'esc'
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "vec.h"

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 2000 / 3

#define TIMER_PERIOD  10 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

int xL = 0, yL = 0,            //coordinats of the lake
    wL = WINDOW_HEIGHT, hL = WINDOW_HEIGHT;     //sizes of the lake

int xLa = -100, yLa = -50;          //coordinats of the land

int xF = 0, yF = 200,       //coordinats of the fly
    wF = 8, hF = 25;        //sizes of the fly

int xFG = 0, yFG = -200,        //coordinats of the frog
    rFG = 40,                   //radius of the head of the frog
    xFM, yFM,                   //coordinats of the frog's mouth
    rFM;                        //radius of the mouth of the frog


int fly = 0;                //If the fly appears on the screen or not

int eaten = 0, flycnt = 0;          //If the fly reaches to the mouth of the frog eaten becomes 1 end flycnt which is the counter of the flies eaten increments

int redfrog = 0;                //The frog becomes red when it eats a certain amount of flies.

int second = 19, splitsecond = 99;      //the time that is left and shown in the up right corner

double xRIP = -400., yRIP = -500.;      //The memorial speech's locations

typedef struct {
    vec_t pos;
} player_t;

typedef struct {
    vec_t pos;
    float angle;
    vec_t vel;
} enemy_t;

player_t P = { { xFG, yFG } };
enemy_t  E = { { xF, yF }, 0 };

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
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

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
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

void drawInfoCard()
{
    glColor3f(0, 0, 0);
    vprint(320, 320, GLUT_BITMAP_8_BY_13, "Time Reamining: %d.%.2d", second, splitsecond);                  //shows the timer
    vprint(320, 300, GLUT_BITMAP_8_BY_13, "Eaten Fly: %d", flycnt);                 //shows the amount of the eaten flies

    
    if (flycnt == 20)
    {
        vprint(320, 280, GLUT_BITMAP_8_BY_13, "Feed The Frog Game!!!");         //purpose of the game title
        glColor3f(1, 1, 1);
        glLineWidth(2);
        //Message when the user kills the frog
        vprint2(P.pos.x + 60, P.pos.y, 0.2, "There, it died.");                 
        vprint2(P.pos.x + 60, P.pos.y - 20, 0.2, "Think about what you have done");
        vprint(P.pos.x + 60, P.pos.y - 32, GLUT_BITMAP_8_BY_13, "Rest In Peace Little Frodo");
    }

    else
    {
        if (splitsecond >= 50)
        {

            if (flycnt < 5)
                vprint(320, 280, GLUT_BITMAP_8_BY_13, "Feed The Frog Game!!!");         //purpose of the game title

            if (flycnt >= 5)
            {
                glLineWidth(1);
                glColor3f(1, 0, 0);
                vprint2(250, 200, 0.1, "!!FROG IS GONNA EXPLODE!!");            //error message
                
                if (flycnt < 10)
                {
                    glColor3f(1, 1, 1);
                    vprint(280, 280, GLUT_BITMAP_8_BY_13, "Feed The Frog!!(Be Careful)");       //error message
                }

                if (flycnt >= 10)
                {
                    glColor3f(1, 0, 0);
                    glLineWidth(1);
                    vprint2(-250, 150, 0.1, "!!STOP THIS MADNESS!!");       //error message

                    if (flycnt < 15)
                    {
                        glColor3f(1, 1, 1);
                        vprint(320, 280, GLUT_BITMAP_8_BY_13, "Feed The Frog???");      //purpose of the game title

                    }

                    if (flycnt >= 15)
                    {
                        glColor3f(1, 0, 0);
                        glLineWidth(2);
                        vprint2(-150, 0, 0.2, "!!DEATH IS BEHIND TWO FLIES!!");         //error message
                            
                        if (flycnt < 18)
                        {
                            glColor3f(1, 1, 1);
                            vprint(310, 280, GLUT_BITMAP_8_BY_13, "Do NOT Feed The Frog!!!");           //purpose of the game title
                        }

                        if (flycnt >= 18)
                        {
                            glColor3f(1, 100 / 225., 100 / 225.);
                            glLineWidth(2);
                            vprint2(-320, -275, 0.25, "!!YOU DONT WANT TO THROW THAT LAST FLY!!");              //error message
                            vprint(320, 280, GLUT_BITMAP_8_BY_13, ":(");            //purpose of the game title
                        }
                    }
                }
            }
        }
    }
}

void displaygameover()      //the 'game over' screen
{
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    if (flycnt == 20)
    {
        if (yRIP < 180)
        {
            glColor3f(1, 1, 0);
            vprint2(-200, 200, 0.5, "GAME OVER");
        }

        //memorial speech
        glLineWidth(1);
        vprint2(xRIP, yRIP, 0.15, "Today, we gather here to remember and pay tribute");
        vprint2(xRIP, yRIP - 20, 0.15, "to a beloved companion, our dear frog Frodo.");
        vprint2(xRIP, yRIP - 40, 0.15, "Though small in size, Frodo left a");
        vprint2(xRIP, yRIP - 60, 0.15, "big impression on our hearts.");
        vprint2(xRIP, yRIP - 80, 0.15, "It brought joy, laughter, and a sense of");
        vprint2(xRIP, yRIP - 100, 0.15, "wonder into our lives with its playful hops, ");
        vprint2(xRIP, yRIP - 120, 0.15, "its soothing croaks, and ");
        vprint2(xRIP, yRIP - 140, 0.15, "its graceful presence.");
        vprint2(xRIP, yRIP - 160, 0.15, "Frodo was more than just a pet; it was a part ");
        vprint2(xRIP, yRIP - 180, 0.15, "of our family, a loyal friend who provided companionshipand ");
        vprint2(xRIP, yRIP - 200, 0.15, "shared moments of happiness with us.");
        vprint2(xRIP, yRIP - 220, 0.15, "We remember the times spent watching it explore ");
        vprint2(xRIP, yRIP - 240, 0.15, "its habitat, leaping with agility, and bringing smiles to our faces.");
        vprint2(xRIP, yRIP - 300, 0.15, "Rest in peace, dear Frodo.You will be deeply missed, ");
        vprint2(xRIP, yRIP - 320, 0.15, "but your legacy will live on within us.");
    }

    else
    {
        glColor3f(1, 1, 0);
        vprint2(-200, 200, 0.5, "GAME OVER");

        glLineWidth(1);
        vprint2(-300, 0, 0.2, "You have eaten %d flies", flycnt);           //amount of flies eaten
        vprint2(-300, -20, 0.2, "Thank you for not exploding our beloved frog Frodo!");         //thank you message
    }
}

void displayLand()          //the brown land which touching the lake
{
    glColor3f(139 / 225., 98 / 225., 76 / 225.);

    circle(xLa - 400, yLa - 100, 100);
    circle(xLa - 150, yLa - 200, 250);
    circle(xLa  + 0, yLa - 50, 100);
    circle(xLa + 50, yLa - 50, 100);
    circle(xLa + 150, yLa - 100, 100);
    circle(xLa + 300, yLa - 95, 125);
    circle(xLa + 300, yLa - 95, 125);
    circle(xLa + 400, yLa - 95, 125);
    circle(xLa + 500, yLa - 75, 130);
    circle(xLa + 600, yLa - 65, 150);

    glRectf(0 - 1000 / 2, -300 - 300 / 2, 0 + 1000 / 2, -300 + 300 / 2);

}

void vertex(enemy_t P, double angle) {          //for rotation of the fly
    double xp = (P.pos.x * cos(angle) - P.pos.y * sin(angle));
    double yp = (P.pos.x * sin(angle) + P.pos.y * cos(angle));
    glVertex2d(xp, yp);
}

void drawFly(enemy_t e)
{  

    double fangle = atan2(P.pos.y, P.pos.x);

    //body
    glColor3f(0, 0, 0);
    /*glLineWidth(wF);
    glBegin(GL_LINE_LOOP);
    glVertex2i(e.pos.x, e.pos.y - hF / 2);
    glVertex2i(e.pos.x, e.pos.y + hF / 2);
    glEnd();*/

    glBegin(GL_LINE_LOOP);
    vertex({ e.pos.x, e.pos.y - hF / 2 }, fangle);
    vertex({ e.pos.x, e.pos.y + hF / 2 }, fangle);
    glEnd();

    //lEft EyE
    glColor3f(1, 0, 0);
    circle(e.pos.x - wF / 2 - 5, e.pos.y + hF / 2, 7);

    glLineWidth(1);
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(e.pos.x - wF / 2 - 5 - 7, e.pos.y + hF / 2 + 2);
    glVertex2i(e.pos.x - wF / 2 - 5 + 7, e.pos.y + hF / 2 + 2);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2i(e.pos.x - wF / 2 - 5 - 7, e.pos.y + hF / 2 - 2);
    glVertex2i(e.pos.x - wF / 2 - 5 + 7, e.pos.y + hF / 2 - 2);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2i(e.pos.x - wF / 2 - 5 + 2, e.pos.y + hF / 2 - 7);
    glVertex2i(e.pos.x - wF / 2 - 5 + 2, e.pos.y + hF / 2 + 7);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2i(e.pos.x - wF / 2 - 5 - 2, e.pos.y + hF / 2 - 7);
    glVertex2i(e.pos.x - wF / 2 - 5 - 2, e.pos.y + hF / 2 + 7);
    glEnd();

    //right eye
    glColor3f(1, 0, 0);
    circle(e.pos.x + wF / 2 + 5, e.pos.y + hF / 2, 7);

    glLineWidth(1);
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(e.pos.x + wF / 2 + 5 - 7, e.pos.y + hF / 2 + 2);
    glVertex2i(e.pos.x + wF / 2 + 5 + 7, e.pos.y + hF / 2 + 2);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2i(e.pos.x + wF / 2 + 5 - 7, e.pos.y + hF / 2 - 2);
    glVertex2i(e.pos.x + wF / 2 + 5 + 7, e.pos.y + hF / 2 - 2);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2i(e.pos.x + wF / 2 + 5 + 2, e.pos.y + hF / 2 - 7);
    glVertex2i(e.pos.x + wF / 2 + 5 + 2, e.pos.y + hF / 2 + 7);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2i(e.pos.x + wF / 2 + 5 - 2, e.pos.y + hF / 2 - 7);
    glVertex2i(e.pos.x + wF / 2 + 5 - 2, e.pos.y + hF / 2 + 7);
    glEnd();

    //left wing
    glColor3f(242 / 225., 242 / 225., 242);

    glBegin(GL_TRIANGLES);
    glVertex2f(e.pos.x - 16, e.pos.y - 8);
    glVertex2f(e.pos.x - 8, e.pos.y - 16);
    glVertex2f(e.pos.x, e.pos.y + 3);
    glEnd();

    /*glBegin(GL_TRIANGLES);
    vertex({ e.pos.x - 16, e.pos.y - 8 }, fangle);
    vertex({ e.pos.x - 8, e.pos.y - 16 }, fangle);
    vertex({ e.pos.x, e.pos.y + 3 }, fangle);
    glEnd(); --> with the rotation*/

    circle(e.pos.x - 15, e.pos.y - 15, 6);

    //right wing
    glBegin(GL_TRIANGLES);
    glVertex2f(e.pos.x + 16, e.pos.y - 8);
    glVertex2f(e.pos.x + 8, e.pos.y - 16);
    glVertex2f(e.pos.x, e.pos.y + 3);
    glEnd();

    circle(e.pos.x + 15, e.pos.y - 15, 6);

}

void drawFrog(player_t p)
{


    if (flycnt < 20)
    {
        //head
        if (!redfrog)
            glColor3f(71 / 225., 156 / 225., 82 / 225.);
        else if (redfrog)
            glColor3f(1, 100 / 225., 100 / 225.);
        circle(p.pos.x, p.pos.y, rFG);

        //left eye
        if (!redfrog)
            glColor3f(71 / 225., 156 / 225., 82 / 225.);
        else if (redfrog)
            glColor3f(1, 100 / 225., 100 / 225.);
        circle(p.pos.x - rFG * 2 / 3, p.pos.y + rFG * 2 / 3, rFG / 2);
        glColor3f(1, 1, 1);
        circle(p.pos.x - rFG * 2 / 3, p.pos.y + rFG * 2 / 3, rFG / 3);
        glColor3f(0, 0, 0);
        circle(p.pos.x - rFG * 2 / 3, p.pos.y + rFG * 2 / 3, rFG / 5);
        glColor3f(1, 1, 1);
        circle(p.pos.x - rFG * 2 / 3 + rFG / 20, p.pos.y + rFG * 2 / 3 + rFG / 20, rFG / 20);

        //right eye
        if (!redfrog)
            glColor3f(71 / 225., 156 / 225., 82 / 225.);
        else if (redfrog)
            glColor3f(1, 100 / 225., 100 / 225.);
        circle(p.pos.x + rFG * 2 / 3, p.pos.y + rFG * 2 / 3, rFG / 2);
        glColor3f(1, 1, 1);
        circle(p.pos.x + rFG * 2 / 3, p.pos.y + rFG * 2 / 3, rFG / 3);
        glColor3f(0, 0, 0);
        circle(p.pos.x + rFG * 2 / 3, p.pos.y + rFG * 2 / 3, rFG / 5);
        glColor3f(1, 1, 1);
        circle(p.pos.x + rFG * 2 / 3 + rFG / 20, p.pos.y + rFG * 2 / 3 + rFG / 20, rFG / 20);

        //mouth
        glColor3f(1, 0, 0);
        xFM = p.pos.x;
        yFM = p.pos.y - rFG / 5;
        rFM = rFG * 2 / 3;
        circle(xFM, yFM, rFM);


        //teeth
        //left
        glColor3f(1, 1, 1);
        glLineWidth(rFG / 5);
        glBegin(GL_LINE_LOOP);
        glVertex2i(p.pos.x - rFG / 8, p.pos.y - rFG / 5 + rFG * 2 / 3);
        glVertex2i(p.pos.x - rFG / 8, p.pos.y - rFG / 5 + rFG * 2 / 3 - rFG / 4);
        glEnd();
        circle(p.pos.x - rFG / 8, p.pos.y - rFG / 5 + rFG * 2 / 3 - rFG / 4, rFG / 10);
        //right
        glColor3f(1, 1, 1);
        glLineWidth(rFG / 5);
        glBegin(GL_LINE_LOOP);
        glVertex2i(p.pos.x + rFG / 8, p.pos.y - rFG / 5 + rFG * 2 / 3);
        glVertex2i(p.pos.x + rFG / 8, p.pos.y - rFG / 5 + rFG * 2 / 3 - rFG / 4);
        glEnd();
        circle(p.pos.x + rFG / 8, p.pos.y - rFG / 5 + rFG * 2 / 3 - rFG / 4, rFG / 10);

        glLineWidth(rFG / 20);
        glColor3f(0, 0, 0);
        circle_wire(p.pos.x, p.pos.y - rFG / 5, rFG * 2 / 3);

        //tongue
        glColor3f(199 / 225., 80 / 225., 118 / 225.);
        glLineWidth(rFG / 20);
        circle_wire(p.pos.x, p.pos.y - rFG / 3, rFG / 5);
        glColor3f(229 / 225., 110 / 225., 148 / 225.);
        circle(p.pos.x, p.pos.y - rFG / 3, rFG / 5);
    }

    else
    {

        glColor3f(1, 0, 0);

        glLineWidth(5);
        glBegin(GL_LINE_LOOP);
        glVertex2i(p.pos.x, p.pos.y + 30);
        glVertex2i(p.pos.x, p.pos.y + 50);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex2i(p.pos.x, p.pos.y - 30);
        glVertex2i(p.pos.x, p.pos.y - 50);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex2i(p.pos.x + 30, p.pos.y);
        glVertex2i(p.pos.x + 50, p.pos.y);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex2i(p.pos.x - 30, p.pos.y);
        glVertex2i(p.pos.x - 50, p.pos.y);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex2i(p.pos.x + 20, p.pos.y + 20);
        glVertex2i(p.pos.x + 40, p.pos.y + 40);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex2i(p.pos.x - 20, p.pos.y + 20);
        glVertex2i(p.pos.x - 40, p.pos.y + 40);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex2i(p.pos.x + 20, p.pos.y - 20);
        glVertex2i(p.pos.x + 40, p.pos.y - 40);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex2i(p.pos.x - 20, p.pos.y - 20);
        glVertex2i(p.pos.x - 40, p.pos.y - 40);
        glEnd();


    }

}

void isEaten(int x, int y)
{
    if (eaten != 1)
    {
        if (E.pos.x >= x - rFM && x + rFM >= E.pos.x)
        {
            if (E.pos.y >= y - rFM && y + rFM >= E.pos.y)
            {
                if (rFG < 100)
                {
                    rFG += 10;          //when the fly goes to the frog's mouth, the frog gets bigger
                    if (rFG == 100)
                        redfrog = 1;        //gets red if its big enough
                }

                if (second > 0)
                    flycnt++;               //amount of the fly eaten
                eaten = 1;
                fly = 0;
            }
        }
    }

}


//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(140 / 225., 205 / 225., 106 / 225., 0);
    glClear(GL_COLOR_BUFFER_BIT);

    if (second != 0)
    {
        displayLand();

        drawFrog(P);

        if (fly)
            drawFly(E);

        drawInfoCard();
    }

    else
    {
        displaygameover();
    }

    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
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

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
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

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.

    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
    {
        if (fly == 0)
        {
            E.pos.x = x - WINDOW_WIDTH / 2;
            E.pos.y = WINDOW_WIDTH / 3 - y;
        }

    } 

    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
    {
        if (fly == 0)
        {
            fly = 1;
            eaten = 0;
        }
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    if (flycnt == 20)
    {
        if (second > 3)
            second == 3;
    }

    if (flycnt < 20)
    {
        if (P.pos.y <= 500 && P.pos.y >= -500)
        {
            (P.pos.y)++;            //the frog goes upward

            if (P.pos.y >= 350 + rFG)
                P.pos.y -= 700 + rFG;
        }

        isEaten(P.pos.x, P.pos.y - rFG / 5);

        //chasing
        E.vel = mulV(4, unitV(subV(P.pos, E.pos)));
        E.pos = addV(E.pos, E.vel);

        //rotation
        E.angle = angleV(E.pos);
    }

    if (splitsecond > 0)        //timer
    {
        splitsecond--;

        if (splitsecond == 0)
        {
            if (second > 0)
            {
                second--;
                splitsecond = 99;
            }

        }
    }

    if (second < 1)
    {
        yRIP += 0.5;            //memorial speech rises
    }


    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(260, 90);
    glutCreateWindow("Hasan Kerem Demirel");

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
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}