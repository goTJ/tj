#ifndef VIEW_H
#define VIEW_H

#include "utils.h"
#include <cmath>

struct View{
    double mm[4][4];
    double lookAt[4][4];
    double matrix[4][4];
    double scale[3];
    double center[3];
    double translate[3];
    double rotate[3];
    double shear[2];
    double lRotate[3];
    double lTranslate[3];
    double dis;
    
    View(){
        memset(mm, 0, sizeof(mm));
        memset(lookAt, 0, sizeof(lookAt));
        scale[0] = scale[1] = scale[2] = 1.0;
        memset(center, 0, sizeof(center));
        memset(translate, 0, sizeof(translate));
        memset(rotate, 0, sizeof(rotate));
        memset(shear, 0, sizeof(shear));
        memset(lRotate, 0, sizeof(lRotate));
        memset(lTranslate, 0, sizeof(lTranslate));
    }
    void init(double center[3], int dis){
        for(int i=0; i<4; i++)
            mm[i][i] = lookAt[i][i] = 1.0;
        for(int i=0; i<3; i++){
            mm[i][3] = -center[i];
            this->center[i] = -center[i];
        }
        this->dis = dis;
        lookAt[2][3] = dis;
        lTranslate[2] = dis;
        matrixMul(lookAt, mm, matrix);
    }
    void reset(){
        memset(mm, 0, sizeof(mm));
        memset(lookAt, 0, sizeof(lookAt));
        scale[0] = scale[1] = scale[2] = 1.0;
        memset(translate, 0, sizeof(translate));
        memset(rotate, 0, sizeof(rotate));
        memset(shear, 0, sizeof(shear));
        memset(lRotate, 0, sizeof(lRotate));
        memset(lTranslate, 0, sizeof(lTranslate));
        lTranslate[2] = dis;
        genMM();
        genLookAt();
        matrixMul(lookAt, mm, matrix);
    }
    void genLookAt(){
        double tmp[4][4], tmp2[4][4];
        // lTranslate
        memset(tmp, 0, sizeof(tmp));
        tmp[3][3] = 1.0;
        for(int i=0; i<3; i++){
            tmp[i][i] = 1.0;
            tmp[i][3] = lTranslate[i];
        }
        // rotate y
        memset(tmp2, 0, sizeof(tmp2));
        tmp2[1][1] = tmp2[3][3] = 1.0;
        tmp2[0][0] = tmp2[2][2] = cos(lRotate[1]);
        tmp[0][2] = -sin(lRotate[1]);
        tmp[2][0] = sin(lRotate[1]);
        matrixMul(tmp2, tmp, lookAt);
        // generate matrix
        matrixMul(lookAt, mm, matrix);
    }
    void genMM(){
        double tmp[4][4], tmp2[4][4];
        // scale
        memset(mm, 0, sizeof(mm));
        mm[3][3] = 1.0;
        for(int i=0; i<3; i++){
            mm[i][i] = scale[i];
            mm[i][3] = center[i]*scale[i];
        }
        // shear
        memset(tmp, 0, sizeof(tmp));
        for(int i=0; i<4; i++)
            tmp[i][i] = 1.0;
        tmp[0][2] = shear[0];
        tmp[1][2] = shear[1];
        matrixMul(tmp, mm, tmp2);
        // rotate x
        memset(mm, 0, sizeof(mm));
        mm[0][0] = mm[3][3] = 1.0;
        mm[1][1] = mm[2][2] = cos(rotate[0]);
        mm[1][2] = -sin(rotate[0]);
        mm[2][1] = sin(rotate[0]);
        matrixMul(mm, tmp2, tmp);
        // rotate y
        memset(mm, 0, sizeof(mm));
        mm[1][1] = mm[3][3] = 1.0;
        mm[0][0] = mm[2][2] = cos(rotate[1]);
        mm[0][2] = -sin(rotate[1]);
        mm[2][0] = sin(rotate[1]);
        matrixMul(mm, tmp, tmp2);
        // rotate x
        memset(mm, 0, sizeof(mm));
        mm[2][2] = mm[3][3] = 1.0;
        mm[1][1] = mm[0][0] = cos(rotate[2]);
        mm[1][0] = -sin(rotate[2]);
        mm[0][1] = sin(rotate[2]);
        matrixMul(mm, tmp2, tmp);
        // translate
        memset(tmp2, 0, sizeof(tmp2));
        tmp2[3][3] = 1.0;
        for(int i=0; i<3; i++){
            tmp2[i][i] = 1.0;
            tmp2[i][3] = translate[i];
        }
        matrixMul(tmp2, tmp, mm);
        // generate matrix
        matrixMul(lookAt, mm, matrix);
    }
};


#endif
