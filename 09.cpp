
#include <iostream>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <mpi.h>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>


#define MAX_MESSAGE_LENGTH 10000
#define MODULE_COUNT 5

using namespace std;


/**
* примые подстановки
* 
* @param
* strI - Начальный адрес строки
* length - длиндельность
* numB - отклонение подстановки
*/
void encrypt(char* strI, int length, int numB)
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
  for (int i = 1; i < size; ++i) {
    key.push_back(i);
  }
  // +++++++ random key +++++++  
  random_device rd;
  mt19937 g(rd());
  shuffle(key.begin(), key.end(), g);
  cout << "key: \n";
  print_vector(key);
  // +++++++++++++++++++++++++
  return key;
}


int main(int argc, char** argv) {
  int rank, size;
  MPI_Status status;  
  vector<int> key;
  int total_text_length;
    
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size != MODULE_COUNT) {
    if (rank == 0) {
      cout << "size = " << size << '\n';
      cout << "[ERROR]: program requires " << MODULE_COUNT << " processes to run" << "\n";
    }
    MPI_Finalize();
    return 0;
  }

  //char message[MAX_MESSAGE_LENGTH];
  char* message = (char*)malloc(MAX_MESSAGE_LENGTH * sizeof(char));
  
  if (rank == 0) {            
    key = generate_random_key(MODULE_COUNT);
    cout << "\nEnter message to encryptpt: ";
    cin.getline(message, MAX_MESSAGE_LENGTH);
    
    total_text_length = strlen(message);
    // text is devided into(MODULE_COUNT - 1) paragraphs
    int chunk_size = total_text_length / (MODULE_COUNT - 1); // length of paragraph 
    cout << "\n\n >> total_text_length : " << total_text_length << "\n";

    for (int i = 0; i < MODULE_COUNT - 2; i++) {
      //cout << "0 -----> " << key[i] << " size : " << chunk_size << " sent paragraph : ";
      //print_chars(&message[i * chunk_size], chunk_size);
      MPI_Send(&chunk_size, 1, MPI_INT, key[i], 0, MPI_COMM_WORLD);  // length of paragraph     
      MPI_Send(&message[  i * chunk_size  ], chunk_size, MPI_CHAR, key[i], 0, MPI_COMM_WORLD);  // start address of paragraph
    }    
    int lastpart_size = total_text_length - (MODULE_COUNT - 2) * chunk_size;
    //cout << "0 -----> " << key[MODULE_COUNT - 2] << " size : " << lastpart_size << " sent paragraph : ";
    //print_chars(&message[(MODULE_COUNT - 2) * chunk_size], lastpart_size);
    MPI_Send(&lastpart_size, 1, MPI_INT, key[MODULE_COUNT - 2], 0, MPI_COMM_WORLD);  // length of paragraph  
    MPI_Send(&message[  (MODULE_COUNT - 2) * chunk_size ], lastpart_size, MPI_CHAR, key[MODULE_COUNT - 2], 0, MPI_COMM_WORLD); // start address of paragraph

  } 
  else
  {    
    int chunk_size = 0;
    int total_text_length = 0;    
    MPI_Recv(&chunk_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    char* chunk = (char*)malloc(chunk_size * sizeof(char));
    MPI_Recv(chunk, chunk_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
    
    //cout << "-------------------\nprocess " << rank << " : ";
    //cout << "before: ";
    //print_chars(chunk, chunk_size);
    
    encrypt(chunk, chunk_size, rank);
    
    //cout << "\n >after: ";
    //print_chars(chunk, chunk_size);
    //cout << "\n";
    
    MPI_Send(&chunk_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); // 字符数量
    MPI_Send(chunk, chunk_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD); // 起始地址
  }

  if (rank == 0) {
    string received_str = "";

    for (int i = 0; i < MODULE_COUNT - 1; i++) {
      int chunk_size = 0;
      MPI_Recv(&chunk_size, 1, MPI_INT, key[i], 0, MPI_COMM_WORLD, &status);
      char* chunk = (char*)malloc(chunk_size * sizeof(char));
      MPI_Recv(chunk, chunk_size, MPI_CHAR, key[i], 0, MPI_COMM_WORLD, &status);            
      
      for (int j = 0; j < chunk_size; j++) {        
        received_str += chunk[j];
      }
    }

    cout << "\n >>>>>>>>> [SUCCEED!] ENCRYPT FINISHED : <<<<<<<<<< \n";
    cout << received_str;
  }
  
  MPI_Finalize();
  return 0;
}
