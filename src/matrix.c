#include "matrix.h"
#include <stdlib.h>
#include <math.h>

#define MAT_GET(matrix, row, col) (matrix)->data[(row) * (matrix)->cols + (col)]

static inline float sigmoidf(float data) { return 1 / (1 + expf(-data)); }
static inline float dersigmoidf(float data) { return sigmoidf(data) * (1 - sigmoidf(data)); }

static inline float reluf(float data) { return fmaxf(0, data); }
static inline float derreluf(float data)
{
    if (data <= 0) { return 0; }
    return 1;
}

Matrix MatCreate(size_t rows, size_t cols)
{
    Matrix matrix;
    matrix.data = (float*)malloc(sizeof(float) * rows * cols);
    matrix.rows = rows;
    matrix.cols = cols;
    MatFill(&matrix, 0);
    return matrix;
}

void MatFill(Matrix* m, float data)
{
    for (size_t i = 0; i < m->rows; i++)
    {
        for (size_t j = 0; j < m->cols; j++)
        {
            MAT_GET(m, i, j) = data;
        }
    }
}

void MatRand(Matrix* m, float min, float max)
{
    for (size_t i = 0; i < m->rows; i++)
    {
        for (size_t j = 0; j < m->cols; j++)
        {
            MAT_GET(m, i, j) = (((max - min) * ((float)rand() / RAND_MAX)) + min);
        }
    }
}

void MatScale(Matrix* m, float scale)
{
    for (size_t i = 0; i < m->rows; i++)
    {
        for (size_t j = 0; j < m->cols; j++)
        {
            MAT_GET(m, i, j) *= scale; 
        }
    }
}

void MatPow(Matrix* m, float pow)
{
    for (size_t i = 0; i < m->rows; i++)
    {
        for (size_t j = 0; j < m->cols; j++)
        {
            MAT_GET(m, i, j) = powf(MAT_GET(m, i, j), pow); 
        }
    }
}

void MatAdd(Matrix* m1, Matrix* m2, Matrix* out)
{
    for (size_t i = 0; i < m1->rows; i++)
    {
        for (size_t j = 0; j < m1->cols; j++)
        {
            MAT_GET(out, i, j) = MAT_GET(m1, i, j) + MAT_GET(m2, i, j);
        }
    }
}

void MatSubtract(Matrix* m1, Matrix* m2, Matrix* out)
{
    for (size_t i = 0; i < m1->rows; i++)
    {
        for (size_t j = 0; j < m1->cols; j++)
        {
            MAT_GET(out, i, j) = MAT_GET(m1, i, j) - MAT_GET(m2, i, j);
        }
    }
}

void MatMul(Matrix* m1, bool tranpose_m1, Matrix* m2, bool tranpose_m2, Matrix* out)
{
    Matrix temp = MatCreate(out->rows, out->cols);

    size_t final_i = tranpose_m1 ? m1->cols : m1->rows;
    size_t final_j = tranpose_m2 ? m2->rows : m2->cols;
    size_t final_k = tranpose_m2 ? m2->cols : m2->rows;

    for (size_t i = 0; i < final_i; i++)
    {
        for (size_t j = 0; j < final_j; j++)
        {
            for (size_t k = 0; k < final_k; k++)
            {
                if (!tranpose_m1 && !tranpose_m2) { MAT_GET(&temp, i, j) += MAT_GET(m1, i, k) * MAT_GET(m2, k, j); }
                else if (tranpose_m1 && !tranpose_m2) { MAT_GET(&temp, i, j) += MAT_GET(m1, k, i) * MAT_GET(m2, k, j); }
                else if (!tranpose_m1 && tranpose_m2) { MAT_GET(&temp, i, j) += MAT_GET(m1, i, k) * MAT_GET(m2, j, k); }
                else { MAT_GET(&temp, i, j) += MAT_GET(m1, k, i) * MAT_GET(m2, j, k); }
            }
        }
    }

    MatCopy(&temp, out);
    MatFree(&temp);
}

void MatHadamard(Matrix* m1, Matrix* m2, Matrix* out)
{
    for (size_t i = 0; i < m1->rows; i++)
    {
        for (size_t j = 0; j < m1->cols; j++)
        {
            MAT_GET(out, i, j) = MAT_GET(m1, i, j) * MAT_GET(m2, i, j);
        }
    }
}

void MatPrint(Matrix* m)
{
    printf("[");
    for (size_t i = 0; i < m->rows; i++)
    {
        if (i != 0) { printf(" "); }
        printf("[");
        for (size_t j = 0; j < m->cols; j++)
        {
            printf("%.2f", MAT_GET(m, i, j));
            if (j != m->cols - 1) { printf(", "); }
        }
        printf("]");
        if (i == m->rows -1) { printf("]"); }
        printf("\n");
    }
}

float MatSum(Matrix* m)
{
    float sum = 0;
    for (size_t i = 0; i < m->rows; i++)
    {
        for (size_t j = 0; j < m->cols; j++)
        {
            sum += MAT_GET(m, i, j);
        }
    }
    return sum;
}

void MatReshape(Matrix* m, size_t rows, size_t cols)
{
    MatFree(m);
    m->data = malloc(sizeof(float) * rows * cols);
    m->rows = rows;
    m->cols = cols;
}

void MatCopy(Matrix* m, Matrix* dest)
{
    for (size_t i = 0; i < m->rows; i++)
    {
        for (size_t j = 0; j < m->cols; j++)
        {
            MAT_GET(dest, i, j) = MAT_GET(m, i, j);
        }
    }
}

void MatFree(Matrix* m)
{
    free(m->data);
}

void MatLog(Matrix* m)
{
    for (size_t i = 0; i < m->rows; i++)
    {
        for (size_t j = 0; j < m->cols; j++)
        {
            MAT_GET(m, i, j) = logf(MAT_GET(m, i, j));
        }
    }
}

void MatSigmoid(Matrix* m)
{
    for (size_t i = 0; i < m->rows; i++)
    {
        for (size_t j = 0; j < m->cols; j++)
        {
            MAT_GET(m, i, j) = sigmoidf(MAT_GET(m, i, j));
        }
    }
}

void MatDerSigmoid(Matrix* m)
{
    for (size_t i = 0; i < m->rows; i++)
    {
        for (size_t j = 0; j < m->cols; j++)
        {
            MAT_GET(m, i, j) = dersigmoidf(MAT_GET(m, i, j));
        }
    }
}

void MatReLU(Matrix* m)
{
    for (size_t i = 0; i < m->rows; i++)
    {
        for (size_t j = 0; j < m->cols; j++)
        {
            MAT_GET(m, i, j) = reluf(MAT_GET(m, i, j));
        }
    }
}

void MatDerReLU(Matrix* m)
{
    for (size_t i = 0; i < m->rows; i++)
    {
        for (size_t j = 0; j < m->cols; j++)
        {
            MAT_GET(m, i, j) = derreluf(MAT_GET(m, i, j));
        }
    }
}

void MatSoftmax(Matrix* m)
{
    for (size_t i = 0; i < m->rows; i++)
    {
        for (size_t j = 0; j < m->cols; j++)
        {
            MAT_GET(m, i, j) = expf(MAT_GET(m, i, j));
        }
    }
    MatScale(m, 1 / MatSum(m));
}