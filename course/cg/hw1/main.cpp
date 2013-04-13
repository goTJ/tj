/*
 * This example draw a line on the screen....
 * you can modify the display() to calculate transformation and draw the model.
 * in this example also demonstrate how to use mouse and keyboard callback function.
 *
 * For more GLUT API information...you can visit the GLUT website : http://www.xmission.com/~nate/glut.html
 * you can find some useful functions there, ex..GLUTMotionFunc() to acheive smooth translation.
 */

#include "GL\glut.h"
#include "TRIModel.h"
#include "View.h"
#include "utils.h"
#include <cstdio>
#include <cmath>

#define WINDOW_SIZE_X 800
#define WINDOW_SIZE_Y 600
//#define MODEL_NUM 2
#define MODEL_NUM 5
#define M_PI 3.1415926535
#define TRANSLATE_STEP 10
#define LTRANSLATE_STEP 10
#define SCALE_RATE 1.1
#define ROTATE_DEGREE M_PI/12
#define LROTATE_DEGREE M_PI/24
#define SHEAR_STEP 0.1

char *modelName[MODEL_NUM] = { "pot.tri", "csie.tri", "Tomcat.tri", "F1.tri", "Car_road.tri" };
//char *modelName[MODEL_NUM] = { "pot.tri", "csie.tri" };
TRIModel triModel[MODEL_NUM];
int model_idx=0;

int zNear, zFar, aov;
double pm[4][4];
View view[MODEL_NUM];
View *nowView;
double matrix[4][4];

void genPM(double m[4][4])
{
    m[0][0] = 1.0/tan((double)aov*M_PI/360);
    m[1][1] = 1.0/tan((double)aov*M_PI/360);
    m[2][2] = (double)(zFar+zNear)/(zFar-zNear);
    m[2][3] = (double)(-2*zFar*zNear)/(zFar-zNear);
    m[3][2] = 1.0;
}

void initialize(){
    aov = 120;
    zNear = 100;
    zFar = 1000;
	genPM(pm);
	
	for(int i=0; i<MODEL_NUM; i++){
		if(!triModel[i].loadFromFile(modelName[i])){
			fprintf(stderr, "Can't load file %s\n", modelName[i]);
			exit(-1);
		}
		view[i].init(triModel[i].center, (zFar-zNear)/2);
	}
	nowView = &view[0];
	matrixMul(pm, nowView->matrix, matrix);
}

void init(void){
	glClearColor(0., 0., 0., 0.);
	glShadeModel(GL_SMOOTH);
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	vector<Triangle> &tri_vec=triModel[model_idx].triangleList;
	double ver[3][3];
    double tmp[3], tmp2[3];
	
    for(vector<Triangle>::iterator iter=tri_vec.begin(); iter!=tri_vec.end(); iter++){
        double normal[3];
        int n=0;
        for(int i=0; i<3; i++){
            matrixMul(matrix, iter->vertex[i], ver[n]);
            matrixMul3(nowView->matrix, iter->normal[i], tmp);
            matrixMul(nowView->matrix, iter->vertex[i], tmp2);
            normal[n] = 0.0;
            for(int j=0; j<3; j++)
                normal[n] -= tmp2[j]*tmp[j];
            normal[n] /= sqrt(tmp2[0]*tmp2[0]+tmp2[1]*tmp2[1]+tmp2[2]*tmp2[2]);
            n++;
        }
        for(int i=0; i<n; i++){
            if(normal[i]+normal[(i+1)%n] > -0.3)
            	glColor3f((double)iter->foreColor[0]/255, (double)iter->foreColor[1]/255, (double)iter->foreColor[2]/255);
            else{
                continue;
            	glColor3f((double)iter->backColor[0]/255, (double)iter->backColor[1]/255, (double)iter->backColor[2]/255);
            }
            double v1[3], v2[3];
            if(insertNode(ver[i], ver[(i+1)%n], v1, v2)){
            	glBegin(GL_LINES);
            	glVertex2f(v1[0]*WINDOW_SIZE_X/2+WINDOW_SIZE_X/2, v1[1]*WINDOW_SIZE_Y/2+WINDOW_SIZE_Y/2);
            	glVertex2f(v2[0]*WINDOW_SIZE_X/2+WINDOW_SIZE_X/2, v2[1]*WINDOW_SIZE_Y/2+WINDOW_SIZE_Y/2);
            	glEnd();
            }
        }
    }
	glutSwapBuffers();
}

void reshape(int w, int h){
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
}

void keyboard(unsigned char key, int x, int y){
	switch(key){
		case 'z':
			exit(0);
			break;
		case 'v':
            model_idx = (model_idx+1)%MODEL_NUM;
            nowView = &view[model_idx];
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case 'c':
            model_idx = (model_idx+MODEL_NUM-1)%MODEL_NUM;
            nowView = &view[model_idx];
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case 'd':  // scale + all
            for(int i=0; i<3; i++)
                nowView->scale[i] *= SCALE_RATE;
            nowView->genMM();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case 'f':  // scale + x
            nowView->scale[0] *= SCALE_RATE;
            nowView->genMM();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case 'e':  // scale + y
            nowView->scale[1] *= SCALE_RATE;
            nowView->genMM();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case 'r':  // scale + z
            nowView->scale[2] *= SCALE_RATE;
            nowView->genMM();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case 's':  // scale - all
            for(int i=0; i<3; i++)
                nowView->scale[i] /= SCALE_RATE;
            nowView->genMM();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case 'a':  // scale + x
            nowView->scale[0] /= SCALE_RATE;
            nowView->genMM();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case 'w':  // scale - y
            nowView->scale[1] /= SCALE_RATE;
            nowView->genMM();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case 'q':  // scale + z
            nowView->scale[2] /= SCALE_RATE;
            nowView->genMM();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case 'l':  // translate + all
            for(int i=0; i<3; i++)
                nowView->translate[i] += TRANSLATE_STEP;
            nowView->genMM();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case ';':  // translate + x
            nowView->translate[0] += TRANSLATE_STEP;
            nowView->genMM();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case 'o':  // translate + y
            nowView->translate[1] += TRANSLATE_STEP;
            nowView->genMM();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case 'p':  // translate + z
            nowView->translate[2] += TRANSLATE_STEP;
            nowView->genMM();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case 'k':  // translate - all
            for(int i=0; i<3; i++)
                nowView->translate[i] -= TRANSLATE_STEP;
            nowView->genMM();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case 'j':  // translate - x
            nowView->translate[0] -= TRANSLATE_STEP;
            nowView->genMM();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case 'i':  // translate - y
            nowView->translate[1] -= TRANSLATE_STEP;
            nowView->genMM();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case 'u':  // translate - z
            nowView->translate[2] -= TRANSLATE_STEP;
            nowView->genMM();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case '8':  // rotate + x
            nowView->rotate[0] += ROTATE_DEGREE;
            nowView->genMM();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case '5':  // rotate - x
            nowView->rotate[0] -= ROTATE_DEGREE;
            nowView->genMM();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case '6':  // rotate + y
            nowView->rotate[1] += ROTATE_DEGREE;
            nowView->genMM();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case '4':  // rotate - y
            nowView->rotate[1] -= ROTATE_DEGREE;
            nowView->genMM();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case '9':  // rotate + z
            nowView->rotate[2] += ROTATE_DEGREE;
            nowView->genMM();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case '7':  // rotate - z
            nowView->rotate[2] -= ROTATE_DEGREE;
            nowView->genMM();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case '.':  // shear + x
            nowView->shear[0] += SHEAR_STEP;
            nowView->genMM();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case ',':  // shear - x
            nowView->shear[0] -= SHEAR_STEP;
            nowView->genMM();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case '/':  // shear + y
            nowView->shear[1] += SHEAR_STEP;
            nowView->genMM();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case 'm':  // shear - y
            nowView->shear[1] -= SHEAR_STEP;
            nowView->genMM();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
    	case '[':  // lookat rotate + y
            nowView->lRotate[1] += LROTATE_DEGREE;
            nowView->genLookAt();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
    	case ']':  // lookat rotate - y
            nowView->lRotate[1] -= LROTATE_DEGREE;
            nowView->genLookAt();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case '=':  // lookat translate + y
            nowView->lTranslate[1] += LTRANSLATE_STEP;
            nowView->genLookAt();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case '-':  // lookat translate- y
            nowView->lTranslate[1] -= LTRANSLATE_STEP;
            nowView->genLookAt();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
		case 'x':  // reset
            nowView->reset();
          	matrixMul(pm, nowView->matrix, matrix);
			glutPostRedisplay();
			break;
	}
}

void mouse(int button, int state, int x, int y){
/*
	static float preX, preY;
	switch(state){
		case GLUT_DOWN:
			preX = x;
			preY = y;
			glutPostRedisplay();
			break;

		case GLUT_UP:
			startPosX += x - preX;
			startPosY += preY - y;
			glutPostRedisplay();
			break;
	}
	*/
}

void motion(int x, int y){
    static int preX, preY;
    
    preX = x;
    preY = y;
}

int main(int argc, char **argv){
	initialize();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_SIZE_X, WINDOW_SIZE_Y);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("CG2006 HW1");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutMainLoop();
	return 0;
}
