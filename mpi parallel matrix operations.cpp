#include <mpi.h>

#include <iostream>

#include <chrono>



using namespace std;



#define N 10   //  matrix size for testing

#define P 10 //  matrix size for testing

#define M 50   // size for testing



double a[N][P]; // Matrix-1 for addition

double d[N][P]; // Matrix-2 for addition

double e[N][P]; // Resultant matrix

double b[P][M]; //matrix-2  for multi

double c[N][M]; //resultant for multi



double f[P][N]; //resultant for transpose



int i, j,k;

void initializationfortranspose()

{

	for(i=0;i<N;i++)

	{

		for(j=0;j<P;j++)

		{

			

			a[i][j] = i+j;

		}

	}	

	

	

	for(i=0;i<P;i++)

	{

		for(j=0;j<N;j++)

		{

			

			f[i][j] = 0 ;

		}

	}

}



void intializationformultiplication(){

	for(i=0;i<N;i++)

	{

		for(j=0;j<P;j++)

		{

			

			a[i][j] = i+j ;

		}

	}	

	

	for(i=0;i<P;i++)

	{

		for(j=0;j<M;j++)

		{

			b[i][j] = i+j ;

		}

	}

	

	for(i=0;i<N;i++)

	{

		for(j=0;j<M;j++)

		{

			

			c[i][j] = 0 ;

		}

	}

}

void initializationForSerialAddition() {

    for (i = 0; i < N; ++i) {

        for (j = 0; j < P; ++j) {

            a[i][j] = i + j;

            d[i][j] = i + j;

            e[i][j] = 0;

        }

    }

}

void Transpose(int size,int rank)

{

    if(rank ==0)

    {

         cout << "\nmatrix:    " << N << " * " << P;

         cout << "\nResultant: " << P << " * " << N;

         initializationfortranspose();

    }

    auto start5 = std::chrono::high_resolution_clock::now();

    int recvbufA[N][P / size];

    MPI_Scatter(a, N * (P / size), MPI_INT, recvbufA, N * (P / size), MPI_INT, 0, MPI_COMM_WORLD);

     MPI_Barrier(MPI_COMM_WORLD);

    double localTranspose[P / size][N];

    for (i = 0; i < P / size; ++i) {

        for (j = 0; j < N; ++j) {

            localTranspose[i][j] = recvbufA[j][i];

        }

    }

    MPI_Gather(localTranspose, (P / size) * N, MPI_INT, f, (P / size) * N, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    auto end5 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration5 = end5 - start5;

    if(rank==0)

    {

    	cout << "\nMatrix Transpose done!\n";

        cout << "Total Execution time is " << duration5.count() << " seconds" << endl;

    }

}

void Multiplication(int size,int rank)

{

    

    if(rank ==0)

    {

    	cout << "\nmatrix1:   " << N << " * " << P;

        cout << "\nmatrix2:   " << P << " * " << M;

        cout << "\nResultant: " << N << " * " << M;

        intializationformultiplication();

    }

    auto start1 = std::chrono::high_resolution_clock::now();

    int recvbufA[N][P], recvbufB[P][M];

    MPI_Scatter(a, P * (N / size), MPI_INT, recvbufA, P * (N / size), MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(b, P * (M / size), MPI_INT, recvbufB, P * (M / size), MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    double localResult[N][M];

    for (i = 0; i < N; ++i) {

        for (j = 0; j < M; ++j) {

            localResult[i][j] = 0;

            for (k = 0; k < P; ++k) {

                localResult[i][j] += recvbufA[i][k] * recvbufB[k][j];

            }

        }

    }

   MPI_Gather(localResult, N * (M / size), MPI_INT, c, N * (M / size), MPI_INT, 0, MPI_COMM_WORLD);

   MPI_Barrier(MPI_COMM_WORLD);

   auto end1 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration1 = end1 - start1;

    if(rank==0)

    {

    	cout << "\nMatrix multiplication done!\n";

        cout << "Total Execution time is " << duration1.count() << " seconds" << endl;

    }	

   

}



void Addition(int size, int rank) {

    int recvbuf[N][P / 2];

    int recvbuf1[N][P / 2];



    if (rank == 0) {

        cout<<"\nmatrix1:   "<<N<<" * "<<P;

	cout<<"\nmatrix2:   "<<N<<" * "<<P;

	cout<<"\nResultant: "<<N<<" * "<<P;

        initializationForSerialAddition();

    }

    auto start3 = std::chrono::high_resolution_clock::now();

    MPI_Scatter(a, P * (N / size), MPI_INT, recvbuf, P * N / size, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(d, P * (N / size), MPI_INT, recvbuf1, P * N / size, MPI_INT, 0, MPI_COMM_WORLD);



    MPI_Barrier(MPI_COMM_WORLD); // Barrier after scatter operations



    int result[N][P / 2];

    for (int i = 0; i < N; ++i) {

        for (int j = 0; j < P/ 2; ++j) {

            result[i][j] = recvbuf[i][j] + recvbuf1[i][j];

        }

    }

    MPI_Gather(result, P * N / size, MPI_INT, e, P* N / size, MPI_INT, 0, MPI_COMM_WORLD);



    MPI_Barrier(MPI_COMM_WORLD); // Barrier after gather operation

    auto end3 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration3 = end3 - start3;



    if (rank == 0) {

        cout << "\nMatrix addition done!\n";

        cout << "Total Execution time is " << duration3.count() << " seconds" << endl;

    }

}



int main(int argc, char* argv[]) {

    int rank, size, choice = -1;



    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(size<2)

    {

    	cout<<"Processes size should be greater than or equals to 2"<<endl;

    	MPI_Finalize();

    	return 1;

    }



    while (choice != 0) {

        if (rank == 0) {

            cout << "\n\n===================== Welcome to Matrix Solver =====================\n\n";

            cout << "\n0-Exit\n1-Matrix Addition\n2-Matrix Multiplication\n3-Transpose\n\nEnter choice: ";

            cin >> choice;

        }



        MPI_Bcast(&choice, 1, MPI_INT, 0, MPI_COMM_WORLD);// ensuring that all processes have the same choice value as process 0 for parallel execution



        switch (choice) {

            case 0:

                MPI_Finalize();                

                return 0;

            case 1:

                Addition(size, rank);

                break;

            case 2:

            	Multiplication(size,rank);

            	break;

            case 3:

            	Transpose(size,rank);

            	break;

            default:

                if (rank == 0) {

                    cout << "\nWrong input, try again!\n";

                }

        }

    }



    MPI_Finalize();

    return 0;

}


