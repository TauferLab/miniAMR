#ifndef INSTRUMENTATION_H
#define INSTRUMENTATION_H

#include <mpi.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>

static FILE* event_file;
static char event_file_name[128];

// Timestamps for start of run
// These will be set exactly once
static struct timespec run_start_time_ts;
static double run_start_time_sec;
static double run_start_time_nsec;
static double run_start_time;

static struct timespec refine_start_time_ts;
static double refine_start_time_sec;
static double refine_start_time_nsec;
static double refine_start_time;

static struct timespec refine_stop_time_ts;
static double refine_stop_time_sec;
static double refine_stop_time_nsec;
static double refine_stop_time;

static double refine_elapsed_time;

static int refine_step_idx = 0;

void init_instrumentation();
void finalize_instrumentation();
void start_refine_timing();
void stop_refine_timing();

#endif // INSTRUMENTATION_H
