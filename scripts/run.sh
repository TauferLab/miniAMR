
{
  dir=$(pwd)

  # . clean.sh

  # ======================================
  # Generate miniAMR data
  # ======================================

  echo "Running miniAMR..."

  miniamr_path="${RESEARCH}/dynamic_chunking/miniAMR/ref"
  miniamr_data_path="${RESEARCH}/dynamic_chunking/miniAMR/ref/data"

  cd $miniamr_path

  # Test 1 Refinement on
  mpirun -np 1 ma.x \
    --num_refine 5 \
    --refine_freq 1 \
    --max_blocks 32768 \
    --init_x 1 --init_y 1 --init_z 1 \
    --npx 1 --npy 1 --npz 1 \
    --nx 4 --ny 4 --nz 4 \
    --num_objects 1 \
    --object 2 0 -1.10 -1.10 -1.10 0.030 0.030 0.030 1.5 1.5 1.5 0.0 0.0 0.0 \
    --num_tsteps 50

  # Test 2  –  NO refinement
  # mpirun -np 1 ma.x \
  #   --num_refine 0 \
  #   --refine_freq 1 \
  #   --max_blocks 32768 \
  #   --init_x 32 --init_y 32 --init_z 32 \
  #   --npx 1 --npy 1 --npz 1 \
  #   --nx 4 --ny 4 --nz 4 \
  #   --num_objects 1 \
  #   --object 2 0 -1.10 -1.10 -1.10 0.030 0.030 0.030 1.5 1.5 1.5 0.0 0.0 0.0 \
  #   --num_tsteps 50

  # mkdir -p $miniamr_data_path

  cd $dir
} &> test.out