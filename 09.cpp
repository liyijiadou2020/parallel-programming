
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
//#define MODULE_COUNT 5
#define MODULE_COUNT 6

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

void print_vector(vector<int> vec) {
  for (auto i : vec) {
    cout << i << " ";
  }
}

void print_chars(const char* massage, int size) {
  for (int i = 0; i < size; i++) {
    cout << massage[i];
  }
  cout << "\n";
}

vector<int>generate_random_key(int size) {
  vector<int> key;
  for (int i = 0; i < size; ++i) {
    key.push_back(i);
  }
  // +++++++ random  +++++++  
  random_device rd;
  mt19937 g(rd());
  shuffle(key.begin(), key.end(), g);
  cout << "key: \n";
  print_vector(key);
  // ++++++++++++++++++++++++
  return key;
}


int main(int argc, char** argv) {
  int rank, size;
  MPI_Status status;  
  
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
    vector<int> key = generate_random_key(MODULE_COUNT);
    cout << "\nEnter message to encrypt: ";
    cin.getline(message, MAX_MESSAGE_LENGTH); // #TODO
    
    int total_text_length = strlen(message);
    int chunk_size = total_text_length / MODULE_COUNT; // 段落长度

    for (int i = 1; i < MODULE_COUNT; i++) {
      if (key[i] == MODULE_COUNT - 1) {
        int lastpart_size = total_text_length - (MODULE_COUNT - 1) * chunk_size;
        MPI_Send(&lastpart_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);  // 字符数量
        MPI_Send(&key[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);  // 偏移量
        MPI_Send(&total_text_length, 1, MPI_INT, i, 0, MPI_COMM_WORLD);  // 消息总长度
        MPI_Send(&message[key[i] * chunk_size], lastpart_size, MPI_CHAR, i, 0, MPI_COMM_WORLD);  // 起始地址
      }
      else {
        MPI_Send(&chunk_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);  // 字符数量
        MPI_Send(&key[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);  // 偏移量
        MPI_Send(&total_text_length, 1, MPI_INT, i, 0, MPI_COMM_WORLD);  // 消息总长度
        MPI_Send(&message[key[i] * chunk_size], chunk_size, MPI_CHAR, i, 0, MPI_COMM_WORLD);  // 起始地址
      }      
      //MPI_Send(&message[(i - 1) * chunk_size], chunk_size, MPI_CHAR, i, 0, MPI_COMM_WORLD);
    }    
    
    int local_key = key[0];
    cout << "process " << rank << " key = " << local_key << "\n";        
    // 自己的偏移量是key[0]
    if (local_key == MODULE_COUNT - 1) {
      // 包括最后的所有
      encry(&message[(MODULE_COUNT - 1) * chunk_size], total_text_length - (MODULE_COUNT - 1) * chunk_size, rank);
      print_chars(&message[(MODULE_COUNT - 1) * chunk_size], total_text_length - (MODULE_COUNT - 1) * chunk_size);
    }
    else {
      // 常规长度
      encry(&message[local_key * chunk_size], chunk_size, rank);
      print_chars(&message[local_key * chunk_size], chunk_size);
    }
    //encry(&message[(MODULE_COUNT - 1) * chunk_size], total_text_length - (MODULE_COUNT - 1) * chunk_size, rank);
  } 
  else // ****************** 其他线程 ********************
  {    
    int chunk_size = 0;
    int local_key = 0;
    int total_text_length = 0;
    char* chunk = (char*)malloc(chunk_size * sizeof(char));

    MPI_Recv(&chunk_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);    // 字符数量
    MPI_Recv(&local_key, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);    // 偏移量
    MPI_Recv(&total_text_length, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);    // 消息总长度
    MPI_Recv(chunk, chunk_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status); // 起始地址
    
    cout << "process " << rank << " key = " << local_key << "\n";
    cout << "before: \n";
    print_chars(chunk, chunk_size);
    encry(chunk, chunk_size, rank);
    cout << "after: \n";
    print_chars(chunk, chunk_size);
    
    MPI_Send(&chunk_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); // 字符数量
    MPI_Send(&total_text_length, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); // 消息总长度
    MPI_Send(chunk, chunk_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD); // 起始地址
    
  }

  if (rank == 0) {
    int total_text_length = 0;
    for (int i = 1; i < MODULE_COUNT; i++) {
      int chunk_size = 0;      
      char* chunk = (char*)malloc(chunk_size * sizeof(char));
      MPI_Recv(&chunk_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);     
      MPI_Recv(&total_text_length, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);      
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
