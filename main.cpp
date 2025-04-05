/* CSCI 3515 Lab 10 STA/LTA Code
    Does not process signals in real time */

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

using namespace plt;
using namespace std;

void helper(const vector<double>& values) {
    const int STAWindow = 5;     // short term window
    const int LTAWindow = 50;    // long term window
    const double triggerThreshold = 1.0;
    const double detriggerThreshold = 1.0;

    const int n = values.size();

    // fill in these vectors for window averages
    vector<double> sta(n);
    vector<double> lta(n);
    vector<double> ratio(n);

    double LTASum = 0.0;
    double STASum = 0.0;

    for (int i=0; i<n; i++) {
        double x = abs(values[i]);
        LTASum += x;
        STASum += x;
        if (i >= STAWindow-1) {
            sta[i] = STASum/STAWindow;
            STASum -= abs(values[i-STAWindow+1]);
        }
        if (i >= LTAWindow-1) {
            lta[i] = LTASum/LTAWindow;
            LTASum -= abs(values[i-LTAWindow+1]);
            if (lta[i] > 0) {
                ratio[i] = sta[i]/lta[i];
            }
        }
    }
    bool detected = false;
    vector<int> starts;
    vector<int> ends;

    for (int i=0; i<n; i++) {
        cout << ratio[i] << endl;
        if (!detected && ratio[i] > triggerThreshold) {
            detected = true;
            starts.push_back(i);

        } else if (detected && ratio[i] < detriggerThreshold) {
            detected = false;
            ends.push_back(i);
        }
    }
    figure_size(1200, 600);
    plot(values, {{"label", "Signal"}});
    plot(ratio, {{"label", "STA/LTA Ratio"}, {"color", "orange"}});

    for (int i=0; i < starts.size(); i++) {
        axvline(starts[i], 0.0, 1.0, {{"color", "green"}, {"linestyle", "--"}});
    }

    for (int i=0; i < ends.size(); i++) {
        axvline(ends[i], 0.0, 1.0, {{"color", "red"}, {"linestyle", "--"}});
    }

    legend();
    title("STA/LTA Event Detection");
    xlabel("Sample Index");
    ylabel("Amplitude / Ratio");

    // show();  Run this if not working inside a remote terminal (needs GUI). Otherwise, save graph as png file to repository
    save("sta_lta_plot.png");
}

vector<double> readAccZFromCSV(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return {};
    }
    vector<double> signal;
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string value;
        int col = 0;
        double accZ = 0.0;

        while (getline(ss, value, ',')) {
            col++;
            if (col == 3) {  // AccZ is the 3rd column
                accZ = stod(value);  // Convert string to double
                signal.push_back(accZ);
                break; // No need to read the rest of the line
            }
        }
    }
    return signal;
}

int main() {
    vector<double> signal = readAccZFromCSV("LAB10.CSV");
    helper(signal);
}