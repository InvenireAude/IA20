
#include <ia20/commonlib/commonlib.h>
#include <ia20/iot/tcp/Listener.h>
#include <ia20/iot/tools/spin/TasksRing.h>
#include <ia20/iot/Engine.h>

using namespace IA20;
using namespace IA20::IOT;
using namespace std;

int main(){

	SYS::Signal::ThreadRegistration tr;

    unique_ptr<TCP::Listener> ptrListener(new TCP::Listener(
            IA20::IOT::Tools::SPIN::TasksRing<Listener::Task>::CreateInterface(50), 1000));

    ptrListener->start();

    unique_ptr<Engine> ptrEngine(new Engine(ptrListener.get()));

    try{
    
        while(!SYS::Signal::GetInstance()->isStopping()){    
            ptrEngine->serve();
        }
    
     }catch(Exception& e){
        e.printToStream(std::cerr);
    }

    ptrListener->join();
}