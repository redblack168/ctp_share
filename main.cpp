#include "ThostFtdcMdApi.h"
#include "md_recoder.h"
#include <thread>
#include <chrono>
int main(int argc, char** argv)
{
	auto mdapi = CThostFtdcMdApi::CreateFtdcMdApi();
	MD_data_recoder inst(mdapi);
	mdapi->RegisterSpi(&inst);
	mdapi->RegisterFront("tcp://asp-sim2-md1.financial-trading-platform.com:26213");
	mdapi->Init();

	while(true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	return 0;
}
