/* This code was created by Jeff Molofee '99
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

/* Whether or not lighting is on */
int 				light = 1;

/* Perform blend or not */
int				blend = 1;

GLfloat xrot;         /* X Rotation */
GLfloat yrot;         /* Y Rotation */
GLfloat zrot;         /* Y Rotation */
GLfloat xspeed=0.25f; /* X Rotation Speed */
GLfloat yspeed=0.25f; /* Y Rotation Speed */
GLfloat zspeed=0.05f; /* Y Rotation Speed */
GLfloat z=-6.0f;      /* Depth Into The Screen */
GLfloat alpha=0.01f;      /* Depth Into The Screen */
GLfloat aspeed=0.05f; /* Y Rotation Speed */

/* Ambient Light Values (NEW) */
GLfloat LightAmbient[]={0.5f, 0.5f, 0.5f, 1.0f};
/* Diffuse Light Values (NEW) */
GLfloat LightDiffuse[]={1.0f, 1.0f, 1.0f, 1.0f};
/* Light Position (NEW) */
GLfloat LightPosition[]={0.0f, 0.0f, 2.0f, 1.0f};

/* Default OpenGL ES material settings */
GLfloat box_material_amb[4]={0.2f, 0.2f, 0.2f, 0.2f};
GLfloat box_material_dif[4]={0.8f, 0.8f, 0.8f, 0.8f};

GLuint filter=0;   /* Which Filter To Use */
GLuint texture[3]; /* Storage for 3 textures */

/* function to load in bitmap as a GL texture */
int LoadGLTextures()
{
    Image *image1;

    // allocate space for texture we will use
    image1 = (Image *) malloc(sizeof(Image));
    if (image1 == NULL) 
    {
	printf("Error allocating space for image");
	return 0;
    }	

    if (LoadBMP( "Bmp.bmp", image1))  
    {
        /* Create The Textures */
        glGenTextures(3, &texture[0]);

        /* Load in texture 1 */
        /* Typical Texture Generation Using Data From The Bitmap */
        glBindTexture(GL_TEXTURE_2D, texture[0]);

        /* Generate The Texture */
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,image1->sizeX,image1->sizeY,0,GL_RGB,GL_UNSIGNED_BYTE,image1->data);

        /* Nearest Filtering */
        glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        /* Load in texture 2 */
        /* Typical Texture Generation Using Data From The Bitmap */
        glBindTexture(GL_TEXTURE_2D, texture[1]);

        /* Linear Filtering */
        glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        /* Generate The Texture */
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,image1->sizeX,image1->sizeY,0,GL_RGB,GL_UNSIGNED_BYTE,image1->data);

        /* Load in texture 3 using mipmaps */
        glBindTexture(GL_TEXTURE_2D, texture[2]);
        glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        /*gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB,
                          image1->sizeX, image1->sizeY,
                          GL_RGB, GL_UNSIGNED_BYTE, image1->data);*/
        
    }else return 0;

    /* Free up any memory we may have used */
    if (image1 != NULL)
    {
        free(image1);
    }

    return 1;
}

/* Here goes our drawing code */
int render()
{
    GLfloat texcoords[4][2];
    GLfloat vertices[4][3];
    GLubyte indices[4]={1, 0, 2, 3}; /* QUAD to TRIANGLE_STRIP conversion; */

    /* Clear The Screen And The Depth Buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);

    /* Reset the view */
    glLoadIdentity();

    /* Translate Into/Out Of The Screen By z */
    glTranslatef(0.0f, 0.0f, z);

    glRotatef(xrot, 1.0f, 0.0f, 0.0f); /* Rotate On The X Axis By xrot */
    glRotatef(yrot, 0.0f, 1.0f, 0.0f); /* Rotate On The Y Axis By yrot */

    /* Select A Texture Based On filter */
    glBindTexture(GL_TEXTURE_2D, texture[filter]);

    /* Set pointers to vertices and texcoords */
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, texcoords);

    /* Enable vertices and texcoords arrays */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    /* Front Face */
    /* Normal Pointing Towards Viewer */
    glNormal3f(0.0f, 0.0f, 1.0f);

    /* Point 1 (Front) */
    texcoords[0][0]=1.0f; texcoords[0][1]=0.0f;
    vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=1.0f;
    /* Point 2 (Front) */
    texcoords[1][0]=0.0f; texcoords[1][1]=0.0f;
    vertices[1][0]=1.0f;  vertices[1][1]=-1.0f; vertices[1][2]=1.0f;
    /* Point 3 (Front) */
    texcoords[2][0]=0.0f; texcoords[2][1]=1.0f;
    vertices[2][0]=1.0f;  vertices[2][1]=1.0f;  vertices[2][2]=1.0f;
    /* Point 4 (Front) */
    texcoords[3][0]=1.0f; texcoords[3][1]=1.0f;
    vertices[3][0]=-1.0f; vertices[3][1]=1.0f;  vertices[3][2]=1.0f;

    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    /* Draw one textured plane using two stripped triangles */
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

    /* Back Face */
    /* Normal Pointing Away From Viewer */
    glNormal3f(0.0f, 0.0f, -1.0f);

    /* Point 1 (Back) */
    texcoords[0][0]=0.0f; texcoords[0][1]=0.0f;
    vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=-1.0f;
    /* Point 2 (Back) */
    texcoords[1][0]=0.0f; texcoords[1][1]=1.0f;
    vertices[1][0]=-1.0f; vertices[1][1]=1.0f;  vertices[1][2]=-1.0f;
    /* Point 3 (Back) */
    texcoords[2][0]=1.0f; texcoords[2][1]=1.0f;
    vertices[2][0]=1.0f;  vertices[2][1]=1.0f;  vertices[2][2]=-1.0f;
    /* Point 4 (Back) */
    texcoords[3][0]=1.0f; texcoords[3][1]=0.0f;
    vertices[3][0]=1.0f;  vertices[3][1]=-1.0f; vertices[3][2]=-1.0f;

    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    /* Draw one textured plane using two stripped triangles */
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

    /* Top Face */
    /* Normal Pointing Up */
    glNormal3f(0.0f, 1.0f, 0.0f);

    /* Point 1 (Top) */
    texcoords[0][0]=1.0f; texcoords[0][1]=1.0f;
    vertices[0][0]=-1.0f; vertices[0][1]=1.0f; vertices[0][2]=-1.0f;
    /* Point 2 (Top) */
    texcoords[1][0]=1.0f; texcoords[1][1]=0.0f;
    vertices[1][0]=-1.0f; vertices[1][1]=1.0f; vertices[1][2]=1.0f;
    /* Point 3 (Top) */
    texcoords[2][0]=0.0f; texcoords[2][1]=0.0f;
    vertices[2][0]=1.0f;  vertices[2][1]=1.0f; vertices[2][2]=1.0f;
    /* Point 4 (Top) */
    texcoords[3][0]=0.0f; texcoords[3][1]=1.0f;
    vertices[3][0]=1.0f;  vertices[3][1]=1.0f; vertices[3][2]=-1.0f;

    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    /* Draw one textured plane using two stripped triangles */
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

    /* Bottom Face */
    /* Normal Pointing Down */
    glNormal3f(0.0f, -1.0f, 0.0f);

    /* Point 1 (Bottom) */
    texcoords[0][0]=0.0f; texcoords[0][1]=1.0f;
    vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=-1.0f;
    /* Point 2 (Bottom) */
    texcoords[1][0]=1.0f; texcoords[1][1]=1.0f;
    vertices[1][0]=1.0f;  vertices[1][1]=-1.0f; vertices[1][2]=-1.0f;
    /* Point 3 (Bottom) */
    texcoords[2][0]=1.0f; texcoords[2][1]=0.0f;
    vertices[2][0]=1.0f;  vertices[2][1]=-1.0f; vertices[2][2]=1.0f;
    /* Point 4 (Bottom) */
    texcoords[3][0]=0.0f; texcoords[3][1]=0.0f;
    vertices[3][0]=-1.0f; vertices[3][1]=-1.0f; vertices[3][2]=1.0f;

    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    /* Draw one textured plane using two stripped triangles */
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

    /* Right face */
    /* Normal Pointing Right */
    glNormal3f(1.0f, 0.0f, 0.0f);

      /* Point 1 (Right) */
    texcoords[0][0]=0.0f; texcoords[0][1]=0.0f;
    vertices[0][0]=1.0f;  vertices[0][1]=-1.0f; vertices[0][2]=-1.0f;
    /* Point 2 (Right) */
    texcoords[1][0]=0.0f; texcoords[1][1]=1.0f;
    vertices[1][0]=1.0f;  vertices[1][1]=1.0f;  vertices[1][2]=-1.0f;
    /* Point 3 (Right) */
    texcoords[2][0]=1.0f; texcoords[2][1]=1.0f;
    vertices[2][0]=1.0f;  vertices[2][1]=1.0f;  vertices[2][2]=1.0f;
    /* Point 4 (Right) */
    texcoords[3][0]=1.0f; texcoords[3][1]=0.0f;
    vertices[3][0]=1.0f;  vertices[3][1]=-1.0f; vertices[3][2]=1.0f;

    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    /* Draw one textured plane using two stripped triangles */
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

    /* Left Face*/
    /* Normal Pointing Left */
    glNormal3f(-1.0f, 0.0f, 0.0f);

    /* Point 1 (Left) */
    texcoords[0][0]=1.0f; texcoords[0][1]=0.0f;
    vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=-1.0f;
    /* Point 2 (Left) */
    texcoords[1][0]=0.0f; texcoords[1][1]=0.0f;
    vertices[1][0]=-1.0f; vertices[1][1]=-1.0f; vertices[1][2]=1.0f;
      /* Point 3 (Left) */
    texcoords[2][0]=0.0f; texcoords[2][1]=1.0f;
    vertices[2][0]=-1.0f; vertices[2][1]=1.0f;  vertices[2][2]=1.0f;
      /* Point 4 (Left) */
    texcoords[3][0]=1.0f; texcoords[3][1]=1.0f;
    vertices[3][0]=-1.0f; vertices[3][1]=1.0f;  vertices[3][2]=-1.0f;

    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Draw one textured plane using two stripped triangles */
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

    /* Disable texcoords and vertices arrays */
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glFinish();

    xrot+=xspeed; /* Add xspeed To xrot */
    yrot+=yspeed; /* Add yspeed To yrot */
    //z+=zspeed; /* Add yspeed To zrot */

    alpha+=aspeed;

    return 1;
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

    /* Load in the texture */
    if (!LoadGLTextures())
    {
        return 0;
    }

    /* Enable Texture Mapping (NEW) */
    glEnable(GL_TEXTURE_2D);

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

    /*enable cullface*/
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );

    /* Really Nice Perspective Calculations */
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    /* Setup The Ambient Light */
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);

    /* Setup The Diffuse Light */
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);

    /* Position The Light */
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);

    /* Enable light */
    glEnable(GL_LIGHT1);

    /* Enable/Disable Lighting */
    if (!light)
    {
        glDisable(GL_LIGHTING);
    }
    else
    {
        glEnable(GL_LIGHTING);
    }

    if (blend)
    {
       glEnable(GL_BLEND);
       glDisable(GL_DEPTH_TEST);
       glDisable( GL_CULL_FACE );
    }
    else
    {
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
	glEnable( GL_CULL_FACE );
    }


    /* Full Brightness, 50% Alpha (NEW) */
    glColor4f(1.0f, 1.0f, 1.0f, 0.0f);

    /* Blending Function For Translucency Based On Source Alpha Value  */
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, box_material_amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, box_material_dif);

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
	printf("Lesson 08 \n");

	assert( init() );

	while (!quit)
	{
		render();
		eglSwapBuffers(egldisplay, eglsurface);
	}

	deinit();

	return 1;
}

