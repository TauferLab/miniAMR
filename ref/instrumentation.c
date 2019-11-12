#include "instrumentation.h"

#include <time.h>

void init_instrumentation()
{
  clock_gettime( CLOCK_MONOTONIC, &run_start_time_ts );
  if ( run_start_time_ts.tv_nsec < 0 ) {
    run_start_time_sec  = run_start_time_ts.tv_sec - 1;
    run_start_time_nsec = 1.0e9 + run_start_time_ts.tv_nsec;
  }
  else {
    run_start_time_sec  = run_start_time_ts.tv_sec;
    run_start_time_nsec = run_start_time_ts.tv_nsec;
  }
  run_start_time = run_start_time_sec + 1.0e-9*run_start_time_nsec;

  int mpi_rc, rank;
  mpi_rc = MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  snprintf( event_file_name, 
            sizeof(event_file_name), 
            "events_rank_%d.txt", rank );

  event_file = fopen( event_file_name, "w" );
}

void finalize_instrumentation()
{
  fclose( event_file );
}

void start_refine_timing()
{
  clock_gettime( CLOCK_MONOTONIC, &refine_start_time_ts );
  if ( refine_start_time_ts.tv_nsec < 0 ) {
    refine_start_time_sec  = refine_start_time_ts.tv_sec - 1;
    refine_start_time_nsec = 1.0e9 + refine_start_time_ts.tv_nsec;
  }
  else {
    refine_start_time_sec  = refine_start_time_ts.tv_sec;
    refine_start_time_nsec = refine_start_time_ts.tv_nsec; 
  }
  refine_start_time = refine_start_time_sec + 1.0e-9*refine_start_time_nsec;

  // Adjust w/r/t start time of run
  refine_start_time -= run_start_time;
}

void stop_refine_timing()
{
  clock_gettime( CLOCK_MONOTONIC, &refine_stop_time_ts );
  if ( refine_stop_time_ts.tv_nsec < 0 ) {
    refine_stop_time_sec  = refine_stop_time_ts.tv_sec - 1;
    refine_stop_time_nsec = 1.0e9 + refine_stop_time_ts.tv_nsec;
  }
  else {
    refine_stop_time_sec  = refine_stop_time_ts.tv_sec;
    refine_stop_time_nsec = refine_stop_time_ts.tv_nsec; 
  }
  refine_stop_time = refine_stop_time_sec + 1.0e-9*refine_stop_time_nsec;

  // Adjust w/r/t start time of run
  refine_stop_time -= run_start_time;

  refine_elapsed_time = refine_stop_time - refine_start_time;

  fprintf( event_file, 
           //"Refinement Step: %d, Start Time (s): %f, Start Time (ns): %f, Stop Time (s): %f, Stop Time (ns): %f\n", 
           "Refinement Step: %d, Start Time (s): %f, Stop Time (s): %f, Elapsed Time (s): %f\n", 
           refine_step_idx, 
           refine_start_time,
           refine_stop_time,
           refine_elapsed_time
         );
  refine_step_idx++;
}
