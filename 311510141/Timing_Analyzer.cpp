#include "Timing_Analyzer.h"

bool outload_compare(Gate s1, Gate s2){
    if(s1.outload == s2.outload){
        if(s1.name.size() == s2.name.size()){
            return s1.name < s2.name;
        }else{
            return s1.name.size() < s2.name.size();
        }

    }else{
        return s1.outload > s2.outload;
    }
    
}

bool delay_compare(Gate s1, Gate s2){
    if(s1.delay == s2.delay){
        if(s1.name.size() == s2.name.size()){
            return s1.name < s2.name;
        }else{
            return s1.name.size() < s2.name.size();
        }

    }else{
        return s1.delay > s2.delay;
    }
    
}

Tablevalue::Tablevalue(){

}

Input_Pin::Input_Pin(string temp_string, double temp_value){

    pinname = temp_string;
    capacitance = temp_value;

}

Output_Pin::Output_Pin(){

}

Output_Pin::Output_Pin(string temp_string, double temp_value){

    pinname = temp_string;
    capacitance = temp_value;

}

cell::cell(){

}

Lu_Table::Lu_Table(){

}

Gate::Gate(){
    outload = 0;
    delay = -1;
    transtime = -1;
    delayorder = 0;
    logic = 2;
}

Inputcase::Inputcase(){
    value = 2;
}

Logic::Logic(){
    value = 2;
}

Timing_Analyzer::Timing_Analyzer(){

}

Timing_Analyzer::~Timing_Analyzer(){

}

void Timing_Analyzer::parsetestlib(ifstream& FILEIN){
    string dump_string;
    double temp_value;
    char dump_char;
    char unit[5];

    getline(FILEIN, dump_string);
    FILEIN >> dump_string >> dump_char >> dump_char >> unit[0] >> unit[1] >> unit[2] >> dump_string;
    string unitsting(unit);
    lu_table.time_unit = unitsting;
    strcpy(unit, "  ");
    FILEIN >> dump_string >> dump_char >> dump_char >> unit[0] >> unit[1]  >> dump_string;
    unitsting = unit;
    lu_table.voltage_unit = unitsting;
    strcpy(unit, " ");
    FILEIN >> dump_string >> dump_char >> dump_char >> unit[0] >> unit[1]  >> unit[2] >> dump_string;
    unitsting = unit;
    lu_table.current_unit = unitsting;
    strcpy(unit, "  ");
    FILEIN >> dump_string >> dump_char  >> unit[0] >> unit[1] >> unit[2] >> dump_char >> unit[3] >> unit[4] >> dump_string;
    unitsting = unit;
    lu_table.capacitive_load_unit = unitsting;
    strcpy(unit, "    ");

    getline(FILEIN, dump_string);
    getline(FILEIN, dump_string);
    getline(FILEIN, dump_string);
    getline(FILEIN, dump_string);
    getline(FILEIN, dump_string);

    FILEIN >> dump_string >> dump_char >> dump_char;
    for(int i = 0; i<7 ;i++){
        FILEIN >> temp_value;
        lu_table.index_1.push_back(temp_value);
        FILEIN >> dump_char;
    }
    FILEIN >> dump_string >> dump_string >> dump_char >> dump_char;

    for(int i = 0; i<7 ;i++){
        FILEIN >> temp_value;
        lu_table.index_2.push_back(temp_value);
        FILEIN >> dump_char;
    }

    bool flag_NOR2X1 = false;
    bool flag_INVX1 = false;
    bool flag_NANDX1 = false;
    bool flag_A1 = false;
    bool flag_A2 = false;
    bool flag_ZN = false;
    bool flag_I = false;
    
    while(!FILEIN.eof()){
        FILEIN >> dump_string;
        
        if(dump_string == "(NOR2X1)" || flag_NOR2X1){
            flag_NOR2X1 = true;
            if(dump_string == "pin(A1)" || flag_A1){
                flag_A1 = true;
                if(dump_string == "capacitance"){

                    FILEIN >> dump_char >> temp_value ;
                    Input_Pin temp_pin("A1",temp_value); 
                    lu_table.NOR2X1.input_pin.push_back(temp_pin);

                }
                if(dump_string == "pin(A2)"){
                    flag_A1 = false;
                    flag_A2 = true;
                }
            }
            if(flag_A2){
                if(dump_string == "capacitance"){

                    FILEIN >> dump_char >> temp_value ;
                    Input_Pin temp_pin("A2",temp_value); 
                    lu_table.NOR2X1.input_pin.push_back(temp_pin);
                }
                if(dump_string == "pin(ZN)"){
                    flag_A2 = false;
                    flag_ZN = true;
                }  
            }
            if(flag_ZN){
                
                if(dump_string == "capacitance"){
                    FILEIN >> dump_char >> temp_value;
                    Output_Pin temp_pin("ZN",temp_value); 
                    lu_table.NOR2X1.output_pin = temp_pin;
                }
                //cout << dump_string;
                if(dump_string == "rise_power(table10){"){
                    
                    while(1){
                        FILEIN >> dump_char;
                        //cout << dump_char;
                        if(dump_char == '"'){
                            for(int i = 0; i<7; i++){

                                vector<Tablevalue> temp_row;

                                for(int j = 0;j<7; j++){

                                    Tablevalue temp_tablevalue;
                                    temp_tablevalue.input_time = lu_table.index_2[i];
                                    temp_tablevalue.output_load = lu_table.index_1[j];

                                    FILEIN >> temp_tablevalue.rise_power >> dump_char;
                                    //cout << temp_tablevalue.input_time << endl;
                                    //cout << temp_tablevalue.output_load << endl;
                                    //cout << temp_tablevalue.rise_power << endl;
                                    
                                    temp_row.push_back(temp_tablevalue);
                                    //cout << dump_char << endl;;
                                    
                                    /*int inter_size = lu_table.NOR2X1.output_pin.tablevalue[i].size();
                                    for(int k = 0; k < inter_size; k++){
                                        cout << lu_table.NOR2X1.output_pin.tablevalue[0][k].input_time << endl;
                                    }*/
                                }

                                lu_table.NOR2X1.output_pin.tablevalue.push_back(temp_row);
                                
                                if(i == 6){
                                    FILEIN >> dump_char;
                                }else{
                                    FILEIN >> dump_char >> dump_char >> dump_char;
                                }
                            }
                        }
                        if(dump_char == ';'){
                            break;
                        }
                    }
                }

                if(dump_string == "fall_power(table10){"){
                    
                     while(1){
                        FILEIN >> dump_char;
                        //cout << dump_char;
                        int k = 0;
                        if(dump_char == '"'){
                            for(int i = 0; i<7; i++){
                                
                                for(int j = 0;j<7; j++){

                                    FILEIN >> lu_table.NOR2X1.output_pin.tablevalue[i][j].fall_power >> dump_char;
                                    k++;
                                }

                                if(i == 6){
                                    FILEIN >> dump_char;
                                }else{
                                    FILEIN >> dump_char >> dump_char >> dump_char;
                                }
                            }
                        }
                        if(dump_char == ';'){
                            break;
                        }
                    }
                }

                if(dump_string == "cell_rise(table10){"){
                    
                     while(1){
                        FILEIN >> dump_char;
                        //cout << dump_char;
                        int k = 0;
                        if(dump_char == '"'){
                            for(int i = 0; i<7; i++){
                                
                                for(int j = 0;j<7; j++){

                                    FILEIN >> lu_table.NOR2X1.output_pin.tablevalue[i][j].cell_rise >> dump_char;
                                    k++;
                                }

                                if(i == 6){
                                    FILEIN >> dump_char;
                                }else{
                                    FILEIN >> dump_char >> dump_char >> dump_char;
                                }
                            }
                        }
                        if(dump_char == ';'){
                            break;
                        }
                    }
                }

                if(dump_string == "cell_fall(table10){"){
                    
                     while(1){
                        FILEIN >> dump_char;
                        //cout << dump_char;
                        int k = 0;
                        if(dump_char == '"'){
                            for(int i = 0; i<7; i++){
                                
                                for(int j = 0;j<7; j++){

                                    FILEIN >> lu_table.NOR2X1.output_pin.tablevalue[i][j].cell_fall >> dump_char;
                                    k++;
                                }

                                if(i == 6){
                                    FILEIN >> dump_char;
                                }else{
                                    FILEIN >> dump_char >> dump_char >> dump_char;
                                }
                            }
                        }
                        if(dump_char == ';'){
                            break;
                        }
                    }
                }
 
                if(dump_string == "rise_transition(table10){"){
                    
                     while(1){
                        FILEIN >> dump_char;
                        //cout << dump_char;
                        int k = 0;
                        if(dump_char == '"'){
                            for(int i = 0; i<7; i++){
                                
                                for(int j = 0;j<7; j++){

                                    FILEIN >> lu_table.NOR2X1.output_pin.tablevalue[i][j].rise_transition >> dump_char;
                                    k++;
                                }

                                if(i == 6){
                                    FILEIN >> dump_char;
                                }else{
                                    FILEIN >> dump_char >> dump_char >> dump_char;
                                }
                            }
                        }
                        if(dump_char == ';'){
                            break;
                        }
                    }
                }

                if(dump_string == "fall_transition(table10){"){
                    
                     while(1){
                        FILEIN >> dump_char;
                        //cout << dump_char;
                        int k = 0;
                        if(dump_char == '"'){
                            for(int i = 0; i<7; i++){
                                
                                for(int j = 0;j<7; j++){

                                    FILEIN >> lu_table.NOR2X1.output_pin.tablevalue[i][j].fall_transition >> dump_char;
                                    k++;
                                }

                                if(i == 6){
                                    FILEIN >> dump_char;
                                }else{
                                    FILEIN >> dump_char >> dump_char >> dump_char;
                                }
                            }
                        }
                        if(dump_char == ';'){
                            flag_ZN = false;
                            flag_NOR2X1 = false;
                            break;
                        }
                    }
                }
            }


        }

        if(dump_string == "(INVX1)" || flag_INVX1){
            flag_INVX1 = true;
            
            if(dump_string == "pin(I)" || flag_I){
                flag_I = true;
                if(dump_string == "capacitance"){

                    FILEIN >> dump_char >> temp_value ;
                    Input_Pin temp_pin("I",temp_value); 
                    lu_table.INVX1.input_pin.push_back(temp_pin);

                }
                if(dump_string == "pin(ZN)"){
                    flag_I = false;
                    flag_ZN = true;
                }
            }
            if(flag_ZN){
                
                if(dump_string == "capacitance"){
                    FILEIN >> dump_char >> temp_value;
                    Output_Pin temp_pin("ZN",temp_value); 
                    lu_table.INVX1.output_pin = temp_pin;
                }

                if(dump_string == "rise_power(table10){"){
                    
                    while(1){
                        FILEIN >> dump_char;

                        if(dump_char == '"'){
                            for(int i = 0; i<7; i++){
                                vector<Tablevalue> temp_row;
                                for(int j = 0;j<7; j++){

                                    Tablevalue temp_tablevalue;
                                    temp_tablevalue.input_time = lu_table.index_2[i];
                                    temp_tablevalue.output_load = lu_table.index_1[j];

                                    FILEIN >> temp_tablevalue.rise_power >> dump_char;

                                    temp_row.push_back(temp_tablevalue);

                                }
                                lu_table.INVX1.output_pin.tablevalue.push_back(temp_row);
                                
                                if(i == 6){
                                    FILEIN >> dump_char;
                                }else{
                                    FILEIN >> dump_char >> dump_char >> dump_char;
                                }
                            }
                        }
                        if(dump_char == ';'){
                            break;
                        }
                    }
                }
                
                if(dump_string == "fall_power(table10){"){
                    
                     while(1){
                        FILEIN >> dump_char;
                        //cout << dump_char;
                        int k = 0;
                        if(dump_char == '"'){
                            for(int i = 0; i<7; i++){
                                
                                for(int j = 0;j<7; j++){

                                    FILEIN >> lu_table.INVX1.output_pin.tablevalue[i][j].fall_power >> dump_char;
                                    k++;
                                }

                                if(i == 6){
                                    FILEIN >> dump_char;
                                }else{
                                    FILEIN >> dump_char >> dump_char >> dump_char;
                                }
                            }
                        }
                        if(dump_char == ';'){
                            break;
                        }
                    }
                }

                if(dump_string == "cell_rise(table10){"){
                    
                     while(1){
                        FILEIN >> dump_char;
                        //cout << dump_char;
                        int k = 0;
                        if(dump_char == '"'){
                            for(int i = 0; i<7; i++){
                                
                                for(int j = 0;j<7; j++){

                                    FILEIN >> lu_table.INVX1.output_pin.tablevalue[i][j].cell_rise >> dump_char;
                                    k++;
                                }

                                if(i == 6){
                                    FILEIN >> dump_char;
                                }else{
                                    FILEIN >> dump_char >> dump_char >> dump_char;
                                }
                            }
                        }
                        if(dump_char == ';'){
                            break;
                        }
                    }
                }

                if(dump_string == "cell_fall(table10){"){
                    
                     while(1){
                        FILEIN >> dump_char;
                        //cout << dump_char;
                        int k = 0;
                        if(dump_char == '"'){
                            for(int i = 0; i<7; i++){
                                
                                for(int j = 0;j<7; j++){

                                    FILEIN >> lu_table.INVX1.output_pin.tablevalue[i][j].cell_fall >> dump_char;
                                    k++;
                                }

                                if(i == 6){
                                    FILEIN >> dump_char;
                                }else{
                                    FILEIN >> dump_char >> dump_char >> dump_char;
                                }
                            }
                        }
                        if(dump_char == ';'){
                            break;
                        }
                    }
                }
 
                if(dump_string == "rise_transition(table10){"){
                    
                     while(1){
                        FILEIN >> dump_char;
                        //cout << dump_char;
                        int k = 0;
                        if(dump_char == '"'){
                            for(int i = 0; i<7; i++){
                                
                                for(int j = 0;j<7; j++){

                                    FILEIN >> lu_table.INVX1.output_pin.tablevalue[i][j].rise_transition >> dump_char;
                                    k++;
                                }

                                if(i == 6){
                                    FILEIN >> dump_char;
                                }else{
                                    FILEIN >> dump_char >> dump_char >> dump_char;
                                }
                            }
                        }
                        if(dump_char == ';'){
                            break;
                        }
                    }
                }

                if(dump_string == "fall_transition(table10){"){
                    
                     while(1){
                        FILEIN >> dump_char;
                        //cout << dump_char;
                        int k = 0;
                        if(dump_char == '"'){
                            for(int i = 0; i<7; i++){
                                
                                for(int j = 0;j<7; j++){

                                    FILEIN >> lu_table.INVX1.output_pin.tablevalue[i][j].fall_transition >> dump_char;
                                    k++;
                                }

                                if(i == 6){
                                    FILEIN >> dump_char;
                                }else{
                                    FILEIN >> dump_char >> dump_char >> dump_char;
                                }
                            }
                        }
                        if(dump_char == ';'){
                            flag_ZN = false;
                            flag_INVX1 = false;
                            break;
                        }
                    }
                }
            }


        }

        if(dump_string == "(NANDX1)" || flag_NANDX1){
            flag_NANDX1 = true;
            if(dump_string == "pin(A1)" || flag_A1){
                flag_A1 = true;
                if(dump_string == "capacitance"){

                    FILEIN >> dump_char >> temp_value ;
                    Input_Pin temp_pin("A1",temp_value); 
                    lu_table.NANDX1.input_pin.push_back(temp_pin);

                }
                if(dump_string == "pin(A2)"){
                    flag_A1 = false;
                    flag_A2 = true;
                }
            }
            if(flag_A2){
                if(dump_string == "capacitance"){

                    FILEIN >> dump_char >> temp_value ;
                    Input_Pin temp_pin("A2",temp_value); 
                    lu_table.NANDX1.input_pin.push_back(temp_pin);
                }
                if(dump_string == "pin(ZN)"){
                    flag_A2 = false;
                    flag_ZN = true;
                }  
            }
            if(flag_ZN){
                
                if(dump_string == "capacitance"){
                    FILEIN >> dump_char >> temp_value;
                    Output_Pin temp_pin("ZN",temp_value); 
                    lu_table.NANDX1.output_pin = temp_pin;
                }
                //cout << dump_string;
                if(dump_string == "rise_power(table10){"){
                    
                    while(1){
                        FILEIN >> dump_char;
                        //cout << dump_char;
                        if(dump_char == '"'){
                            for(int i = 0; i<7; i++){
                                vector<Tablevalue> temp_row;
                                for(int j = 0;j<7; j++){

                                    Tablevalue temp_tablevalue;
                                    temp_tablevalue.input_time = lu_table.index_2[i];
                                    temp_tablevalue.output_load = lu_table.index_1[j];

                                    FILEIN >> temp_tablevalue.rise_power >> dump_char;

                                    temp_row.push_back(temp_tablevalue);
                                }
                                lu_table.NANDX1.output_pin.tablevalue.push_back(temp_row);
                                if(i == 6){
                                    FILEIN >> dump_char;
                                }else{
                                    FILEIN >> dump_char >> dump_char >> dump_char;
                                }
                            }
                        }
                        if(dump_char == ';'){
                            break;
                        }
                    }
                }

                if(dump_string == "fall_power(table10){"){
                    
                     while(1){
                        FILEIN >> dump_char;
                        //cout << dump_char;
                        int k = 0;
                        if(dump_char == '"'){
                            for(int i = 0; i<7; i++){
                                
                                for(int j = 0;j<7; j++){

                                    FILEIN >> lu_table.NANDX1.output_pin.tablevalue[i][j].fall_power >> dump_char;
                                    k++;
                                }

                                if(i == 6){
                                    FILEIN >> dump_char;
                                }else{
                                    FILEIN >> dump_char >> dump_char >> dump_char;
                                }
                            }
                        }
                        if(dump_char == ';'){
                            break;
                        }
                    }
                }

                if(dump_string == "cell_rise(table10){"){
                    
                     while(1){
                        FILEIN >> dump_char;
                        //cout << dump_char;
                        int k = 0;
                        if(dump_char == '"'){
                            for(int i = 0; i<7; i++){
                                
                                for(int j = 0;j<7; j++){

                                    FILEIN >> lu_table.NANDX1.output_pin.tablevalue[i][j].cell_rise >> dump_char;
                                    k++;
                                }

                                if(i == 6){
                                    FILEIN >> dump_char;
                                }else{
                                    FILEIN >> dump_char >> dump_char >> dump_char;
                                }
                            }
                        }
                        if(dump_char == ';'){
                            break;
                        }
                    }
                }

                if(dump_string == "cell_fall(table10){"){
                    
                     while(1){
                        FILEIN >> dump_char;
                        //cout << dump_char;
                        int k = 0;
                        if(dump_char == '"'){
                            for(int i = 0; i<7; i++){
                                
                                for(int j = 0;j<7; j++){

                                    FILEIN >> lu_table.NANDX1.output_pin.tablevalue[i][j].cell_fall >> dump_char;
                                    k++;
                                }

                                if(i == 6){
                                    FILEIN >> dump_char;
                                }else{
                                    FILEIN >> dump_char >> dump_char >> dump_char;
                                }
                            }
                        }
                        if(dump_char == ';'){
                            break;
                        }
                    }
                }
 
                if(dump_string == "rise_transition(table10){"){
                    
                     while(1){
                        FILEIN >> dump_char;
                        //cout << dump_char;
                        int k = 0;
                        if(dump_char == '"'){
                            for(int i = 0; i<7; i++){
                                
                                for(int j = 0;j<7; j++){

                                    FILEIN >> lu_table.NANDX1.output_pin.tablevalue[i][j].rise_transition >> dump_char;
                                    k++;
                                }

                                if(i == 6){
                                    FILEIN >> dump_char;
                                }else{
                                    FILEIN >> dump_char >> dump_char >> dump_char;
                                }
                            }
                        }
                        if(dump_char == ';'){
                            break;
                        }
                    }
                }

                if(dump_string == "fall_transition(table10){"){
                    
                     while(1){
                        FILEIN >> dump_char;
                        //cout << dump_char;
                        int k = 0;
                        if(dump_char == '"'){
                            for(int i = 0; i<7; i++){
                                
                                for(int j = 0;j<7; j++){

                                    FILEIN >> lu_table.NANDX1.output_pin.tablevalue[i][j].fall_transition >> dump_char;
                                    k++;
                                }

                                if(i == 6){
                                    FILEIN >> dump_char;
                                }else{
                                    FILEIN >> dump_char >> dump_char >> dump_char;
                                }
                            }
                        }
                        if(dump_char == ';'){
                            flag_ZN = false;
                            flag_NANDX1 = false;
                            break;
                        }
                    }
                }
            }


        }
        
        
    }

    /*for(int i = 0; i<lu_table.NOR2X1.input_pin.size(); i++){
        cout << lu_table.NOR2X1.input_pin[i].capacitance << endl;
    }

    for(int i = 0; i<lu_table.index_1.size(); i++){
        cout << lu_table.index_1[i] << endl;
    }  */
}

void Timing_Analyzer::parsemodule(ifstream& FILEIN){
    string dump_string;
    string temp_modulename;
    char dump_char;

    FILEIN >> dump_string;

    while(1){
        FILEIN >> dump_char;
        if(dump_char == '('){
            break;
        }
        temp_modulename.push_back(dump_char); 
    }
    module_name = temp_modulename;
    
    //cout << module_name << endl;
    while(!FILEIN.eof()){
        FILEIN >> dump_string;
        //cout << dump_string << endl;
        if(dump_string[0] == '/' && dump_string[1] == '/'){
            //cout << "我有進來";
            getline(FILEIN, dump_string);
            //cout << dump_string;
        }
        if(dump_string[0] == '/' && dump_string[1] == '*'){
            bool break_commemt_0 = false;
            bool break_commemt_1 = false;
        
            int dump_string_size = dump_string.size();
            if(dump_string[dump_string_size - 2] == '*'&& dump_string[dump_string_size - 1] == '/'){
            
            }else{

                while(FILEIN >> dump_char){

                    if(dump_char == '*'){
                        break_commemt_0 = true;
                        continue;
                    }

                    if(break_commemt_0 == true && dump_char == '/'){
                        break_commemt_1 = true;
                    }else{
                        break_commemt_0 = false;
                    }

                    if(break_commemt_1 == true){
                        break;
                    }
                }

            }

            
        }
        if(dump_string == "input"){
            dump_char = ' ';
            while(dump_char != ';'){
                string temp_name = "";                 
                while(1){
                    FILEIN >> dump_char;
                    if(dump_char == ',' || dump_char == ';'){
                        break;
                    }
                    temp_name.push_back(dump_char);              
                }
                input.push_back(temp_name);  
            }
        }
        if(dump_string == "output"){
            dump_char = ' ';
            while(dump_char != ';'){
                string temp_name = "";                 
                while(1){
                    FILEIN >> dump_char;
                    if(dump_char == ',' || dump_char == ';'){
                        break;
                    }
                    temp_name.push_back(dump_char);              
                }
                output.push_back(temp_name);  
            }
        }
        if(dump_string == "wire"){
            dump_char = ' ';
            while(dump_char != ';'){
                string temp_name = "";                 
                while(1){
                    FILEIN >> dump_char;
                    if(dump_char == ',' || dump_char == ';'){
                        break;
                    }
                    temp_name.push_back(dump_char);              
                }
                wire.push_back(temp_name);  
            }
        }

        if(dump_string == "NOR2X1"){
            Gate temp_gate;
            temp_gate.gate_type = "NOR2X1";

            string temp_gatename;
            while(1){
                FILEIN >> dump_char;
                if(dump_char == '('){
                    break;
                }
                temp_gatename.push_back(dump_char); 
            }

            temp_gate.name = temp_gatename;
            //cout << temp_gate.name << endl;
            dump_char = ' ';                          
            while(1){
                    
                FILEIN >> dump_char;
                if(dump_char == '.'){
                    string temp_name = "";
                    string port_name = "";
                    while(1){
                        FILEIN >> dump_char;
                        if(dump_char == '('){
                            break;
                        }
                        temp_name.push_back(dump_char); 
                        
                    }
                    if(temp_name == "A1"){

                        while(1){
                            FILEIN >> dump_char;
                            if(dump_char == ')'){
                                break;
                            }
                            port_name.push_back(dump_char);
                        }
                        Gate_input temp_gate_input;
                        temp_gate_input.input_type = "A1";
                        temp_gate_input.name = port_name;
                        temp_gate.gate_input.push_back(temp_gate_input);
                    }
                    if(temp_name == "A2"){
                        while(1){
                            FILEIN >> dump_char;
                            if(dump_char == ')'){
                                break;
                            }
                            port_name.push_back(dump_char);
                        } 
                        Gate_input temp_gate_input;
                        temp_gate_input.input_type = "A2";
                        temp_gate_input.name = port_name;
                        temp_gate.gate_input.push_back(temp_gate_input);
                        
                    }
                    if(temp_name == "ZN"){
                        while(1){
                            FILEIN >> dump_char;
                            if(dump_char == ')'){
                                break;
                            }
                            port_name.push_back(dump_char);
                        } 
                        temp_gate.gate_output = port_name;
                        
                    }
                    
                }
                if(dump_char == ';'){
                    break;
                }
             
            }     
            gate.push_back(temp_gate);
        }

        if(dump_string == "INVX1"){
            Gate temp_gate;
            temp_gate.gate_type = "INVX1";
            string temp_gatename;
            while(1){
                FILEIN >> dump_char;
                if(dump_char == '('){
                    break;
                }
                temp_gatename.push_back(dump_char); 
            }

            temp_gate.name = temp_gatename;
            //cout << temp_gate.name << endl;
            dump_char = ' ';                          
            while(1){
                    
                FILEIN >> dump_char;
                if(dump_char == '.'){
                    string temp_name = "";
                    string port_name = "";
                    while(1){
                        FILEIN >> dump_char;
                        if(dump_char == '('){
                            break;
                        }
                        temp_name.push_back(dump_char); 
                        
                    }
                    if(temp_name == "I"){
                        while(1){
                            FILEIN >> dump_char;
                            if(dump_char == ')'){
                                break;
                            }
                            port_name.push_back(dump_char);
                        } 
                        Gate_input temp_gate_input;
                        temp_gate_input.input_type = "I";
                        temp_gate_input.name = port_name;
                        temp_gate.gate_input.push_back(temp_gate_input);
                    }
                    if(temp_name == "ZN"){
                        while(1){
                            FILEIN >> dump_char;
                            if(dump_char == ')'){
                                break;
                            }
                            port_name.push_back(dump_char);
                        } 
                        temp_gate.gate_output = port_name;   
                    } 
                }
                if(dump_char == ';'){
                    break;
                }
             
            }     
            gate.push_back(temp_gate);
        }        

        if(dump_string == "NANDX1"){
            Gate temp_gate;
            temp_gate.gate_type = "NANDX1";
            string temp_gatename;
            while(1){
                FILEIN >> dump_char;
                if(dump_char == '('){
                    break;
                }
                temp_gatename.push_back(dump_char); 
            }

            temp_gate.name = temp_gatename;
            //cout << temp_gate.name << endl;
            dump_char = ' ';                          
            while(1){
                    
                FILEIN >> dump_char;
                if(dump_char == '.'){
                    string temp_name = "";
                    string port_name = "";
                    while(1){
                        FILEIN >> dump_char;
                        if(dump_char == '('){
                            break;
                        }
                        temp_name.push_back(dump_char); 
                        
                    }
                    if(temp_name == "A1"){
                        while(1){
                            FILEIN >> dump_char;
                            if(dump_char == ')'){
                                break;
                            }
                            port_name.push_back(dump_char);
                        } 
                        Gate_input temp_gate_input;
                        temp_gate_input.input_type = "A1";
                        temp_gate_input.name = port_name;
                        temp_gate.gate_input.push_back(temp_gate_input);
                    }
                    if(temp_name == "A2"){
                        while(1){
                            FILEIN >> dump_char;
                            if(dump_char == ')'){
                                break;
                            }
                            port_name.push_back(dump_char);
                        } 
                        Gate_input temp_gate_input;
                        temp_gate_input.input_type = "A2";
                        temp_gate_input.name = port_name;
                        temp_gate.gate_input.push_back(temp_gate_input);
                        
                    }
                    if(temp_name == "ZN"){
                        while(1){
                            FILEIN >> dump_char;
                            if(dump_char == ')'){
                                break;
                            }
                            port_name.push_back(dump_char);
                        } 
                        temp_gate.gate_output = port_name;
                        
                    }
                    
                }
                if(dump_char == ';'){
                    break;
                }
             
            }     
            gate.push_back(temp_gate);
        }

    }

    //show_gate_port();
    //show_input();
    //show_output();
    //show_wire();


}

void Timing_Analyzer::parsepattern(ifstream& FILEIN){

    string dump_string;
    string first_line;
    string temp_name;
    char dump_char;

    vector<string> name_array;

    getline(FILEIN, first_line);
    istringstream first_string(first_line);

    first_string >> dump_string;
    while(first_string >> dump_char){

        if(dump_char == ','){
            name_array.push_back(temp_name);
            temp_name = "";
        }else{
            temp_name.push_back(dump_char);  
        }
             
    }
    name_array.push_back(temp_name);

    vector<Inputcase> temp_inputcase;
    int input_size = input.size();
    bool value;
    while(FILEIN >> value){
        Inputcase temp_input;
        temp_input.value = value;
        temp_input.name = name_array[0];
        temp_inputcase.push_back(temp_input);
        for(int i = 1; i < input_size ; i++){
            FILEIN >> value;
            Inputcase temp_input;
            temp_input.value = value;
            temp_input.name = name_array[i];
            temp_inputcase.push_back(temp_input);
        }
        getline(FILEIN, dump_string);
        inputcase.push_back(temp_inputcase);
        temp_inputcase.clear();
    }

}

void Timing_Analyzer::setload(ofstream & FILEOUT_load){

    int gate_size = gate.size();
    //cout << gate_size << endl;
    for(int i = 0; i < gate_size; i++){

        for(int j =0; j < gate_size; j++){

            int gate_input_size = gate[j].gate_input.size(); 
            for(int k = 0; k < gate_input_size; k++){

                if(gate[i].gate_output == gate[j].gate_input[k].name){

                    if(gate[j].gate_input[k].input_type == "A1"){

                        if(gate[j].gate_type == "NOR2X1"){
                            gate[i].outload += lu_table.NOR2X1.input_pin[0].capacitance;
                        }
                        if(gate[j].gate_type == "NANDX1"){
                            gate[i].outload += lu_table.NANDX1.input_pin[0].capacitance;
                        }
                    }

                    if(gate[j].gate_input[k].input_type == "A2"){

                        if(gate[j].gate_type == "NOR2X1"){
                            gate[i].outload += lu_table.NOR2X1.input_pin[1].capacitance;
                        }
                        if(gate[j].gate_type == "NANDX1"){
                            gate[i].outload += lu_table.NANDX1.input_pin[1].capacitance;
                        }
                    }

                    if(gate[j].gate_input[k].input_type == "I"){

                        if(gate[j].gate_type == "INVX1"){
                            gate[i].outload += lu_table.INVX1.input_pin[0].capacitance;
                        }
                    }

                }

            }

        } 

        
        int out_size = output.size(); 
        for(int k = 0; k < out_size; k++){
            if(gate[i].gate_output == output[k]){
                gate[i].outload += 0.03;
            }
        }


    }

    sort(gate.begin(), gate.end(), outload_compare);
    for(int i = 0; i < gate_size; i++){
    FILEOUT_load << gate[i].name << " " << gate[i].outload << endl;
    }

}

double Timing_Analyzer::interpolation(double trantime, double capload, string gatename, string dataname){

    double trantime_ratio, capload_ratio;
    int t_index = INT16_MAX;
    int c_index = INT16_MAX;
    double capload_high, capload_low;

    int capload_size = lu_table.index_1.size();
    for(int i = 0; i < capload_size; i ++){

        if(capload < lu_table.index_1[i]){
            c_index = i - 1;
            capload_ratio = (capload - lu_table.index_1[i-1]) / (lu_table.index_1[i] - lu_table.index_1[i-1]);
            break;
        }

    }

    if(c_index == -1){ /// less than capload[0]
       capload_ratio = (lu_table.index_1[0] - capload)/ (lu_table.index_1[1] - lu_table.index_1[0]);
    }
    if(c_index == INT16_MAX){ /// less than capload[0]
       capload_ratio = (capload - lu_table.index_1[capload_size - 1])/ (lu_table.index_1[capload_size - 1] - lu_table.index_1[capload_size - 2]);
    }
 
    int trantime_size = lu_table.index_2.size();
    for(int i = 0; i < trantime_size; i ++){
        
        if(trantime < lu_table.index_2[i]){
            t_index = i - 1;
            trantime_ratio = (trantime - lu_table.index_2[i-1]) / (lu_table.index_2[i] - lu_table.index_2[i-1]);
            break;
        }

    }

    if(t_index == -1){ /// less than tran[0]
       trantime_ratio = (lu_table.index_2[0] - trantime)/ (lu_table.index_2[1] - lu_table.index_2[0]);
    }
    if(t_index == INT16_MAX){ /// less than tran[0]
       trantime_ratio = (trantime - lu_table.index_2[trantime_size - 1])/ (lu_table.index_2[trantime_size - 1] - lu_table.index_2[trantime_size - 2]);
    }

    if(t_index != -1 && t_index != INT16_MAX){

        if(gatename == "NOR2X1"){
            if(dataname == "cell_rise"){
                if(c_index == -1){
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[t_index][0].cell_rise - (lu_table.NOR2X1.output_pin.tablevalue[t_index][1].cell_rise - lu_table.NOR2X1.output_pin.tablevalue[t_index][0].cell_rise) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[t_index+1][0].cell_rise - (lu_table.NOR2X1.output_pin.tablevalue[t_index+1][1].cell_rise - lu_table.NOR2X1.output_pin.tablevalue[t_index+1][0].cell_rise) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[t_index][trantime_size - 1].cell_rise + (lu_table.NOR2X1.output_pin.tablevalue[t_index][trantime_size - 1].cell_rise - lu_table.NOR2X1.output_pin.tablevalue[t_index][trantime_size - 2].cell_rise) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[t_index+1][trantime_size - 1].cell_rise + (lu_table.NOR2X1.output_pin.tablevalue[t_index+1][trantime_size - 1].cell_rise - lu_table.NOR2X1.output_pin.tablevalue[t_index+1][trantime_size - 2].cell_rise) * capload_ratio;
                }else{
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[t_index][c_index].cell_rise + (lu_table.NOR2X1.output_pin.tablevalue[t_index][c_index+1].cell_rise - lu_table.NOR2X1.output_pin.tablevalue[t_index][c_index].cell_rise) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[t_index+1][c_index].cell_rise + (lu_table.NOR2X1.output_pin.tablevalue[t_index+1][c_index+1].cell_rise - lu_table.NOR2X1.output_pin.tablevalue[t_index+1][c_index].cell_rise) * capload_ratio;
                }
            }
            if(dataname == "cell_fall"){
               if(c_index == -1){
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[t_index][0].cell_fall - (lu_table.NOR2X1.output_pin.tablevalue[t_index][1].cell_fall - lu_table.NOR2X1.output_pin.tablevalue[t_index][0].cell_fall) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[t_index+1][0].cell_fall - (lu_table.NOR2X1.output_pin.tablevalue[t_index+1][1].cell_fall - lu_table.NOR2X1.output_pin.tablevalue[t_index+1][0].cell_fall) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[t_index][trantime_size - 1].cell_fall + (lu_table.NOR2X1.output_pin.tablevalue[t_index][trantime_size - 1].cell_fall - lu_table.NOR2X1.output_pin.tablevalue[t_index][trantime_size - 2].cell_fall) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[t_index+1][trantime_size - 1].cell_fall + (lu_table.NOR2X1.output_pin.tablevalue[t_index+1][trantime_size - 1].cell_fall - lu_table.NOR2X1.output_pin.tablevalue[t_index+1][trantime_size - 2].cell_fall) * capload_ratio;
                }else{              
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[t_index][c_index].cell_fall + (lu_table.NOR2X1.output_pin.tablevalue[t_index][c_index+1].cell_fall - lu_table.NOR2X1.output_pin.tablevalue[t_index][c_index].cell_fall) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[t_index+1][c_index].cell_fall + (lu_table.NOR2X1.output_pin.tablevalue[t_index+1][c_index+1].cell_fall - lu_table.NOR2X1.output_pin.tablevalue[t_index+1][c_index].cell_fall) * capload_ratio;

                }
            }
            if(dataname == "rise_transition"){
                if(c_index == -1){
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[t_index][0].rise_transition - (lu_table.NOR2X1.output_pin.tablevalue[t_index][1].rise_transition - lu_table.NOR2X1.output_pin.tablevalue[t_index][0].rise_transition) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[t_index+1][0].rise_transition - (lu_table.NOR2X1.output_pin.tablevalue[t_index+1][1].rise_transition - lu_table.NOR2X1.output_pin.tablevalue[t_index+1][0].rise_transition) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[t_index][trantime_size - 1].rise_transition + (lu_table.NOR2X1.output_pin.tablevalue[t_index][trantime_size - 1].rise_transition - lu_table.NOR2X1.output_pin.tablevalue[t_index][trantime_size - 2].rise_transition) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[t_index+1][trantime_size - 1].rise_transition + (lu_table.NOR2X1.output_pin.tablevalue[t_index+1][trantime_size - 1].rise_transition - lu_table.NOR2X1.output_pin.tablevalue[t_index+1][trantime_size - 2].rise_transition) * capload_ratio;
                }else{
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[t_index][c_index].rise_transition + (lu_table.NOR2X1.output_pin.tablevalue[t_index][c_index+1].rise_transition - lu_table.NOR2X1.output_pin.tablevalue[t_index][c_index].rise_transition) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[t_index+1][c_index].rise_transition + (lu_table.NOR2X1.output_pin.tablevalue[t_index+1][c_index+1].rise_transition - lu_table.NOR2X1.output_pin.tablevalue[t_index+1][c_index].rise_transition) * capload_ratio;
                }
            }
            if(dataname == "fall_transition"){
                if(c_index == -1){
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[t_index][0].fall_transition - (lu_table.NOR2X1.output_pin.tablevalue[t_index][1].fall_transition - lu_table.NOR2X1.output_pin.tablevalue[t_index][0].fall_transition) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[t_index+1][0].fall_transition - (lu_table.NOR2X1.output_pin.tablevalue[t_index+1][1].fall_transition - lu_table.NOR2X1.output_pin.tablevalue[t_index+1][0].fall_transition) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[t_index][trantime_size - 1].fall_transition + (lu_table.NOR2X1.output_pin.tablevalue[t_index][trantime_size - 1].fall_transition - lu_table.NOR2X1.output_pin.tablevalue[t_index][trantime_size - 2].fall_transition) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[t_index+1][trantime_size - 1].fall_transition + (lu_table.NOR2X1.output_pin.tablevalue[t_index+1][trantime_size - 1].fall_transition - lu_table.NOR2X1.output_pin.tablevalue[t_index+1][trantime_size - 2].fall_transition) * capload_ratio;
                }else{
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[t_index][c_index].fall_transition + (lu_table.NOR2X1.output_pin.tablevalue[t_index][c_index+1].fall_transition - lu_table.NOR2X1.output_pin.tablevalue[t_index][c_index].fall_transition) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[t_index+1][c_index].fall_transition + (lu_table.NOR2X1.output_pin.tablevalue[t_index+1][c_index+1].fall_transition - lu_table.NOR2X1.output_pin.tablevalue[t_index+1][c_index].fall_transition) * capload_ratio;
                }
            }
        }

        if(gatename == "INVX1"){
            if(dataname == "cell_rise"){
                if(c_index == -1){
                    capload_high = lu_table.INVX1.output_pin.tablevalue[t_index][0].cell_rise - (lu_table.INVX1.output_pin.tablevalue[t_index][1].cell_rise - lu_table.INVX1.output_pin.tablevalue[t_index][0].cell_rise) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[t_index+1][0].cell_rise - (lu_table.INVX1.output_pin.tablevalue[t_index+1][1].cell_rise - lu_table.INVX1.output_pin.tablevalue[t_index+1][0].cell_rise) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.INVX1.output_pin.tablevalue[t_index][trantime_size - 1].cell_rise + (lu_table.INVX1.output_pin.tablevalue[t_index][trantime_size - 1].cell_rise - lu_table.INVX1.output_pin.tablevalue[t_index][trantime_size - 2].cell_rise) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[t_index+1][trantime_size - 1].cell_rise + (lu_table.INVX1.output_pin.tablevalue[t_index+1][trantime_size - 1].cell_rise - lu_table.INVX1.output_pin.tablevalue[t_index+1][trantime_size - 2].cell_rise) * capload_ratio;
                }else{
                    capload_high = lu_table.INVX1.output_pin.tablevalue[t_index][c_index].cell_rise + (lu_table.INVX1.output_pin.tablevalue[t_index][c_index+1].cell_rise - lu_table.INVX1.output_pin.tablevalue[t_index][c_index].cell_rise) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[t_index+1][c_index].cell_rise + (lu_table.INVX1.output_pin.tablevalue[t_index+1][c_index+1].cell_rise - lu_table.INVX1.output_pin.tablevalue[t_index+1][c_index].cell_rise) * capload_ratio;
                }
            }
            if(dataname == "cell_fall"){
               if(c_index == -1){
                    capload_high = lu_table.INVX1.output_pin.tablevalue[t_index][0].cell_fall - (lu_table.INVX1.output_pin.tablevalue[t_index][1].cell_fall - lu_table.INVX1.output_pin.tablevalue[t_index][0].cell_fall) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[t_index+1][0].cell_fall - (lu_table.INVX1.output_pin.tablevalue[t_index+1][1].cell_fall - lu_table.INVX1.output_pin.tablevalue[t_index+1][0].cell_fall) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.INVX1.output_pin.tablevalue[t_index][trantime_size - 1].cell_fall + (lu_table.INVX1.output_pin.tablevalue[t_index][trantime_size - 1].cell_fall - lu_table.INVX1.output_pin.tablevalue[t_index][trantime_size - 2].cell_fall) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[t_index+1][trantime_size - 1].cell_fall + (lu_table.INVX1.output_pin.tablevalue[t_index+1][trantime_size - 1].cell_fall - lu_table.INVX1.output_pin.tablevalue[t_index+1][trantime_size - 2].cell_fall) * capload_ratio;
                }else{              
                    capload_high = lu_table.INVX1.output_pin.tablevalue[t_index][c_index].cell_fall + (lu_table.INVX1.output_pin.tablevalue[t_index][c_index+1].cell_fall - lu_table.INVX1.output_pin.tablevalue[t_index][c_index].cell_fall) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[t_index+1][c_index].cell_fall + (lu_table.INVX1.output_pin.tablevalue[t_index+1][c_index+1].cell_fall - lu_table.INVX1.output_pin.tablevalue[t_index+1][c_index].cell_fall) * capload_ratio;
                }
            }
            if(dataname == "rise_transition"){
                if(c_index == -1){
                    capload_high = lu_table.INVX1.output_pin.tablevalue[t_index][0].rise_transition - (lu_table.INVX1.output_pin.tablevalue[t_index][1].rise_transition - lu_table.INVX1.output_pin.tablevalue[t_index][0].rise_transition) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[t_index+1][0].rise_transition - (lu_table.INVX1.output_pin.tablevalue[t_index+1][1].rise_transition - lu_table.INVX1.output_pin.tablevalue[t_index+1][0].rise_transition) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.INVX1.output_pin.tablevalue[t_index][trantime_size - 1].rise_transition + (lu_table.INVX1.output_pin.tablevalue[t_index][trantime_size - 1].rise_transition - lu_table.INVX1.output_pin.tablevalue[t_index][trantime_size - 2].rise_transition) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[t_index+1][trantime_size - 1].rise_transition + (lu_table.INVX1.output_pin.tablevalue[t_index+1][trantime_size - 1].rise_transition - lu_table.INVX1.output_pin.tablevalue[t_index+1][trantime_size - 2].rise_transition) * capload_ratio;
                }else{
                    capload_high = lu_table.INVX1.output_pin.tablevalue[t_index][c_index].rise_transition + (lu_table.INVX1.output_pin.tablevalue[t_index][c_index+1].rise_transition - lu_table.INVX1.output_pin.tablevalue[t_index][c_index].rise_transition) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[t_index+1][c_index].rise_transition + (lu_table.INVX1.output_pin.tablevalue[t_index+1][c_index+1].rise_transition - lu_table.INVX1.output_pin.tablevalue[t_index+1][c_index].rise_transition) * capload_ratio;
                }
            }
            if(dataname == "fall_transition"){
                if(c_index == -1){
                    capload_high = lu_table.INVX1.output_pin.tablevalue[t_index][0].fall_transition - (lu_table.INVX1.output_pin.tablevalue[t_index][1].fall_transition - lu_table.INVX1.output_pin.tablevalue[t_index][0].fall_transition) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[t_index+1][0].fall_transition - (lu_table.INVX1.output_pin.tablevalue[t_index+1][1].fall_transition - lu_table.INVX1.output_pin.tablevalue[t_index+1][0].fall_transition) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.INVX1.output_pin.tablevalue[t_index][trantime_size - 1].fall_transition + (lu_table.INVX1.output_pin.tablevalue[t_index][trantime_size - 1].fall_transition - lu_table.INVX1.output_pin.tablevalue[t_index][trantime_size - 2].fall_transition) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[t_index+1][trantime_size - 1].fall_transition + (lu_table.INVX1.output_pin.tablevalue[t_index+1][trantime_size - 1].fall_transition - lu_table.INVX1.output_pin.tablevalue[t_index+1][trantime_size - 2].fall_transition) * capload_ratio;
                }else{
                    capload_high = lu_table.INVX1.output_pin.tablevalue[t_index][c_index].fall_transition + (lu_table.INVX1.output_pin.tablevalue[t_index][c_index+1].fall_transition - lu_table.INVX1.output_pin.tablevalue[t_index][c_index].fall_transition) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[t_index+1][c_index].fall_transition + (lu_table.INVX1.output_pin.tablevalue[t_index+1][c_index+1].fall_transition - lu_table.INVX1.output_pin.tablevalue[t_index+1][c_index].fall_transition) * capload_ratio;
                }
            }
        }

        if(gatename == "NANDX1"){

            if(dataname == "cell_rise"){

                if(c_index == -1){
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[t_index][0].cell_rise - (lu_table.NANDX1.output_pin.tablevalue[t_index][1].cell_rise - lu_table.NANDX1.output_pin.tablevalue[t_index][0].cell_rise) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[t_index+1][0].cell_rise - (lu_table.NANDX1.output_pin.tablevalue[t_index+1][1].cell_rise - lu_table.NANDX1.output_pin.tablevalue[t_index+1][0].cell_rise) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[t_index][trantime_size - 1].cell_rise + (lu_table.NANDX1.output_pin.tablevalue[t_index][trantime_size - 1].cell_rise - lu_table.NANDX1.output_pin.tablevalue[t_index][trantime_size - 2].cell_rise) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[t_index+1][trantime_size - 1].cell_rise + (lu_table.NANDX1.output_pin.tablevalue[t_index+1][trantime_size - 1].cell_rise - lu_table.NANDX1.output_pin.tablevalue[t_index+1][trantime_size - 2].cell_rise) * capload_ratio;
                }else{
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[t_index][c_index].cell_rise + (lu_table.NANDX1.output_pin.tablevalue[t_index][c_index+1].cell_rise - lu_table.NANDX1.output_pin.tablevalue[t_index][c_index].cell_rise) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[t_index+1][c_index].cell_rise + (lu_table.NANDX1.output_pin.tablevalue[t_index+1][c_index+1].cell_rise - lu_table.NANDX1.output_pin.tablevalue[t_index+1][c_index].cell_rise) * capload_ratio;
            
                }
            }
            if(dataname == "cell_fall"){

               if(c_index == -1){
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[t_index][0].cell_fall - (lu_table.NANDX1.output_pin.tablevalue[t_index][1].cell_fall - lu_table.NANDX1.output_pin.tablevalue[t_index][0].cell_fall) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[t_index+1][0].cell_fall - (lu_table.NANDX1.output_pin.tablevalue[t_index+1][1].cell_fall - lu_table.NANDX1.output_pin.tablevalue[t_index+1][0].cell_fall) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[t_index][trantime_size - 1].cell_fall + (lu_table.NANDX1.output_pin.tablevalue[t_index][trantime_size - 1].cell_fall - lu_table.NANDX1.output_pin.tablevalue[t_index][trantime_size - 2].cell_fall) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[t_index+1][trantime_size - 1].cell_fall + (lu_table.NANDX1.output_pin.tablevalue[t_index+1][trantime_size - 1].cell_fall - lu_table.NANDX1.output_pin.tablevalue[t_index+1][trantime_size - 2].cell_fall) * capload_ratio;
                }else{    
                       
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[t_index][c_index].cell_fall + (lu_table.NANDX1.output_pin.tablevalue[t_index][c_index+1].cell_fall - lu_table.NANDX1.output_pin.tablevalue[t_index][c_index].cell_fall) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[t_index+1][c_index].cell_fall + (lu_table.NANDX1.output_pin.tablevalue[t_index+1][c_index+1].cell_fall - lu_table.NANDX1.output_pin.tablevalue[t_index+1][c_index].cell_fall) * capload_ratio;
                }
            }
            if(dataname == "rise_transition"){

                if(c_index == -1){

                    capload_high = lu_table.NANDX1.output_pin.tablevalue[t_index][0].rise_transition - (lu_table.NANDX1.output_pin.tablevalue[t_index][1].rise_transition - lu_table.NANDX1.output_pin.tablevalue[t_index][0].rise_transition) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[t_index+1][0].rise_transition - (lu_table.NANDX1.output_pin.tablevalue[t_index+1][1].rise_transition - lu_table.NANDX1.output_pin.tablevalue[t_index+1][0].rise_transition) * capload_ratio;
                }
                else if(c_index == INT16_MAX){

                    capload_high = lu_table.NANDX1.output_pin.tablevalue[t_index][trantime_size - 1].rise_transition + (lu_table.NANDX1.output_pin.tablevalue[t_index][trantime_size - 1].rise_transition - lu_table.NANDX1.output_pin.tablevalue[t_index][trantime_size - 2].rise_transition) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[t_index+1][trantime_size - 1].rise_transition + (lu_table.NANDX1.output_pin.tablevalue[t_index+1][trantime_size - 1].rise_transition - lu_table.NANDX1.output_pin.tablevalue[t_index+1][trantime_size - 2].rise_transition) * capload_ratio;
                }else{

                    capload_high = lu_table.NANDX1.output_pin.tablevalue[t_index][c_index].rise_transition + (lu_table.NANDX1.output_pin.tablevalue[t_index][c_index+1].rise_transition - lu_table.NANDX1.output_pin.tablevalue[t_index][c_index].rise_transition) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[t_index+1][c_index].rise_transition + (lu_table.NANDX1.output_pin.tablevalue[t_index+1][c_index+1].rise_transition - lu_table.NANDX1.output_pin.tablevalue[t_index+1][c_index].rise_transition) * capload_ratio;
                }
            }
            if(dataname == "fall_transition"){

                if(c_index == -1){
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[t_index][0].fall_transition - (lu_table.NANDX1.output_pin.tablevalue[t_index][1].fall_transition - lu_table.NANDX1.output_pin.tablevalue[t_index][0].fall_transition) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[t_index+1][0].fall_transition - (lu_table.NANDX1.output_pin.tablevalue[t_index+1][1].fall_transition - lu_table.NANDX1.output_pin.tablevalue[t_index+1][0].fall_transition) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[t_index][trantime_size - 1].fall_transition + (lu_table.NANDX1.output_pin.tablevalue[t_index][trantime_size - 1].fall_transition - lu_table.NANDX1.output_pin.tablevalue[t_index][trantime_size - 2].fall_transition) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[t_index+1][trantime_size - 1].fall_transition + (lu_table.NANDX1.output_pin.tablevalue[t_index+1][trantime_size - 1].fall_transition - lu_table.NANDX1.output_pin.tablevalue[t_index+1][trantime_size - 2].fall_transition) * capload_ratio;
                }else{
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[t_index][c_index].fall_transition + (lu_table.NANDX1.output_pin.tablevalue[t_index][c_index+1].fall_transition - lu_table.NANDX1.output_pin.tablevalue[t_index][c_index].fall_transition) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[t_index+1][c_index].fall_transition + (lu_table.NANDX1.output_pin.tablevalue[t_index+1][c_index+1].fall_transition - lu_table.NANDX1.output_pin.tablevalue[t_index+1][c_index].fall_transition) * capload_ratio;
                }
            }
        }
        return capload_high + (capload_low - capload_high) * trantime_ratio;


    }else if(t_index == -1){

        if(gatename == "NOR2X1"){
            
            if(dataname == "cell_rise"){
                if(c_index == -1){
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[0][0].cell_rise - (lu_table.NOR2X1.output_pin.tablevalue[0][1].cell_rise - lu_table.NOR2X1.output_pin.tablevalue[0][0].cell_rise) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[1][0].cell_rise - (lu_table.NOR2X1.output_pin.tablevalue[1][1].cell_rise - lu_table.NOR2X1.output_pin.tablevalue[1][0].cell_rise) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[0][trantime_size - 1].cell_rise + (lu_table.NOR2X1.output_pin.tablevalue[0][trantime_size - 1].cell_rise - lu_table.NOR2X1.output_pin.tablevalue[0][trantime_size - 2].cell_rise) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[1][trantime_size - 1].cell_rise + (lu_table.NOR2X1.output_pin.tablevalue[1][trantime_size - 1].cell_rise - lu_table.NOR2X1.output_pin.tablevalue[1][trantime_size - 2].cell_rise) * capload_ratio;
                }else{
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[0][c_index].cell_rise + (lu_table.NOR2X1.output_pin.tablevalue[0][c_index+1].cell_rise - lu_table.NOR2X1.output_pin.tablevalue[0][c_index].cell_rise) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[1][c_index].cell_rise + (lu_table.NOR2X1.output_pin.tablevalue[1][c_index+1].cell_rise - lu_table.NOR2X1.output_pin.tablevalue[1][c_index].cell_rise) * capload_ratio;
                }
            }
            if(dataname == "cell_fall"){
               if(c_index == -1){
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[0][0].cell_fall - (lu_table.NOR2X1.output_pin.tablevalue[0][1].cell_fall - lu_table.NOR2X1.output_pin.tablevalue[0][0].cell_fall) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[1][0].cell_fall - (lu_table.NOR2X1.output_pin.tablevalue[1][1].cell_fall - lu_table.NOR2X1.output_pin.tablevalue[1][0].cell_fall) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[0][trantime_size - 1].cell_fall + (lu_table.NOR2X1.output_pin.tablevalue[0][trantime_size - 1].cell_fall - lu_table.NOR2X1.output_pin.tablevalue[0][trantime_size - 2].cell_fall) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[1][trantime_size - 1].cell_fall + (lu_table.NOR2X1.output_pin.tablevalue[1][trantime_size - 1].cell_fall - lu_table.NOR2X1.output_pin.tablevalue[1][trantime_size - 2].cell_fall) * capload_ratio;
                }else{                
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[0][c_index].cell_fall + (lu_table.NOR2X1.output_pin.tablevalue[0][c_index+1].cell_fall - lu_table.NOR2X1.output_pin.tablevalue[0][c_index].cell_fall) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[1][c_index].cell_fall + (lu_table.NOR2X1.output_pin.tablevalue[1][c_index+1].cell_fall - lu_table.NOR2X1.output_pin.tablevalue[1][c_index].cell_fall) * capload_ratio;
                }
            }
            if(dataname == "rise_transition"){
                
                if(c_index == -1){
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[0][0].rise_transition - (lu_table.NOR2X1.output_pin.tablevalue[0][1].rise_transition - lu_table.NOR2X1.output_pin.tablevalue[0][0].rise_transition) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[1][0].rise_transition - (lu_table.NOR2X1.output_pin.tablevalue[1][1].rise_transition - lu_table.NOR2X1.output_pin.tablevalue[1][0].rise_transition) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[0][trantime_size - 1].rise_transition + (lu_table.NOR2X1.output_pin.tablevalue[0][trantime_size - 1].rise_transition - lu_table.NOR2X1.output_pin.tablevalue[0][trantime_size - 2].rise_transition) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[1][trantime_size - 1].rise_transition + (lu_table.NOR2X1.output_pin.tablevalue[1][trantime_size - 1].rise_transition - lu_table.NOR2X1.output_pin.tablevalue[1][trantime_size - 2].rise_transition) * capload_ratio;
                }else{
                
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[0][c_index].rise_transition + (lu_table.NOR2X1.output_pin.tablevalue[0][c_index+1].rise_transition - lu_table.NOR2X1.output_pin.tablevalue[0][c_index].rise_transition) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[1][c_index].rise_transition + (lu_table.NOR2X1.output_pin.tablevalue[1][c_index+1].rise_transition - lu_table.NOR2X1.output_pin.tablevalue[1][c_index].rise_transition) * capload_ratio;
                }
            }
            if(dataname == "fall_transition"){
                
                if(c_index == -1){
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[0][0].fall_transition - (lu_table.NOR2X1.output_pin.tablevalue[0][1].fall_transition - lu_table.NOR2X1.output_pin.tablevalue[0][0].fall_transition) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[1][0].fall_transition - (lu_table.NOR2X1.output_pin.tablevalue[1][1].fall_transition - lu_table.NOR2X1.output_pin.tablevalue[1][0].fall_transition) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[0][trantime_size - 1].fall_transition + (lu_table.NOR2X1.output_pin.tablevalue[0][trantime_size - 1].fall_transition - lu_table.NOR2X1.output_pin.tablevalue[0][trantime_size - 2].fall_transition) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[1][trantime_size - 1].fall_transition + (lu_table.NOR2X1.output_pin.tablevalue[1][trantime_size - 1].fall_transition - lu_table.NOR2X1.output_pin.tablevalue[1][trantime_size - 2].fall_transition) * capload_ratio;
                }else{
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[0][c_index].fall_transition + (lu_table.NOR2X1.output_pin.tablevalue[0][c_index+1].fall_transition - lu_table.NOR2X1.output_pin.tablevalue[0][c_index].fall_transition) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[1][c_index].fall_transition + (lu_table.NOR2X1.output_pin.tablevalue[1][c_index+1].fall_transition - lu_table.NOR2X1.output_pin.tablevalue[1][c_index].fall_transition) * capload_ratio;

                }
            }
        }

        if(gatename == "INVX1"){
            if(dataname == "cell_rise"){
                if(c_index == -1){
                    capload_high = lu_table.INVX1.output_pin.tablevalue[0][0].cell_rise - (lu_table.INVX1.output_pin.tablevalue[0][1].cell_rise - lu_table.INVX1.output_pin.tablevalue[0][0].cell_rise) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[1][0].cell_rise - (lu_table.INVX1.output_pin.tablevalue[1][1].cell_rise - lu_table.INVX1.output_pin.tablevalue[1][0].cell_rise) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.INVX1.output_pin.tablevalue[0][trantime_size - 1].cell_rise + (lu_table.INVX1.output_pin.tablevalue[0][trantime_size - 1].cell_rise - lu_table.INVX1.output_pin.tablevalue[0][trantime_size - 2].cell_rise) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[1][trantime_size - 1].cell_rise + (lu_table.INVX1.output_pin.tablevalue[1][trantime_size - 1].cell_rise - lu_table.INVX1.output_pin.tablevalue[1][trantime_size - 2].cell_rise) * capload_ratio;
                }else{
                    capload_high = lu_table.INVX1.output_pin.tablevalue[0][c_index].cell_rise + (lu_table.INVX1.output_pin.tablevalue[0][c_index+1].cell_rise - lu_table.INVX1.output_pin.tablevalue[0][c_index].cell_rise) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[1][c_index].cell_rise + (lu_table.INVX1.output_pin.tablevalue[1][c_index+1].cell_rise - lu_table.INVX1.output_pin.tablevalue[1][c_index].cell_rise) * capload_ratio;
                }
            }
            if(dataname == "cell_fall"){
               if(c_index == -1){
                    capload_high = lu_table.INVX1.output_pin.tablevalue[0][0].cell_fall - (lu_table.INVX1.output_pin.tablevalue[0][1].cell_fall - lu_table.INVX1.output_pin.tablevalue[0][0].cell_fall) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[1][0].cell_fall - (lu_table.INVX1.output_pin.tablevalue[1][1].cell_fall - lu_table.INVX1.output_pin.tablevalue[1][0].cell_fall) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.INVX1.output_pin.tablevalue[0][trantime_size - 1].cell_fall + (lu_table.INVX1.output_pin.tablevalue[0][trantime_size - 1].cell_fall - lu_table.INVX1.output_pin.tablevalue[0][trantime_size - 2].cell_fall) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[1][trantime_size - 1].cell_fall + (lu_table.INVX1.output_pin.tablevalue[1][trantime_size - 1].cell_fall - lu_table.INVX1.output_pin.tablevalue[1][trantime_size - 2].cell_fall) * capload_ratio;
                }else{               
                    capload_high = lu_table.INVX1.output_pin.tablevalue[0][c_index].cell_fall + (lu_table.INVX1.output_pin.tablevalue[0][c_index+1].cell_fall - lu_table.INVX1.output_pin.tablevalue[0][c_index].cell_fall) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[1][c_index].cell_fall + (lu_table.INVX1.output_pin.tablevalue[1][c_index+1].cell_fall - lu_table.INVX1.output_pin.tablevalue[1][c_index].cell_fall) * capload_ratio;
                }
            }
            if(dataname == "rise_transition"){
                if(c_index == -1){
                    capload_high = lu_table.INVX1.output_pin.tablevalue[0][0].rise_transition - (lu_table.INVX1.output_pin.tablevalue[0][1].rise_transition - lu_table.INVX1.output_pin.tablevalue[0][0].rise_transition) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[1][0].rise_transition - (lu_table.INVX1.output_pin.tablevalue[1][1].rise_transition - lu_table.INVX1.output_pin.tablevalue[1][0].rise_transition) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.INVX1.output_pin.tablevalue[0][trantime_size - 1].rise_transition + (lu_table.INVX1.output_pin.tablevalue[0][trantime_size - 1].rise_transition - lu_table.INVX1.output_pin.tablevalue[0][trantime_size - 2].rise_transition) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[1][trantime_size - 1].rise_transition + (lu_table.INVX1.output_pin.tablevalue[1][trantime_size - 1].rise_transition - lu_table.INVX1.output_pin.tablevalue[1][trantime_size - 2].rise_transition) * capload_ratio;
                }else{
                    capload_high = lu_table.INVX1.output_pin.tablevalue[0][c_index].rise_transition + (lu_table.INVX1.output_pin.tablevalue[0][c_index+1].rise_transition - lu_table.INVX1.output_pin.tablevalue[0][c_index].rise_transition) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[1][c_index].rise_transition + (lu_table.INVX1.output_pin.tablevalue[1][c_index+1].rise_transition - lu_table.INVX1.output_pin.tablevalue[1][c_index].rise_transition) * capload_ratio;
                }
            }
            if(dataname == "fall_transition"){
                if(c_index == -1){
                    capload_high = lu_table.INVX1.output_pin.tablevalue[0][0].fall_transition - (lu_table.INVX1.output_pin.tablevalue[0][1].fall_transition - lu_table.INVX1.output_pin.tablevalue[0][0].fall_transition) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[1][0].fall_transition - (lu_table.INVX1.output_pin.tablevalue[1][1].fall_transition - lu_table.INVX1.output_pin.tablevalue[1][0].fall_transition) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.INVX1.output_pin.tablevalue[0][trantime_size - 1].fall_transition + (lu_table.INVX1.output_pin.tablevalue[0][trantime_size - 1].fall_transition - lu_table.INVX1.output_pin.tablevalue[0][trantime_size - 2].fall_transition) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[1][trantime_size - 1].fall_transition + (lu_table.INVX1.output_pin.tablevalue[1][trantime_size - 1].fall_transition - lu_table.INVX1.output_pin.tablevalue[1][trantime_size - 2].fall_transition) * capload_ratio;
                }else{
                    capload_high = lu_table.INVX1.output_pin.tablevalue[0][c_index].fall_transition + (lu_table.INVX1.output_pin.tablevalue[0][c_index+1].fall_transition - lu_table.INVX1.output_pin.tablevalue[0][c_index].fall_transition) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[1][c_index].fall_transition + (lu_table.INVX1.output_pin.tablevalue[1][c_index+1].fall_transition - lu_table.INVX1.output_pin.tablevalue[1][c_index].fall_transition) * capload_ratio;
                }
            }
        }

        if(gatename == "NANDX1"){
            if(dataname == "cell_rise"){
                if(c_index == -1){
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[0][0].cell_rise - (lu_table.NANDX1.output_pin.tablevalue[0][1].cell_rise - lu_table.NANDX1.output_pin.tablevalue[0][0].cell_rise) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[1][0].cell_rise - (lu_table.NANDX1.output_pin.tablevalue[1][1].cell_rise - lu_table.NANDX1.output_pin.tablevalue[1][0].cell_rise) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[0][trantime_size - 1].cell_rise + (lu_table.NANDX1.output_pin.tablevalue[0][trantime_size - 1].cell_rise - lu_table.NANDX1.output_pin.tablevalue[0][trantime_size - 2].cell_rise) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[1][trantime_size - 1].cell_rise + (lu_table.NANDX1.output_pin.tablevalue[1][trantime_size - 1].cell_rise - lu_table.NANDX1.output_pin.tablevalue[1][trantime_size - 2].cell_rise) * capload_ratio;
                }else{
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[0][c_index].cell_rise + (lu_table.NANDX1.output_pin.tablevalue[0][c_index+1].cell_rise - lu_table.NANDX1.output_pin.tablevalue[0][c_index].cell_rise) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[1][c_index].cell_rise + (lu_table.NANDX1.output_pin.tablevalue[1][c_index+1].cell_rise - lu_table.NANDX1.output_pin.tablevalue[1][c_index].cell_rise) * capload_ratio;
                }
            }
            if(dataname == "cell_fall"){
               if(c_index == -1){
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[0][0].cell_fall - (lu_table.NANDX1.output_pin.tablevalue[0][1].cell_fall - lu_table.NANDX1.output_pin.tablevalue[0][0].cell_fall) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[1][0].cell_fall - (lu_table.NANDX1.output_pin.tablevalue[1][1].cell_fall - lu_table.NANDX1.output_pin.tablevalue[1][0].cell_fall) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[0][trantime_size - 1].cell_fall + (lu_table.NANDX1.output_pin.tablevalue[0][trantime_size - 1].cell_fall - lu_table.NANDX1.output_pin.tablevalue[0][trantime_size - 2].cell_fall) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[1][trantime_size - 1].cell_fall + (lu_table.NANDX1.output_pin.tablevalue[1][trantime_size - 1].cell_fall - lu_table.NANDX1.output_pin.tablevalue[1][trantime_size - 2].cell_fall) * capload_ratio;
                }else{                
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[0][c_index].cell_fall + (lu_table.NANDX1.output_pin.tablevalue[0][c_index+1].cell_fall - lu_table.NANDX1.output_pin.tablevalue[0][c_index].cell_fall) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[1][c_index].cell_fall + (lu_table.NANDX1.output_pin.tablevalue[1][c_index+1].cell_fall - lu_table.NANDX1.output_pin.tablevalue[1][c_index].cell_fall) * capload_ratio;
                }
            }
            if(dataname == "rise_transition"){
                if(c_index == -1){
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[0][0].rise_transition - (lu_table.NANDX1.output_pin.tablevalue[0][1].rise_transition - lu_table.NANDX1.output_pin.tablevalue[0][0].rise_transition) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[1][0].rise_transition - (lu_table.NANDX1.output_pin.tablevalue[1][1].rise_transition - lu_table.NANDX1.output_pin.tablevalue[1][0].rise_transition) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[0][trantime_size - 1].rise_transition + (lu_table.NANDX1.output_pin.tablevalue[0][trantime_size - 1].rise_transition - lu_table.NANDX1.output_pin.tablevalue[0][trantime_size - 2].rise_transition) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[1][trantime_size - 1].rise_transition + (lu_table.NANDX1.output_pin.tablevalue[1][trantime_size - 1].rise_transition - lu_table.NANDX1.output_pin.tablevalue[1][trantime_size - 2].rise_transition) * capload_ratio;
                }else{
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[0][c_index].rise_transition + (lu_table.NANDX1.output_pin.tablevalue[0][c_index+1].rise_transition - lu_table.NANDX1.output_pin.tablevalue[0][c_index].rise_transition) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[1][c_index].rise_transition + (lu_table.NANDX1.output_pin.tablevalue[1][c_index+1].rise_transition - lu_table.NANDX1.output_pin.tablevalue[1][c_index].rise_transition) * capload_ratio;
                }
            }
            if(dataname == "fall_transition"){
                if(c_index == -1){
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[0][0].fall_transition - (lu_table.NANDX1.output_pin.tablevalue[0][1].fall_transition - lu_table.NANDX1.output_pin.tablevalue[0][0].fall_transition) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[1][0].fall_transition - (lu_table.NANDX1.output_pin.tablevalue[1][1].fall_transition - lu_table.NANDX1.output_pin.tablevalue[1][0].fall_transition) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[0][trantime_size - 1].fall_transition + (lu_table.NANDX1.output_pin.tablevalue[0][trantime_size - 1].fall_transition - lu_table.NANDX1.output_pin.tablevalue[0][trantime_size - 2].fall_transition) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[1][trantime_size - 1].fall_transition + (lu_table.NANDX1.output_pin.tablevalue[1][trantime_size - 1].fall_transition - lu_table.NANDX1.output_pin.tablevalue[1][trantime_size - 2].fall_transition) * capload_ratio;
                }else{
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[0][c_index].fall_transition + (lu_table.NANDX1.output_pin.tablevalue[0][c_index+1].fall_transition - lu_table.NANDX1.output_pin.tablevalue[0][c_index].fall_transition) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[1][c_index].fall_transition + (lu_table.NANDX1.output_pin.tablevalue[1][c_index+1].fall_transition - lu_table.NANDX1.output_pin.tablevalue[1][c_index].fall_transition) * capload_ratio;
                }
            }
        }
        return capload_high - (capload_low - capload_high) * trantime_ratio;
    }
    
    else if(t_index == INT16_MAX){
        if(gatename == "NOR2X1"){
            if(dataname == "cell_rise"){
                if(c_index == -1){
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][0].cell_rise - (lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][1].cell_rise - lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][0].cell_rise) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][0].cell_rise - (lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][1].cell_rise - lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][0].cell_rise) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][trantime_size - 1].cell_rise + (lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][trantime_size - 1].cell_rise - lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][trantime_size - 2].cell_rise) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][trantime_size - 1].cell_rise + (lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][trantime_size - 1].cell_rise - lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][trantime_size - 2].cell_rise) * capload_ratio;
                }else{
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][c_index].cell_rise + (lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][c_index+1].cell_rise - lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][c_index].cell_rise) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][c_index].cell_rise + (lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][c_index+1].cell_rise - lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][c_index].cell_rise) * capload_ratio;
                }
            }
            if(dataname == "cell_fall"){
               if(c_index == -1){
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][0].cell_fall - (lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][1].cell_fall - lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][0].cell_fall) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][0].cell_fall - (lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][1].cell_fall - lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][0].cell_fall) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][trantime_size - 1].cell_fall + (lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][trantime_size - 1].cell_fall - lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][trantime_size - 2].cell_fall) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][trantime_size - 1].cell_fall + (lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][trantime_size - 1].cell_fall - lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][trantime_size - 2].cell_fall) * capload_ratio;
                }else{                
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][c_index].cell_fall + (lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][c_index+1].cell_fall - lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][c_index].cell_fall) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][c_index].cell_fall + (lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][c_index+1].cell_fall - lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][c_index].cell_fall) * capload_ratio;
                }
            }
            if(dataname == "rise_transition"){
                if(c_index == -1){
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][0].rise_transition - (lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][1].rise_transition - lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][0].rise_transition) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][0].rise_transition - (lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][1].rise_transition - lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][0].rise_transition) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][trantime_size - 1].rise_transition + (lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][trantime_size - 1].rise_transition - lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][trantime_size - 2].rise_transition) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][trantime_size - 1].rise_transition + (lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][trantime_size - 1].rise_transition - lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][trantime_size - 2].rise_transition) * capload_ratio;
                }else{
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][c_index].rise_transition + (lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][c_index+1].rise_transition - lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][c_index].rise_transition) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][c_index].rise_transition + (lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][c_index+1].rise_transition - lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][c_index].rise_transition) * capload_ratio;
                }
            }
            if(dataname == "fall_transition"){
                if(c_index == -1){
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][0].fall_transition - (lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][1].fall_transition - lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][0].fall_transition) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][0].fall_transition - (lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][1].fall_transition - lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][0].fall_transition) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][trantime_size - 1].fall_transition + (lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][trantime_size - 1].fall_transition - lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][trantime_size - 2].fall_transition) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][trantime_size - 1].fall_transition + (lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][trantime_size - 1].fall_transition - lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][trantime_size - 2].fall_transition) * capload_ratio;
                }else{
                    capload_high = lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][c_index].fall_transition + (lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][c_index+1].fall_transition - lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 2][c_index].fall_transition) * capload_ratio;
                    capload_low = lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][c_index].fall_transition + (lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][c_index+1].fall_transition - lu_table.NOR2X1.output_pin.tablevalue[trantime_size - 1][c_index].fall_transition) * capload_ratio;
                }
            }
        }

        if(gatename == "INVX1"){
            if(dataname == "cell_rise"){
                if(c_index == -1){
                    capload_high = lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][0].cell_rise - (lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][1].cell_rise - lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][0].cell_rise) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][0].cell_rise - (lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][1].cell_rise - lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][0].cell_rise) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][trantime_size - 1].cell_rise + (lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][trantime_size - 1].cell_rise - lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][trantime_size - 2].cell_rise) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][trantime_size - 1].cell_rise + (lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][trantime_size - 1].cell_rise - lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][trantime_size - 2].cell_rise) * capload_ratio;
                }else{
                    capload_high = lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][c_index].cell_rise + (lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][c_index+1].cell_rise - lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][c_index].cell_rise) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][c_index].cell_rise + (lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][c_index+1].cell_rise - lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][c_index].cell_rise) * capload_ratio;
                }
            }
            if(dataname == "cell_fall"){
               if(c_index == -1){
                    capload_high = lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][0].cell_fall - (lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][1].cell_fall - lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][0].cell_fall) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][0].cell_fall - (lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][1].cell_fall - lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][0].cell_fall) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][trantime_size - 1].cell_fall + (lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][trantime_size - 1].cell_fall - lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][trantime_size - 2].cell_fall) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][trantime_size - 1].cell_fall + (lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][trantime_size - 1].cell_fall - lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][trantime_size - 2].cell_fall) * capload_ratio;
                }else{                
                    capload_high = lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][c_index].cell_fall + (lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][c_index+1].cell_fall - lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][c_index].cell_fall) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][c_index].cell_fall + (lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][c_index+1].cell_fall - lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][c_index].cell_fall) * capload_ratio;
                }
            }
            if(dataname == "rise_transition"){
                if(c_index == -1){
                    capload_high = lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][0].rise_transition - (lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][1].rise_transition - lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][0].rise_transition) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][0].rise_transition - (lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][1].rise_transition - lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][0].rise_transition) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][trantime_size - 1].rise_transition + (lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][trantime_size - 1].rise_transition - lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][trantime_size - 2].rise_transition) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][trantime_size - 1].rise_transition + (lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][trantime_size - 1].rise_transition - lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][trantime_size - 2].rise_transition) * capload_ratio;
                }else{
                    capload_high = lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][c_index].rise_transition + (lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][c_index+1].rise_transition - lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][c_index].rise_transition) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][c_index].rise_transition + (lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][c_index+1].rise_transition - lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][c_index].rise_transition) * capload_ratio;
                }
            }
            if(dataname == "fall_transition"){
                if(c_index == -1){
                    capload_high = lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][0].fall_transition - (lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][1].fall_transition - lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][0].fall_transition) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][0].fall_transition - (lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][1].fall_transition - lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][0].fall_transition) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][trantime_size - 1].fall_transition + (lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][trantime_size - 1].fall_transition - lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][trantime_size - 2].fall_transition) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][trantime_size - 1].fall_transition + (lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][trantime_size - 1].fall_transition - lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][trantime_size - 2].fall_transition) * capload_ratio;
                }else{
                    capload_high = lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][c_index].fall_transition + (lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][c_index+1].fall_transition - lu_table.INVX1.output_pin.tablevalue[trantime_size - 2][c_index].fall_transition) * capload_ratio;
                    capload_low = lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][c_index].fall_transition + (lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][c_index+1].fall_transition - lu_table.INVX1.output_pin.tablevalue[trantime_size - 1][c_index].fall_transition) * capload_ratio;
                }
            }
        }
        

        if(gatename == "NANDX1"){
            if(dataname == "cell_rise"){
                if(c_index == -1){
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][0].cell_rise - (lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][1].cell_rise - lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][0].cell_rise) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][0].cell_rise - (lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][1].cell_rise - lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][0].cell_rise) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][trantime_size - 1].cell_rise + (lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][trantime_size - 1].cell_rise - lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][trantime_size - 2].cell_rise) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][trantime_size - 1].cell_rise + (lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][trantime_size - 1].cell_rise - lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][trantime_size - 2].cell_rise) * capload_ratio;
                }else{
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][c_index].cell_rise + (lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][c_index+1].cell_rise - lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][c_index].cell_rise) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][c_index].cell_rise + (lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][c_index+1].cell_rise - lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][c_index].cell_rise) * capload_ratio;
                }
            }
            if(dataname == "cell_fall"){
                
               if(c_index == -1){
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][0].cell_fall - (lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][1].cell_fall - lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][0].cell_fall) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][0].cell_fall - (lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][1].cell_fall - lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][0].cell_fall) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][trantime_size - 1].cell_fall + (lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][trantime_size - 1].cell_fall - lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][trantime_size - 2].cell_fall) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][trantime_size - 1].cell_fall + (lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][trantime_size - 1].cell_fall - lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][trantime_size - 2].cell_fall) * capload_ratio;
                }else{                
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][c_index].cell_fall + (lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][c_index+1].cell_fall - lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][c_index].cell_fall) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][c_index].cell_fall + (lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][c_index+1].cell_fall - lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][c_index].cell_fall) * capload_ratio;
                }
            }
            if(dataname == "rise_transition"){
                if(c_index == -1){
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][0].rise_transition - (lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][1].rise_transition - lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][0].rise_transition) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][0].rise_transition - (lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][1].rise_transition - lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][0].rise_transition) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][trantime_size - 1].rise_transition + (lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][trantime_size - 1].rise_transition - lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][trantime_size - 2].rise_transition) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][trantime_size - 1].rise_transition + (lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][trantime_size - 1].rise_transition - lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][trantime_size - 2].rise_transition) * capload_ratio;
                }else{
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][c_index].rise_transition + (lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][c_index+1].rise_transition - lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][c_index].rise_transition) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][c_index].rise_transition + (lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][c_index+1].rise_transition - lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][c_index].rise_transition) * capload_ratio;
                }
            }
            if(dataname == "fall_transition"){
                if(c_index == -1){
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][0].fall_transition - (lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][1].fall_transition - lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][0].fall_transition) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][0].fall_transition - (lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][1].fall_transition - lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][0].fall_transition) * capload_ratio;
                }
                else if(c_index == INT16_MAX){
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][trantime_size - 1].fall_transition + (lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][trantime_size - 1].fall_transition - lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][trantime_size - 2].fall_transition) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][trantime_size - 1].fall_transition + (lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][trantime_size - 1].fall_transition - lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][trantime_size - 2].fall_transition) * capload_ratio;
                }else{
                    capload_high = lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][c_index].fall_transition + (lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][c_index+1].fall_transition - lu_table.NANDX1.output_pin.tablevalue[trantime_size - 2][c_index].fall_transition) * capload_ratio;
                    capload_low = lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][c_index].fall_transition + (lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][c_index+1].fall_transition - lu_table.NANDX1.output_pin.tablevalue[trantime_size - 1][c_index].fall_transition) * capload_ratio;
                }
            }
        }
        
        return capload_low + (capload_low - capload_high) * trantime_ratio;
    }
    
}

void Timing_Analyzer::settiming(ofstream& FILEOUT_delay, ofstream& FILEOUT_path){

    int inputcase_size = inputcase.size();
    for(int k = 0;  k < inputcase_size; k++){

        vector<Logic> logic_array;
        int input_size = inputcase[k].size();
        for(int i = 0; i < input_size; i ++){
            Logic temp_logic;
            temp_logic.name = inputcase[k][i].name;
            temp_logic.value = inputcase[k][i].value;
            logic_array.push_back(temp_logic);
        }
        int wire_size = wire.size();
        for(int i = 0; i < wire_size; i ++){
            Logic temp_logic;
            temp_logic.name = wire[i];
            logic_array.push_back(temp_logic);

        }
        int output_size = output.size();
        for(int i = 0; i < output_size; i ++){
            Logic temp_logic;
            temp_logic.name = output[i];
            logic_array.push_back(temp_logic);

        }

        int array_size =  logic_array.size();
        /*for(int i = 0; i < array_size; i++){
            cout << logic_array[i].name << " " << logic_array[i].value << endl;
        }*/

        while(1){

            bool over = false;
            int gate_size = gate.size();
            vector<Logic>::iterator logic_itr;


            for(int i = 0; i < gate_size; i++){
                
                //cout << gate[i].name << " " << gate[i].gate_type << " " ;

                int gate_input_size = gate[i].gate_input.size();
                vector<int> invalue;

                for(int j = 0; j < gate_input_size; j++){
                    
                    for(logic_itr = logic_array.begin(); logic_itr != logic_array.end(); logic_itr ++){

                        if(gate[i].gate_input[j].name == (*logic_itr).name){

                            invalue.push_back((*logic_itr).value);
                            
                            
                            for(int l = 0; l < input_size; l++){

                                if(inputcase[k][l].name ==  gate[i].gate_input[j].name){
                                    double temp_input_tran = 0;
                                    double temp_input_delayorder = 0;

                                    gate[i].input_tran.push_back(temp_input_tran);
                                    gate[i].input_delayorder.push_back(temp_input_delayorder);
                                    
                                    string temp_wire = inputcase[k][l].name;
                                    vector<string> temp_intput_path;
                                    temp_intput_path.push_back(temp_wire);
                                    gate[i].intput_path.push_back(temp_intput_path);


                                    
                                }
                            }
                            for(int l = 0; l < gate_size; l++){
                                if(gate[l].gate_output ==  gate[i].gate_input[j].name){
                                    double temp_input_tran = gate[l].transtime;
                                    double temp_input_delayorder = gate[l].delayorder;

                                    gate[i].input_tran.push_back(temp_input_tran);
                                    gate[i].input_delayorder.push_back(temp_input_delayorder);
                                    
                                    vector<string> temp_intput_path;
                                    int input_path_size = gate[l].pathorder.size();                   
                                    for(int h = 0; h < input_path_size; h++){
                                        string temp_wire = gate[l].pathorder[h];
                                        temp_intput_path.push_back(temp_wire);
                                    }                   
                                    gate[i].intput_path.push_back(temp_intput_path);
                                    
                                    
                                }                        
                            }
                            
                        }

                    }
                }


                    if(find(invalue.begin(), invalue.end(), 2) == invalue.end()){
                        if(gate[i].gate_type == "NOR2X1"){
                            gate[i].logic = !(invalue[0] | invalue[1]);

                            for(logic_itr = logic_array.begin(); logic_itr != logic_array.end(); logic_itr ++){
                                if(gate[i].gate_output == (*logic_itr).name){
                                    (*logic_itr).value = gate[i].logic;
                                }
                            }

                            if(gate[i].logic == 1){ // last 0 is determine delay

                                if(gate[i].input_delayorder[0] > gate[i].input_delayorder[1]){ //[0] come last
                                    gate[i].transtime = interpolation(gate[i].input_tran[0], gate[i].outload, "NOR2X1", "rise_transition");
                                    gate[i].delay = interpolation(gate[i].input_tran[0], gate[i].outload, "NOR2X1", "cell_rise");
                                    if(gate[i].delayorder == 0){
                                         gate[i].delayorder = gate[i].input_delayorder[0] + gate[i].delay;
                                    }
                                    if(gate[i].pathorder.size() == 0){
                                        
                                        int intput_path_size = gate[i].intput_path[0].size();
                                        
                                        for(int s = 0; s < intput_path_size; s++){
                                            gate[i].pathorder.push_back(gate[i].intput_path[0][s]);
                                        }
                                        gate[i].pathorder.push_back(gate[i].gate_output);

                                    }
                                }else{
                                    gate[i].transtime = interpolation(gate[i].input_tran[1], gate[i].outload, "NOR2X1", "rise_transition");
                                    gate[i].delay = interpolation(gate[i].input_tran[1], gate[i].outload, "NOR2X1", "cell_rise");
                                    if(gate[i].delayorder == 0){
                                         gate[i].delayorder = gate[i].input_delayorder[1] + gate[i].delay;
                                    }
                                    if(gate[i].pathorder.size() == 0){
                                                                         
                                        int intput_path_size = gate[i].intput_path[1].size();
                                        for(int s = 0; s < intput_path_size; s++){
                                            gate[i].pathorder.push_back(gate[i].intput_path[1][s]);
                                        }
                                        gate[i].pathorder.push_back(gate[i].gate_output);

                                    }
                                }
                            }else{ // first 1 is determine delay

                                if(invalue[0] && invalue[1]){

                                    if(gate[i].input_delayorder[0] < gate[i].input_delayorder[1]){ //[0] come first

                                        gate[i].transtime = interpolation(gate[i].input_tran[0], gate[i].outload, "NOR2X1", "fall_transition");
                                        gate[i].delay = interpolation(gate[i].input_tran[0], gate[i].outload, "NOR2X1", "cell_fall");
                                        if(gate[i].delayorder == 0){
                                             gate[i].delayorder = gate[i].input_delayorder[0] + gate[i].delay;
                                        }
                                        if(gate[i].pathorder.size() == 0){

                                            int intput_path_size = gate[i].intput_path[0].size();
                                            for(int s = 0; s < intput_path_size; s++){
                                                gate[i].pathorder.push_back(gate[i].intput_path[0][s]);
                                            }
                                            gate[i].pathorder.push_back(gate[i].gate_output);

                                        }                                        
                                    }else{

                                        gate[i].transtime = interpolation(gate[i].input_tran[1], gate[i].outload, "NOR2X1", "fall_transition");
                                        gate[i].delay = interpolation(gate[i].input_tran[1], gate[i].outload, "NOR2X1", "cell_fall");
                                        if(gate[i].delayorder == 0){
                                             gate[i].delayorder = gate[i].input_delayorder[1] + gate[i].delay;
                                        }
                                        if(gate[i].pathorder.size() == 0){
                                        
                                            int intput_path_size = gate[i].intput_path[1].size();
                                            for(int s = 0; s < intput_path_size; s++){
                                                gate[i].pathorder.push_back(gate[i].intput_path[1][s]);
                                            }
                                            gate[i].pathorder.push_back(gate[i].gate_output);

                                        }
                                    }
                                }
                                else if(invalue[0]){
                                    gate[i].transtime = interpolation(gate[i].input_tran[0], gate[i].outload, "NOR2X1", "fall_transition");
                                    gate[i].delay = interpolation(gate[i].input_tran[0], gate[i].outload, "NOR2X1", "cell_fall");
                                    if(gate[i].delayorder == 0){
                                    gate[i].delayorder = gate[i].input_delayorder[0] + gate[i].delay;
                                    }
                                    if(gate[i].pathorder.size() == 0){
                                        
                                        int intput_path_size = gate[i].intput_path[0].size();
                                        for(int s = 0; s < intput_path_size; s++){
                                            gate[i].pathorder.push_back(gate[i].intput_path[0][s]);
                                        }
                                        gate[i].pathorder.push_back(gate[i].gate_output);

                                    }
                                }
                                else if(invalue[1]){
                                        gate[i].transtime = interpolation(gate[i].input_tran[1], gate[i].outload, "NOR2X1", "fall_transition");
                                        gate[i].delay = interpolation(gate[i].input_tran[1], gate[i].outload, "NOR2X1", "cell_fall");
                                    if(gate[i].delayorder == 0){
                                        gate[i].delayorder = gate[i].input_delayorder[1] + gate[i].delay;
                                    }
                                    if(gate[i].pathorder.size() == 0){
                                        
                                        int intput_path_size = gate[i].intput_path[1].size();
                                        for(int s = 0; s < intput_path_size; s++){
                                            gate[i].pathorder.push_back(gate[i].intput_path[1][s]);
                                        }
                                        gate[i].pathorder.push_back(gate[i].gate_output);

                                    }

                                }
                            }

                        }

                        if(gate[i].gate_type == "INVX1"){
                            gate[i].logic = !invalue[0];

                            for(logic_itr = logic_array.begin(); logic_itr != logic_array.end(); logic_itr ++){
                                if(gate[i].gate_output == (*logic_itr).name){
                                    (*logic_itr).value = gate[i].logic;
                                }
                            }

                            if(gate[i].logic == 1){
                                
                                gate[i].transtime = interpolation(gate[i].input_tran[0], gate[i].outload, "INVX1", "rise_transition");
                                gate[i].delay = interpolation(gate[i].input_tran[0], gate[i].outload, "INVX1", "cell_rise");
                                if(gate[i].delayorder == 0){
                                    
                                    gate[i].delayorder = gate[i].input_delayorder[0] + gate[i].delay;
                                }
                                if(gate[i].pathorder.size() == 0){
                                        
                                    int intput_path_size = gate[i].intput_path[0].size();
                                    for(int s = 0; s < intput_path_size; s++){
                                        gate[i].pathorder.push_back(gate[i].intput_path[0][s]);
                                    }
                                    gate[i].pathorder.push_back(gate[i].gate_output);

                                }
                            }else{
                                gate[i].transtime = interpolation(gate[i].input_tran[0], gate[i].outload, "INVX1", "fall_transition");
                                gate[i].delay = interpolation(gate[i].input_tran[0], gate[i].outload, "INVX1", "cell_fall");
                                if(gate[i].delayorder == 0){
                                    gate[i].delayorder = gate[i].input_delayorder[0] + gate[i].delay;
                                } 
                                if(gate[i].pathorder.size() == 0){
                                        
                                    int intput_path_size = gate[i].intput_path[0].size();
                                    for(int s = 0; s < intput_path_size; s++){
                                        gate[i].pathorder.push_back(gate[i].intput_path[0][s]);
                                    }
                                    gate[i].pathorder.push_back(gate[i].gate_output);

                                }
                            }         

                        }

                        if(gate[i].gate_type == "NANDX1"){

                            gate[i].logic = !(invalue[0] & invalue[1]);

                            for(logic_itr = logic_array.begin(); logic_itr != logic_array.end(); logic_itr ++){
                                if(gate[i].gate_output == (*logic_itr).name){
                                    (*logic_itr).value = gate[i].logic;
                                    
                                }
                            } 

                            if(gate[i].logic == 0){ // last 1 is determine delay

                                if(gate[i].input_delayorder[0] > gate[i].input_delayorder[1]){ //[0] come last
                                    gate[i].transtime = interpolation(gate[i].input_tran[0], gate[i].outload, "NANDX1", "fall_transition");
                                    gate[i].delay = interpolation(gate[i].input_tran[0], gate[i].outload, "NANDX1", "cell_fall");
                                    if(gate[i].delayorder == 0){
                                        gate[i].delayorder = gate[i].input_delayorder[0] + gate[i].delay;
                                    } 
                                    if(gate[i].pathorder.size() == 0){

                                        int intput_path_size = gate[i].intput_path[0].size();
                                        for(int s = 0; s < intput_path_size; s++){
                                            gate[i].pathorder.push_back(gate[i].intput_path[0][s]);
                                        }
                                        gate[i].pathorder.push_back(gate[i].gate_output);

                                    }
                                }else{

                                    gate[i].transtime = interpolation(gate[i].input_tran[1], gate[i].outload, "NANDX1", "fall_transition");
                                    gate[i].delay = interpolation(gate[i].input_tran[1], gate[i].outload, "NANDX1", "cell_fall");
                                    if(gate[i].delayorder == 0){
                                        gate[i].delayorder = gate[i].input_delayorder[1] + gate[i].delay;
                                    } 
                                    if(gate[i].pathorder.size() == 0){

                                        int intput_path_size = gate[i].intput_path[1].size();
                                        for(int s = 0; s < intput_path_size; s++){
                                            gate[i].pathorder.push_back(gate[i].intput_path[1][s]);
                                        }
                                        gate[i].pathorder.push_back(gate[i].gate_output);

                                    }
                                }
                            }else{ // first 0 is determine delay

                                if(invalue[0] == 0 && invalue[1] == 0){
                                    
                                    if(gate[i].input_delayorder[0] < gate[i].input_delayorder[1]){ //[0] come first
                                        
                                        gate[i].transtime = interpolation(gate[i].input_tran[0], gate[i].outload, "NANDX1", "rise_transition");
                                        gate[i].delay = interpolation(gate[i].input_tran[0], gate[i].outload, "NANDX1", "cell_rise");
                                        if(gate[i].delayorder == 0){
                                            gate[i].delayorder = gate[i].input_delayorder[0] + gate[i].delay;
                                        }  
                                        if(gate[i].pathorder.size() == 0){

                                            int intput_path_size = gate[i].intput_path[0].size();
                                            for(int s = 0; s < intput_path_size; s++){
                                                gate[i].pathorder.push_back(gate[i].intput_path[0][s]);
                                            }
                                            gate[i].pathorder.push_back(gate[i].gate_output);

                                        }                                  
                                    }else{
                                        
                                        gate[i].transtime = interpolation(gate[i].input_tran[1], gate[i].outload, "NANDX1", "rise_transition");
                                        gate[i].delay = interpolation(gate[i].input_tran[1], gate[i].outload, "NANDX1", "cell_rise");
                                        if(gate[i].delayorder == 0){
                                            gate[i].delayorder = gate[i].input_delayorder[1] + gate[i].delay;
                                        }
                                        if(gate[i].pathorder.size() == 0){

                                            int intput_path_size = gate[i].intput_path[1].size();
                                            for(int s = 0; s < intput_path_size; s++){
                                                gate[i].pathorder.push_back(gate[i].intput_path[1][s]);
                                            }
                                            gate[i].pathorder.push_back(gate[i].gate_output);

                                        }
                                    }
                                }
                                else if(invalue[0] == 0){

                                    gate[i].transtime = interpolation(gate[i].input_tran[0], gate[i].outload, "NANDX1", "rise_transition");
                                    
                                    gate[i].delay = interpolation(gate[i].input_tran[0], gate[i].outload, "NANDX1", "cell_rise");
                                    if(gate[i].delayorder == 0){
                                        gate[i].delayorder = gate[i].input_delayorder[0] + gate[i].delay;
                                    }
                                    if(gate[i].pathorder.size() == 0){
                                        
                                        int intput_path_size = gate[i].intput_path[0].size();
                                        for(int s = 0; s < intput_path_size; s++){
                                            gate[i].pathorder.push_back(gate[i].intput_path[0][s]);
                                        }
                                        gate[i].pathorder.push_back(gate[i].gate_output);

                                    }
                                }
                                else if(invalue[1] == 0){

                                    gate[i].transtime = interpolation(gate[i].input_tran[1], gate[i].outload, "NANDX1", "rise_transition");
                                    gate[i].delay = interpolation(gate[i].input_tran[1], gate[i].outload, "NANDX1", "cell_rise");
                                    if(gate[i].delayorder == 0){
                                        gate[i].delayorder = gate[i].input_delayorder[1] + gate[i].delay;
                                    }
                                    if(gate[i].pathorder.size() == 0){

                                        int intput_path_size = gate[i].intput_path[1].size();
                                        for(int s = 0; s < intput_path_size; s++){
                                            gate[i].pathorder.push_back(gate[i].intput_path[1][s]);
                                        }
                                        gate[i].pathorder.push_back(gate[i].gate_output);

                                    }
                                    
                                }
                            }                   


                        }
                    }

                //} 

         
                /*for(int j = 0; j < invalue_size; j++){
                    cout << invalue[j] << " " ;
                }

                cout << " logic: "<< gate[i].logic << " " << "delay: " << gate[i].delay << " " << "transtion: " << gate[i].transtime << " outputload: " << gate[i].outload  << " delayorder:" << gate[i].delayorder;

                int input_trans_size = gate[i].input_tran.size();
                
                for(int j = 0; j < input_trans_size; j++){
                    cout << endl;
                    cout << "input transition : "<< gate[i].input_tran[j] << '\t' ;
                    cout << "input delayorder : "<< gate[i].input_delayorder[j] << "\t" ;
                    cout << "input pathorder  : ";
                    
                    int intput_path_size = gate[i].intput_path[j].size();
                    
                    for(int l = 0; l < intput_path_size; l++){
                        
                        cout << gate[i].intput_path[j][l] << "->" ;
                    }
                    
                    
                }
                cout << endl;

                int pathorder_size = gate[i].pathorder.size();

                for(int j = 0; j < pathorder_size; j++){
                    cout << gate[i].pathorder[j] << "->" ;
                }

                cout << endl;*/

                
                gate[i].input_tran.clear();
                gate[i].input_delayorder.clear();
                gate[i].intput_path.clear();

            }


            for(int i = 0; i < array_size; i++){
                
                if(logic_array[i].value == 2){
                    over = false;
                    break;
                } 
                over = true;
            }
            if(over == true){
                break;
            }

            //cout << "============================" << endl;
            
        }

        sort(gate.begin(), gate.end(), delay_compare);
        int gate_size = gate.size(); 

        Gate longest_gate;
        longest_gate.delayorder = 0;
        Gate shortest_gate;
        shortest_gate.delayorder = INT16_MAX;
        int output_start = input_size + wire_size;



        for(int i = 0; i < gate_size; i ++){
            FILEOUT_delay << gate[i].name << " " << gate[i].logic << " " << gate[i].delay << " " << gate[i].transtime << endl;
            
            for(int j = output_start; j < array_size; j++){

                if(logic_array[j].name == gate[i].gate_output){

                    if(gate[i].delayorder > longest_gate.delayorder){
                        longest_gate = gate[i];
                    }
                    if(gate[i].delayorder < shortest_gate.delayorder){
                        shortest_gate = gate[i];
                    }
                }

            }
            
            gate[i].delay = -1;
            gate[i].transtime = -1;
            gate[i].delayorder = 0;
            gate[i].logic = 2;
            gate[i].pathorder.clear();
        }

        FILEOUT_path << "Longest delay = " << longest_gate.delayorder << ", the path is: " ;
        int longest_path_size = longest_gate.pathorder.size();
        for(int j = 0; j < longest_path_size; j++){
            if(j == longest_path_size - 1 ){
                FILEOUT_path << longest_gate.pathorder[j] << endl ;
            }else{
                FILEOUT_path << longest_gate.pathorder[j] << " -> " ;
            }
            
        }
        FILEOUT_path << "Shortest delay = " << shortest_gate.delayorder << ", the path is: " ;
        int shortest_gate_size = shortest_gate.pathorder.size();
        for(int j = 0; j < shortest_gate_size; j++){
            if(j == shortest_gate_size - 1 ){
                FILEOUT_path << shortest_gate.pathorder[j] << endl ;
            }else{
                FILEOUT_path << shortest_gate.pathorder[j] << " -> " ;
            }
            
        }
        FILEOUT_delay << endl;
        FILEOUT_path << endl;

    }


}

void Timing_Analyzer::show_NOR2X1_table(){
    int size = lu_table.NOR2X1.output_pin.tablevalue.size();
    for(int i = 0; i<size; i++){
        int inter_size = lu_table.NOR2X1.output_pin.tablevalue[i].size();
        for(int j = 0; j < inter_size; j++){
        cout << "測試結果["<< i <<"][" << j << "] : " << endl;
        cout << "input_time  : "<<lu_table.NOR2X1.output_pin.tablevalue[i][j].input_time << endl;
        cout << "output_load : "<<lu_table.NOR2X1.output_pin.tablevalue[i][j].output_load << endl;
        cout << "rise_power  : "<<lu_table.NOR2X1.output_pin.tablevalue[i][j].rise_power << endl; 
        cout << "fall_power  : "<<lu_table.NOR2X1.output_pin.tablevalue[i][j].fall_power << endl;
        cout << "cell_rise  : "<<lu_table.NOR2X1.output_pin.tablevalue[i][j].cell_rise << endl;  
        cout << "cell_fall  : "<<lu_table.NOR2X1.output_pin.tablevalue[i][j].cell_fall << endl;   
        cout << "rise_transition  : "<<lu_table.NOR2X1.output_pin.tablevalue[i][j].rise_transition << endl; 
         cout << "fall_transition  : "<<lu_table.NOR2X1.output_pin.tablevalue[i][j].fall_transition << endl;  
        } 
    }
}

void Timing_Analyzer::show_INVX1_table(){
    int size = lu_table.INVX1.output_pin.tablevalue.size();
    for(int i = 0; i<size; i++){
        int inter_size = lu_table.INVX1.output_pin.tablevalue[i].size();
        for(int j = 0; j < inter_size; j++){    
        cout << "測試結果["<< i <<"][" << j << "] : " << endl;
        cout << "input_time  : "<<lu_table.INVX1.output_pin.tablevalue[i][j].input_time << endl;
        cout << "output_load : "<<lu_table.INVX1.output_pin.tablevalue[i][j].output_load << endl;
        cout << "rise_power  : "<<lu_table.INVX1.output_pin.tablevalue[i][j].rise_power << endl; 
        cout << "fall_power  : "<<lu_table.INVX1.output_pin.tablevalue[i][j].fall_power << endl;
        cout << "cell_rise  : "<<lu_table.INVX1.output_pin.tablevalue[i][j].cell_rise << endl;  
        cout << "cell_fall  : "<<lu_table.INVX1.output_pin.tablevalue[i][j].cell_fall << endl;   
        cout << "rise_transition  : "<<lu_table.INVX1.output_pin.tablevalue[i][j].rise_transition << endl; 
         cout << "fall_transition  : "<<lu_table.INVX1.output_pin.tablevalue[i][j].fall_transition << endl;   
        }
    }
}

void Timing_Analyzer::show_NANDX1_table(){
    int size = lu_table.NANDX1.output_pin.tablevalue.size();
    for(int i = 0; i<size; i++){
        int inter_size = lu_table.NANDX1.output_pin.tablevalue[i].size();
        for(int j = 0; j < inter_size; j++){    
        cout << "測試結果["<< i <<"][" << j << "] : " << endl;
        cout << "input_time  : "<<lu_table.NANDX1.output_pin.tablevalue[i][j].input_time << endl;
        cout << "output_load : "<<lu_table.NANDX1.output_pin.tablevalue[i][j].output_load << endl;
        cout << "rise_power  : "<<lu_table.NANDX1.output_pin.tablevalue[i][j].rise_power << endl; 
        cout << "fall_power  : "<<lu_table.NANDX1.output_pin.tablevalue[i][j].fall_power << endl;
        cout << "cell_rise  : "<<lu_table.NANDX1.output_pin.tablevalue[i][j].cell_rise << endl;  
        cout << "cell_fall  : "<<lu_table.NANDX1.output_pin.tablevalue[i][j].cell_fall << endl;   
        cout << "rise_transition  : "<<lu_table.NANDX1.output_pin.tablevalue[i][j].rise_transition << endl; 
         cout << "fall_transition  : "<<lu_table.NANDX1.output_pin.tablevalue[i][j].fall_transition << endl; 
        }
    }
}

void Timing_Analyzer::show_input(){
    int size = input.size();
    for(int i = 0; i<size; i++){
        cout << input[i] << endl; 
    }
}

void Timing_Analyzer::show_output(){
    int size = output.size();
    for(int i = 0; i<size; i++){
        cout << output[i] << endl; 
    }
}

void Timing_Analyzer::show_wire(){
    int size = wire.size();
    for(int i = 0; i<size; i++){
        cout << wire[i] << endl; 
    }
}

void Timing_Analyzer::show_gate_port(){
    int size = gate.size();
    for(int i = 0; i<size; i++){
        cout << gate[i].name << " : (" << gate[i].gate_type << ")" << endl;
        
        cout << "inport  > " ;
        int size_input = gate[i].gate_input.size();
        for(int j = 0; j < size_input; j++){
            cout << gate[i].gate_input[j].input_type << " : " <<  gate[i].gate_input[j].name << " " ;
        }
        cout << endl;
        cout << "outport > " ;

        cout << gate[i].gate_output ;

        cout << endl;
        cout << "===================="<<endl;
    }
}

void Timing_Analyzer::show_outload(){

    int gate_size = gate.size();
    stable_sort(gate.begin(), gate.end(), outload_compare);
    for(int i = 0; i < gate_size; i++){
    cout  << gate[i].name << " " << gate[i].outload << endl;
    }

}

void Timing_Analyzer::show_inputcase(){
    int inputcase_size = inputcase.size();
    for(int i = 0; i < inputcase_size; i++){
        int temp_inputcase_size = inputcase[i].size();
        for(int j = 0; j < temp_inputcase_size; j++){
            cout << inputcase[i][j].name << " " <<  inputcase[i][j].value << endl;
        }
        cout << endl;
    }   
}