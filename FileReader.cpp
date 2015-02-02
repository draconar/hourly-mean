#include "FileReader.h"
#include <boost/tokenizer.hpp>

bool FileReader::doFileRip(){

    try {
	fs::path fileName( file_name );
	fs::ifstream fileContent(fileName);

	createTempVecs(fileContent);

	createParsedVecs(tempParsedVec);

        if(fileName == 0)
            throw std::runtime_error("where is the file??");

    } catch (...){
        std::cout << "nao ha arquivos" ;
    }



	return true;
}


void FileReader::createTempVecs(fs::ifstream &fileContent) {

	while(fileContent) {
		std::string s;
		std::getline (fileContent, s, '\n');
		tempParsedVec.push_back(s);
	}
}

void FileReader::createParsedVecs(std::vector<std::string> tempVector) {

	using namespace boost;
	escaped_list_separator<char> csept("","\t","");

	for(int i=0; i != tempVector.size(); ++i){
		tokenizer<escaped_list_separator<char> > tokens(tempVector[i], csept);
		std::vector<std::string> tempColumn;

		for(tokenizer<escaped_list_separator<char> >::const_iterator tkiter = tokens.begin() ; tkiter != tokens.end(); ++tkiter) {
			//se não houve separação da linha vinda de tempVector em tab, ou é bullcrap ou é varName
		if( !(*tkiter == tempVector[i]) )
			//AQUI: fazer f() que chama regexp pra verificar se var name ou não
				tempColumn.push_back(*tkiter);
			}
		if (tempColumn.size() > 0)
			row.push_back(tempColumn);
	}
}

void FileReader::doFileClean(const std::string& junkData, int restrictToCol=0){


	std::vector< std::vector<std::string> >::iterator row_it = row.begin();
	std::vector< std::vector<std::string> >::iterator row_end = row.end();

	for (row_it; row_it != row_end;) {
		std::vector<std::string>::iterator col_it = (row_it)->begin();

		std::vector< std::vector<std::string> >::iterator actual_row = row_it;

		std::vector<std::string>::iterator col_end = (row_it)->end();

                
                if( (*row_it)[restrictToCol - 1] == junkData ) {
                    row_it->clear();
                    row_it = row.erase(row_it);
                }  else {
                    ++row_it;
                }                
                
        }
}

void FileReader::showYourGuts() {
	std::cout << "showing guts " << row.size() << std::endl;

	std::vector< std::vector<std::string> >::iterator row_it = row.begin();
	std::vector< std::vector<std::string> >::iterator row_end = row.end();

	for (row_it; row_it != row_end; ++row_it) {
		std::vector<std::string>::iterator col_it = (row_it)->begin();
		std::vector<std::string>::iterator col_end = (row_it)->end();

		for (col_it; col_it != col_end; ++col_it) {
			std::cout << " " << *col_it;
		}
		std::cout << std::endl;
	}

}

void FileReader::ripColumn(int cnum) {

	std::vector< std::vector<std::string> >::iterator row_it = row.begin();
	std::vector< std::vector<std::string> >::iterator row_end = row.end();

	for (row_it; row_it != row_end; ++row_it) {
		std::vector<std::string>::iterator col_it = (row_it)->begin();
		std::vector<std::string>::iterator col_end = (row_it)->end();

		std::advance(col_it, cnum-1);
		col_it = (*row_it).erase(col_it);
	}
}

bool compare(const std::vector<std::string>& s1, const std::vector<std::string>& s2) {
    return s1[2] < s2[2];
}

void FileReader::writeFile(std::string fileName, bool sort) {
std::cout << "writing file " << fileName << " ----- #lines " << row.size() << std::endl;
	fs::ofstream ofs(fileName);

	std::vector< std::vector<std::string> >::iterator row_it = row.begin();
	std::vector< std::vector<std::string> >::iterator row_end = row.end();

        /**@TODO: row.begin()+1 é para pular 1. linha... */
        /**@TODO: fazer funções de sort separadas */

        if (sort)
            std::stable_sort(row.begin()+1, row.end(), compare);

	for (row_it; row_it != row_end; ++row_it) {
		std::vector<std::string>::iterator col_it = (row_it)->begin();
		std::vector<std::string>::iterator col_end = (row_it)->end();

		for (col_it; col_it != col_end; ++col_it) {
                    if(col_it != (row_it)->begin())
			ofs << '\t' << *col_it ;
                    else
                        ofs << *col_it;
		}
		ofs << '\n';
	}
        std::cout << "done!" << '\n' << std::endl;
}

void FileReader::insertColumn(std::string data, int cnum) {
    
	std::vector< std::vector<std::string> >::iterator row_it = row.begin();
	std::vector< std::vector<std::string> >::iterator row_end = row.end();
        std::cout << "row size " << row.size() << std::endl;
	for (row_it; row_it != row_end; ++row_it) {
		std::vector<std::string>::iterator col_it = (row_it)->begin();
		std::vector<std::string>::iterator col_end = (row_it)->end();

                
                std::advance(col_it, cnum-1);

                row_it->insert( col_it, 1, data);


                //*col_it = data;
	}
}

std::vector< std::vector<std::string> > FileReader::getData() {
    return row;
}


bool FileReader::trimFileToMonth(int month) {
    std::cout << "Month data untrimmed; #of lines = " << row.size() << std::endl;

    std::vector< std::vector<std::string> > tempVec;

    std::vector< std::vector<std::string> >::iterator row_it  = row.begin();
    std::vector< std::vector<std::string> >::iterator row_end = row.end();

    std::vector< std::vector<std::string> >::iterator erase_begin  = row.begin();
    std::vector< std::vector<std::string> >::iterator erase_end = row.end();

    std::stringstream is;    
    is << std::setfill('0') << std::setw(2) << month;

    bool b_erase_begin, b_erase_end = false;

    for(row_it; row_end != row_it; ++row_it ) {
        std::vector<std::string>::iterator col_it = (row_it)->begin();
        std::vector<std::string>::iterator col_end = (row_it)->end();

        std::string sub_str = (col_it)[0].substr(5,2);
        bool checkIfExists = sub_str == is.str();

        if( checkIfExists ){            
            tempVec.push_back(*row_it) ;
        }

    }

    row = tempVec;

    std::cout << "Month data trimmed; #of lines = " << tempVec.size() << std::endl;
    
    return true;
}

