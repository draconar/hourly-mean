#include <vector>
#include <sstream>
#include <iomanip>

#include "MeanCalculator.h"

bool MeanCalculator::init(int xCol, int yCol, std::vector< std::vector<std::string> >& spreadSheet){
    ripColumnsFillmap(xCol, yCol, spreadSheet);
}

bool MeanCalculator::ripColumnsFillmap(int xCol, int yCol, std::vector< std::vector<std::string> >& spreadSheet){
        
    std::vector< std::vector<std::string> >::iterator row_it = spreadSheet.begin();
    std::vector< std::vector<std::string> >::iterator row_end = spreadSheet.end();
    
    for(row_it; row_it != row_end; ++row_it){
        std::vector<std::string>::iterator col_it = row_it->begin();
        std::vector<std::string>::iterator col_end = row_it->end();

        int xTemp = 0;
        double yTemp = 0;

        std::vector<double> tempVec;

        //aqui é col != row_it->size() mesmo. O dereference de col_it me levaria para o elemento std::string
        for(int col = 0; col != row_it->size(); ++col){
            if (col == xCol - 1) {                
                xTemp = floor( atof( col_it[col].c_str() ) );
            } else if (col > (xCol - 1) )  {                
                yTemp = atof( col_it[col].c_str() ); 
                tempVec.push_back(yTemp);
            }
            
        }        

        if(tempVec.size()){
            values_map[xTemp].push_back(tempVec);
            tempVec.clear();
        }
        xTemp = 0;
        yTemp = 0;
    }


}

bool MeanCalculator::initMeansMap(int numKeys) {    
    for(int i = 0; i == numKeys; i++){
        values_map[i];
    }    
}

void MeanCalculator::showMapGuts(){

    if(values_map.empty())
        return;

    std::map<int, std::vector<std::vector<double> > >::iterator map_it = values_map.begin();

    for(map_it; map_it != values_map.end(); ++map_it){
        if( !map_it->second.empty() ){
            std::vector<std::vector<double> >::iterator row_it = map_it->second.begin();
            std::cout << map_it->first << std::endl;
            for (row_it; row_it != map_it->second.end(); ++row_it ){
                    std::vector<double>::iterator col_it = row_it->begin();                    
                    for(col_it; col_it != row_it->end(); ++col_it){
                            std::cout << "   " << *col_it << '\t';
                    }
                    std::cout << std::endl;

            }
        }
    }


}

void MeanCalculator::ShowMeanMapGuts(){
    if(means_map.empty())
        return;

    std::map<int, std::vector<double> >::iterator map_it = means_map.begin();

    for(map_it; map_it != means_map.end(); ++map_it) {
        if(!map_it->second.empty()){
            std::vector<double>::iterator vec_it = map_it->second.begin();
            std::cout << map_it->first;
            for(vec_it; vec_it != map_it->second.end(); ++vec_it){
               std::cout << '\t' << *vec_it;
            }
            std::cout << '\n';
        }
    }
    

}

bool MeanCalculator::calcHourlyMean(){
    if(values_map.empty())
        return false;

    std::map<int, std::vector<std::vector<double> > >::iterator values_map_it = values_map.begin();

    for(values_map_it; values_map_it != values_map.end(); ++values_map_it){
        if( !values_map_it->second.empty() ){
            means_map[values_map_it->first] = getMean( values_map_it->second );
        }
    }
    return true;
}

bool containsBogus(double data) {
    
    if(data == -99.9)
        return true;
    else if(data == 99.9)
        return true;
    else if(data == -999.99)
        return true;
    else if(data == 999.99)
        return true;
    else
        return false;

}

std::vector<double> MeanCalculator::getMean(std::vector<std::vector<double> >& values_spreadsheet) {
    
    std::vector<accumulator_set<double, stats<tag::mean> > > vec_acc;
    
    std::vector<std::vector<double> >::iterator row_it = values_spreadsheet.begin();
    
    //cria accs para cada variavel
    for(int i = 0; i < row_it->size(); ++i){
       accumulator_set<double, stats<tag::mean> > temp_acc;
       vec_acc.push_back(temp_acc);
    }

    for(row_it; row_it != values_spreadsheet.end(); ++row_it){
        std::vector<double>::iterator col_it = row_it->begin();
        int j = 0;
        for(col_it; col_it != row_it->end(); ++col_it){
            //chamando funcao do vec_acc() c/ valor de *col_i
            if( !containsBogus(*col_it) ) {
                vec_acc[j](*col_it);                
            }
            j++;
        }

    }

    std::vector<double> calculated_values;

    std::vector<accumulator_set<double, stats<tag::mean> > >::iterator acc_iter = vec_acc.begin();

    for(acc_iter; acc_iter != vec_acc.end(); ++acc_iter){
        calculated_values.push_back(mean(*acc_iter));
    }    

     return calculated_values;
}

std::vector< std::vector<std::string> > MeanCalculator::convertMeanMapToString(){
     
    std::map<int, std::vector<double> >::iterator map_it = means_map.begin();
    std::vector<std::vector<std::string> > tempStringMap;

    for(map_it; map_it != means_map.end(); ++map_it) {
        std::vector<std::string> tempStringRow;
        std::ostringstream tos;
        tos << map_it->first;
        tempStringRow.push_back(tos.str());
        if(!map_it->second.empty()){
            std::vector<double>::iterator vec_it = map_it->second.begin();
            for(vec_it; vec_it != map_it->second.end(); ++vec_it){
                std::ostringstream tos;

                tos <<  std::fixed << std::setprecision(3) << *vec_it;
               tempStringRow.push_back(tos.str());
            }
            
        }
        tempStringMap.push_back(tempStringRow);
    }
    return tempStringMap;
}