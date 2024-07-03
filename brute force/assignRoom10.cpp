#include<bits/stdc++.h>
using namespace std;
int n, m, numTeacher = 0, t = 1;
int subject[1001][4], room[101];
vector<int> constraint[101];
int best = 0, solution[1001], curr = 0;
bool assigned[1001];
clock_t start, stop;

// update continuously 
void update(){
    if(best < curr){
        best = curr;
        for(int i = 1; i <= n; ++i){
            solution[i] = subject[i][3];
        }
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
    // check time stop condition, check valid solution and not over number of rooms
    if(stop - start > 100000 || !check(sub, tiet) || tiet > m*60-4){
        update();
        return;
    }
    // check time stop condition 
    if(stop - start > 100000 * t){
        update();
        ++t;
    }
    // come here is this branch pass the check valid 
    // so assign it
    subject[sub][3] = tiet;
    ++curr;
    assigned[sub] = true;
    
    if(n*60-tiet >  (best-curr)*4){
        vector<int> constrain = constraint[subject[sub][1]];
        // continue assign subject from same teacher
        for(int i : constrain){
            if(!assigned[i]){
                assign(i, tiet + subject[sub][0] + 1);
            }
        }
        // try assign subject by first fit
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
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    start = clock();
    cin >> n >> m;
    for(int i = 1; i <= n; i++){
        // input each element in subject (t[i], g[i], s[i])
        cin >> subject[i][0] >> subject[i][1] >> subject[i][2];
        // mark -1 mean not assigned yet
        subject[i][3] = -1;
        // add subject to the teacher constraint
        constraint[subject[i][1]].push_back(i);
        numTeacher = max(numTeacher, subject[i][1]);
    }
    // input c[i]
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
}
