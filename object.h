#include "model.h"

struct Object {
    Model model;
    Transform transform;
    const char* shaderPath;
    Mat mat;
};