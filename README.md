# Parallel-Matrix-Operations-MPI
parallel implementation of matrix operations using MPI's in cpp

a parallel implementation of matrix operations (addition, multiplication, and transpose) using MPI (Message Passing Interface) for distributed computing in c++.
The program allows the user to choose between matrix addition, multiplication, or transpose operations. The matrices involved are initialized either for serial addition, multiplication, or transpose, and the computations are distributed among different MPI processes for parallel execution. The code uses MPI functions such as MPI_Scatter, MPI_Gather, MPI_Barrier, and MPI_Bcast to coordinate the parallel tasks among processes.

To run the MPI program on the command line (CMD), you'll need to follow these steps:

1)
**mpic++ your_program_name.cpp -o executable_file_name**
  
2)
**mpiexec -n <total number of processes> executable_file_name**
