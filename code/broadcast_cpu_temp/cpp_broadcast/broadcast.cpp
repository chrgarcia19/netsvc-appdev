#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std; 

double convert_from_celsius(float temp){
    double temp_f = temp * (9.0/5.0) + 32;
    return temp_f;
}

int main(){
    //solution to get CPU temp
    //https://forums.raspberrypi.com/viewtopic.php?t=170112
    float systemp, millideg;
    FILE *thermal;
    int n;

    thermal = fopen("/sys/class/thermal/thermal_zone0/temp","r");
    n = fscanf(thermal,"%f",&millideg);
    fclose(thermal);
    systemp = millideg / 1000;

    cout << "CPU temperature: " << convert_from_celsius(systemp) << "° F" << endl;    
}