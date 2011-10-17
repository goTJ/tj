#ifndef MATRIX_H
#define MATRIX_H

void displayMatrix(double a[4][4])
{
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++)
            printf("%.2f ", a[i][j]);
        printf("\n");
    }
}

inline void matrixMul(double a[4][4], double b[3], double m[3])
{
    double w=a[3][0]*b[0]
            +a[3][1]*b[1]
            +a[3][2]*b[2]
            +a[3][3]*1.0;
    for(int i=0; i<3; i++){
        double sum=a[i][3]*1.0;
        for(int j=0; j<3; j++)
            sum += a[i][j]*b[j];
        m[i] = sum/w;
    }
}

inline void matrixMul3(double a[4][4], double b[3], double m[3])
{
    for(int i=0; i<3; i++){
        m[i]=a[i][0]*b[0]
            +a[i][1]*b[1]
            +a[i][2]*b[2];
    }
}

inline void matrixMul(double a[4][4], double b[4][4], double m[4][4])
{
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            m[i][j]=a[i][0]*b[0][j]
                   +a[i][1]*b[1][j]
                   +a[i][2]*b[2][j]
                   +a[i][3]*b[3][j];
        }
    }
}

inline void matrixMul3(double a[4][4], double b[4][4], double m[4][4])
{
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            m[i][j]=a[i][0]*b[0][j]
                   +a[i][1]*b[1][j]
                   +a[i][2]*b[2][j];
        }
    }
}

inline void matrixAdd(double a[3], double m[3])
{
    m[0] += a[0];
    m[1] += a[1];
    m[2] += a[2];
}

bool insertNode(double a[3], double b[3], double v1[3], double v2[3])
{
    int other[3][2] = { 1,2, 2,0, 0,1 };
    for(int i=0; i<3; i++){
        v1[i] = a[i];
        v2[i] = b[i];
    }
    for(int i=0; i<3; i++){
        double rate;
        if(v1[i] > 1.0 && v2[i] > 1.0){
            return false;
        }else if(v1[i] > 1.0){
            rate = (1-v2[i])/(v1[i]-v2[i]);
            v1[other[i][0]] = rate*(v1[other[i][0]]-v2[other[i][0]])+v2[other[i][0]];
            v1[other[i][1]] = rate*(v1[other[i][1]]-v2[other[i][1]])+v2[other[i][1]];
            v1[i] = 1.0;
        }else if(v2[i] > 1.0){
            rate = (1-v1[i])/(v2[i]-v1[i]);
            v2[other[i][0]] = rate*(v2[other[i][0]]-v1[other[i][0]])+v1[other[i][0]];
            v2[other[i][1]] = rate*(v2[other[i][1]]-v1[other[i][1]])+v1[other[i][1]];
            v2[i] = 1.0;
        }
        if(v1[i] < -1.0 && v2[i] < -1.0){
            return false;
        }else if(v1[i] < -1.0){
            rate = (-1-v2[i])/(v1[i]-v2[i]);
            v1[other[i][0]] = rate*(v1[other[i][0]]-v2[other[i][0]])+v2[other[i][0]];
            v1[other[i][1]] = rate*(v1[other[i][1]]-v2[other[i][1]])+v2[other[i][1]];
            v1[i] = -1.0;
        }else if(v2[i] < -1.0){
            rate = (-1-v1[i])/(v2[i]-v1[i]);
            v2[other[i][0]] = rate*(v2[other[i][0]]-v1[other[i][0]])+v1[other[i][0]];
            v2[other[i][1]] = rate*(v2[other[i][1]]-v1[other[i][1]])+v1[other[i][1]];
            v2[i] = -1.0;
        }
    }
    return true;
}

#endif
