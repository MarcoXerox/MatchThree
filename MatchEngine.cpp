#include "MatchEngine.h"
using namespace std;

uniform_int_distribution<int> dist(0, 4);

char ToChar(Tile x) {
    switch (x) {
        case Tile::CROSS : return 'X';
        case Tile::CIRCLE: return 'O';
        case Tile::SHARP : return '#';
        case Tile::EXCLAM: return '!';
        case Tile::NIL   : return '%';
        default: return 0x00;
    }
}

Board::Board(int w, int h, unsigned int seed) {
    this->gen = new default_random_engine(seed);
    this->ts.resize(w);
    for (int i=0; i<w; ++i) for (int j=0; j<h; ++j) {
        ts[i].push_back(_generate(i, j));
    }
}

inline Tile Board::_random_tile() {
    return static_cast<Tile>(dist(*this->gen));
}

Tile Board::_generate(int i, int j) {
    auto t = _random_tile();
    auto x = i ? ts[i - 1][j] : Tile::LIM;
    auto y = j ? ts[i][j - 1] : Tile::LIM;
    while (t == x or t == y) t = _random_tile();
    return t;
}

vector<int> kills(const SPair<vector<Points>>& s) {
    vector<int> ans;
    for (auto& ps : s.first) for (auto& qs : s.second) {
        Points intersect;
        set_intersection(ps.begin(), ps.end(),
                         qs.begin(), qs.end(),
                         back_inserter(intersect));
        if (intersect.empty()) {
            ans.push_back(ps.size());
            ans.push_back(qs.size());
        } else ans.push_back(ps.size() + qs.size() - 1);
    }
    return ans;
}

Points concat(const SPair<vector<Points>>& s) {
    Points ans;
    for (auto& ps : s.first)  for (Point p : ps) ans.push_back(p);
    for (auto& ps : s.second) for (Point p : ps) ans.push_back(p);
    return ans;
}

SPair<vector<Points>> Board::_matches() {
    const int w = width(), h = height();
    SPair<vector<Points>> s; // [VERTI, HORIZ]
    s.first.emplace_back();
    s.second.emplace_back();
    _collect(s.first, w, h, false);
    _collect(s.second, h, w, true);
    return s;
}

void Board::_collect(vector<Points>& v, int x, int y, bool rev) {
    for (int i=0; i<x; ++i) {
        int cnt = 1;
        auto LAST = Tile::LIM;
        for (int j=0; j<=y; ++j) {
            Tile t = j == y ? Tile::LIM : (rev ? ts[j][i] : ts[i][j]);
            if (LAST == t) ++cnt;
            else {
                if (cnt >= 3) {
                    v.emplace_back();
                    for (int k = cnt; k > 0; --k) {
                        Point p;
                        p.first  = rev ? j - k : i;
                        p.second = rev ? i : j - k;
                        v.back().push_back(p); 
                    }
                }
                cnt = 1;
            }
            LAST = t;
        }
    }
}

void Board::_destroyRegen(const Points& byes) {
    const int w = width(), h = height();
    vector<vector<int>> its;
    its.resize(w);

    for (Point p : byes) its[p.first].push_back(p.second);
    for (auto& is : its) sort(is.begin(), is.end(), greater<int>());
    for (int i=0; i<w; ++i) {
        auto& is = its[i];
        is.erase(unique(is.begin(), is.end()), is.end());
        for (int x : is) ts[i].erase(ts[i].begin() + x);
        ts[i].resize(h, Tile::LIM);
    }
    for (int i=0; i<w; ++i) for (int j=0; j<h; ++j) {
        if (ts[i][j] == Tile::LIM) ts[i][j] = _generate(i, j);
    }
}

int Board::scoreIt(int px, int py, int qx, int qy) {
    Point p(px, py), q(qx, qy);
    swap_tiles(p, q);
    int ans = 0;
    auto s = _matches();
    auto v = concat(s);
    if (v.empty()) {
        swap_tiles(p, q);
        return 0;
    } else {
        while (!v.empty()) {
            _destroyRegen(v);
            for (int x : kills(s)) ans += score[x];
            s = _matches();
            v = concat(s);
        }
        return ans;
    }
}

void Board::swap_tiles(Point p, Point q) {
    int px, py, qx, qy;
    tie(px, py) = p;
    tie(qx, qy) = q;
    swap(ts[px][py], ts[qx][qy]);
}

string Board::str() {
    stringstream ss;
    int w = this->width(), h = this->height();
    for (int i=1; i<=h; ++i) {
        for (int j=0; j<w; ++j) ss << ToChar(this->ts[j].end()[-i]);
        ss << endl;
    }
    return ss.str();
}

Tile Board::at(int x, int y) {
    return ts.at(x).at(y);
}
