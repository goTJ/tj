#include <stdio.h>    /* for printf and NULL */
#include <stdlib.h>   /* for exit */
#include <math.h>     /* for sqrt, sin, and cos */
#include <assert.h>   /* for assert */
#if __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <Cg/cg.h>    /* Can't include this?  Is Cg Toolkit installed! */
#include <Cg/cgGL.h>
#include "TRIModel.h"

#define M_PI 3.14159265358979323846

// Can't declare NULL
struct _CGcontext{ char a; };
struct _CGprogram{ char a; };
struct _CGparameter{ char a; };


static CGcontext   myCgContext;
static CGprofile   myCgVertexProfile,
                   myCgFragmentProfile;
static CGprogram   myCgVertexProgram,
                   myCgFragmentProgram;
static CGparameter myCgVertexParam_modelViewProj,
                   myCgFragmentParam_LR,
                   myCgFragmentParam_LG,
                   myCgFragmentParam_LB;

static const char *myProgramName = "PRT",
                  *myVertexProgramFileName = "prt_vertex.cg",
                  *myVertexProgramName = "prt_vertex",
                  *myFragmentProgramFileName = "prt_fragment.cg",
                  *myFragmentProgramName = "prt_fragment";

static float myEyeAngle = 0;   /* Angle eye rotates around scene. */
static float myProjectionMatrix[16];
TRIModel triModel;
static float lr[16], lg[16], lb[16];

static void checkForCgError(const char *situation)
{
  CGerror error;
  const char *string = cgGetLastErrorString(&error);

  if (error != CG_NO_ERROR) {
    printf("%s: %s: %s\n",
      myProgramName, situation, string);
    if (error == CG_COMPILER_ERROR) {
      printf("%s\n", cgGetLastListing(myCgContext));
    }
    exit(1);
  }
}

/* Forward declared GLUT callbacks registered by main. */
static void reshape(int width, int height);
static void display(void);
static void keyboard(unsigned char c, int x, int y);
static void menu(int item);

int main(int argc, char **argv)
{
	FILE *fin=fopen("light.txt", "r");
	if(fin == NULL)
		exit(1);
	int i;
	for(i=0; i<16; i++){
		fscanf(fin, "%f", lr+i);
	}
	for(i=0; i<16; i++){
		fscanf(fin, "%f", lg+i);
	}
	for(i=0; i<16; i++){
		fscanf(fin, "%f", lb+i);
	}
	triModel.loadFromFile("pot.tri");

	glutInitWindowSize(400, 400);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInit(&argc, argv);

	glutCreateWindow(myProgramName);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);

	glClearColor(0.1, 0.1, 0.1, 0);  /* Gray background. */
	glEnable(GL_DEPTH_TEST);         /* Hidden surface removal. */

	myCgContext = cgCreateContext();
	checkForCgError("creating context");

	// Set vertex program
	myCgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
	cgGLSetOptimalOptions(myCgVertexProfile);
	checkForCgError("selecting vertex profile");

	myCgVertexProgram =
	cgCreateProgramFromFile(
		myCgContext,              /* Cg runtime context */
		CG_SOURCE,                /* Program in human-readable form */
		myVertexProgramFileName,  /* Name of file containing program */
		myCgVertexProfile,        /* Profile: OpenGL ARB vertex program */
		myVertexProgramName,      /* Entry function name */
		NULL);                    /* No extra comM_PIler options */
	checkForCgError("creating vertex program from file");
	cgGLLoadProgram(myCgVertexProgram);
	checkForCgError("loading vertex program");

	myCgVertexParam_modelViewProj =
	cgGetNamedParameter(myCgVertexProgram, "modelViewProj");
	checkForCgError("could not get modelViewProj parameter");
	// Set fragment program
	myCgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
	cgGLSetOptimalOptions(myCgFragmentProfile);
	checkForCgError("selecting fragment profile");

	myCgFragmentProgram =
	cgCreateProgramFromFile(
		myCgContext,              /* Cg runtime context */
		CG_SOURCE,                /* Program in human-readable form */
		myFragmentProgramFileName,
		myCgFragmentProfile,      /* Profile: latest fragment profile */
		myFragmentProgramName,                   /* Entry function name */
		NULL); /* No extra comM_PIler options */
	checkForCgError("creating fragment program from string");
	cgGLLoadProgram(myCgFragmentProgram);
	checkForCgError("loading fragment program");

	myCgFragmentParam_LR =
	cgGetNamedParameter(myCgFragmentProgram, "LR");
	checkForCgError("could not get LR parameter");
	myCgFragmentParam_LG =
	cgGetNamedParameter(myCgFragmentProgram, "LG");
	checkForCgError("could not get LG parameter");
	myCgFragmentParam_LB =
	cgGetNamedParameter(myCgFragmentProgram, "LB");
	checkForCgError("could not get LB parameter");

	glutCreateMenu(menu);
	glutAddMenuEntry("[ ] Animate", ' ');
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
	return 0;
}

/* Forward declared routine used by reshape callback. */
static void buildPerspectiveMatrix(double, double, double, double, float m[16]);
static void reshape(int width, int height)
{
  double aspectRatio = (float) width / (float) height;
  double fieldOfView = 40.0; /* Degrees */

  /* Build projection matrix once. */
  buildPerspectiveMatrix(fieldOfView, aspectRatio,
                         1.0, 20.0,  /* Znear and Zfar */
                         myProjectionMatrix);
  glViewport(0, 0, width, height);
}
static void buildPerspectiveMatrix(double fieldOfView,
                                   double aspectRatio,
                                   double zNear, double zFar,
                                   float m[16])
{
  double sine, cotangent, deltaZ;
  double radians = fieldOfView / 2.0 * M_PI / 180.0;
  
  deltaZ = zFar - zNear;
  sine = sin(radians);
  /* Should be non-zero to avoid division by zero. */
  assert(deltaZ);
  assert(sine);
  assert(aspectRatio);
  cotangent = cos(radians) / sine;
  
  m[0*4+0] = cotangent / aspectRatio;
  m[1*4+0] = 0.0;
  m[2*4+0] = 0.0;
  m[3*4+0] = 0.0;
  
  m[0*4+1] = 0.0;
  m[1*4+1] = cotangent;
  m[2*4+1] = 0.0;
  m[3*4+1] = 0.0;
  
  m[0*4+2] = 0.0;
  m[1*4+2] = 0.0;
  m[2*4+2] = -(zFar + zNear) / deltaZ;
  m[3*4+2] = -2 * zNear * zFar / deltaZ;
  
  m[0*4+3] = 0.0;
  m[1*4+3] = 0.0;
  m[2*4+3] = -1;
  m[3*4+3] = 0;
}

/* Build a row-major (C-style) 4x4 matrix transform based on the
   parameters for gluLookAt. */
static void buildLookAtMatrix(double eyex, double eyey, double eyez,
                              double centerx, double centery, double centerz,
                              double upx, double upy, double upz,
                              float m[16])
{
  double x[3], y[3], z[3], mag;

  /* Difference eye and center vectors to make Z vector. */
  z[0] = eyex - centerx;
  z[1] = eyey - centery;
  z[2] = eyez - centerz;
  /* Normalize Z. */
  mag = sqrt(z[0]*z[0] + z[1]*z[1] + z[2]*z[2]);
  if (mag) {
    z[0] /= mag;
    z[1] /= mag;
    z[2] /= mag;
  }

  /* Up vector makes Y vector. */
  y[0] = upx;
  y[1] = upy;
  y[2] = upz;

  /* X vector = Y cross Z. */
  x[0] =  y[1]*z[2] - y[2]*z[1];
  x[1] = -y[0]*z[2] + y[2]*z[0];
  x[2] =  y[0]*z[1] - y[1]*z[0];

  /* Recompute Y = Z cross X. */
  y[0] =  z[1]*x[2] - z[2]*x[1];
  y[1] = -z[0]*x[2] + z[2]*x[0];
  y[2] =  z[0]*x[1] - z[1]*x[0];

  /* Normalize X. */
  mag = sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);
  if (mag) {
    x[0] /= mag;
    x[1] /= mag;
    x[2] /= mag;
  }

  /* Normalize Y. */
  mag = sqrt(y[0]*y[0] + y[1]*y[1] + y[2]*y[2]);
  if (mag) {
    y[0] /= mag;
    y[1] /= mag;
    y[2] /= mag;
  }

  /* Build resulting view matrix. */
  m[0*4+0] = x[0];  m[0*4+1] = x[1];
  m[0*4+2] = x[2];  m[0*4+3] = -x[0]*eyex + -x[1]*eyey + -x[2]*eyez;

  m[1*4+0] = y[0];  m[1*4+1] = y[1];
  m[1*4+2] = y[2];  m[1*4+3] = -y[0]*eyex + -y[1]*eyey + -y[2]*eyez;

  m[2*4+0] = z[0];  m[2*4+1] = z[1];
  m[2*4+2] = z[2];  m[2*4+3] = -z[0]*eyex + -z[1]*eyey + -z[2]*eyez;

  m[3*4+0] = 0.0;   m[3*4+1] = 0.0;  m[3*4+2] = 0.0;  m[3*4+3] = 1.0;
}

static void makeRotateMatrix(float angle,
                             float ax, float ay, float az,
                             float m[16])
{
  float radians, sine, cosine, ab, bc, ca, tx, ty, tz;
  float axis[3];
  float mag;

  axis[0] = ax;
  axis[1] = ay;
  axis[2] = az;
  mag = sqrt(axis[0]*axis[0] + axis[1]*axis[1] + axis[2]*axis[2]);
  if (mag) {
    axis[0] /= mag;
    axis[1] /= mag;
    axis[2] /= mag;
  }

  radians = angle * M_PI / 180.0;
  sine = sin(radians);
  cosine = cos(radians);
  ab = axis[0] * axis[1] * (1 - cosine);
  bc = axis[1] * axis[2] * (1 - cosine);
  ca = axis[2] * axis[0] * (1 - cosine);
  tx = axis[0] * axis[0];
  ty = axis[1] * axis[1];
  tz = axis[2] * axis[2];

  m[0]  = tx + cosine * (1 - tx);
  m[1]  = ab + axis[2] * sine;
  m[2]  = ca - axis[1] * sine;
  m[3]  = 0.0f;
  m[4]  = ab - axis[2] * sine;
  m[5]  = ty + cosine * (1 - ty);
  m[6]  = bc + axis[0] * sine;
  m[7]  = 0.0f;
  m[8]  = ca + axis[1] * sine;
  m[9]  = bc - axis[0] * sine;
  m[10] = tz + cosine * (1 - tz);
  m[11] = 0;
  m[12] = 0;
  m[13] = 0;
  m[14] = 0;
  m[15] = 1;
}

static void makeTranslateMatrix(float x, float y, float z, float m[16])
{
  m[0]  = 1;  m[1]  = 0;  m[2]  = 0;  m[3]  = x;
  m[4]  = 0;  m[5]  = 1;  m[6]  = 0;  m[7]  = y;
  m[8]  = 0;  m[9]  = 0;  m[10] = 1;  m[11] = z;
  m[12] = 0;  m[13] = 0;  m[14] = 0;  m[15] = 1;
}

/* Simple 4x4 matrix by 4x4 matrix multiply. */
static void multMatrix(float dst[16],
                       const float src1[16], const float src2[16])
{
  float tmp[16];
  int i, j;

  for (i=0; i<4; i++) {
    for (j=0; j<4; j++) {
      tmp[i*4+j] = src1[i*4+0] * src2[0*4+j] +
                   src1[i*4+1] * src2[1*4+j] +
                   src1[i*4+2] * src2[2*4+j] +
                   src1[i*4+3] * src2[3*4+j];
    }
  }
  /* Copy result to dst (so dst can also be src1 or src2). */
  for (i=0; i<16; i++)
    dst[i] = tmp[i];
}

static void display(void)
{
  float translateMatrix[16], rotateMatrix[16],
        modelMatrix[16], viewMatrix[16],
        modelViewMatrix[16], modelViewProjMatrix[16];

  buildLookAtMatrix(400*sin(myEyeAngle), 400, 400*cos(myEyeAngle),  /* eye position */
                    0, 0, 0, /* view center */
                    0, 1, 0, /* up vector */
                    viewMatrix);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  cgGLBindProgram(myCgVertexProgram);
  checkForCgError("binding vertex program");

  cgGLEnableProfile(myCgVertexProfile);
  checkForCgError("enabling vertex profile");

  cgGLBindProgram(myCgFragmentProgram);
  checkForCgError("binding fragment program");

  cgGLEnableProfile(myCgFragmentProfile);
  checkForCgError("enabling fragment profile");

  /* modelView = rotateMatrix * translateMatrix */
  makeRotateMatrix(0, 1, 0, 0, rotateMatrix);
  makeTranslateMatrix(-triModel.center[0], -triModel.center[1], -triModel.center[2], translateMatrix);
  multMatrix(modelMatrix, translateMatrix, rotateMatrix);
  /* modelViewMatrix = viewMatrix * modelMatrix */
  multMatrix(modelViewMatrix, viewMatrix, modelMatrix);
  /* modelViewProj = projectionMatrix * modelViewMatrix */
  multMatrix(modelViewProjMatrix, myProjectionMatrix, modelViewMatrix);
  /* Set matrix parameter with row-major matrix. */
  cgSetMatrixParameterfr(myCgVertexParam_modelViewProj, modelViewProjMatrix);
  cgSetMatrixParameterfr(myCgFragmentParam_LR, lr);
  cgSetMatrixParameterfr(myCgFragmentParam_LG, lg);
  cgSetMatrixParameterfr(myCgFragmentParam_LB, lb);
  vector<Triangle> triangleList;
  for(vector<Triangle>::iterator iter=triModel.triangleList.begin(); iter != triModel.triangleList.end(); iter++){
	  glColor3f((double)(iter->foreColor[0])/256, (double)(iter->foreColor[1])/256, (double)(iter->foreColor[2])/256);
	  glBegin(GL_TRIANGLES);
	  for(int i=0; i<3; i++){
		  glNormal3f(iter->normal[i][0], iter->normal[i][1], iter->normal[i][2]);
		  glVertex3f(iter->vertex[i][0], iter->vertex[i][1], iter->vertex[i][2]);
	  }
	  glEnd();
  }

  cgGLDisableProfile(myCgVertexProfile);
  checkForCgError("disabling vertex profile");

  cgGLDisableProfile(myCgFragmentProfile);
  checkForCgError("disabling fragment profile");

  glutSwapBuffers();
}

static void idle(void)
{
  myEyeAngle += 0.016;  /* Add a small angle (in radians). */
  if (myEyeAngle > 2*M_PI) {
    myEyeAngle -= 2*M_PI;
  }
  glutPostRedisplay();
}


static void keyboard(unsigned char c, int x, int y)
{
  static int animating = 0;

  switch (c) {
  case ' ':
    animating = !animating; /* Toggle */
    if (animating) {
      glutIdleFunc(idle);
    } else {
      glutIdleFunc(NULL);
    }    
    break;
  case 27:  /* Esc key */
    /* Demonstrate proper deallocation of Cg runtime data structures.
       Not strictly necessary if we are simply going to exit. */
    cgDestroyProgram(myCgVertexProgram);
    cgDestroyContext(myCgContext);
    exit(0);
    break;
  }
}

static void menu(int item)
{
  /* Pass menu item character code to keyboard callback. */
  keyboard((unsigned char)item, 0, 0);
}
