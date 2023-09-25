//#include <cstdio>
//#include <stdlib.h>
//#include <ctime>
//#include <cstdint>
//#include <iostream>
//#include <vector>
//#include <omp.h>
//
//using std::vector;
//using std::endl;
//using std::cout;
//
//int MAX_THREADS_NUMBER = 0;
//
//void getIdentityMatrix(int n, int **array);
//
//void getInvertibleMatrix(int n, int **array);
//
//vector<vector<double>> array_cast(int **array, int n);
//
//vector<vector<double>> generate_identity(int size);
//
//vector<vector<double>> generate_inverse_serial(vector<vector<double>> input_matrix);
//
//vector<vector<double>> generate_inverse_parallel(vector<vector<double>> input_matrix, int threads);
//
//void print_matrix(vector<vector<double>> matrix);
//
//void method_1(vector<vector<double>> matrix, int threads_num);
//
//void method_2(vector<vector<double>> matrix);
//
//vector<vector<double>> generate_inverse_parallel(vector<vector<double>> input_matrix, int threads) {
//    int size = input_matrix.size();
//    // 设置主对角线的标记
//    vector<vector<double>> I = generate_identity(size);
//    for (int i = 0; i < size; i++) {
//        if (input_matrix[i][i] == 0) {
//            // swap nearest subsequent row s.t input_matrix[i][i] != 0 after swapping
//            // 如果交换后 input_matrix[i][i] != 0 ，则交换最近的后一行
//            for (int j = i + 1; j < size; j++) {
//                if (input_matrix[j][i] != 0.0) {
//                    swap(input_matrix[i], input_matrix[j]);
//                    break;
//                }
//                if (j == size - 1) {
//                    cout << "Inverse does not exist for this matrix";
//                    exit(1);
//                }
//            }
//        }
//        double scale = input_matrix[i][i];
//        if (MAX_THREADS_NUMBER < threads) {
//            std::cout << "Not enough thread.\n";
//        } else {
//            omp_set_num_threads(threads);
//        }
//
//#pragma omp parallel for
//        for (int col = 0; col < size; col++) {
//            input_matrix[i][col] = input_matrix[i][col] / scale;
//            I[i][col] = I[i][col] / scale;
//        }
//        if (i < size - 1) {
//#pragma omp parallel for
//            for (int row = i + 1; row < size; row++) {
//                double factor = input_matrix[row][i];
//                for (int col = 0; col < size; col++) {
//                    input_matrix[row][col] -= factor * input_matrix[i][col];
//                    I[row][col] -= factor * I[i][col];
//                }
//            }
//        }
//    }
//    for (int zeroing_col = size - 1; zeroing_col >= 1; zeroing_col--) {
//#pragma omp parallel for
//        for (int row = zeroing_col - 1; row >= 0; row--) {
//            double factor = input_matrix[row][zeroing_col];
//            for (int col = 0; col < size; col++) {
//                input_matrix[row][col] -= factor * input_matrix[zeroing_col][col];
//                I[row][col] -= factor * I[zeroing_col][col];
//            }
//        }
//    }
//    return I;
//}
//
//vector<vector<double>> generate_identity(int size) {
//    vector<vector<double>> I;
//    for (int i = 0; i < size; i++) {
//        vector<double> row;
//        for (int j = 0; j < size; j++) {
//            if (i == j) {
//                row.push_back(1);
//                continue;
//            }
//            row.push_back(0);
//        }
//        I.push_back(row);
//    }
//    return I;
//}
//
//vector<vector<double>> generate_inverse_serial(vector<vector<double>> input_matrix) {
//    signed int size = input_matrix.size();
//    signed int i = 0;
//    vector<vector<double>> I = generate_identity(size);
//    for (i = 0; i < size; i++) {
//        if (input_matrix[i][i] == 0) {
//            // swap nearest subsequent row s.t input_matrix[i][i] != 0 after swapping
//            // 如果交换后 input_matrix[i][i] != 0 ，则交换最近的后一行
//            for (int j = i + 1; j < size; j++) {
//                if (input_matrix[j][i] != 0.0) {
//                    swap(input_matrix[i], input_matrix[j]);
//                    break;
//                }
//                if (j == size - 1) {
//                    cout << "Inverse does not exist for this matrix";
//                    exit(0);
//                }
//            }
//        }
//        double scale = input_matrix[i][i];
//        for (int col = 0; col < size; col++) {
//            input_matrix[i][col] = input_matrix[i][col] / scale;
//            I[i][col] = I[i][col] / scale;
//        }
//        if (i < size - 1) {
//            for (int row = i + 1; row < size; row++) {
//                double factor = input_matrix[row][i];
//                for (int col = 0; col < size; col++) {
//                    input_matrix[row][col] -= factor * input_matrix[i][col];
//                    I[row][col] -= factor * I[i][col];
//                }
//            }
//        }
//    }
//    for (int zeroing_col = size - 1; zeroing_col >= 1; zeroing_col--) {
//        for (int row = zeroing_col - 1; row >= 0; row--) {
//            double factor = input_matrix[row][zeroing_col];
//            for (int col = 0; col < size; col++) {
//                input_matrix[row][col] -= factor * input_matrix[zeroing_col][col];
//                I[row][col] -= factor * I[zeroing_col][col];
//            }
//        }
//    }
//    return I;
//}
//
//vector<vector<double>> array_cast(int **array, int n) { // int** -> vector<vector<double>>
//    vector<vector<double>> cast_result;
//    for (int r = 0; r < n; ++r) {
//        vector<double> row;
//        for (int c = 0; c < n; ++c) {
//            row.push_back(array[r][c]);
//        }
//        cast_result.push_back(row);
//    }
//    return cast_result;
//}
//
//void getInvertibleMatrix(int n, int **array) {
//    int mainRowNum = 0;
//
//    int *tempArray = NULL;
//
//    srand((int) time(NULL));
//    int transformTime = (int) (rand() % 10 + 1); // transformation limits to 10 times.
//    printf("# We will do %d times tansformations to matrix E get a invertibel matrix.\n", transformTime);
//
//    tempArray = (int *) malloc(sizeof(int) * n);
//
//    for (int i = 0; i < transformTime; ++i) {
//        mainRowNum = (int) (rand() % (n - 1));
//        for (int k = 0; k < n; ++k)
//            if (((UINT16_MAX - (array[mainRowNum][k]) * ((int) (rand() % 5 - 10))) < 0) ||
//                ((UINT16_MAX * (-1)) - (array[mainRowNum][k]) * ((int) (rand() % 5 - 10)) > tempArray[k]))
//                tempArray[k] = (array[mainRowNum][k]);
//            else
//                tempArray[k] = (array[mainRowNum][k]) * ((int) (rand() % 5 - 10));
//
//        for (int j = 0; j < n; ++j)
//            if (mainRowNum != j)
//                for (int k = 0; k < n; ++k) {
//                    if (((UINT16_MAX - array[j][k]) < tempArray[k]) ||
//                        ((UINT16_MAX * (-1)) - array[j][k] > tempArray[k]))
//                        array[j][k] = array[j][k] / 4;
//                    else
//                        array[j][k] = array[j][k] + tempArray[k];
//                }
//    }
//
//    free(tempArray);
//}
//
//void getIdentityMatrix(int n, int **array) {
//    for (int r = 0; r < n; ++r) {
//        for (int c = 0; c < n; ++c) {
//            if (r == c)
//                array[r][c] = 1;
//            else
//                array[r][c] = 0;
//        }
//    }
//}
//
//void print_matrix(vector<vector<double>> matrix) {
//    for (size_t i = 0; i < matrix.size(); i++) {
//        for (size_t j = 0; j < matrix.size(); j++) {
//            cout << matrix[i][j] << " ";
//        }
//        cout << endl;
//    }
//    cout << endl;
//}
//
//void method_1(vector<vector<double>> matrix, int threads_num) {
//    printf(" -------------- \n");
//    printf("| Method 1: threads number: %d, matrix A:\n", threads_num);
//    print_matrix(matrix);
//
//    printf("# A^(-1) (Parallel computing): \n");
//    auto inv_matrix_2 = generate_inverse_parallel(matrix, threads_num);
//    print_matrix(inv_matrix_2);
//}
//
//void method_2(vector<vector<double>> matrix) {
//    printf(" -------------- \n");
//    int threads_num = (int) (rand() % MAX_THREADS_NUMBER + 1);
//
//    printf("| Method 2: threads number(random): %d, matrix A:\n", threads_num);
//    print_matrix(matrix);
//
//    printf("# A^(-1) (Parallel computing): \n");
//    auto inv_matrix_2 = generate_inverse_parallel(matrix, threads_num);
//    print_matrix(inv_matrix_2);
//}
//
//
//int main(int argc, char *argv[]) {
//    int n = 5;
//    int threads_num = 3;
//    MAX_THREADS_NUMBER = omp_get_max_threads();
//    printf("# Max threads number of computer: %d\n", MAX_THREADS_NUMBER);
//
//    printf("# Start create a %d * %d matrix.\n", n, n);
//    int **A; //Create matrix
//    A = (int **) malloc(sizeof(int *) * n);
//    for (int k = 0; k < n; ++k) {
//        A[k] = (int *) malloc(sizeof(int) * n);
//    }
//    getIdentityMatrix(n, A); //get indentity matrix
//    getInvertibleMatrix(n, A); //get invertible matrix
//    vector<vector<double>> matrix = array_cast(A, n);
//    printf("# Generated invertible matrix(A): \n");
//    print_matrix(matrix);
//
//    printf("# A^(-1) (Serial computing): \n");
//    auto inv_matrix = generate_inverse_serial(matrix);
//    print_matrix(inv_matrix);
//
//    method_1(matrix, threads_num);
//    method_2(matrix);
//
//    return 0;
//}
