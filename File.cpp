#include "File.h"

bool File::init(int xColNumber, int yColNumber, std::string& fname) {

        fr = new FileReader(fname);
        fr->doFileRip();
        fr->doFileClean("-999.90", 3);
        //fr->showYourGuts();
     return true;
}

void File::convertDayOfYearToHour(int coluna, std::string& format) {
        
        std::vector< std::vector<std::string> >& row = fr->row;

        std::vector< std::vector<std::string> >::iterator row_it = row.begin();
	std::vector< std::vector<std::string> >::iterator row_end = row.end();

        //pula primeira linha
        std::advance(row_it, 1);

        
	for (row_it; row_it != row_end; ++row_it) {
		std::vector<std::string>::iterator col_it = (row_it)->begin();
                std::vector<std::string>::iterator col_end = (row_it)->end();

                std::advance(col_it, coluna-1);

               
                boost::gregorian::date* d = convert(*col_it, format);
                std::ostringstream os;
                os << d->day_of_year();

                //UGH: preciso o dereference do ++col_it, pois quero a coluna seguinte,
                //que possui o valor da hora Fracionada
                std::ostringstream os_date;
                
                os_date << dayOfYearToFracHour(*(++col_it), os.str());
                
                *col_it = os_date.str() ;                
                delete d;                
	}

}

void File::convertStrDateToYYYYMMDD(int coluna, const std::string& format) {
        
        std::vector< std::vector<std::string> >& row = fr->row;

        std::vector< std::vector<std::string> >::iterator row_it = row.begin();
	std::vector< std::vector<std::string> >::iterator row_end = row.end();

        //pula primeira linha
        std::advance(row_it, 1);

        
	for (row_it; row_it != row_end; ++row_it) {
		std::vector<std::string>::iterator col_it = (row_it)->begin();
                std::vector<std::string>::iterator col_end = (row_it)->end();

                std::advance(col_it, coluna-1);

               
                boost::gregorian::date* d = convert(*col_it, format);

                boost::gregorian::date_facet* df = new boost::gregorian::date_facet();
                std::ostringstream os;
                os.imbue(std::locale(os.getloc(), df));

                df->format("%Y-%m-%d");
            

                os << *d;

                //UGH: preciso o dereference do ++col_it, pois quero a coluna seguinte,
                //que possui o valor da hora Fracionada
                *col_it = os.str() ;
                delete d;                
	}

}

void File::convertStrTimeToHHMMSS(int coluna, const std::string& format) {
        

        std::vector< std::vector<std::string> >& row = fr->row;

        std::vector< std::vector<std::string> >::iterator row_it = row.begin();
	std::vector< std::vector<std::string> >::iterator row_end = row.end();

        //pula primeira linha
        std::advance(row_it, 1);


	for (row_it; row_it != row_end; ++row_it) {
		std::vector<std::string>::iterator col_it = (row_it)->begin();
                std::vector<std::string>::iterator col_end = (row_it)->end();

                std::advance(col_it, coluna-1);


                //INPUT
                boost::posix_time::time_input_facet* tif = new boost::posix_time::time_input_facet(format.c_str());
                std::istringstream is( *col_it );
                is.imbue(std::locale(is.getloc(), tif));
                boost::posix_time::ptime td;
                
                is >> td;

                //OUTPUT
                boost::posix_time::time_facet* df = new boost::posix_time::time_facet();
                std::ostringstream os;
                os.imbue(std::locale(os.getloc(), df));
                df->format("%H:%M:%S");

                os << td;



                //os << *d;

                //UGH: preciso o dereference do ++col_it, pois quero a coluna seguinte,
                //que possui o valor da hora Fracionada

                *col_it = os.str() ;
                
	}
}

std::string File::dayOfYearToFracHour(std::string fracDay, std::string intDay){
    double iFracHour=0;
    double iFracDay=0;
    double iIntDay=0;
        
    std::istringstream issFracDay;
    std::istringstream issIntDay;

    issFracDay.str(fracDay);
    issIntDay.str(intDay);

    issFracDay >> iFracDay;
    issIntDay >> iIntDay;

    iFracHour = (iFracDay - iIntDay) * 24;   
    double iFracMin = ( iFracHour - int(iFracHour)  ) * 60;
    double iFracSec = ( iFracMin - int(iFracMin) ) * 60;

    std::ostringstream ostr_return;


    ostr_return << std::setfill('0') << std::setw(2) << int(iFracHour) << ":" << std::setw(2) << int(iFracMin) << ":" << std::setw(2) << int(iFracSec);
    
    return ostr_return.str();
}

boost::gregorian::date* File::convert(std::string& date, const std::string& format) {
                boost::gregorian::date_input_facet* df = new boost::gregorian::date_input_facet(format.c_str());
                std::istringstream* is = new std::istringstream( date );
                is->imbue(std::locale(is->getloc(), df));
                boost::gregorian::date* d = new boost::gregorian::date; //não chamo d(), deixo compilador chamar ctor padrão c/ not_a_date_time
                *is >> *d;                

               delete is;

               //não é preciso deleter df
               //vide: Q: * valgrind tells me I do not have a memory leak. Who owns facets?
               //A: The stream takes ownership when you imbue.
               //http://lists.boost.org/boost-users/2007/11/31984.php
               
               //delete df;


                return d;
}

void File::free() {
    delete fr;
    delete meanCalc;
}

void File::writeFile(std::string& fileName){    
    if(varNames.size()) {
        fr->row.insert(fr->row.begin(),varNames);
    }
    if(fr)
        fr->writeFile(fileName, false);
}

void File::calcMean(int colX, int colY){    
    if(fr) {
        meanCalc = new MeanCalculator;
        meanCalc->init(colX, colY, fr->row);
        //meanCalc->showMapGuts();
        meanCalc->calcHourlyMean();
        meanCalc->ShowMeanMapGuts();
        fr->row = meanCalc->convertMeanMapToString();
    }
}

std::vector< std::vector<std::string> >& File::getSpreadSheet() {
    return fr->row;
}

bool Month::init(int xColNumber, int yColNumber, std::string& fname) {
    fr = new FileReader(fname);
    fr->doFileRip();
       // fr->doFileClean("-999.90", 5);
    return true;
}

bool Month::init(int month, std::vector< std::vector<std::string> >& spreadSheet) {
    fr = new FileReader;
    fr->row = spreadSheet;
    //fr->showYourGuts();

    //fr->trimFileToMonth(month);
    
    return true;
}


bool Month::insertColumn(int data, int cnum) {
    std::stringstream is;
    is << data+1;
    if(fr->row.size())
    fr->insertColumn( is.str(), cnum );
}