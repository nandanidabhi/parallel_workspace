#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
using namespace std::chrono;

#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif

  
int main (int argc, char* argv[]) {

  if (argc < 6) {
    std::cerr << "usage: " << argv[0] << " <functionid> <a> <b> <n> <intensity>" << std::endl;
    return -1;
  }

  auto start = high_resolution_clock::now();

  // sscanf(argv[1],"%d",&functionid);
  // sscanf(arvg[2],"%d",&a);
  // sscanf(argv[3],"%d",&b);
  // sscanf(argv[4],"%d",&n);
  // sscanf(argv[5],"%d",&intesity);
  
  int functionid = atoi(argv[1]);
  float a = strtof(argv[2],NULL);
  float b = strtof(argv[3],NULL);
  float n = strtof(argv[4],NULL);
  int intensity = atoi(argv[5]);
  float x;
  float t1 = (b - a) / n;
  float sum = 0;
  for(int i = 0; i < n; i++){
    x = a + ((i + 0.5) * t1);
    // if(functionid == 1) f = f1(x,intensity);
    //  else if(functionid == 2) f = f2(x,intensity);
    //   else if(functionid == 3) f = f3(x,intensity);
    //   else if(functionid == 4) f = f4(x,intensity);
    //   else return -1;

    switch(functionid){
      case 1: sum += f1(x,intensity);
	break;
	case 2: sum += f2(x,intensity);
	  break;
	  case 3: sum += f3(x,intensity);
	    break;
	    case 4:sum += f4(x,intensity);
	      break;
	      default: return -1;
    }
  }
  std::cout << sum * t1 << std::endl;
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<seconds>(stop - start);
  //printf(duration.count());
  std::cerr << duration.count() << std::endl;
  
  return 0;
}
