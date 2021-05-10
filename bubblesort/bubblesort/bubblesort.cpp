#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <cmath>
#include <sys/time.h>
#include "parallel_loop.hpp"

#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (const int* arr, size_t n);

#ifdef __cplusplus
}
#endif

void swap(std::vector<int>& arr, int i, int j) {
  int temp = arr[i];
  arr[i] = arr[j];
  arr[j] = temp;
}

int main (int argc, char* argv[]) {
  if (argc < 3) { std::cerr<<"usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int nbthread = atoi(argv[2]);
  
  ParLoop par;

  // get arr data
  int * arr = new int [n];
  generateMergeSortData (arr, n);

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  
  par.parfor<int>(0, n, 1, nbthread,[&](std::vector<int>& tls) -> void{
    for(int i=0; i<n; i++){
      tls[i] = arr[i];
    }
  },
  [&](std::vector<int>& tls, int a) -> void{
    for(int i=a; i<n-1; i+=2){
      if(tls[i] > tls[i+1]){
        swap(tls, i, i+1);
      }
    } 
  },
  [&](std::vector<int>& tls) -> void{
    for(int i=0; i<n; i++){
      arr[i] = tls[i];
      }  
    });

  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elpased_seconds = end-start;

  checkMergeSortResult (arr, n);
  std::cerr<<elpased_seconds.count()<<std::endl; 

  delete[] arr;
  
  return 0;
}
