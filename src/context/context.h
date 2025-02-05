#pragma once
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libconfig.h>

#include "global.h"
#include "type.h"

#define STRING_SIZE 50

typedef struct {
  const char* context_path;

  // ouput
  char* storage_dir;
  char* train_dat_path;
  char* test_dat_path;

  // dataset
  int max_per_folder;
  char** train_dirs;
  char** test_dirs;

  // nn
  int* topology;
  int nn_size;

  // training
  int do_test;
  int max_epoch;
  double precision;
  double alpha_;
  double eta_;

  //
  int width;
  int height;
  int filters;// TODO

} Context;

int load_context(Context* context, const char* filename);
int info_context(Context* context);
int free_context(Context* context);
