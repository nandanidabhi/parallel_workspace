#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <functional>
#define DEBUG 0 

#ifdef __cplusplus
extern "C" {
#endif

  void generateLCS(char* X, int m, char* Y, int n);
  void checkLCS(char* X, int m, char* Y, int n, int result);

#ifdef __cplusplus
}
#endif

void parallel_for(unsigned elements, unsigned trds,std:: function < void(int start, int end) > functor) {
    unsigned batch_size = elements / trds;
	 unsigned batch_remainder = elements % trds;
	
    std::vector < std::thread > my_threads(trds);
    
        for (unsigned i = 0; i < trds; ++i) {
            int start = i * batch_size;
            my_threads[i] = std::thread(functor, start, start + batch_size);
        }
		int st = trds * batch_size;
		functor(st,st+batch_remainder);
     std::for_each(my_threads.begin(), my_threads.end(), std::mem_fn( & std::thread::join));
}

int LCS(char* X, int m, char* Y, int n,int t) {

  int** C = new int*[m+1];
  for (int i=0; i<=m; ++i) {
	
    C[i] = new int[n+1];
    C[i][0] = 0;
  }
  for (int j=0; j<=n; ++j) {
    C[0][j] = 0;
  }

  for (int a=1; a<=m; ++a) {
    parallel_for(n, t, [ & ](int start, int end) {
	for (int b=1; b<=n; ++b) {
      if (X[a-1] == Y[b-1]) {

        C[a][b] = C[a-1][b-1] + 1; 
      } else {
        C[a][b] = std::max(C[a-1][b], C[a][b-1]);
      }
 
	}
	 });
  }
  

  int result = C[m][n];

  for (int i=0; i<=m; ++i) { 
    delete[] C[i];
  }
  delete[] C;
  
  return result;
}


int main (int argc, char* argv[]) {
  if (argc < 3) { std::cerr<<"usage: "<<argv[0]<<" <m> <n>"<<std::endl;
    return -1;
  }

  int m = atoi(argv[1]);
  int n = atoi(argv[2]);
  int t = atoi(argv[3]);

  // get string data 
  char *X = new char[m];
  char *Y = new char[n];
  generateLCS(X, m, Y, n);

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  int lcs = LCS(X, m, Y, n,t);
   

  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elpased_seconds = end-start;

  checkLCS(X, m, Y, n, lcs);
  std::cerr<<elpased_seconds.count()<<std::endl;

  delete[] X;
  delete[] Y;


  return 0;
}
