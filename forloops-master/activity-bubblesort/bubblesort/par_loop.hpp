#ifndef __PAR_LOOP_H
#define __PAR_LOOP_H

#include <functional>

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
  /// Each thread will execute function before prior to any
  /// calculation on a TLS object.
  ///
  /// Each thread will be passed the TLS object it executed before on.
  ///
  /// Once the iterations are complete, each thread will execute after
  /// on the TLS object. No two thread can execute after at the same time.
  template<typename TLS>

  void parfor (size_t beg, size_t end, size_t increment, size_t nbthread,
	       std::function<void(TLS&)> before,
	       std::function<void(TLS&, int)> f,
	       std::function<void(TLS&, int)> g,
	       std::function<void(TLS&)> after
	       ) {
    TLS tls;
    before(tls);
    // for(size_t t = 0;t<end; ++t){
    //std::cout<<tls[t];
    //}
    //int x =0;
    int x;
    for(size_t t = 0;t<end; ++t){
       x =t%2;
       if(x==1){
	 f(tls,2);
       }
       else{
	 g(tls,1);
       }
       
     
     }
    after(tls);
  }
  
};

#endif

