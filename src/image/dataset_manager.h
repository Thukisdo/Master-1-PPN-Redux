#pragma once

#include <dirent.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include "image_io.h"


#include "../../src/global.h"
#include "../../src/type.h"


/* mri_image data struct used to manipulate dataset */
typedef struct {
  char* filename;// Name of current image
  int value;     // Value from 0 to 3, ranging from NonDemented to ModerateDemented

  u8* pixels;
  size_t original_width;
  size_t original_height;

  u8* inputs;// Result of image processing, which will be fed to the input layer of the NN
  size_t width;
  size_t height;// find better name
} mri_image;

typedef struct {
  u64 size;
  mri_image* images;
} Dataset;

// int load_dataset( char * dirs[], int dir_num, mri_image * dataset, int max_per_folder);
int load_dataset(char** dirs, int dir_num, Dataset* dataset, int max_per_folder);
int count_file_in_directory(char* foldername);


int free_dataset(Dataset* dataset);
