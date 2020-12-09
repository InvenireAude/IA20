#include <memory>

#include <ia20/commonlib/commonlib.h>
#include <ia20/net/engine/raft/LogFileReader.h>



using namespace IA20;
using namespace IA20::Net::Engine;
using namespace std;


int main(int argc, char* argv[]){

  SYS::Signal::GetInstance();

  for(int i=1; i < argc; i++){
    std::unique_ptr<Raft::LogFileReader> ptrReader(new Raft::LogFileReader(argv[i]));
    ptrReader->dump(std::cout);
  }
}