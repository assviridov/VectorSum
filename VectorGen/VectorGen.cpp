#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>


const int ARRAY_SIZE = 25;

int main() {

    std::srand(std::time(nullptr));


    int* array1 = new int[ARRAY_SIZE];
    int* array2 = new int[ARRAY_SIZE];


    if (array1 == nullptr || array2 == nullptr) {
        std::cout << "memory error" << std::endl;
        return 1;
    }


    for (int i = 0; i < ARRAY_SIZE; i++) {
        array1[i] = std::rand() % 201 - 100;  // числа от -100 до 100
        array2[i] = std::rand() % 201 - 100;
    }

    const char* filename = "vector.txt";


    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cout << "file error" << filename << "!" << std::endl;


        delete[] array1;
        delete[] array2;

        return 1;
    }


    for (int i = 0; i < ARRAY_SIZE; i++) {
        file << array1[i] << " " << array2[i] << std::endl;
    }

    file.close();

    delete[] array1;
    delete[] array2;


    std::cout << "Done" << std::endl;

    return 0;
}