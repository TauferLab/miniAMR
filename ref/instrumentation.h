#ifndef INSTRUMENTATION_H
#define INSTRUMENTATION_H

#include <mpi.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>

static FILE* event_file;
static char event_file_name[128];
static time_t run_start_time;
static time_t refine_time_start;
static time_t refine_time_stop;
static time_t lb_time_start;
static time_t lb_time_stop;
static int refine_step_idx = 0;

void init_instrumentation();
void finalize_instrumentation();
void start_refine_timing();
void stop_refine_timing();

#endif // INSTRUMENTATION_H
