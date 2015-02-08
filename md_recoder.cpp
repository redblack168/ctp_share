#include "md_recoder.h"

#include <iostream>
#include <string.h>

#include <stdio.h>
#include <time.h>

#include "FileStore.h"


const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);

    return buf;
}


MD_data_recoder::MD_data_recoder(CThostFtdcMdApi* api, const std::set<std::string>& subscribelist)
: md_api_ (api)
, request_id_(0)
, file_store_(new FileStore("./"))
, subscribe_list_(subscribelist)
{
}


MD_data_recoder::~MD_data_recoder()
{
}


void MD_data_recoder::OnFrontConnected()
{
	std::cout << "OnFrontConnected " << std::endl;
	ReqUserLogin();
}

void MD_data_recoder::OnFrontDisconnected(int nReason)
{
	std::cout << __FUNCTION__ << std::endl;
}

void MD_data_recoder::OnHeartBeatWarning(int nTimeLapse)
{
	std::cout << __FUNCTION__ << std::endl;
}

void MD_data_recoder::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin,
		CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout  << "--->>> " << "OnRspUserLogin" << std::endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		std::cout  << "Login Successful  Trading Day = " << md_api_->GetTradingDay() << std::endl;
		//current_day_ = md_api_->GetTradingDay();
		current_day_ = currentDateTime();
		file_store_->ResetFiles(current_day_, GetInstrumentList(current_day_));
		SubscribeMarketData();
	}
	else
	{
		std::cout  << "--->>> " << "Login Fail" << std::endl;
	}
}

void MD_data_recoder::OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout,
		CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << __FUNCTION__ << std::endl;
}

void MD_data_recoder::OnRspError(CThostFtdcRspInfoField* pRspInfo,
		int nRequestID, bool bIsLast)
{
	std::cout << __FUNCTION__ << std::endl;
}

void MD_data_recoder::OnRspSubMarketData(
		CThostFtdcSpecificInstrumentField* pSpecificInstrument,
		CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << __FUNCTION__ << std::endl;
}

void MD_data_recoder::OnRspUnSubMarketData(
		CThostFtdcSpecificInstrumentField* pSpecificInstrument,
		CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << __FUNCTION__ <<  pSpecificInstrument->InstrumentID <<  std::endl;
	IsErrorRspInfo(pRspInfo);
}

void MD_data_recoder::OnRtnDepthMarketData(
		CThostFtdcDepthMarketDataField* pDepthMarketData)
{
	std::cout << __FUNCTION__ <<  pDepthMarketData->InstrumentID
			<< " " <<  pDepthMarketData->UpdateTime
			<< " " << pDepthMarketData->AskPrice1 <<  " " << pDepthMarketData ->AskVolume1
			<< " " << pDepthMarketData->BidPrice1 <<  " " << pDepthMarketData ->BidVolume1
			<< std::endl;

	file_store_->OnData(pDepthMarketData);
}

void MD_data_recoder::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	int ret = md_api_->ReqUserLogin(&req, ++request_id_);
	std::cout << "--->>> ReqUserLogin " << ((ret == 0) ? "ok" : "fail") << std::endl;
}

void MD_data_recoder::SubscribeMarketData()
{
	auto list = GetInstrumentList(current_day_);
	const char** subscirbebuff = new const char*[list.size()];
	int i = 0;
	for(auto each : list)
	{
		subscirbebuff[i++] = each.c_str();
	}
	auto sublist = const_cast<char**>(subscirbebuff);
	int ret = md_api_->SubscribeMarketData(sublist, list.size());
	std::cout << "--->>> SubscribeMarketData " << ((ret == 0) ? "ok" : "fail") << std::endl;
}

std::set<std::string> MD_data_recoder::GetInstrumentList(const std::string& day)
{
//	std::set<std::string> ret
//	{
//		"IF1502", "IF1503", "cu1502"
//	};
//	return ret;
	return subscribe_list_;
}

bool MD_data_recoder::IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo)
{
	bool ret = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (ret)
		std::cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
	return ret;
}
