#ifndef __PAR_LOOP_H
#define __PAR_LOOP_H

#include <functional>
#include <omp.h>

class ParLoop {
public:
  /// @brief execute the function f multiple times with different
  /// parameters possibly in parallel
  ///
  /// f will be executed multiple times with parameters starting at
  /// beg, no greater than end, in inc increment. These execution may
  /// be in parallel
  void parfor (size_t beg, size_t end, size_t inc,
	       std::function<void(int)> f) {
    for (size_t i=beg; i<end; i+= inc) {
      f(i);
    }
  }

  /// @brief execute the function f multiple times with different
  /// parameters possibly in parallel
  ///
  /// f will be executed multiple times with parameters starting at
  /// beg, no greater than end, in inc increment. These execution may
  /// be in parallel.
  ///
  /// Each thread that participate in the calculation will have its
  /// own TLS object.
  ///
  /// Each thread will execute
  //function before prior to any
  /// calculation on a TLS object.
  ///
  /// Each thread will be passed the TLS object it executed before on.
  ///
  /// Once the iterations are complete, each thread will execute after
  /// on the TLS object. No two thread can execute after at the same time.
  template<typename TLS>
  
  void parfor (size_t beg, size_t end_X, size_t end_Y, size_t nbthread,
	       std::function<void(TLS&)> before,
	       std::function<void(TLS&, int, int, int)> f,
	       std::function<void(TLS&)> after
	       ) {
    //TLS tls= new int*[end_X+1];
    TLS tls;
    before(tls);    
    
    //int diag = std::min(end_X, end_Y);
    //for(int i = 1; i<=diag; ++i){
    // f(tls,i,i);
    //for(int j = i;j<=end_Y; ++j){
    //	f(tls,i,j);
    //	
    //}
    //for(int k = i;k<=end_X;++k){
    //	f(tls,k,i);
    //}  
    //}
    int ad = 0;
    for( int i =0,j=0;j<=end_Y &&i<=end_X;j++){
      ad = std::min(j,(int)end_X-i);
      if(ad<=nbthread){
	#pragma omp parallel for
      for(int k=0;k<=ad;++k){

	f(tls,k,i,j);
	if(j>=end_Y)
	  j=end_Y-1,i++;
	
      }
      }
      else{
	for(int t=0;t<ad;t+=nbthread){
	  #pragma omp parallel for
	  for(int k =t;k<t+nbthread && k<=ad;++k){
	    f(tls,k,i,j);
	     if(j>=end_Y)
	       j=end_Y-1,i++;
	  }
	}
      }     
      }

    after(tls);
  }  
};

#endif
