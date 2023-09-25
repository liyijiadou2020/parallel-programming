
#include <iostream>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <mpi.h>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>


#define MAX_MESSAGE_LENGTH 1000
#define MODULE_COUNT 5
//#define MODULE_COUNT 6

using namespace std;


/**
* примые подстановки
* @param
* strI - Начальный адрес строки
* length - длиндельность
* numB - отклонение подстановки
*/
void encry(char* strI, int length, int numB)
{    
  for (int i = 0; i < length; i++)
  {
    if (strI[i] >= 'A' && strI[i] <= 'Z')
    {
      strI[i] = ((strI[i] - 'A') + numB) % 26 + 'A';
    }

    else if (strI[i] >= 'a' && strI[i] <= 'z')
    {
      strI[i] = ((strI[i] - 'a') + numB) % 26 + 'a';
    }
  }  
}

void print_chars(const char* massage, int size) {
  for (int i = 0; i < size; i++) {
    cout << massage[i];
  }
  cout << "\n";
}

int main(int argc, char** argv) {
  int rank, size;
  MPI_Status status;
  int total_text_length = 0;

  // +++++++ random key +++++++
  vector<int> key = { 0, 1, 2, 3, 4 };  
  random_device rd;
  mt19937 g(rd());
  shuffle(key.begin(), key.end(), g);
  cout << "key: \n";
  for (auto i : key) {
    cout << i << " ";
  }
  // ++++++++++++++++++++++++++

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size != MODULE_COUNT) {
    if (rank == 0) {
      cout << "size = " << size << '\n';
      cout << "Error: program requires " << MODULE_COUNT << " processes to run" << endl;
    }
    MPI_Finalize();
    return 0;
  }

  char message[MAX_MESSAGE_LENGTH]; // #TODO，改成从文件读入
  
  if (rank == 0) {            
    cout << "\nEnter message to encrypt: ";
    cin.getline(message, MAX_MESSAGE_LENGTH); // #TODO
    
    total_text_length = strlen(message);
    int chunk_size = total_text_length / MODULE_COUNT; // 段落长度

    for (int i = 1; i < MODULE_COUNT; i++) {
      MPI_Send(&chunk_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);  // i - 发送对象， 0 - tag    
      MPI_Send(&message[(i - 1) * chunk_size], chunk_size, MPI_CHAR, i, 0, MPI_COMM_WORLD); // #TODO 根据题意，发送到那个进程由key决定
    }
    
    encry(&message[(MODULE_COUNT - 1) * chunk_size], total_text_length - (MODULE_COUNT - 1) * chunk_size, rank); // #TODO 根据题意，发送到那个进程由key决定
  } else {    
    int chunk_size = 0;
    char* chunk = (char*)malloc(chunk_size * sizeof(char));
    MPI_Recv(&chunk_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);    // 0 - 接收对象，第二个0 - tag
    MPI_Recv(chunk, chunk_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status); // 0 - 接收对象
    
    encry(chunk, chunk_size, rank);

    MPI_Send(&chunk_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    MPI_Send(chunk, chunk_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  }

  if (rank == 0) {    
    for (int i = 1; i < MODULE_COUNT; i++) {
      int chunk_size = 0;
      MPI_Recv(&chunk_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);      
      char* chunk = (char*)malloc(chunk_size * sizeof(char));
      MPI_Recv(chunk, chunk_size, MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);      

      for (int j = 0; j < chunk_size; j++) {
        message[(i - 1) * chunk_size + j] = chunk[j];
      }
    }
    cout << "ENCRPT FINISHED : " << endl;
    print_chars(message, total_text_length);
  }

  MPI_Finalize();
  return 0;
}
