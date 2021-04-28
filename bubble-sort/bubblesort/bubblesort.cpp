#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include "par_loop.hpp"
#include <unistd.h>
#include <chrono>

#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (const int* arr, size_t n);

#ifdef __cplusplus
}
#endif

void swap(int* arr, int i, int j) {
  int temp = arr[i];
  arr[i] = arr[j];
  arr[j] = temp;
}

int main (int argc, char* argv[]) {
  if (argc < 3) { std::cerr<<"usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }
  auto start = std::chrono::steady_clock::now();
  int n = atoi(argv[1]);
  int nbthread = atoi(argv[2]);
  ParLoop p1;
  
  // get arr data
  int * arr = new int [n];
  generateMergeSortData (arr, n);

  //insert sorting code here.
  
  p1.parfor<void>(0,n,1,nbthread,[&](int* tls) -> void{
  tls = arr;
  },
    [&](int*tls,int x) -> void{ 
      for(int i=x;i<n-1;i+=2){
	//	std::lock_guard<std::mutex> lg(mu);
	if(tls[i] > tls[i+1]){
	  swap(tls, i, i+1);
	}
      }
    },
      [&](int* tls) -> void{
	arr = tls;
      });

  auto stop = std::chrono::stedy_clock::now();
  std::chrono::duration<doble> count1= stop-start;
  fprintf(stderr,"%f\n",count1.count());
  
  checkMergeSortResult (arr, n);
  
  delete[] arr;

  return 0;
}
