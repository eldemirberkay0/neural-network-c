#include <time.h>
#include <stdlib.h>
#include "model.h"

int main()
{
    srand((unsigned int)time(NULL));
    
    CreateModel();
    TrainModel();
    CalculateAccuracy();

    return 0;
}