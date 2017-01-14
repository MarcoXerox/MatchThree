#include <tuple>
#include <vector>
#include <string>
#include <random>
#include <cassert>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <functional>
#define PPRINT(p) printf("(%d, %d)", p.first, p.second);

using namespace std;

template<class T>
using SPair = pair<T, T>;
using Point = SPair<int>;
using Points = vector<Point>;

enum class Tile : char {
    NIL, CROSS, CIRCLE, SHARP, EXCLAM, LIM
};

const int score[] = {0, 0, 0, 10, 25, 60, 100, 180, 360, 800};

class Board {
    public:
        Board(int, int, unsigned int);
        void swap_tiles(Point, Point);
        string str(void);
        int scoreIt(int, int, int, int);
        int width()  { return ts.size(); };
        int height() { return ts.front().size(); };
        Tile at(int, int);
    private:
        vector<vector<Tile>> ts;
        default_random_engine* gen;
        Tile _random_tile(void);
        Tile _generate(int, int);
        void _collect(vector<Points>&, int, int, bool);
        void _destroyRegen(const Points&);
        SPair<vector<Points>> _matches(void);
};
