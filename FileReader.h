#ifndef _FILEREADER_
#define _FILEREADER_

#include <iostream>
#include <boost/math/distributions.hpp>
#include <list>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/progress.hpp>
#include <string>
#include <algorithm>

namespace fs = boost::filesystem;

class FileReader {
private:
	const std::string file_name;
	int numColumns;
	int numLines;
	void createTempVecs(fs::ifstream &fileContent);
	void createParsedVecs(std::vector<std::string> tempVector);
	std::vector<std::string> tempParsedVec;       


public:

        std::vector< std::vector<std::string> > row;

        FileReader(){}

	FileReader(const std::string &fname):file_name(fname){};

	bool doFileRip();

	std::vector<std::string> dataList();

	int getNumCols();

	int getNumLines();

	std::vector< std::vector<std::string> >  getData();

	void  printFileName() {std::cout << file_name << std::endl;}

	void doFileClean(const std::string& junkData, int restrictToCol);

	void showYourGuts();

	void ripColumn(int cnum);

	void writeFile(std::string fileName, bool sort);

	void insertColumn(std::string data, int cnum);

        bool trimFileToMonth(int month);

};

#endif


