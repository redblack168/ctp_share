#include "ThostFtdcMdApi.h"
#include "md_recoder.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <signal.h>
#include <unistd.h>

bool g_runing = true;
void sig_handler(int sig)
{
  if (sig == SIGUSR1)
  {
	  std::cerr << "Receive Signal " << std::endl;
	  g_runing = false;
  }
  else
  {
	  std::cerr << "Receive Signal Not SIGUSR1" << std::endl;
  }
}

void SetupSIGHandler()
{
  struct sigaction act;
  act.sa_handler = &sig_handler;
  act.sa_flags = 0;

  if (sigaction(SIGUSR1, &act, NULL) != 0)
  {
	std::cout << "sigaction Signal error" << std::endl;
    perror("sigaction");
    throw std::runtime_error("Can not resiger signal handler");
  }
  else
  {
	std::cout << "sigaction Signal ok" << std::endl;
  }
}


int main(int argc, char** argv)
{
	if (argc < 3)
	{
		std::cout << "md_recoder tcp://asp-sim2-md1.financial-trading-platform.com:26213 IF1501 IF1502 IF1503"
				<< std::endl;

		return 1;
	}

	std::set<std::string> subscribelist;
	for (int i = 2 ; i < argc; ++ i)
	{
		subscribelist.insert(argv[i]);
	}
	SetupSIGHandler();
	auto mdapi = CThostFtdcMdApi::CreateFtdcMdApi();
	MD_data_recoder inst(mdapi, subscribelist);
	mdapi->RegisterSpi(&inst);
	mdapi->RegisterFront(argv[1]);
	mdapi->Init();

	while(g_runing)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	}
	mdapi->RegisterSpi(NULL);
	mdapi->Join();
	mdapi->Release();
	return 0;
}
