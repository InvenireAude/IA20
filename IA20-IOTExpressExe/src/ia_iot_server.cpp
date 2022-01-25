
#include <ia20/commonlib/commonlib.h>
#include <ia20/iot/tcp/Listener.h>
#include <ia20/iot/tools/sys/TaskPort.h>
#include <ia20/iot/Engine.h>

#include <fcntl.h>
#include <unistd.h>

using namespace IA20;
using namespace IA20::IOT;
using namespace std;

int main(){

   unique_ptr<TCP::Listener> ptrListener(new TCP::Listener(1000));

   unique_ptr<Engine> ptrEngine(new Engine());

   ptrEngine->addListener(ptrListener.get());

   ptrListener->start();

    try{
    
        while(!SYS::Signal::GetInstance()->isStopping()){    
            ptrEngine->serve();
        }
    
     }catch(Exception& e){
        e.printToStream(std::cerr);
    }

    ptrListener->join();
}