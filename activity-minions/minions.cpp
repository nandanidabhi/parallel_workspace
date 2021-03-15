#include <iostream>
#include <thread>
#include <vector>

void createMinions(int n){
  std::cout << "Hello! I am a minion. " <<n <<std::endl;
}
void finish(){
  using namespace std::chrono_literals;
  std::cout<<"Hello Minions!, I am the Overlord!"<<"\n";
}



int main (int argc, char** argv) {
  if (argc < 2) {
    std::cerr<<"usage: "<<argv[0]<<" <nbminions>\n";
    return -1;
  }

  int j = atoi(argv[1]);
  std::cout<<"argv:"<<j<<"\n";
  std::vector<std::thread> threads;
  for(int i =1; i<j+1;i++){
    threads.push_back(std::thread(createMinions,i));
    // std::thread printThread(createMinions, i);
    //printThread.detach();
  }

  for(auto &th : threads){
    th.join();
  }
  
  std::thread endThread(finish);
    
  //Your code goes here
  endThread.join();
  
  return 0;
}
