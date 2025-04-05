// CSCI 3515 Lab 10 STA/LTA Code

#include <iostream>
#include <vector>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

using namespace plt;
using namespace std;

void helper(const vector<double>& values) {
    const int STAWindow = 5;     // short term window
    const int LTAWindow = 20;    // long term window
    const double triggerThreshold = 3.5;
    const double detriggerThreshold = 1.0;

    const int n = values.size();

    // fill in these vectors for window averages
    vector<double> sta(n);
    vector<double> lta(n);
    vector<double> ratio(n);

    double LTASum = 0;
    double STASum = 0;

    for (int i=0; i<n; i++) {
        const unsigned int x = abs(values[i]);
        LTASum += x;
        STASum += x;
        if (i >= STAWindow-1) {
            sta[i] = STASum/STAWindow;
            STASum -= values[i-STAWindow];
        }
        if (i >= LTAWindow-1) {
            lta[i] = LTASum/LTAWindow;
            LTASum -= values[i-LTAWindow];
            if (lta[i] > 0) {
                ratio[i] = sta[i]/lta[i];
            }
        }
    }
    bool detected = false;
    vector<int> starts;
    vector<int> ends;

    for (int i=0; i<n; i++) {
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

    // show();  Run this if not working inside a remote terminal (needs GUI). Otherwise, save graph as png file
    save("sta_lta_plot.png");
}

int main() {
    vector<double> signal = {
        // background noise (small random values)
        0.2, -0.1, 0.05, 0.1, -0.2, 0.15, -0.05, 0.1, 0.2, -0.1,
        0.0, -0.1, 0.05, 0.1, -0.05, 0.2, 0.1, -0.15, 0.0, 0.1,
        
        // sudden event (spike)
        5.0, 6.2, 5.8, 6.5, 5.9, 6.1, 6.3, 5.7, 6.0, 6.1,
        
        // back to background noise
        0.1, -0.1, 0.05, 0.0, -0.05, 0.1, 0.15, -0.1, 0.05, 0.0,
        0.2, -0.2, 0.1, -0.1, 0.05, 0.1, -0.05, 0.2, 0.0, -0.1
    };
    helper(signal);
}