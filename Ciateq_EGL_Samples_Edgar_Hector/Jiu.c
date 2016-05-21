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
#include "FSL/fslutil.h"

EGLDisplay			egldisplay;
EGLConfig			eglconfig;
EGLSurface			eglsurface;
EGLContext			eglcontext;
EGLNativeWindowType eglNativeWindow;
EGLNativeDisplayType eglNativeDisplayType;
volatile sig_atomic_t quit = 0;

float xrot = 0.0f;
float yrot = 0.0f;
float yval = 1.0f;
int yup = 1;

GLfloat floorVertices[] =
{
    -3.0f, 0.0f, 3.0f,
     3.0f, 0.0f, 3.0f,
    -3.0f, 0.0f,-3.0f,
     3.0f, 0.0f,-3.0f
};

GLfloat box[] =
{
    /* FRONT */
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    /* BACK */
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    /* LEFT */
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    /* RIGHT */
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    /* TOP */
    -0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    /* BOTTOM */
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f, -0.5f
};

GLfloat boxColors[] =
{
    /* FRONT */
    0.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    /* BACK */
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f,
    /* LEFT */
    0.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    /* RIGHT */
    0.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    /* TOP */
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f,
    /* BOTTOM */
    0.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f
};

/* function to release/destroy our resources and restoring the old desktop */
void Cleanup()
{

}

/* general EGL/OpenGL initialization function */
int init(void)
{

    static const EGLint s_configAttribs[] =
    {
	EGL_RED_SIZE,		5,
	EGL_GREEN_SIZE, 	6,
	EGL_BLUE_SIZE,		5,
	EGL_ALPHA_SIZE, 	0,
	EGL_STENCIL_SIZE,	1,
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

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepthf(1.0f);

    glClearStencil(0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glColorPointer(4, GL_FLOAT, 0, boxColors);

    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);

    /*set perspective*/
    EGLint h, w;
    /*get width and height from egl*/
    eglQuerySurface(egldisplay, eglsurface, EGL_WIDTH, &w);
    eglQuerySurface(egldisplay, eglsurface, EGL_HEIGHT, &h);

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

    return 1;
}

void drawCube()
{
    glPushMatrix();

    glEnableClientState(GL_COLOR_ARRAY);

    glColor4f(1.0f, 0.5f, 0.7f, 1.0f);

    glTranslatef(0.0f, yval, 0.0f);

    glRotatef(xrot, 1.0f, 0.0f, 0.0f);
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);

    glVertexPointer(3, GL_FLOAT, 0, box);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);

    glDisableClientState(GL_COLOR_ARRAY);

    glPopMatrix();
}

void display_x()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();


    GLUmat4 lookat;
    GLUvec4 eye;
    GLUvec4 center;
    GLUvec4 up;
    eye.values[0] = 0.0f;
    eye.values[1] = 3.0f;
    eye.values[2] = 7.0f;
    eye.values[3] = 1.0f;
    center.values[0] = 0.0f;
    center.values[1] = 0.0f;
    center.values[2] = 0.0f;
    center.values[3] = 1.0f;
    up.values[0] = 0.0f;
    up.values[1] = 1.0f;
    up.values[2] = 0.0f;
    up.values[3] = 1.0f;
    /*use glu to set perspective*/
    gluLookAt4v(&lookat, &eye, &center, &up);
    glMultMatrixf(&lookat.col[0].values[0]);

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);

    glEnable(GL_STENCIL_TEST);

    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 1);

    /* Floor */
    glVertexPointer(3, GL_FLOAT, 0, floorVertices);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);

    glStencilFunc(GL_EQUAL, 1, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    /* Cube */
    glPushMatrix();
    glScalef(1.0f, -1.0f, 1.0f);
    glCullFace(GL_FRONT);
    drawCube();
    glCullFace(GL_BACK);
    glPopMatrix();

    glDisable(GL_STENCIL_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Floor */
    glVertexPointer(3, GL_FLOAT, 0, floorVertices);
    glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisable(GL_BLEND);

    drawCube();

    glFlush();
}

void idle()
{
    xrot += 2.0f;
    yrot += 3.0f;

    yval += yup ? 0.02f : -0.02f;

    if (yval > 2.0f)
    {
        yup = 0;
    }
    if (yval < 1.0f)
    {
        yup = 1;
    }
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

/* Here goes our drawing code */
int render()
{
 
    idle();
    display_x();

    return 1;
}

void deinit(void)
{
	//call clean up to release any memory allocated
	Cleanup();
	//Make a empty surface current
	eglMakeCurrent(egldisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	assert(eglGetError() == EGL_SUCCESS);
	//destroy egl desplay
	eglTerminate(egldisplay);
	assert(eglGetError() == EGL_SUCCESS);
	//end egl thread
	eglReleaseThread();
}

void sighandler(int signal)
{
	printf("Caught signal %d, setting flaq to quit.\n", signal);
	quit = 1;
}

int main(void)
{
	signal(SIGINT, sighandler);
	signal(SIGTERM, sighandler);

    	assert( init() );

	while (!quit)
	{
		EGLint width = 0;
		EGLint height = 0;
		eglQuerySurface(egldisplay, eglsurface, EGL_WIDTH, &width);
		eglQuerySurface(egldisplay, eglsurface, EGL_HEIGHT, &height);
		render();
		eglSwapBuffers(egldisplay, eglsurface);
	}
	
    deinit();
    return 1;
}
