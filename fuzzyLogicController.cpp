#include <iostream>
#include<map>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

double triangular_func(double x, double a, double b, double c){
    if(x <= a ||x >= c) return 0.0;
    else if(x == b) return 1.0;
    else if(x < b) return (x-a)/(b-a);
    else return (c-x)/(c-b);
}

double dirt_func(string label, double x){
    if(label == "VSD") return triangular_func(x, 0.0, 0.0, 25.0);
    if(label == "SD") return triangular_func(x, 0.0, 25.0, 50.0);
    if(label == "MD") return triangular_func(x, 25, 50, 75);
    if(label == "HD") return triangular_func(x, 50, 75, 100);
    if(label == "VHD") return triangular_func(x, 75, 100, 100);
    else return 0;
}
double grease_func(string label, double x){
    if(label == "SG") return triangular_func(x, 0.0, 0.0, 25.0);
    if(label == "MG") return triangular_func(x, 0.0, 25.0, 50.0);
    if(label == "HG") return triangular_func(x, 25, 50, 50);
    else return 0;
}

double time_func(string label, double x){
    if(label == "VST") return triangular_func(x, 0.0, 0.0, 15.0);
    if(label == "ST") return triangular_func(x, 0.0, 15.0, 30.0);
    if(label == "MT") return triangular_func(x, 15, 30, 45);
    if(label == "HT") return triangular_func(x, 30, 45, 60);
    if(label == "VHT") return triangular_func(x, 45, 60, 60);
    else return 0;
}
map<string, map<string,string>> rules = {
    {"VSD", {{"SG", "VST"}, {"MG", "VST"}, {"HG", "ST"}}},
    {"SD", {{"SG", "VST"}, {"MG", "ST"}, {"HG", "MT"}}},
    {"MD", {{"SG", "ST"}, {"MG", "MT"}, {"HG", "HT"}}},
    {"HD", {{"SG", "MT"}, {"MG", "HT"}, {"HG", "VHT"}}},
    {"VHD", {{"SG", "HT"}, {"MG", "VHT"}, {"HG", "VHT"}}}
};

double defuzzification_module(map<string, double> &activation){
    double step = 0.1;
    double numerator = 0, denominator = 0;

    for(double x = 0; x <= 60; x += step){
        double max_mu = 0;
        for(auto& it : activation){
            string label = it.first;
            double strength = it.second;
            max_mu = max(max_mu, min(strength, time_func(label,x)));
        }
        numerator += x * max_mu;
        denominator += max_mu;
    }
    return (denominator == 0) ? 0 : numerator/ denominator;
}

int main(){
    double dirt, grease;
    cout<< "Enter Dirt Level (0-100) :";
    cin>>dirt;
    cout<<"Enter Grese Level (0-50) : ";
    cin>>grease;
    cout<<"\n";
    vector<string> dirt_labels = {"VSD", "SD", "MD", "HD", "VHD"};
    vector<string> grease_labels = {"SG", "MG", "HG"};

    map<string, double> dirt_fuzzy, grease_fuzzy;
    for(auto& d : dirt_labels){
        dirt_fuzzy[d] = dirt_func(d, dirt);
        if(dirt_fuzzy[d] > 0)
            cout<<"Dirt Fuzzy Value : "<<dirt_fuzzy[d]<<" for "<<d<<endl;
    } 
    cout<<"\n";
    for(auto& g : grease_labels){
        grease_fuzzy[g] = grease_func(g, grease);
        if(grease_fuzzy[g] > 0)
            cout<<"Grease Fuzzy Value : "<<grease_fuzzy[g]<<" for "<<g<<endl;
    } 
    cout<<"\n";
    map<string, double> output;
    for(auto &d : dirt_labels){
        for(auto& g : grease_labels){
            double fuzzy = min(dirt_fuzzy[d], grease_fuzzy[g]);
            if(fuzzy > 0){
                string op_label = rules[d][g];
                output[op_label] =max(output[op_label], fuzzy);
                cout<<"Output after finding fuzzy values of "<<op_label<<" "<<output[op_label]<<endl;
            }
        }
    }
    double result = defuzzification_module(output);
    cout<<"\nThe Washing Time is: "<< result <<" minutes"<< endl;
    return 0;
}