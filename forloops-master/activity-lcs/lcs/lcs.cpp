#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include "par_loop.hpp"
#include <mutex>
#include <thread>





#ifdef __cplusplus
extern "C" {
#endif

  void generateLCS(char* X, int m, char* Y, int n);
  void checkLCS(char* X, int m, char* Y, int n, int result);

#ifdef __cplusplus
}
#endif
int LCS(char* X, int m, char* Y, int n, int nbthread){

  ParLoop pl;		   
  //int** C = new int*[m+1];
  int result =0;
  std::mutex mu;
  pl.parfor<std::vector<std::vector<int>>>(0,m,n,nbthread,
  [&](std::vector<std::vector<int>> &tls) -> void{
    tls = std::vector<std::vector<int>>(m+1);
		     for(int i=0; i<=m; i++){
		       // tls[i] = new int[n+1];
		       tls[i] = std::vector<int>(n+1);
		       tls[i][0] =0;
		     }
		     for(int j =0; j<=n; j++){
		       tls[0][j] = 0;
		     }},
		     [&](std::vector<std::vector<int>> &tls, int k, int i, int j) -> void{
		       int a=i+k,b=j-k;
		        if(a==0||b==0){
		          tls[a][b] = 0;
		       }
		       else
		       //std::lock_guard<std::mutex> lg(mu);
			 if(X[a-1]==Y[b-1]){
			   tls[a][b] = tls[a-1][b-1]+1;
			 }
			 else
			   tls[a][b] = std::max(tls[a-1][b],tls[a][b-1]);			 
			 },
		    [&](std::vector<std::vector<int>> &tls) -> void{
		    // for(int i=0;i<=m;++i){
		    //	     for(int j=0;j<=n;++j){
		    //	       C[i][j]=tls[i][j];
		    //	     }
		    //	   }

		    result = tls[m][n];
		    //std::cout<<"result"+result<<std::endl;
			 }

		   );
  //int result = C[m][n];
  //for(int i=0; i<=m;++i){
  //delete[] C[i];
  //}
  //delete[] C;
  return result;
}


int main (int argc, char* argv[]) {

  if (argc < 4) { std::cerr<<"usage: "<<argv[0]<<" <m> <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int m = atoi(argv[1]);
  int n = atoi(argv[2]);
  int nbthread = atoi(argv[3]);

  // get string data 
  char *X = new char[m];
  char *Y = new char[n];
  generateLCS(X, m, Y, n);
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  
  //insert LCS code here.
  int result = LCS(X, m, Y, n, nbthread); // length of common subsequence
  
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elpased_seconds = end-start;


  checkLCS(X, m, Y, n, result);
  std::cerr<<elpased_seconds.count()<<std::endl;
  delete[] X;
  delete[] Y;


  return 0;
}
