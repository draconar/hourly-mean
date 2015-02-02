/* 
 * File:   MeanCalculator.h
 * Author: fabio
 *
 * Created on October 26, 2010, 4:47 PM
 */

#ifndef MEANCALCULATOR_H
#define	MEANCALCULATOR_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <algorithm>
#include <math.h>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>

using namespace boost::accumulators;


class MeanCalculator {
private:
    std::map<int, std::vector< std::vector<double> > > values_map;
    std::map<int, std::vector<double> > means_map;
    bool ripColumnsFillmap(int xCol, int yCol, std::vector< std::vector<std::string> >& spreadSheet);
    bool initMeansMap(int numKeys);
    std::vector<double> getMean(std::vector<std::vector<double> >& vec_double);
    
public:
   bool init(int xCol, int yCol, std::vector< std::vector<std::string> >& spreadSheet);
   bool free();
   void showMapGuts();
   void ShowMeanMapGuts();
   bool calcHourlyMean();
   std::vector< std::vector<std::string> > convertMeanMapToString();
   
};

#endif	/* MEANCALCULATOR_H */

