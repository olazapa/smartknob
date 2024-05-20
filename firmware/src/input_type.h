#pragma once

typedef enum {
    INPUT_BACK          = 0,
    INPUT_FORWARD,
    INPUT_NEXT,
    INPUT_PREV,
    INPUT_WITH_DATA
} input_t;

typedef struct {
  input_t inputType;
  int inputData;
} userInput_t;