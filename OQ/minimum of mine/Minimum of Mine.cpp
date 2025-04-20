#include <vector>
#include <string>
#include <iostream>
#include <queue>
using namespace std;

int m, n;
vector<string> grid, best_grid;
int best_count = INT_MAX;

// 检查以 (i,j) 为右下角的 2×2 子矩形合法性：
// 1) 不是全白
// 2) 不存在对角黑格（* . / . *）模式
bool ok2x2(int i, int j) {
    if (i <= 0 || j <= 0) return true;
    char a = grid[i-1][j-1], b = grid[i-1][j];
    char c = grid[i][j-1], d = grid[i][j];
    // 全白剪枝
    if (a == '.' && b == '.' && c == '.' && d == '.') return false;
    // 对角黑格剪枝：
    // a,d 为黑，而 b,c 为白
    if (a == '*' && d == '*' && b == '.' && c == '.') return false;
    // b,c 为黑，而 a,d 为白
    if (b == '*' && c == '*' && a == '.' && d == '.') return false;
    return true;
}

// 检查所有白格是否连通
bool white_connected() {
    vector<vector<bool>> vis(m, vector<bool>(n, false));
    queue<pair<int,int>> q;
    int total_white = 0;
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            if (grid[i][j] == '.') total_white++;
    if (total_white == 0) return true;
    // BFS 从第一个白格开始
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (grid[i][j] == '.') {
                q.emplace(i, j);
                vis[i][j] = true;
                goto bfs_white;
            }
        }
    }
bfs_white:
    int cnt = 0;
    int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    while (!q.empty()) {
        auto [x,y] = q.front(); q.pop();
        cnt++;
        for (auto &d : dirs) {
            int nx = x + d[0], ny = y + d[1];
            if (nx>=0 && nx<m && ny>=0 && ny<n && !vis[nx][ny] && grid[nx][ny]=='.') {
                vis[nx][ny] = true;
                q.emplace(nx, ny);
            }
        }
    }
    return cnt == total_white;
}

// 检查所有黑格是否连通到边缘
bool black_to_border() {
    vector<vector<bool>> vis(m, vector<bool>(n, false));
    queue<pair<int,int>> q;
    int total_black = 0;
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            if (grid[i][j] == '*') {
                total_black++;
                if (i==0 || j==0 || i==m-1 || j==n-1) {
                    vis[i][j] = true;
                    q.emplace(i, j);
                }
            }
    if (total_black == 0) return true;
    int reached = 0;
    int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    while (!q.empty()) {
        auto [x,y] = q.front(); q.pop();
        reached++;
        for (auto &d : dirs) {
            int nx = x + d[0], ny = y + d[1];
            if (nx>=0 && nx<m && ny>=0 && ny<n && !vis[nx][ny] && grid[nx][ny]=='*') {
                vis[nx][ny] = true;
                q.emplace(nx, ny);
            }
        }
    }
    return reached == total_black;
}

void dfs(int pos, int black_count) {
    if (black_count >= best_count) return; // 黑格数剪枝
    if (pos == m*n) {
        if (white_connected() && black_to_border()) {
            best_count = black_count;
            best_grid = grid;
        }
        return;
    }
    int i = pos / n;
    int j = pos % n;
    // 尝试涂白
    grid[i][j] = '.';
    if (ok2x2(i, j)) {
        dfs(pos + 1, black_count);
    }
    // 尝试涂黑
    grid[i][j] = '*';
    // 黑格无须 2x2 检查（不会新增全白或对角）
    dfs(pos + 1, black_count + 1);
    // 恢复
    grid[i][j] = '.';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> m >> n;
    grid.assign(m, string(n, '.'));
    dfs(0, 0);
    // 输出答案
    cout << best_count << '\n';
    for (auto &row : best_grid) cout << row << '\n';
    return 0;
}