// VectorSumCL.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <CL/cl.h>


const char* kernelSource =
"__kernel void sum(__global const int *A, __global const int *B, __global  int *C) "
" { int i = get_global_id(0);   C[i] = A[i] + B[i]; }";

const int ARRAY_SIZE = 25;

int main()
{

    int* A = new int[ARRAY_SIZE];
    int* B = new int[ARRAY_SIZE];
    int* C = new int[ARRAY_SIZE];

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
        file >> A[i] >> B[i];
    }

    file.close();



    // --- OpenCL setup ---
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;
    cl_int err;

    //получение первой платформы из списка
    clGetPlatformIDs(1, &platform, NULL);
    //получение первого устройства на платформе. тип устройства: CL_DEVICE_TYPE_CPU CL_DEVICE_TYPE_GPU
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    //создание контекста и очереди
    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    queue = clCreateCommandQueueWithProperties(context, device, 0, &err);

    //создание программы из исходного кода ядра
    program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, &err);
    //компиляция программы
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    //создание ядра
    kernel = clCreateKernel(program, "sum", &err);

    // выделение памяти в контексте устройства
    cl_mem bufA = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int) * ARRAY_SIZE, NULL, &err);
    cl_mem bufB = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int) * ARRAY_SIZE, NULL, &err);
    cl_mem bufC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(int) * ARRAY_SIZE, NULL, &err);

    size_t global = ARRAY_SIZE;


    // Обновляем буферы на устройстве
    clEnqueueWriteBuffer(queue, bufA, CL_TRUE, 0, sizeof(int) * ARRAY_SIZE, A, 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, bufB, CL_TRUE, 0, sizeof(int) * ARRAY_SIZE, B, 0, NULL, NULL);

    // Настраиваем аргументы кернела
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufA);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufB);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufC);


    // Запускаем кернел
    clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global, NULL, 0, NULL, NULL);
    clFinish(queue);

    // Читаем результаты (метки кластеров)
    clEnqueueReadBuffer(queue, bufC, CL_TRUE, 0, sizeof(int) * ARRAY_SIZE, C, 0, NULL, NULL);

 

    // --- Вывод результатов ---
    for (int i = 0; i < ARRAY_SIZE; i++)
        std::cout << C[i] << " ";


    // Очистка
    clReleaseMemObject(bufA);
    clReleaseMemObject(bufB);
    clReleaseMemObject(bufC);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    delete[] A;
    delete[] B;
    delete[] C;
    return 0;
}

