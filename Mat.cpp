//#include <cstdio>
//#include <stdlib.h>
//#include <ctime>
//#include <cstdint>
//#include <iostream>
//#include <vector>
//
//
//void getInvertibleMatrix(int n, int** array)
//{
//  int i = 0;
//  int j = 0;
//  int k = 0;
//  int mainRowNum = 0;
//
//  int* tempArray = NULL;
//
//  srand((int)time(NULL));
//  int transformTime = (int)(rand() % 1000);
//  printf("# We will do %d times tansformations to matrix E get a invertibel matrix.\n", transformTime);
//
//  tempArray = (int*)malloc(sizeof(int) * n);
//
//  for (i = 0; i < transformTime; ++i)
//  {
//    mainRowNum = (int)(rand() % (n - 1));
//    for (k = 0; k < n; ++k)
//      if (((UINT16_MAX - (array[mainRowNum][k]) * ((int)(rand() % 5 - 10))) < 0) || ((UINT16_MAX * (-1)) - (array[mainRowNum][k]) * ((int)(rand() % 5 - 10)) > tempArray[k]))
//        tempArray[k] = (array[mainRowNum][k]);
//      else
//        tempArray[k] = (array[mainRowNum][k]) * ((int)(rand() % 5 - 10));
//
//    for (j = 0; j < n; ++j)
//      if (mainRowNum != j)
//        for (k = 0; k < n; ++k)
//        {
//          if (((UINT16_MAX - array[j][k]) < tempArray[k]) || ((UINT16_MAX * (-1)) - array[j][k] > tempArray[k]))
//            array[j][k] = array[j][k] / 4;
//          else
//            array[j][k] = array[j][k] + tempArray[k];
//        }
//  }
//
//  free(tempArray);
//}
//
//void getIdentityMatrix(int n, int** array)
//{
//  int r = 0;
//  int c = 0;
//
//  for (r = 0; r < n; ++r)
//  {
//    for (c = 0; c < n; ++c)
//    {
//      if (r == c)
//        array[r][c] = 1;
//      else
//        array[r][c] = 0;
//    }
//  }
//
//}
//
//void printMatrix(int n, int **array) {
//  for (int r = 0; r < n; ++r)
//  {
//    for (int c = 0; c < n; ++c)
//    {
//      std::cout << array[r][c] << " ";
//    }
//    std::cout << '\n';
//  }
//}
//
//void freeMatrix(int n, int** array)
//{
//  int k = 0;
//  for (k = 0; k < n; ++k)
//  {
//    free(array[k]);
//  }
//  free(array);
//}
//
//int main(int argc, char* argv[])
//{
//  int k = 0;
//  int n = 0;
//  int flag = 0;
//  int** A;
//  n = 5;
//
//  printf("# The diamond of the matrix:\n");
//
//
//  //Create matrix
//  printf("# Start create a %d * %d matrix.\n", n, n);
//  A = (int**)malloc(sizeof(int*) * n);
//
//  for (k = 0; k < n; ++k)
//  {
//    A[k] = (int*)malloc(sizeof(int) * n);
//  }
//
//  getIdentityMatrix(n, A); //get indentity matrix
//  getInvertibleMatrix(n, A); //get invertible matrix
//  printf("# Generated invertible matrix: \n");
//  printMatrix(n, A); //print invertible matrix
//
//  return 0;
//}
