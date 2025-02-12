
void calc_systematics() {
    double sum_m1 = 0, sum_g1 = 0, sum_m2 = 0, sum_g2 = 0;

    // the array should have the systematic uncertainty due to
    // that source. The order should be M1, Gamma1, M2, Mamma2,
    // and phase difference (only applies to the interference model)
    map<string, array<double, 4>> systematics_nInterference = {
        {"event_selection: ", {6, 0.0, 1, 0.0}},
        {"model: ", {4, 13, 2, 8}},
        {"acc. corr.: ", {1, 12, 1, 4}},
        {"binning: ", {1, 1, 1, 2}}
    };

    map<string, array<double, 5>> systematics_wInterference = {
        {"event_selection: ", {0.0, 0.0, 0.0, 4, 0.24}},
        {"model: ", {14, 19, 9, 2, 0.49}},
        {"acc. corr.: ", {7, 8, 4, 0.0, 0.21}},
        {"binning: ", {4, 5, 3, 4, 0.11}}
    };

    sum_m1 = 0;
    sum_g1 = 0;
    sum_m2 = 0;
    sum_g2 = 0;

    for(const auto& systematic : systematics_nInterference) {
        sum_m1 += systematic.second[0]*systematic.second[0];
        sum_g1 += systematic.second[1]*systematic.second[1];
        sum_m2 += systematic.second[2]*systematic.second[2];
        sum_g2 += systematic.second[3]*systematic.second[3];
    }

    cout << "No interference model systematics:" << endl;
    cout << std::setprecision(3);
    cout << "m1 systematic: " << sqrt(sum_m1) << endl;
    cout << "g1 systematic: " << sqrt(sum_g1) << endl;
    cout << "m2 systematic: " << sqrt(sum_m2) << endl;
    cout << "g2 systematic: " << sqrt(sum_g2) << endl;
    cout << endl;

    //////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////

    sum_m1 = 0;
    sum_g1 = 0;
    sum_m2 = 0;
    sum_g2 = 0;

    for(const auto& systematic : systematics_wInterference) {
        sum_m1 += systematic.second[0]*systematic.second[0];
        sum_g1 += systematic.second[1]*systematic.second[1];
        sum_m2 += systematic.second[2]*systematic.second[2];
        sum_g2 += systematic.second[3]*systematic.second[3];
    }

    cout << "With interference model systematics:" << endl;
    cout << "m1 systematic: " << sqrt(sum_m1) << endl;
    cout << "g1 systematic: " << sqrt(sum_g1) << endl;
    cout << "m2 systematic: " << sqrt(sum_m2) << endl;
    cout << "g2 systematic: " << sqrt(sum_g2) << endl;
    cout << endl;
}