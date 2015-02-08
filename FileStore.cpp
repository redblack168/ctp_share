#include "FileStore.h"
#include <iostream>
#include "ThostFtdcUserApiStruct.h"

FileStore::FileStore(const std::string& path)
: base_path_(path)
{
	std::cout << "Base Save Path " << path << std::endl;
}

FileStore::~FileStore()
{
	std::cout << "~FileStore " << std::endl;
	SaveAllFiles();
}

void FileStore::OnData(CThostFtdcDepthMarketDataField* data)
{
	const std::string instrumet = data->InstrumentID;

	auto iter = store_files_.find(instrumet);
	if (iter != store_files_.end())
	{
		(*iter->second)
				<< data->TradingDay  << ','
				<< data->InstrumentID  << ','
				<< data->ExchangeID  << ','
				<< data->ExchangeInstID  << ','
				<< data->LastPrice  << ','
				<< data->PreSettlementPrice  << ','
				<< data->PreClosePrice  << ','
				<< data->PreOpenInterest  << ','
				<< data->OpenPrice  << ','
				<< data->HighestPrice  << ','
				<< data->LowestPrice  << ','
				<< data->Volume  << ','
				<< data->Turnover  << ','
				<< data->OpenInterest  << ','
				<< data->ClosePrice  << ','
				<< data->SettlementPrice  << ','
				<< data->UpperLimitPrice  << ','
				<< data->LowerLimitPrice  << ','
				<< data->PreDelta  << ','
				<< data->CurrDelta  << ','
				<< data->UpdateTime  << ','
				<< data->BidPrice1  << ','
				<< data->BidVolume1  << ','
				<< data->AskPrice1  << ','
				<< data->AskVolume1  << ','
				<< data->AveragePrice  << ','
				<< data->ActionDay  << std::endl;
	}
	else
	{
		std::cout << __FUNCTION__  << " file not exist for " <<  instrumet << std::endl;
	}
}

void FileStore::ResetFiles(const std::string& day,
		const std::set<std::string>& instruments)
{
	std::cout << __FUNCTION__  << " Start" << std::endl;
	SaveAllFiles();
	store_files_.clear();
	day_ = day;
	for (auto each : instruments)
	{
		auto filepath = base_path_ + '_' + each + '_' + day_ + ".csv";
		std::cout << __FUNCTION__  << " Create File " <<  filepath << std::endl;
		auto filestream = std::make_shared<std::fstream>(filepath, std::ios_base::out | std::ios_base::app);
		store_files_[each] = filestream;
	}
	std::cout << __FUNCTION__  << " End" << std::endl;
}

void FileStore::SaveAllFiles()
{
	std::cout << "SaveAllFiles Base Path "<<  base_path_  << " Day "<< day_ <<std::endl;
	for (auto each : store_files_)
	{
		std::cout << each.first << std::endl;
		each.second->close();
	}
	std::cout <<" SaveAllFiles end" << std::endl;
}
