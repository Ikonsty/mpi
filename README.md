# mpi


# Compile
mpic++ -I/urs/include/boost main.cpp -Lmpi -lboost_mpi -lboost_serialization -lboost_program_options ./sources/grid.cpp ./sources/parser.cpp ./sources/diff_scheme.cpp ./sources/helper.cpp -o main

# Run
mpirun -np 4 main ./data/config.cfg 
