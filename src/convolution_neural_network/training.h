#pragma once
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "../../src/global.h"
#include "../../src/type.h"

#include "convolution_layer.h"
#include "neural_network.h"

#include "context.h"
#include "dataset_manager.h"
#include "evaluation.h"


int train(Context* context, Dataset* train_dataset, Dataset* test_dataset, Layer** neural_network,
          FILE* fp_train, FILE* fp_test);
