#include <chrono>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <thread>
#include <algorithm>
#include <functional>

static void parallel_for(unsigned elements, unsigned trds,std:: function < void(int start, int end) > functor) {
    unsigned batch_size = elements / trds;
    std::vector < std::thread > my_threads(trds);
    
        for (unsigned i = 0; i < trds; ++i) {
            int start = i * batch_size;
            my_threads[i] = std::thread(functor, start, start + batch_size);
        }
     std::for_each(my_threads.begin(), my_threads.end(), std::mem_fn( & std::thread::join));
}

#ifdef __cplusplus

extern "C" {

#endif

void generatePrefixSumData(int * arr, size_t n);
void checkPrefixSumResult(int * arr, size_t n);

#ifdef __cplusplus

}

#endif

int main(int argc, char * argv[]) {
    
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <n>" << std::endl;
        return -1;
        
    }
    
    int n = atoi(argv[1]); 
    int t = atoi(argv[2]); 
    int * arr = new int[n];
    
    generatePrefixSumData(arr, n);
    std::chrono::time_point < std::chrono::system_clock > start = std::chrono::system_clock::now();
    int * prefix = new int[n + 1];
    prefix[0] = 0;
    parallel_for(n, t, [ & ](int start, int end) {
        for (int i = 0; i < n; ++i) {
            prefix[i + 1] = prefix[i] + arr[i];
        }
    });
    std::chrono::time_point < std::chrono::system_clock > end = std::chrono::system_clock::now();
    std::chrono::duration < double > elapsed_seconds = end - start;
    std::cerr << elapsed_seconds.count() << std::endl;
    
    checkPrefixSumResult(prefix, n);
    
    delete[] arr;
    delete[] prefix;
    
    return 0;
    
}
