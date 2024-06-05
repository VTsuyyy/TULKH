#include<bits/stdc++.h>
using namespace std;
int n, m, numTeacher = 0, t = 1;
int subject[1001][4], room[101];
vector<int> constraint[101];
int best = 0, solution[1001], curr = 0;
bool assigned[1001];
clock_t start, stop;

void update(){
    // cout << curr << endl; 
    if(best < curr){
        best = curr;
        for(int i = 1; i <= n; ++i){
            solution[i] = subject[i][3];
        }
// cout << best << endl;
// for(int i = 1; i <= n; i++){
//     if(solution[i]) cout << i << " " << solution[i] % 60 << " " << solution[i] / 60 + 1 << endl;
// }
    }
}

bool check(int sub, int tiet){
    // Check number chair
    if(room[tiet/60+1] < subject[sub][2]) return false;
    
    // Check same day
    if(((tiet + subject[sub][0])/60) != (tiet/60)) return false;

    vector<int> constrain = constraint[subject[sub][1]];
    // Check teacher constraint 
    for(int i : constrain){
        int temp = subject[i][3];
        if(temp == -1 || i == sub) continue;
        if(((tiet+subject[sub][0])%60 - temp%60) * ((temp+subject[i][0])%60 - tiet%60) >= 0) return false;
    }
    return true;
}

void assign(int sub, int tiet){
    stop = clock();
    if(stop - start > 1000 || !check(sub, tiet) || tiet >= n*60){
        update();
        return;
    }
    if(stop - start > 10 * t){
        update();
        ++t;
    }
    subject[sub][3] = tiet;
    ++curr;
    assigned[sub] = true;
    // int num = max(n - 2*curr, n / 3);

    // if(60 - tiet % 60 < 4) tiet = ((int) (tiet / 60) + 1) * 60 - 1;
    if(n*60-tiet >  (best-curr)*4){
        vector<int> constrain = constraint[subject[sub][1]];
        for(int i : constrain){
            if(!assigned[i]){
                assign(i, tiet + subject[sub][0] + 1);
            }
        }
        for(int i = 1; i <= n; i++){
            if(!assigned[i] && subject[sub][1] != subject[i][1]){
                assign(i, tiet + subject[sub][0] + 1);
                // break;
            }
        }
    }
    subject[sub][3] = -1;
    --curr;
    assigned[sub] = false;
    return;
}

int main(){
    // freopen("inp.txt", "r", stdin);
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    start = clock();
    cin >> n >> m;
    for(int i = 1; i <= n; i++){
        cin >> subject[i][0] >> subject[i][1] >> subject[i][2];
        subject[i][3] = -1;
        constraint[subject[i][1]].push_back(i);
        numTeacher = max(numTeacher, subject[i][1]);
    }
    for(int i = 1; i <= m; i++){
        cin >> room[i];
    }
    for(int i = 1; i <= n; i++){
        assign(i, 0);
    }
    cout << best << endl;
    for(int i = 1; i <= n; i++){
        if(solution[i] > -1) cout << i << " " << solution[i]%60+1 << " " << solution[i]/60+1 << endl;
    }
    // cout << ((subject[14][3]+subject[14][0])%60 - subject[1][3]%60) * ((subject[1][3]+subject[1][0])%60 - subject[14][3]%60) << endl;
    
    // vector<int> constrain = constraint[subject[1][1]];
    // for(int i : constrain) cout << i << " ";
}
