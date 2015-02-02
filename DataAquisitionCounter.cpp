#include <vector>

#include "DataAquisitionCounter.h"

bool DataAquisitionCounter::init(std::vector< std::vector<std::string> >& data,int col_key) {
    std::vector< std::vector<std::string> >::iterator row_it = data.begin();
    std::vector< std::vector<std::string> >::iterator row_end = data.end();

    col_key--;

    std::string tempDate = (*row_it)[col_key];

    for(row_it; row_it != row_end; ++row_it){
        
        if( (*row_it)[col_key] == tempDate ) {
            data_map[(*row_it)[col_key]]++;
            
        } else {
            tempDate = (*row_it)[2];
            data_map[(*row_it)[col_key]]++;
        }
        
    }
    
}

void DataAquisitionCounter::showData() {
    std::map<std::string, int>::iterator map_it = data_map.begin();
    std::map<std::string, int>::iterator map_end = data_map.end();

    for(map_it; map_it != map_end; ++map_it){
        if( map_it->second){
            std::cout << map_it->first << '\t' << map_it->second << std::endl;
        }
    }
}


void DataAquisitionCounter::writeFile(std::string fileName) {
        std::cout << "writing file " << fileName << std::endl;
	fs::ofstream ofs(fileName);

	std::map<std::string, int>::iterator map_it = data_map.begin();
        std::map<std::string, int>::iterator map_end = data_map.end();

        
	for (map_it; map_it != map_end; ++map_it) {
		
            ofs << map_it->first << '\t' << map_it->second << '\n';
		
	}
        std::cout << "done!" << '\n' << std::endl;
}
