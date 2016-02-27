#include "in-out.h"
#include <string>
#include <sys/select.h>
#include <termios.h>
#include <sys/ioctl.h>

ThreadMutex::ThreadMutex()
{
	int ini3 = pthread_mutex_init(&mtx,NULL);
	if(ini3 != 0)
    {
        printf("mutex init failed \n");
        exit(1);
    }
}

extern ThreadMutex cout_mutex;

void PString(const std::string& str)
{
	cout_mutex.lock();
	std::cout << str << std::endl;
	cout_mutex.unlock();
}

void PInt(const int var)
{
	cout_mutex.lock();
	std::cout << var << std::endl;
	cout_mutex.unlock();
}

 int _kbhit()
 {
        static const int STDIN = 0;
        static bool initialized = false; if (! initialized)
        {
            // Use termios to turn off line buffering
            termios term;
            tcgetattr(STDIN, &term);
            term.c_lflag &= ~ICANON;
            tcsetattr(STDIN, TCSANOW, &term);
            setbuf(stdin, NULL);
            initialized = true;
        } int bytesWaiting;
        ioctl(STDIN, FIONREAD, &bytesWaiting);
        return bytesWaiting;
        
  }

