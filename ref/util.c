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
   FILE *fp;
   char  filename[256];

   sprintf(filename, "dump_%d.bin", timestep);
   fp = fopen(filename, "wb");
   if (!fp) {
      fprintf(stderr, "PE %d: cannot open %s\n", my_pe, filename);
      MPI_Abort(MPI_COMM_WORLD, -1);
   }

   fwrite(&num_blocks[0], sizeof(num_blocks[0]), 1, fp);
   fwrite(&num_blocks[1], sizeof(num_blocks[1]), 1, fp);
   fwrite(&num_blocks[2], sizeof(num_blocks[2]), 1, fp);

   const int nx = x_block_size, ny = y_block_size, nz = z_block_size;

   for (int idx = 0; idx < sorted_index[num_refine + 1]; ++idx) {
      block *bp = &blocks[ sorted_list[idx].n ];
      if (bp->number < 0)
         continue;

      for (int v = 0; v < num_vars; ++v)
         for (int i = 1; i <= nx; ++i)
            for (int j = 1; j <= ny; ++j)
               fwrite(&bp->array[v][i][j][1], sizeof(double), nz, fp);
   }

   fclose(fp);
   if (!my_pe) printf("Wrote %s\n", filename);
}