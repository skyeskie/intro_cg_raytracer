#ifndef LINES_H
#define LINES_H
#include "../mod_base/CommandHandle.h"

void draw_line(float, float, float, float, CommandHandle& );
int near_far_clip(float, float, float *, float *, float *, float *,
                  float *, float *);

#endif