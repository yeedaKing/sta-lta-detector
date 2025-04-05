// CSCI 3515 Lab 10 STA/LTA Code

#include <iostream>
#include <vector>
#include "matplotlibcpp.h"

using namespace std;

void helper(const vector<int>& values) {
    const int STAWindow = 5;     // short term window
    const int LTAWindow = 20;    // long term window
    const double triggerThreshold = 3.5;
    const double detriggerThreshold = 1.0;

    const int n = values.size();

    // fill in these vectors for window averages
    vector<double> sta(n);
    vector<double> lta(n);
    vector<double> ratio(n);

    unsigned long long LTASum = 0;
    unsigned long long STASum = 0;

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

        } else if (detected && ratio[i] < triggerThreshold) {
            detected = false;
            ends.push_back(i);
        }
    }

}
int main() {
    return 0;
}