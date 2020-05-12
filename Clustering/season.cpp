#include <vector>
#include <string>
using namespace std;

class season {
public:
    vector<float> dimensions;
    string name;
    string pos;
    int year;
    int age;

    season(string n) {
        name = n;
    }

    season(string n, string p) {
        name = n;
        pos = p;
    }

    void addValue(float v) {
        dimensions.push_back(v);
    }

    float latestValue() {
        if (dimensions.size() == 0) {
            return -1;
        }
        return dimensions[dimensions.size() - 1];
    }
};
