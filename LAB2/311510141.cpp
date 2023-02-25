#include "Timing_Analyzer.h"
using namespace std;

int main(int argc, char* argv[]){

    Timing_Analyzer TA;

    //////////////////////////////////////////////////////////parse testlib.lib
    ifstream FILEIN;
    FILEIN.open(argv[5], ios::in);

    TA.parsetestlib(FILEIN);

    FILEIN.close();
    //////////////////////////////////////////////////////////parse module

    FILEIN.open(argv[1], ios::in);

    TA.parsemodule(FILEIN);
    
    FILEIN.close();

    //////////////////////////////////////////////////////////parse module
    FILEIN.open(argv[3], ios::in);

    TA.parsepattern(FILEIN);

    FILEIN.close();

    //TA.show_gate_port();

    //////////////////////////////////////////////////////////output file define
    ofstream FILEOUT_load;
    ofstream FILEOUT_delay;
    ofstream FILEOUT_path;

    string filename;
    filename = argv[1];
    filename.pop_back();
    filename.pop_back();
    FILEOUT_load.open("311510141_"+filename+"_load.txt", ios::out);
    FILEOUT_delay.open("311510141_"+filename+"_delay.txt", ios::out);
    FILEOUT_path.open("311510141_"+filename+"_path.txt", ios::out);

    //////////////////////////////////////////////////////////set outputload
    TA.setload(FILEOUT_load);
    //////////////////////////////////////////////////////////set timing information
    TA.settiming(FILEOUT_delay, FILEOUT_path);
    
    FILEOUT_load.close();
    FILEOUT_delay.close();
    FILEOUT_path.close();
    return 0;
}