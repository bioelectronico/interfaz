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
//#include "GL/glut.h"

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

//static GLfloat spin= 0.0;

void render()
{
    /* rotational vars for the triangle and quad, respectively */
    static GLfloat rtri, rquad;

    GLfloat colors[4][4];
    GLfloat vertices[4][3];

    /* Clear The Screen And The Depth Buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Enable COLOR and VERTEX arrays */
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    /* Setup pointers to COLOR and VERTEX arrays */
    glColorPointer(4, GL_FLOAT, 0, colors);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    /* Move Left 1.5 Units And Into The Screen 6.0 */
    glLoadIdentity();
    glTranslatef(-1.5f, 0.0f, -6.0f);

    /* Rotate The Triangle On The Z axis ( NEW ) */
    glRotatef(rtri, 0.0f, 0.0f, 1.0f);

    /* Red */
    colors[0][0]=1.0f; colors[0][1]=0.0f; colors[0][2]=0.0f; colors[0][3]=1.0f;
    /* Top Of Triangle */
    vertices[0][0]=0.0f; vertices[0][1]=1.0f; vertices[0][2]=0.0f;
    /* Green */
    colors[1][0]=0.0f; colors[1][1]=1.0f; colors[1][2]=0.0f; colors[1][3]=1.0f;
    /* Left Of Triangle */
    vertices[1][0]=-1.0f; vertices[1][1]=-1.0f; vertices[1][2]=0.0f;
    /* Blue */
    colors[2][0]=0.0f; colors[2][1]=0.0f; colors[2][2]=1.0f; colors[2][3]=1.0f;
    /* Right Of Triangle */
    vertices[2][0]=1.0f; vertices[2][1]=-1.0f; vertices[2][2]=0.0f;

    /* Drawing Using Triangles, draw triangles using 3 vertices */
    glDrawArrays(GL_TRIANGLES, 0, 3);

    /* Move Right 3 Units */
    glLoadIdentity();
    glTranslatef(1.5f, 0.0f, -6.0f);

    /* Rotate The Quad On The Y axis ( NEW ) */
    glRotatef(rquad, 0.0f, 1.0f, 0.0f);

    /* Set The Color To Blue One Time Only */
    glColor4f(0.5f, 0.5f, 1.0f, 1.0f);

    /* Disable vertex color array, use global color */
    glDisableClientState(GL_COLOR_ARRAY);

    /* Top Right Of The Quad    */
    vertices[0][0]=1.0f;  vertices[0][1]=1.0f;  vertices[0][2]=0.0f;
    /* Top Left Of The Quad     */
    vertices[1][0]=-1.0f; vertices[1][1]=1.0f;  vertices[1][2]=0.0f;
    /* Bottom Left Of The Quad  */
    vertices[2][0]=1.0f;  vertices[2][1]=-1.0f; vertices[2][2]=0.0f;
    /* Bottom Right Of The Quad */
    vertices[3][0]=-1.0f; vertices[3][1]=-1.0f; vertices[3][2]=0.0f;

    /* Drawing using triangle strips, draw triangles using 4 vertices */
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    /* Disable vertex array */
    glDisableClientState(GL_VERTEX_ARRAY);

    /* Flush all drawings */
    glFinish();

    /* Increase The Rotation Variable For The Triangle ( NEW ) */
    rtri+=0.2f;
    /* Decrease The Rotation Variable For The Quad     ( NEW ) */
    rquad-=0.15f;
}


/*void spinDisplay(void) 
{
  spin= spin +2.0;
  if(spin > 360.0)
	spin=spin-360.0;

}

void mouse(int button, int state, int x, int y)
{
	switch(button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN)
				glutIdleFunc(spinDisplay);
			break;
		case GLUT_MIDDLE_BUTTON:
			if (state == GLUT_DOWN)
				glutIdleFunc(NULL);
			break;
		default:
			break;


	}


}

*/




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

    	/* Set the background black */
    	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    	/* Depth buffer setup */
    	glClearDepthf(1.0f);

    	/* Enables Depth Testing */
    	glEnable(GL_DEPTH_TEST);

    	/* The Type Of Depth Test To Do */
    	glDepthFunc(GL_LEQUAL);

    	/* Really Nice Perspective Calculations */
    	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

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

int main (int argc, char** argv)
{
	printf("Lesson 04 \n");
	glutInit(&argc, argv);

	signal(SIGINT, sighandler);
	signal(SIGTERM, sighandler);

	assert( init() );

	while (!quit)
	{
		render();
//		glutMouseFunc(mouse);
		eglSwapBuffers(egldisplay, eglsurface);
	}

	deinit();

	return 1;
}
