/*
 * This code was created by Jeff Molofee '99
 * (ported to Linux by Ti Leggett '01)
 * (ported to i.mx51, i.mx31 and x11 by Freescale '10)
 * If you've found this code useful, please let him know.
 *
 * Visit Jeff at http://nehe.gamedev.net/
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>
#include <math.h>
#include <signal.h>

#include "GLU3/glu3.h"
#include "GLES/gl.h"

#include "EGL/egl.h"
#include "FSL/fsl_egl.h"

EGLDisplay			egldisplay;
EGLConfig			eglconfig;
EGLSurface			eglsurface;
EGLContext			eglcontext;
EGLNativeWindowType eglNativeWindow;
EGLNativeDisplayType eglNativeDisplayType;
volatile sig_atomic_t quit = 0;

/* Here goes our drawing code */
void render()
{
    /* rotational vars for the triangle and quad, respectively */
    static GLfloat rtri, rquad;

    GLfloat colors[24][4];
    GLfloat vertices[24][3];

    /* Clear The Screen And The Depth Buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Move Left 1.5 Units And Into The Screen 6.0 */
    glLoadIdentity();
    glTranslatef(-1.5f, 0.0f, -6.0f);

    /* Rotate The Triangle On The Y axis ( NEW ) */
    glRotatef(rtri, 1.0f, 0.0f, 0.0f);

    /* Enable COLOR and VERTEX arrays */
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    /* Setup pointers to COLOR and VERTEX arrays */
    glColorPointer(4, GL_FLOAT, 0, colors);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    /* Red */
    colors[0][0]=1.0f; colors[0][1]=0.0f; colors[0][2]=0.0f; colors[0][3]=1.0f;
    /* Top Of Triangle (Front) */
    vertices[0][0]=0.0f; vertices[0][1]=1.0f; vertices[0][2]=0.0f;
    /* Green */
    colors[1][0]=0.0f; colors[1][1]=1.0f; colors[1][2]=0.0f; colors[1][3]=1.0f;
    /* Left Of Triangle (Front) */
    vertices[1][0]=-1.0f; vertices[1][1]=-1.0f; vertices[1][2]=1.0f;
    /* Blue */
    colors[2][0]=0.0f; colors[2][1]=0.0f; colors[2][2]=1.0f; colors[2][3]=1.0f;
    /* Right Of Triangle (Front) */
    vertices[2][0]=1.0f; vertices[2][1]=-1.0f; vertices[2][2]=1.0f;

    /* Red */
    colors[3][0]=1.0f; colors[3][1]=0.0f; colors[3][2]=0.0f; colors[3][3]=1.0f;
    /* Top Of Triangle (Right) */
    vertices[3][0]=0.0f; vertices[3][1]=1.0f; vertices[3][2]=0.0f;
    /* Blue */
    colors[4][0]=0.0f; colors[4][1]=0.0f; colors[4][2]=1.0f; colors[4][3]=1.0f;
    /* Left Of Triangle (Right) */
    vertices[4][0]=1.0f; vertices[4][1]=-1.0f; vertices[4][2]=1.0f;
    /* Green */
    colors[5][0]=0.0f; colors[5][1]=1.0f; colors[5][2]=0.0f; colors[5][3]=1.0f;
    /* Right Of Triangle (Right) */
    vertices[5][0]=1.0f; vertices[5][1]=-1.0f; vertices[5][2]=-1.0f;

    /* Red */
    colors[6][0]=1.0f; colors[6][1]=0.0f; colors[6][2]=0.0f; colors[6][3]=1.0f;
    /* Top Of Triangle (Back) */
    vertices[6][0]=0.0f; vertices[6][1]=1.0f; vertices[6][2]=0.0f;
    /* Green */
    colors[7][0]=0.0f; colors[7][1]=1.0f; colors[7][2]=0.0f; colors[7][3]=1.0f;
    /* Left Of Triangle (Back) */
    vertices[7][0]=1.0f; vertices[7][1]=-1.0f; vertices[7][2]=-1.0f;
    /* Blue */
    colors[8][0]=0.0f; colors[8][1]=0.0f; colors[8][2]=1.0f; colors[8][3]=1.0f;
    /* Right Of Triangle (Back) */
    vertices[8][0]=-1.0f; vertices[8][1]=-1.0f; vertices[8][2]=-1.0f;

    /* Red */
    colors[9][0]=1.0f; colors[9][1]=0.0f; colors[9][2]=0.0f; colors[9][3]=1.0f;
    /* Top Of Triangle (Left) */
    vertices[9][0]=0.0f; vertices[9][1]=1.0f; vertices[9][2]=0.0f;
    /* Blue */
    colors[10][0]=0.0f; colors[10][1]=0.0f; colors[10][2]=1.0f; colors[10][3]=1.0f;
    /* Left Of Triangle (Left) */
    vertices[10][0]=-1.0f; vertices[10][1]=-1.0f; vertices[10][2]=-1.0f;
    /* Green */
    colors[11][0]=0.0f; colors[11][1]=1.0f; colors[11][2]=0.0f; colors[11][3]=1.0f;
    /* Right Of Triangle (Left) */
    vertices[11][0]=-1.0f; vertices[11][1]=-1.0f; vertices[11][2]=1.0f;

    /* Drawing Using Triangles, draw triangles using 12 vertices */
    glDrawArrays(GL_TRIANGLES, 0, 12);

    /* Move Right 3 Units */
    glLoadIdentity();
    glTranslatef(1.5f, 0.0f, -6.0f);

    /* Rotate The Quad On The X axis ( NEW ) */
    glRotatef(rquad, 0.0f, 1.0f, 0.0f);

    /* Draw A Quad */

    /* Set The Color To Green */
    colors[0][0]=colors[1][0]=colors[2][0]=colors[3][0]=0.0f;
    colors[0][1]=colors[1][1]=colors[2][1]=colors[3][1]=1.0f;
    colors[0][2]=colors[1][2]=colors[2][2]=colors[3][2]=0.0f;
    colors[0][3]=colors[1][3]=colors[2][3]=colors[3][3]=1.0f;
    /* Top Right Of The Quad (Top) */
    vertices[0][0]=1.0f;  vertices[0][1]=1.0f; vertices[0][2]=-1.0f;
    /* Top Left Of The Quad (Top) */
    vertices[1][0]=-1.0f; vertices[1][1]=1.0f; vertices[1][2]=-1.0f;
    /* Bottom Right Of The Quad (Top) */
    vertices[2][0]=1.0f;  vertices[2][1]=1.0f; vertices[2][2]=1.0f;
    /* Bottom Left Of The Quad (Top) */
    vertices[3][0]=-1.0f; vertices[3][1]=1.0f; vertices[3][2]=1.0f;

    /* Set The Color To Orange */
    colors[4][0]=colors[5][0]=colors[6][0]=colors[7][0]=1.0f;
    colors[4][1]=colors[5][1]=colors[6][1]=colors[7][1]=0.5f;
    colors[4][2]=colors[5][2]=colors[6][2]=colors[7][2]=0.0f;
    colors[4][3]=colors[5][3]=colors[6][3]=colors[7][3]=1.0f;
    /* Top Right Of The Quad (Bottom) */
    vertices[4][0]=1.0f;  vertices[4][1]=-1.0f; vertices[4][2]=1.0f;
    /* Top Left Of The Quad (Bottom) */
    vertices[5][0]=-1.0f; vertices[5][1]=-1.0f; vertices[5][2]=1.0f;
    /* Bottom Right Of The Quad (Bottom) */
    vertices[6][0]=1.0f;  vertices[6][1]=-1.0f; vertices[6][2]=-1.0f;
    /* Bottom Left Of The Quad (Bottom) */
    vertices[7][0]=-1.0f; vertices[7][1]=-1.0f; vertices[7][2]=-1.0f;

    /* Set The Color To Red */
    colors[8][0]=colors[9][0]=colors[10][0]=colors[11][0]=1.0f;
    colors[8][1]=colors[9][1]=colors[10][1]=colors[11][1]=0.0f;
    colors[8][2]=colors[9][2]=colors[10][2]=colors[11][2]=0.0f;
    colors[8][3]=colors[9][3]=colors[10][3]=colors[11][3]=1.0f;
    /* Top Right Of The Quad (Front) */
    vertices[8][0]=1.0f;   vertices[8][1]=1.0f;   vertices[8][2]=1.0f;
    /* Top Left Of The Quad (Front) */
    vertices[9][0]=-1.0f;  vertices[9][1]=1.0f;   vertices[9][2]=1.0f;
    /* Bottom Right Of The Quad (Front) */
    vertices[10][0]=1.0f;  vertices[10][1]=-1.0f; vertices[10][2]=1.0f;
    /* Bottom Left Of The Quad (Front) */
    vertices[11][0]=-1.0f; vertices[11][1]=-1.0f; vertices[11][2]=1.0f;

    /* Set The Color To Yellow */
    colors[12][0]=colors[13][0]=colors[14][0]=colors[15][0]=1.0f;
    colors[12][1]=colors[13][1]=colors[14][1]=colors[15][1]=1.0f;
    colors[12][2]=colors[13][2]=colors[14][2]=colors[15][2]=1.0f;
    colors[12][3]=colors[13][3]=colors[14][3]=colors[15][3]=1.0f;
    /* Top Right Of The Quad (Back) */
    vertices[12][0]=1.0f;  vertices[12][1]=-1.0f; vertices[12][2]=-1.0f;
    /* Top Left Of The Quad (Back) */
    vertices[13][0]=-1.0f; vertices[13][1]=-1.0f; vertices[13][2]=-1.0f;
    /* Bottom Right Of The Quad (Back) */
    vertices[14][0]=1.0f;  vertices[14][1]=1.0f;  vertices[14][2]=-1.0f;
    /* Bottom Left Of The Quad (Back) */
    vertices[15][0]=-1.0f; vertices[15][1]=1.0f;  vertices[15][2]=-1.0f;

    /* Set The Color To Blue */
    colors[16][0]=colors[17][0]=colors[18][0]=colors[19][0]=0.0f;
    colors[16][1]=colors[17][1]=colors[18][1]=colors[19][1]=0.0f;
    colors[16][2]=colors[17][2]=colors[18][2]=colors[19][2]=1.0f;
    colors[16][3]=colors[17][3]=colors[18][3]=colors[19][3]=1.0f;
    /* Top Right Of The Quad (Left) */
    vertices[16][0]=-1.0f; vertices[16][1]=1.0f;  vertices[16][2]=1.0f;
    /* Top Left Of The Quad (Left) */
    vertices[17][0]=-1.0f; vertices[17][1]=1.0f;  vertices[17][2]=-1.0f;
    /* Bottom Right Of The Quad (Left) */
    vertices[18][0]=-1.0f; vertices[18][1]=-1.0f; vertices[18][2]=1.0f;
    /* Bottom Left Of The Quad (Left) */
    vertices[19][0]=-1.0f; vertices[19][1]=-1.0f; vertices[19][2]=-1.0f;

    /* Set The Color To Violet */
    colors[20][0]=colors[21][0]=colors[22][0]=colors[23][0]=1.0f;
    colors[20][1]=colors[21][1]=colors[22][1]=colors[23][1]=0.0f;
    colors[20][2]=colors[21][2]=colors[22][2]=colors[23][2]=1.0f;
    colors[20][3]=colors[21][3]=colors[22][3]=colors[23][3]=1.0f;
    /* Top Right Of The Quad (Right) */
    vertices[20][0]=1.0f; vertices[20][1]=1.0f;  vertices[20][2]=-1.0f;
    /* Top Left Of The Quad (Right) */
    vertices[21][0]=1.0f; vertices[21][1]=1.0f;  vertices[21][2]=1.0f;
    /* Bottom Right Of The Quad (Right) */
    vertices[22][0]=1.0f; vertices[22][1]=-1.0f; vertices[22][2]=-1.0f;
    /* Bottom Left Of The Quad (Right) */
    vertices[23][0]=1.0f; vertices[23][1]=-1.0f; vertices[23][2]=1.0f;

    /* Drawing Using Triangle strips, draw triangle strips using 4 vertices */
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);

    /* Disable COLOR and VERTEX arrays */
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    /* Flush all drawings */
    glFinish();

    /* Increase The Rotation Variable For The Triangle ( NEW ) */
    rtri+=0.6f;
    /* Decrease The Rotation Variable For The Quad     ( NEW ) */
    rquad-=0.4f;

}

void resize(int w, int h)
{
	
	/*change to projection matrix*/
    	glMatrixMode(GL_PROJECTION);
	/*reset the projection matrix*/
    	glLoadIdentity();
	/*set the viewport*/
    	glViewport(0, 0, w, h);

	GLUmat4 perspective;
	/*use glu to set perspective*/
    	gluPerspective4f(&perspective, 45.0f,((GLfloat)w/(GLfloat)h), 1.0f, 100.0f);
	glMultMatrixf(&perspective.col[0].values[0]);
	
	/*get back to model view matrix*/
    	glMatrixMode(GL_MODELVIEW);
	/*reset modevl view matrix*/
    	glLoadIdentity();


}

/* general OpenGL initialization function */
int init(void)
{
    int w = 640;
    int h = 480;

    static const EGLint s_configAttribs[] =
    {
	EGL_RED_SIZE,		5,
	EGL_GREEN_SIZE, 	6,
	EGL_BLUE_SIZE,		5,
	EGL_ALPHA_SIZE, 	0,
      	EGL_SAMPLES, 		0,
      	EGL_NONE
    };

    EGLint numconfigs;

	//get egl display
	eglNativeDisplayType = fsl_getNativeDisplay();
	egldisplay = eglGetDisplay(eglNativeDisplayType);
	//Initialize egl
	eglInitialize(egldisplay, NULL, NULL);
	assert(eglGetError() == EGL_SUCCESS);
	//tell the driver we are using OpenGL ES
	eglBindAPI(EGL_OPENGL_ES_API);

    //pass our egl configuration to egl
    eglChooseConfig(egldisplay, s_configAttribs, &eglconfig, 1, &numconfigs);
    printf("chooseconfig, \n");
    assert(eglGetError() == EGL_SUCCESS);
    assert(numconfigs == 1);
    /* Enable smooth shading */
    glShadeModel(GL_SMOOTH);

	eglNativeWindow = fsl_createwindow(egldisplay, eglNativeDisplayType);	
	assert(eglNativeWindow);	

    eglsurface = eglCreateWindowSurface(egldisplay, eglconfig, (EGLNativeWindowType) eglNativeWindow, NULL);

    printf("createwindow, \n");
    assert(eglGetError() == EGL_SUCCESS);

    //create the egl graphics context
    eglcontext = eglCreateContext(egldisplay, eglconfig, NULL, NULL);
    printf("creatcontext, \n");
    assert(eglGetError() == EGL_SUCCESS);

    //make the context current
    eglMakeCurrent(egldisplay, eglsurface, eglsurface, eglcontext);
    printf("makecurrent, \n");
    assert(eglGetError() == EGL_SUCCESS);

    /* Enable smooth shading */
    glShadeModel(GL_SMOOTH);
    printf("sade model, \n");
    assert(glGetError() == GL_NO_ERROR);

    /* Set the background black */
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    printf("clear color, \n");
    assert(glGetError() == GL_NO_ERROR);

    /* Depth buffer setup */
    glClearDepthf(1.0f);
    printf("clear depth, \n");
    assert(glGetError() == GL_NO_ERROR);

    /* Enables Depth Testing */
    glEnable(GL_DEPTH_TEST);
    printf("depth test, \n");
    assert(glGetError() == GL_NO_ERROR);

    /* The Type Of Depth Test To Do */

    glDepthFunc(GL_LEQUAL);
    printf("depth func, \n");
    assert(glGetError() == GL_NO_ERROR);

    /*enable cullface*/
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    printf("back culling, \n");
    assert(glGetError() == GL_NO_ERROR);

    /* Really Nice Perspective Calculations */
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    printf("hint perspective, \n");
    assert(glGetError() == GL_NO_ERROR);

    /*get width and height from egl*/
    eglQuerySurface(egldisplay, eglsurface, EGL_WIDTH, &w);
    eglQuerySurface(egldisplay, eglsurface, EGL_HEIGHT, &h);
	
    /* Scale the content to the window */
    resize(w,h);

    return 1;
}

void deinit(void)
{
	printf("Cleaning up...\n");
	eglMakeCurrent(egldisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	assert(eglGetError() == EGL_SUCCESS);
	eglDestroyContext(egldisplay, eglcontext);
	eglDestroySurface(egldisplay, eglsurface);	
	fsl_destroywindow(eglNativeWindow, eglNativeDisplayType);
	eglTerminate(egldisplay);
	assert(eglGetError() == EGL_SUCCESS);
	eglReleaseThread();	
}

void sighandler(int signal)
{
	printf("Caught signal %d, setting flaq to quit.\n", signal);
	quit = 1;
}

int main (void)
{
	printf("Lesson 05 \n");

	signal(SIGINT, sighandler);
	signal(SIGTERM, sighandler);

	assert( init() );

	while (!quit)
	{
		render();
		eglSwapBuffers(egldisplay, eglsurface);
	}

	deinit();

	return 1;
}
