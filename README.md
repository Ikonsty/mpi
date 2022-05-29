# mpi


# Compile
mpic++ -I/urs/include/boost -I/usr/include/ImageMagick-6/ main.cpp -lMagick++-6.Q16 -DMAGICKCORE_QUANTUM_DEPTH=16  -DMAGICKCORE_HDRI_ENABLE=0 -Lmpi -lboost_mpi -lboost_program_options -lboost_serialization ./sources/grid.cpp ./sources/parser.cpp ./sources/diff_scheme.cpp ./sources/helper.cpp -o main

# Run
mpirun -np 4 main ./data/config.cfg 
