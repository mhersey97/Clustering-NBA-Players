//Cluster Validation by Prediction Strength
#include <iostream>
#include <fstream>
#include <string>
#include <cfloat>
#include <cstring>
#include <vector>
#include <math.h>
#include <sstream>
#include <time.h>
#include "season.cpp"
using namespace std;

int K = 5; // clusters to be used - 5 by default
int D; // dimensionality of the data
bool V = false; // denotes the verbose output that lists the members of clusters
vector<season> DATA;
vector<season*> MEANS;
vector<vector<season>> NEAREST;
vector<vector<season>> BEST;
vector<pair<float, float>> RANGE;

// parse the CSV input and determine dimension count
void parseInput(istream& input, int flag) {
    string read;
    if (flag == 0) { // large dataset 1952-2017
        D = 9;

        for (int i = 0; i < D; i++) {
            RANGE.push_back(pair<float,float>(FLT_MAX, -FLT_MAX));
        }

        // ignore fields
        getline(input,read);

        while (!input.eof()) {
            getline(input, read);

            if (read.empty()) {
                break;
            }

            vector<string> player;
            string word;
            stringstream s(read);

            // record csv fields
            while (getline(s, word, ',')) {
                player.push_back(word);
            }

            // create season if valid
            if (!player[1].empty() && !player[8].empty() && stoi(player[1]) >= 1952 && stoi(player[8]) >= 400) {
                // calculate multiplier for per 36 stats
                float multi = 36 / (stof(player[8]) / stoi(player[6]));
                season temp = season(player[2], player[3]);
                temp.year = stoi(player[1]);
                temp.addValue(stof(player[10])); // ts
                if (RANGE[0].first > temp.latestValue()) {
                    RANGE[0].first = temp.latestValue();
                }
                else if (RANGE[0].second < stof(player[10])) {
                    RANGE[0].second = temp.latestValue();
                }

                temp.addValue((stof(player[32]) / stoi(player[6])) * multi); // fga
                if (RANGE[1].first > temp.latestValue()) {
                    RANGE[1].first = temp.latestValue();
                }
                else if (RANGE[1].second < temp.latestValue()) {
                    RANGE[1].second = temp.latestValue();
                }

                temp.addValue(stof(player[33])); // fg%
                if (RANGE[2].first > temp.latestValue()) {
                    RANGE[2].first = temp.latestValue();
                }
                else if (RANGE[2].second < temp.latestValue()) {
                    RANGE[2].second = temp.latestValue();
                }

                if (player[35].empty()) {
                    temp.addValue(0.0);
                }
                else {
                    temp.addValue((stof(player[35]) / stoi(player[6])) * multi); // 3pa
                }
                if (RANGE[3].first > temp.latestValue()) {
                    RANGE[3].first = temp.latestValue();
                }
                else if (RANGE[3].second < temp.latestValue()) {
                    RANGE[3].second = temp.latestValue();
                }

                if (player[36].empty()) {
                    temp.addValue(0.0);
                }
                else {
                    temp.addValue(stof(player[36])); // 3p%
                }
                if (RANGE[4].first > temp.latestValue()) {
                    RANGE[4].first = temp.latestValue();
                }
                else if (RANGE[4].second < temp.latestValue()) {
                    RANGE[4].second = temp.latestValue();
                }

                temp.addValue((stof(player[42]) / stoi(player[6])) * multi); // fta
                if (RANGE[5].first > temp.latestValue()) {
                    RANGE[5].first = temp.latestValue();
                }
                else if (RANGE[5].second < temp.latestValue()) {
                    RANGE[5].second = temp.latestValue();
                }

                temp.addValue(stof(player[43])); // ft%
                if (RANGE[6].first > temp.latestValue()) {
                    RANGE[6].first = temp.latestValue();
                }
                else if (RANGE[6].second < temp.latestValue()) {
                    RANGE[6].second = temp.latestValue();
                }

                temp.addValue((stof(player[47]) / stoi(player[6])) * multi); // ast
                if (RANGE[7].first > temp.latestValue()) {
                    RANGE[7].first = temp.latestValue();
                }
                else if (RANGE[7].second < temp.latestValue()) {
                    RANGE[7].second = temp.latestValue();
                }

                temp.addValue((stof(player[52]) / stoi(player[6])) * multi); // pts
                if (RANGE[8].first > temp.latestValue()) {
                    RANGE[8].first = temp.latestValue();
                }
                else if (RANGE[8].second < temp.latestValue()) {
                    RANGE[8].second = temp.latestValue();
                }

                DATA.push_back(temp);
            }
        }
    }

    if (flag == 1) { // medium dataset 1978-2017
        D = 16;

        for (int i = 0; i < D; i++) {
            RANGE.push_back(pair<float, float>(FLT_MAX, -FLT_MAX));
        }

        // ignore fields
        getline(input, read);

        while (!input.eof()) {
            getline(input, read);

            if (read.empty()) {
                break;
            }

            vector<string> player;
            string word;
            stringstream s(read);

            // record csv fields
            while (getline(s, word, ',')) {
                player.push_back(word);
            }

            // create season if valid
            if (!player[1].empty() && !player[8].empty() && stoi(player[1]) >= 1978 && stoi(player[8]) >= 400) {
                // calculate multiplier for per 36 stats
                float multi = 36 / (stof(player[8]) / stoi(player[6]));

                season temp = season(player[2], player[3]);
                temp.year = stoi(player[1]);
                temp.addValue(stof(player[10])); // ts
                if (RANGE[0].first > temp.latestValue()) {
                    RANGE[0].first = temp.latestValue();
                }
                else if (RANGE[0].second < stof(player[10])) {
                    RANGE[0].second = temp.latestValue();
                }

                temp.addValue((stof(player[32]) / stoi(player[6])) * multi); // fga
                if (RANGE[1].first > temp.latestValue()) {
                    RANGE[1].first = temp.latestValue();
                }
                else if (RANGE[1].second < temp.latestValue()) {
                    RANGE[1].second = temp.latestValue();
                }

                temp.addValue(stof(player[33])); // fg%
                if (RANGE[2].first > temp.latestValue()) {
                    RANGE[2].first = temp.latestValue();
                }
                else if (RANGE[2].second < temp.latestValue()) {
                    RANGE[2].second = temp.latestValue();
                }

                if (player[35].empty()) {
                    temp.addValue(0.0);
                }
                else {
                    temp.addValue((stof(player[35]) / stoi(player[6])) * multi); // 3pa
                }
                if (RANGE[3].first > temp.latestValue()) {
                    RANGE[3].first = temp.latestValue();
                }
                else if (RANGE[3].second < temp.latestValue()) {
                    RANGE[3].second = temp.latestValue();
                }

                if (player[36].empty()) {
                    temp.addValue(0.0);
                }
                else {
                    temp.addValue(stof(player[36])); // 3p%
                }
                if (RANGE[4].first > temp.latestValue()) {
                    RANGE[4].first = temp.latestValue();
                }
                else if (RANGE[4].second < temp.latestValue()) {
                    RANGE[4].second = temp.latestValue();
                }

                temp.addValue((stof(player[38]) / stoi(player[6])) * multi); // 2pa
                if (RANGE[5].first > temp.latestValue()) {
                    RANGE[5].first = temp.latestValue();
                }
                else if (RANGE[5].second < temp.latestValue()) {
                    RANGE[5].second = temp.latestValue();
                }

                temp.addValue(stof(player[39])); // 2p%
                if (RANGE[6].first > temp.latestValue()) {
                    RANGE[6].first = temp.latestValue();
                }
                else if (RANGE[6].second < temp.latestValue()) {
                    RANGE[6].second = temp.latestValue();
                }

                temp.addValue((stof(player[42]) / stoi(player[6])) * multi); // fta
                if (RANGE[7].first > temp.latestValue()) {
                    RANGE[7].first = temp.latestValue();
                }
                else if (RANGE[7].second < temp.latestValue()) {
                    RANGE[7].second = temp.latestValue();
                }

                temp.addValue(stof(player[43])); // ft%
                if (RANGE[8].first > temp.latestValue()) {
                    RANGE[8].first = temp.latestValue();
                }
                else if (RANGE[8].second < temp.latestValue()) {
                    RANGE[8].second = temp.latestValue();
                }

                temp.addValue((stof(player[46]) / stoi(player[6])) * multi); // reb
                if (RANGE[9].first > temp.latestValue()) {
                    RANGE[9].first = temp.latestValue();
                }
                else if (RANGE[9].second < temp.latestValue()) {
                    RANGE[9].second = temp.latestValue();
                }

                temp.addValue((stof(player[47]) / stoi(player[6])) * multi); // ast
                if (RANGE[10].first > temp.latestValue()) {
                    RANGE[10].first = temp.latestValue();
                }
                else if (RANGE[10].second < temp.latestValue()) {
                    RANGE[10].second = temp.latestValue();
                }

                temp.addValue((stof(player[48]) / stoi(player[6])) * multi); // stl
                if (RANGE[11].first > temp.latestValue()) {
                    RANGE[11].first = temp.latestValue();
                }
                else if (RANGE[11].second < temp.latestValue()) {
                    RANGE[11].second = temp.latestValue();
                }

                temp.addValue((stof(player[49]) / stoi(player[6])) * multi); // blk
                if (RANGE[12].first > temp.latestValue()) {
                    RANGE[12].first = temp.latestValue();
                }
                else if (RANGE[12].second < temp.latestValue()) {
                    RANGE[12].second = temp.latestValue();
                }

                temp.addValue((stof(player[50]) / stoi(player[6])) * multi); // tov
                if (RANGE[13].first > temp.latestValue()) {
                    RANGE[13].first = temp.latestValue();
                }
                else if (RANGE[13].second < temp.latestValue()) {
                    RANGE[13].second = temp.latestValue();
                }

                temp.addValue((stof(player[52]) / stoi(player[6])) * multi); // pts
                if (RANGE[14].first > temp.latestValue()) {
                    RANGE[14].first = temp.latestValue();
                }
                else if (RANGE[14].second < temp.latestValue()) {
                    RANGE[14].second = temp.latestValue();
                }

                temp.addValue(stof(player[20])); // usg
                if (RANGE[15].first > temp.latestValue()) {
                    RANGE[15].first = temp.latestValue();
                }
                else if (RANGE[15].second < temp.latestValue()) {
                    RANGE[15].second = temp.latestValue();
                }

                DATA.push_back(temp);
            }
        }
    }
}

// recalculate all means from the seasons attributed to it
// output true if change is very small
bool recalculateMean() {
    bool out = true;
    for (int index = 0; index < K; index++) {
        // average of the summation
        for (int i = 0; i < D; i++) {
            float avg = 0;
            for (int j = 0; j < NEAREST[index].size(); j++) {
                avg += NEAREST[index][j].dimensions[i];
            }
            avg = avg / NEAREST[index].size();

            // assign the new value to the cluster model
            if (abs(MEANS[index]->dimensions[i] - avg) > FLT_EPSILON) {
                MEANS[index]->dimensions[i] = avg;
                out = false;
            }
        }
    }

    return out;
}

// calculates smallest euclidean distance between the sample and the means
int nearestMean(int index) {
    float dist = FLT_MAX;
    int closest = 0;
    // sqrt of summation of distances squared
    for (int i = 0; i < K; i++) {
        float sum = 0;
        for (int j = 0; j < D; j++) {
            sum += pow(DATA[index].dimensions[j] - MEANS[i]->dimensions[j], 2);
        }
        sum = sqrt(sum);

        // if closer mean found record it and try to beat new distance
        if (dist > sum) {
            dist = sum;
            closest = i;
        }
    }

    return closest;
}

// calculate the sum of squares for the current clustering
float sumOfSquares() {
    float sum = 0;

    for (int i = 0; i < K; i++) {
        for (int j = 0; j < NEAREST[i].size(); j++) {
            float temp = 0;
            for (int k = 0; k < D; k++) {
                temp += pow(NEAREST[i][j].dimensions[k] - MEANS[i]->dimensions[k], 2);
            }
            sum += sqrt(temp);
        }
    }

    return sum;
}

// determine the silhouette value of the clustering
float silhouette() {
    vector<float> distance;
    float silhouette = 0;
    for (int i = 0; i < K; i++) {
        // iterate through all pairs in this cluster
        for (int j = 0; j < NEAREST[i].size(); j++) {
            float a = 0;
            for (int k = j + 1; k < NEAREST[i].size(); k++) {
                // calculate euclidean distance between pairs
                float sum = 0;
                for (int l = 0; l < D; l++) {
                    sum += pow(NEAREST[i][j].dimensions[l] - NEAREST[i][k].dimensions[l], 2);
                }
                sum = sqrt(sum);
                a += sum;
            }

            // average distance
            if (NEAREST[i].size() == 1) {
                continue;
            }
            a = a / (NEAREST[i].size() - 1);

            // find average distance to all points in 'neighboring cluster'
            float b = FLT_MAX;
            for (int k = 0; k < K; k++) {
                if (k == i) { // ignore same cluster
                    continue;
                }

                float dist = 0;
                for (int x = 0; x < NEAREST[k].size(); x++) {
                    // calculate euclidean distance between pairs
                    float sum = 0;
                    for (int l = 0; l < D; l++) {
                        sum += pow(NEAREST[i][j].dimensions[l] - NEAREST[k][x].dimensions[l], 2);
                    }
                    sum = sqrt(sum);
                    dist += sum;
                }
                dist = dist / NEAREST[k].size();
                if (dist < b) {
                    b = dist;
                }
            }
            
            //cout << a << " - " << b << endl;

            // calculate silhouette for this point
            if (a > b) {
                silhouette += (b / a) - 1;
            }
            else if (a < b) {
                silhouette += 1 - (a / b);
            }
            else {
                continue;
            }
        }
    }

    return silhouette / DATA.size();
}

// perform kmeans clustering from the specified k
void kmeans() {
    cout << "Data parsed, beginning kmeans." << endl;

    int count;
    float error = FLT_MAX;
    vector<int> duplicates;
    // run kmeans 5 times and save best clustering (smallest SSE)
    MEANS.clear();
    duplicates.clear();
    srand(time(NULL));
    // randomly select k samples from the data as initial means
    for (int i = 0; i < K; i++) {
        int r = rand() % DATA.size();

        // look for duplicates in the random selection
        for (int j = 0; j < duplicates.size(); j++) {
            if (duplicates[j] == r) {
                i--;
                continue;
            }
        }
        duplicates.push_back(r);

        // create empty season and copy values
        season* temp = new season("");
        temp->dimensions.resize(D);
        for (int j = 0; j < D; j++) {
            temp->dimensions[j] = DATA[r].dimensions[j];
        }
        MEANS.push_back(temp);
    }

    int currCount = 0;
    // continue clustering until convergence
    do {
        NEAREST.clear();
        NEAREST.resize(K);

        // find nearest mean and add season to that cluster
        for (int i = 0; i < DATA.size(); i++) {
            NEAREST[nearestMean(i)].push_back(DATA[i]);
        }

        currCount++;
        // recalculate mean and go again if possible
    } while (!recalculateMean());

    // compare this clustering to the current best clustering
    float newError = silhouette();
    if (newError < error) {
        error = newError;
        count = currCount;

        // save this cluster for output
        BEST.clear();
        BEST.resize(K);
        for (int i = 0; i < K; i++) {
            BEST[i] = NEAREST[i];
        }
    }

    cout << count << " recalculations until convergence." << endl;
    cout << "Silhouette Value: " << error << endl;

    // evaluate final clusters
    for (int i = 0; i < K; i++) {
        cout << "Cluster " << i << " has " << BEST[i].size() << " members." << endl;

        // count occurances of position
        int PG = 0;
        int SG = 0;
        int SF = 0;
        int PF = 0;
        int C = 0;
        int other = 0;

        for (int j = 0; j < BEST[i].size(); j++) {
            if(V) cout << i << ": " << BEST[i][j].name << " - " << BEST[i][j].pos << " - " << BEST[i][j].year << endl;
            if (BEST[i][j].pos == "PG") {
                PG++;
            }
            else if (BEST[i][j].pos == "SG") {
                SG++;
            }
            else if (BEST[i][j].pos == "SF") {
                SF++;
            }
            else if (BEST[i][j].pos == "PF") {
                PF++;
            }
            else if (BEST[i][j].pos == "C") {
                C++;
            }
            else {
                other++;
            }
        }

        cout << "PG: " << PG << endl;
        cout << "SG: " << SG << endl;
        cout << "SF: " << SF << endl;
        cout << "PF: " << PF << endl;
        cout << "C: " << C << endl;
        cout << "other: " << other << endl;
    }
}

// normalize all values between 0 and 1
void normalizeData() {
    cout << DATA.size() << " seasons loaded." << endl;
    cout << "Normalizing data." << endl;
    for (int i = 0; i < DATA.size(); i++) {
        for (int j = 0; j < D; j++) {
            DATA[i].dimensions[j] = (DATA[i].dimensions[j] - RANGE[j].first) / (RANGE[j].second - RANGE[j].first);
        }
    }

    //for (int i = 0; i < D; i++) {
    //    cout << RANGE[i].first << " - " << RANGE[i].second << endl;
    //}
}

int main(int argc, char *argv[]) {
    if (argc != 2 && argc != 3 && argc != 4) {
        cout << "Invalid arguments, cluster count and dataset required." << endl;
        return 1;
    }
    else if (argc == 2) { // used for determining optimal k value
        if (strcmp(argv[1], "help") == 0) {
            cout << "Arguments are <k-clusters> <large or medium (denotes dimensionality)> <V (optional - outputs every player in a cluster)>" << endl;
            return 0;
        }
        else if (strcmp(argv[1], "large") == 0) {
            cout << "Loading largest data set." << endl;
            parseInput(cin, 0);
        }
        else if (strcmp(argv[1], "medium") == 0) {
            cout << "Loading medium data set." << endl;
            parseInput(cin, 1);
        }

        normalizeData();

        // determining optimal kmeans through \silhouette values
        for (int i = 2; i <= 20; i++) {
            K = i;
            kmeans();
        }
    }
    else { // standard kmeans with predefined value
        K = stoi(argv[1]);
        if (K <= 0) {
            cout << "Cluster count must be greater than 0." << endl;
            return 1;
        }

        if (argc == 4 && strcmp(argv[3], "V") == 0) {
            V = true;
        }

        // denotes the dataset being used
        if (strcmp(argv[2], "large") == 0) {
            cout << "Loading largest data set." << endl;
            parseInput(cin, 0);
        }
        else if (strcmp(argv[2], "medium") == 0) {
            cout << "Loading medium data set." << endl;
            parseInput(cin, 1);
        }
        
        normalizeData();
        kmeans();
    }

    return 0;
}
