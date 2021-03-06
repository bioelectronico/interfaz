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

#include "common/inc/GLU3/glu3.h"
#include "GLES/gl.h"

#include "EGL/egl.h"
#include "common/inc/FSL/fsl_egl.h"

EGLDisplay			egldisplay;
EGLConfig			eglconfig;
EGLSurface			eglsurface;
EGLContext			eglcontext;
EGLNativeWindowType eglNativeWindow;
EGLNativeDisplayType eglNativeDisplayType;
volatile sig_atomic_t quit = 0;

void render()
{
	
//Top
	const GLfloat triVertices [] = { 0.0f, 1.0f, 0.0f, 
					   -1.0f, 0.0f, 0.0f, 
					   1.0f, 0.0f, 0.0f };

	const GLfloat triColors [] = { 1.0f, 0.0f, 0.0f, 
				       1.0f, 0.0f, 0.0f, 
				       1.0f, 0.0f, 0.0f };
	/*
	const GLfloat triColors [] = { 1.0f, 0.0f, 0.0f, 
				       0.0f, 1.0f, 0.0f, 
				        0.0f, 0.0f, 1.0f };
	*/				

	/* Clear The Screen And The Depth Buffer */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Enable VERTEX array */
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	/* Setup pointer to  VERTEX array */
	glColorPointer(4, GL_FLOAT, 0, triColors);
	glVertexPointer(3, GL_FLOAT, 0, triVertices);

	/* Move Left 1.5 Units And Into The Screen 6.0 */
	glLoadIdentity();
	glTranslatef(0.5f, 0.0f, -6.0f);
	
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	/* Disable vertex array */
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	/* Flush all drawings */
	glFinish();
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
	// printf("chooseconfig, \n");
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

//--------------------------------------------
    	/* Set the background black */
    	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    	/* Depth buffer setup */
    	glClearDepthf(1.0f);

    	/* Enables Depth Testing */
    //	glEnable(GL_DEPTH_TEST);

    	/* The Type Of Depth Test To Do */
    //	glDepthFunc(GL_LEQUAL);

    	/* Really Nice Perspective Calculations */
   // 	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	/*get width and height from egl*/
	eglQuerySurface(egldisplay, eglsurface, EGL_WIDTH, &w);
	eglQuerySurface(egldisplay, eglsurface, EGL_HEIGHT, &h);
	
	/* Scale the content to the window */
	resize(w,h);

	printf("exiting init, \n");

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


int main (void)
{
	printf("Lesson 01 \n");

	init(); 
	while (!quit)
	{
		render();
		eglSwapBuffers(egldisplay, eglsurface);
	}

	deinit();

	return 1;
}

