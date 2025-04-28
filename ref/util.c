// ************************************************************************
//
// miniAMR: stencil computations with boundary exchange and AMR.
//
// Copyright (2014) Sandia Corporation. Under the terms of Contract
// DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government 
// retains certain rights in this software.
//
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
// Questions? Contact Courtenay T. Vaughan (ctvaugh@sandia.gov)
//                    Richard F. Barrett (rfbarre@sandia.gov)
//
// ************************************************************************

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h> 
#include <unistd.h> 
#include <mpi.h>

#include "block.h"
#include "comm.h"
#include "proto.h"
#include "timer.h"

double timer(void)
{
   return(MPI_Wtime());
}

void *ma_malloc(size_t size, char *file, int line)
{
   void *ptr;

   ptr = (void *) malloc(size);

   if (ptr == NULL) {
      printf("NULL pointer from malloc call in %s at %d\n", file, line);
      exit(-1);
   }

   counter_malloc++;
   size_malloc += (double) size;

   return(ptr);
}

void dump_binary(int timestep)
{
     char filename[256];
     FILE *fp;
     int in, k;
     block *bp;
     int active_blocks_on_pe = sorted_index[num_refine + 1];

     int precision = 2; // Update to be based on error tolerance
 
     /* construct .txt filename */
     sprintf(filename, "dump_ts%04d_pe%06d.txt", timestep, my_pe);
 
     fp = fopen(filename, "w");
     if (fp == NULL) {
         fprintf(stderr, "PE %d ERROR: Could not open file %s for writing: %s\n",
                 my_pe, filename, strerror(errno));
         MPI_Abort(MPI_COMM_WORLD, 1);
     }
 
     for (in = 0; in < active_blocks_on_pe; in++) {
         bp = &blocks[sorted_list[in].n];
 
         /* assuming num_vars == 1 for now */
         for (int v = 0; v < 1; v++) {
             for (int i = 0; i < x_block_size; i++) {
                 for (int j = 0; j < y_block_size; j++) {
                     for (k = 1; k <= z_block_size; k++) {
                         double val = bp->array[v][i+1][j+1][k];
                         /* "%.*f" prints exactly 'precision' decimals, padding with zeros */
                         if (fprintf(fp, "%.*f", precision, val) < 0) {
                             fprintf(stderr,
                                     "PE %d ERROR: Failed to write text data for "
                                     "block %lld, var %d, i=%d, j=%d, k=%d to file %s\n",
                                     my_pe, (long long)bp->number,
                                     v, i, j, k,
                                     filename);
                             fclose(fp);
                             MPI_Abort(MPI_COMM_WORLD, 1);
                         }
                     }
                 }
             }
         }
     }
 
     if (fclose(fp) != 0) {
         fprintf(stderr, "PE %d ERROR: Could not close file %s properly: %s\n",
                 my_pe, filename, strerror(errno));
     }
 }


// void dump_binary(int timestep)
// {
//      char filename[256];
//      FILE *fp;
//      int in;
//      block *bp;
//      size_t write_count;
//      int active_blocks_on_pe = sorted_index[num_refine + 1]; 

//      sprintf(filename, "dump_ts%04d_pe%06d.bin", timestep, my_pe);

//      fp = fopen(filename, "wb");
//      if (fp == NULL) {
//           fprintf(stderr, "PE %d ERROR: Could not open file %s for writing: %s\n", my_pe, filename, strerror(errno));
//           MPI_Abort(MPI_COMM_WORLD, 1);
//      }

//      for (in = 0; in < active_blocks_on_pe; in++) {
//           bp = &blocks[sorted_list[in].n];

//           for (int v = 0; v < 1; v++) {
//                 for (int i = 0; i < x_block_size; i++) {
//                      for (int j = 0; j < y_block_size; j++) {
//                           write_count = fwrite(&bp->array[v][i + 1][j + 1][1], sizeof(double), z_block_size, fp);
//                           if (write_count != z_block_size) {
//                                  fprintf(stderr, "PE %d ERROR: Failed to write full data for block %lld, var %d, i=%d, j=%d to file %s. Wrote %zu elements.\n",
//                                             my_pe, (long long)bp->number, v, i, j, filename, write_count);
//                                  fclose(fp);
//                                  MPI_Abort(MPI_COMM_WORLD, 1);
//                           }
//                      }
//                 }
//           }
//      }

//      if (fclose(fp) != 0) {
//             fprintf(stderr, "PE %d ERROR: Could not close file %s properly: %s\n", my_pe, filename, strerror(errno));
//      }
// }