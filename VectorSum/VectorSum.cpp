// VectorSum.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>


void sum(const int* A, const int* B, int* C, int n)
{
    for (int i = 0; i < n; i++)
        C[i] = A[i] + B[i];
}


const int ARRAY_SIZE = 25;
int main()
{

    int *A = new int[ARRAY_SIZE];
    int *B = new int[ARRAY_SIZE];
    int *C = new int[ARRAY_SIZE];

    const char* filename = "vector.txt";


    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "file error " << filename << "!" << std::endl;

        delete[] A;
        delete[] B;
        delete[] C;
        return 1;
    }


    for (int i = 0; i < ARRAY_SIZE; i++) {
        file >> A[i] >>  B[i];
    }

    file.close();

    sum(A, B, C, ARRAY_SIZE);

    for (int i = 0; i < ARRAY_SIZE; i++)
        std::cout << C[i] << " ";

    delete[] A;
    delete[] B;
    delete[] C;
}

