/* 
 * File:   File.h
 * Author: fabio
 *
 * Created on October 22, 2010, 5:53 PM
 */

#ifndef FILE_H
#define	FILE_H

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include "FileReader.h"
#include "MeanCalculator.h"

namespace fs = boost::filesystem;

class File {
protected:
    MeanCalculator* meanCalc;
    FileReader* fr;
    std::vector< std::vector<double> > file_matrix;
    int xColumn;
    int yColumn;
    const std::string file_name;
    static boost::gregorian::date* convert(std::string& date, const std::string& format);
    std::string dayOfYearToFracHour(std::string, std::string);

public:
    std::vector<std::string> varNames;
    std::vector<double> x;
    std::vector<double> y;
    virtual bool init(int xColNumber, int yColNumber,  std::string& fname);
    void convertDayOfYearToHour(int coluna, std::string& format);
    void convertStrDateToYYYYMMDD(int coluna, const std::string& format);
    void convertStrTimeToHHMMSS(int coluna, const std::string& format);
    virtual void writeFile(std::string& fileName);
    virtual void free();
    virtual void calcMean(int colX, int colY);
    std::vector< std::vector<std::string> >& getSpreadSheet();
    virtual ~File(){}
};

class Month : public File {
private:
public:
    bool init(int xColNumber, int yColNumber, std::string& fname);
    bool init(int month, std::vector< std::vector<std::string> >& spreadSheet);
    bool insertColumn(int data, int cnum);
};

#endif	/* FILE_H */

