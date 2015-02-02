/* 
 * File:   main.cpp
 * Author: fabio
 *
 * Created on October 22, 2010, 4:44 PM
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "File.h"
#include "DataAquisitionCounter.h"
/*
 * 
 */
static bool deleteAll( Month * theElement ) { delete theElement; return true; }

int main(int argc, char** argv) {

    std::cout << "let's begin the operation" << std::endl;
    File* f = new File();
    std::string fname("/home/draconar/mestrado/factory/hourly_mean/pirata_1999_2006/rad0n23w_2m.ascii");
    f->init(2, 3, fname) ;

    //CO2: Converte coluna com horas em day of year para hora fracionada

    const std::string date_format="%Y%m%d";
    const std::string time_format="%H%M%S";


    //usar para arquivo co2: f->convertDayOfYearToHour(3, format);
    f->convertStrTimeToHHMMSS(2, time_format);
    f->convertStrDateToYYYYMMDD(1, date_format);

    std::string fnameCleaned = fname+".clean.frac";
    f->writeFile(fnameCleaned);

    f->free();
    delete f;
    return 0;
    
    std::vector<Month*> m_vec;

    for(int i = 0; i < 12; ++i){
        m_vec.push_back(new Month);
    }

    for(int i = 0; i < 12; ++i){
        m_vec[i]->init(i+1, f->getSpreadSheet());
        m_vec[i]->calcMean(2,3);
        m_vec[i]->insertColumn(i, 1);
    }


    

    /******* CO2: Usar para contar número dados em cada mês */
    /*
    DataAquisitionCounter* dc = new DataAquisitionCounter;
    dc->init(f->getSpreaSheet(), 3);
    dc->showData();
    std::string fnameDataCounter = fname+".data_cter";
    dc->writeFile(fnameDataCounter);
    delete dc;
    */


    for(int i = 0; i < 12; ++i){
        std::stringstream is;
        is << i+1;
        is.fill('0') << is.width(2);
        std::string meanFileOut = fname + "_hourly_mean_[" + is.str() +"].dat";
        std::string varNames[11] = {"#MONTH","HOUR","UWND","VWND","AIRT","SST","RH"};
        m_vec[i]->varNames.insert(f->varNames.begin(), varNames, varNames+11);
        m_vec[i]->writeFile(meanFileOut);
        m_vec[i]->free();
    }

    std::remove_if( m_vec.begin(), m_vec.end(), deleteAll );

    std::cout << "Done: " << std::endl;
    f->free();
    delete f;
    
    return 0;
}

