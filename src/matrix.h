#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct Matrix
{
    size_t rows;
    size_t cols;
    float *data;
} Matrix;

Matrix MatCreate(size_t rows, size_t cols);
void MatFill(Matrix *m, float data);
void MatRand(Matrix *m, float min, float max);
void MatScale(Matrix *m, float scale);
void MatPow(Matrix* m, float pow);
void MatAdd(Matrix *m1, bool negate_m1, Matrix *m2, bool negate_m2, Matrix *out);
void MatMul(Matrix *m1, bool tranpose_m1, Matrix *m2, bool tranpose_m2, Matrix *out);
void MatHadamard(Matrix *m1, Matrix *m2, Matrix *out);
void MatPrint(Matrix *m);
float MatSum(Matrix *m);
void MatReshape(Matrix* m, size_t rows, size_t cols);
void MatTranpose(Matrix* m);
void MatCopy(Matrix *m, Matrix *dest);
void MatFree(Matrix *m);

void MatSigmoid(Matrix *m);
void MatDerSigmoid(Matrix *m);

void MatReLU(Matrix *m);
void MatDerReLU(Matrix *m);

#endif