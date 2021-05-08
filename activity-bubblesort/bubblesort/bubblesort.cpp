#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include "par_loop.hpp"
#include <mutex>


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

void swap(int *num1, int *num2){
  int temp = *num1;
  *num1 =*num2;
  *num2 = temp;
}


int main (int argc, char* argv[]) {
  if (argc < 3) { std::cerr<<"usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int nbthread = atoi(argv[2]);
  ParLoop p1;
  std::mutex mu;
  auto start = std::chrono::steady_clock::now();
  
  
  // get arr data
  int * arr = new int [n];
  generateMergeSortData (arr, n);

  //insert sorting code here.
  
  p1.parfor<std::vector<int>>(0,n,1,nbthread,[&](std::vector<int>& tls) -> void{
     for( int i=0;i<n;i++){
	  tls[i] = arr[i];
     }				  
  },
    [&](std::vector<int>& tls,int inc) -> void{
    // [&](int* tls, int x) -> void{
      	std::lock_guard<std::mutex> guard(mu);
      for(int i=inc;i<n;i+=2){
       	//std::lock_guard<std::mutex> guard(mu);
	//if(tls[i] > tls[i+1]){
	//swap(tls, i, i+1);
	//}

	if(tls[i-1]>tls[i]){
	  swap(&tls[i-1],&tls[i]);
	}
      }
    },
    [&](std::vector<int>& tls,int inc) -> void{
      	std::lock_guard<std::mutex> guard(mu);
      for(int i=inc;i<n;i+=2){

	if(tls[i-1]>tls[i]){
	  swap(&tls[i-1],&tls[i]);
	}
      }
    },
    
    [&](std::vector<int>& tls) -> void{
	for( int i=0;i<n;i+=1){
	  arr[i] = tls[i];
	  //std::cout<<arr[i];
     }
      });
  auto stop = std::chrono::steady_clock::now();
  std::chrono::duration<double> count1= stop-start;
  fprintf(stderr,"%f\n",count1.count());


  
  checkMergeSortResult (arr, n);
  
  delete[] arr;

  return 0;
}
