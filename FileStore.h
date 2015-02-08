#pragma once

#include <memory>
#include <fstream>
#include <set>
#include <unordered_map>


class CThostFtdcDepthMarketDataField;

class FileStore
{
public:
	FileStore(const std::string& path);
	virtual ~FileStore();

	void OnData(CThostFtdcDepthMarketDataField* data);
	void ResetFiles(const std::string& day, const std::set<std::string>&  instruments);

private:
	void SaveAllFiles();
	std::unordered_map<std::string, std::shared_ptr<std::fstream> > store_files_;
	std::string base_path_;
	std::string	day_;
};
