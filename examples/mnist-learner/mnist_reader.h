#ifndef MNIST_READER_H
#define MNIST_READER_H

#include <stdio.h>
#include <stdint.h>
#include "defs.h"

#define NUM_TRAIN_IMAGE 60000
#define NUM_TEST_IMAGE 10000

#define LEN_INFO_IMAGE 4 * 4 // 4 info, 4 bytes each (uint32_t)
#define LEN_INFO_LABEL 2 * 4 // 2 info, 4 bytes each (uint32_t)

static uint8_t train_image_data_byte[NUM_TRAIN_IMAGE][SIZE_IMAGE];
static float train_image_data[NUM_TRAIN_IMAGE][SIZE_IMAGE];
static uint8_t train_label[NUM_TRAIN_IMAGE];

static uint8_t test_image_data_byte[NUM_TEST_IMAGE][SIZE_IMAGE];
static float test_image_data[NUM_TEST_IMAGE][SIZE_IMAGE];
static uint8_t test_label[NUM_TEST_IMAGE];

bool LoadMNIST()
{
    FILE* fptr = fopen(CMAKE_PATH_TRAIN_IMAGES, "rb");
    if (fptr == NULL) 
    {
        printf("Couldn't find the dataset file: %s\n", CMAKE_PATH_TRAIN_IMAGES); 
        return 0; 
    }
    fseek(fptr, LEN_INFO_IMAGE, SEEK_SET);
    for (uint16_t i = 0; i < NUM_TRAIN_IMAGE; i++) { fread(train_image_data_byte[i], 1, SIZE_IMAGE, fptr); }
    for (uint16_t i = 0; i < NUM_TRAIN_IMAGE; i++) 
    {
        for (uint16_t j = 0; j < SIZE_IMAGE; j++) 
        {
            train_image_data[i][j] = (float)(train_image_data_byte[i][j] / 255.0f);
        }
    }
    fclose(fptr);
    
    fptr = fopen(CMAKE_PATH_TEST_IMAGES, "rb");
    if (fptr == NULL) 
    {
        printf("Couldn't find the dataset file: %s\n", CMAKE_PATH_TEST_IMAGES); 
        return 0; 
    }
    fseek(fptr, LEN_INFO_IMAGE, SEEK_SET);
    for (uint16_t i = 0; i < NUM_TEST_IMAGE; i++) { fread(test_image_data_byte[i], 1, SIZE_IMAGE, fptr); }
    for (uint16_t i = 0; i < NUM_TEST_IMAGE; i++) 
    {
        for (uint16_t j = 0; j < SIZE_IMAGE; j++) 
        {
            test_image_data[i][j] = (float)test_image_data_byte[i][j] / 255.0f;
        }
    }
    fclose(fptr);

    fptr = fopen(CMAKE_PATH_TRAIN_LABELS, "rb");
    if (fptr == NULL) 
    {
        printf("Couldn't find the dataset file: %s\n", CMAKE_PATH_TRAIN_LABELS); 
        return 0; 
    }
    fseek(fptr, LEN_INFO_LABEL, SEEK_SET);
    for (uint16_t i = 0; i < NUM_TRAIN_IMAGE; i++) { fread(train_label, 1, NUM_TRAIN_IMAGE, fptr); }
    fclose(fptr);

    fptr = fopen(CMAKE_PATH_TEST_LABELS, "rb");
    if (fptr == NULL) 
    {
        printf("Couldn't find the dataset file: %s\n", CMAKE_PATH_TEST_LABELS); 
        return 0; 
    }
    fseek(fptr, LEN_INFO_LABEL, SEEK_SET);
    for (uint16_t i = 0; i < NUM_TEST_IMAGE; i++) { fread(test_label, 1, NUM_TEST_IMAGE, fptr); }
    fclose(fptr);

    return 1;
}

#endif