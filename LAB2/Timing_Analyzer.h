#ifndef TIMEING_ANALYZER_H
#define TIMEING_ANALYZER_H
using namespace std;

#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <limits>

class Tablevalue{
public:

    double output_load;
    double input_time;
    double rise_power;
    double fall_power;
    double cell_rise;
    double cell_fall;
    double rise_transition;
    double fall_transition;
    Tablevalue();
};

class Output_Pin{
public:
    string pinname;
    double capacitance;
    Output_Pin();
    Output_Pin(string, double);
    vector<vector<Tablevalue>> tablevalue;

};


class Input_Pin{
public:
    string pinname;
    double capacitance;
    Input_Pin(string, double);
};


class cell{
public:
    vector<Input_Pin> input_pin;
    Output_Pin output_pin;
    cell();

};


class Lu_Table{
public:
    string time_unit;
    string voltage_unit;
    string current_unit;
    string capacitive_load_unit;
    cell NOR2X1;   
    cell INVX1;  
    cell NANDX1; 
    vector<double> index_1;
    vector<double> index_2;
    Lu_Table();

};

class Gate_input{
public:
    string name;
    string input_type;

};

class Gate{
public:
    string name;
    string gate_type;
    vector<Gate_input> gate_input;
    string gate_output;
    double outload;
    double delay;
    double delayorder;
    double transtime;
    vector<string> pathorder;
    vector<vector<string>> intput_path;
    vector<double> input_tran;
    vector<double> input_delayorder;
    int logic;
    Gate();

};

class Inputcase{
public:
    string name;
    bool value;
    Inputcase();

};

class Logic{
public:
    string name;
    int value;
    Logic();

};



class Timing_Analyzer{
public:

    string module_name;
    Lu_Table lu_table;
    vector<Gate> gate;
    vector<string> input;
    vector<string> output;
    vector<string> wire;
    vector<vector<Inputcase>> inputcase;
    void parsetestlib(ifstream &);
    void parsemodule(ifstream &);
    void parsepattern(ifstream &);
    void setload(ofstream &);
    void settiming(ofstream &, ofstream &);

    double interpolation(double, double, string,  string);


    void show_NOR2X1_table();
    void show_INVX1_table();
    void show_NANDX1_table();
    void show_input();
    void show_output();
    void show_wire();
    void show_inputcase();
    void show_gate_port();
    void show_outload();
    Timing_Analyzer();
    ~Timing_Analyzer();
};











#endif