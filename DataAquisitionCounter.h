/* 
 * File:   DataAquisitionCounter.h
 * Author: fabio
 *
 * Created on November 17, 2010, 5:20 PM
 */

#ifndef DATAAQUISITIONCOUNTER_H
#define	DATAAQUISITIONCOUNTER_H
#include <map>
#include <string>
#include <iostream>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>


namespace fs = boost::filesystem;

class DataAquisitionCounter {
private:
    std::map<std::string, int> data_map;
public:
    bool init(std::vector< std::vector<std::string> >& data, int col_key);
    void showData();
    void writeFile(std::string fileName);
};


#endif	/* DATAAQUISITIONCOUNTER_H */

