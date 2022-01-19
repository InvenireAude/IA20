
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

	SYS::Signal::ThreadRegistration tr;

    int fdRequests[2];
   int fdResponses[2];

   int flags = 0 | O_NONBLOCK;

   pipe2(fdRequests,  flags);
   pipe2(fdResponses,  flags);

   unique_ptr<TCP::Listener> ptrListener(new TCP::Listener(fdResponses[0], fdRequests[1], 1000));

   unique_ptr<Engine> ptrEngine(new Engine());

   ptrEngine->addListener(ptrListener.get(), fdRequests[0], fdResponses[1]);

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