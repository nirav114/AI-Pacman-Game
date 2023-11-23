#include "bits/stdc++.h"
using namespace std;

#define all(v) (v).begin(), (v).end()
#define sz(v) (int)v.size()
#define pii pair<int, int>

const int inf = 1e9;
const int win = 10000;
const int lose = -10000;
const int eat = 1000;

int dx[4] = {0, 0, 1, -1};
int dy[4] = {1, -1, 0, 0};

class Board {
      public :
      int n, m; 
      pii pacmanVal, coinVal;
      vector<pii> ghostVal;
      vector<string> g;
      vector<vector<int>> dis;
      Board() {
            g = {
                  "###################",
                  "#CCCC....#..CC...C#",
                  "#.##C###.#.###.##C#",
                  "#.##C###.#.###.##C#",
                  "#...C............C#",
                  "#.##C#.#####.#.##.#",
                  "#...C#...#.C.#....#",
                  "####C###.#.###.####",
                  "####C#.......#C####",
                  "####C#.#####.#.####",
                  "#C..CCC#####......#",
                  "###################"
            };
            n = sz(g);
            m = sz(g[0]);
            pacmanVal = {4, 9};
            ghostVal = 
                  {
                        make_pair(1, 10), 
                        make_pair(10, 14)
                  };
            dis.assign(n*m, vector<int>(n*m, inf));
            floydWarshell();
      }

      int compress(int x, int y) {
            return x*n + y;
      }

      void floydWarshell() {
            for(int xi = 0; xi < n; xi++) for(int yi = 0; yi < m; yi++) {
                  for(int d = 0; d < 4; d++) {
                        int xj = xi + dx[d];
                        int yj = yi + dy[d];
                        if(!isGood(xj, yj)) continue;
                        int i = compress(xi, yi);
                        int j = compress(xj, yj);
                        if(g[xi][yi] != '#') {
                              dis[i][j] = 1;
                        }
                        dis[i][i] = dis[j][j] = 0;
                  }
            }

            for(int xk = 0; xk < n; xk++) for(int yk = 0; yk < m; yk++) {
                  for(int xi = 0; xi < n; xi++) for(int yi = 0; yi < m; yi++) {
                        for(int xj = 0; xj < n; xj++) for(int yj = 0; yj < m; yj++) {
                              if(g[xk][yk] != '#' && g[xi][yi] != '#' && g[xj][yj] != '#') {
                                    int k = compress(xk, yk);
                                    int i = compress(xi, yi);
                                    int j = compress(xj, yj);

                                    if(dis[i][k] < inf && dis[k][j] < inf) {
                                          dis[i][j] = min(dis[i][j], dis[i][k] + dis[k][j]);
                                    }
                              }
                        }
                  }
            }
      }

      bool isGhostHere(int x, int y) {
            pii cur = {x, y};
            for(auto p : ghostVal) {
                  if(p == cur) return true;
            }
            return false;
      }

      bool isGhostHere(pii p) {
            return isGhostHere(p.first, p.second);
      }

      void print() {
            string t = "---------------------------------------";
            cout << t << '\n';
            for(int i = 0; i < n; i++) {
                  for(int j = 0; j < m; j++) {
                        pii cur = {i, j};
                        if(cur == pacmanVal) cout << 'P';
                        else if(isGhostHere(cur)) cout << 'G';
                        else cout << g[i][j];
                  }
                  cout << '\n';
            }
            cout << t << '\n';
      }

      bool isEnded() {
            bool hasCoinsLeft = false;
            for(int i = 0; i < n; i++) {
                  for(int j = 0; j < m; j++) {
                        if(g[i][j] == 'C') hasCoinsLeft = true;
                  }
            }
            return !hasCoinsLeft;
      }

      bool isGood(int x, int y) {
            return x >= 0 && x < n && y >= 0 && y < m && g[x][y] != '#';
      }

      bool isPacmanAlive() {
            return !isGhostHere(pacmanVal);
      }
};


int calcBestOption(Board b, bool turn, int depth, int maxDepth) {
      if(depth == maxDepth) return 0;
      if(turn) {
            int x = b.pacmanVal.first, y = b.pacmanVal.second;
            int extra = 0;
            if(!b.isPacmanAlive()) {
                  extra += lose;
                  return extra;
            }
            else {
                  if(b.g[x][y] == 'C') {
                        b.g[x][y] == '.';
                        extra += eat;
                  }
                  int bestRes = sz(b.ghostVal)*lose - 1;
                  pii newPos = b.pacmanVal;
                  for(int d = 0; d < 4; d++) {
                        int nx = x + dx[d];
                        int ny = y + dy[d];
                        if(!b.isGood(nx, ny)) continue;
                        b.pacmanVal = {nx, ny};
                        int result = calcBestOption(b, turn ^ true, depth + 1, maxDepth);
                        if(result + extra > bestRes) {
                              bestRes = result + extra;
                              newPos = {nx, ny};
                        }
                  }  
                  return bestRes;
            }
      }
      else {
            int totalRes = 0;
            for(auto &ghost : b.ghostVal) {
                  int x = ghost.first, y = ghost.second;
                  int extra = 0;
                  if(!b.isPacmanAlive()) {
                        extra += lose;
                        return extra;
                  }
                  else {
                        int bestRes = win + 1;
                        pii newPos = ghost;
                        int pacmanId = b.compress(b.pacmanVal.first, b.pacmanVal.second);
                        int ghostId = b.compress(ghost.first, ghost.second);
                        extra += b.dis[pacmanId][ghostId];
                        for(int d = 0; d < 4; d++) {
                              int nx = x + dx[d];
                              int ny = y + dy[d];
                              if(!b.isGood(nx, ny)) continue;
                              ghost = {nx, ny};
                              int result = calcBestOption(b, turn ^ true, depth + 1, maxDepth);
                              if(result - (b.n*b.m - extra) < bestRes) {
                                    bestRes = result - (b.n*b.m - extra);
                                    newPos = {nx, ny};
                              }
                        }
                        ghost = newPos;
                        totalRes += bestRes;
                  } 
            }  
            return totalRes;  
      }
}


void runMinMax(int maxDepth) {
      Board b;
      bool turn = true;
      int steps = 100, score = 0;
      while(!b.isEnded() && b.isPacmanAlive() && steps) {
            steps--;
            int extra = 0;
            if(turn == true) {
                  int x = b.pacmanVal.first, y = b.pacmanVal.second;
                  int bestRes = sz(b.ghostVal)*lose - 1;
                  pii newPos = b.pacmanVal;
                  for(int d = 0; d < 4; d++) {
                        int nx = x + dx[d];
                        int ny = y + dy[d];
                        if(!b.isGood(nx, ny)) continue;
                        b.pacmanVal = {nx, ny};
                        int result = calcBestOption(b, turn ^ true, 0, maxDepth);
                        if(result > bestRes) {
                              bestRes = result;
                              newPos = {nx, ny};
                        }
                  }
                  if(b.g[newPos.first][newPos.second] == 'C') {
                        b.g[newPos.first][newPos.second] = '.';
                        score++;
                  }
                  b.pacmanVal = newPos;
            }
            else {
                  for(auto &ghost : b.ghostVal) {
                        int x = ghost.first, y = ghost.second;
                        int bestRes = win + 1;
                        pii newPos = ghost;
                        int pacmanId = b.compress(b.pacmanVal.first, b.pacmanVal.second);
                        int ghostId = b.compress(ghost.first, ghost.second);
                        extra += b.dis[pacmanId][ghostId];
                        for(int d = 0; d < 4; d++) {
                              int nx = x + dx[d];
                              int ny = y + dy[d];
                              if(!b.isGood(nx, ny)) continue;
                              ghost = {nx, ny};
                              int result = calcBestOption(b, turn ^ true, 0, maxDepth);
                              if(result - (b.n*b.m - extra) < bestRes) {
                                    bestRes = result - (b.n*b.m - extra);
                                    newPos = {nx, ny};
                              }
                        }
                        ghost = newPos;
                  }
            }
            b.print();
            cout << "last move was of : " << (turn ? "pacman" : "ghosts") << '\n';
            cout << "pacman : " << b.pacmanVal.first << ' ' << b.pacmanVal.second << '\n';
            // cout << "ghost : " << b.ghostVal.first << ' ' << b.ghostVal.second << '\n';
            cout << "score : " << score << '\n';
            turn ^= true;
      }
}

signed main() {
      int maxDepth = 8;
      runMinMax(maxDepth);
}