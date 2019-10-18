#include "instrumentation.h"


void init_instrumentation()
{
  run_start_time = time(NULL); 
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
  refine_time_start = time(NULL) - run_start_time;
}

void stop_refine_timing()
{
  time_t refine_time_stop = time(NULL) - run_start_time; 
  time_t elapsed = refine_time_stop - refine_time_start;
  fprintf( event_file, 
           "Refinement Step: %d, Start Time: %ju, Stop Time: %ju, Elapsed Time: %ju\n", 
           refine_step_idx, 
           (uintmax_t)refine_time_start, 
           (uintmax_t)refine_time_stop, 
           (uintmax_t)elapsed 
         );
  refine_step_idx++;
}
