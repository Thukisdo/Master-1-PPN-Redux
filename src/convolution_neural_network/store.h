#pragma once
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "../../src/type.h"
#include "context.h"
#include "neural_network.h"

void store_neural_network(Context* context, Layer** layers);
Layer** load_neural_network(Context* context);
