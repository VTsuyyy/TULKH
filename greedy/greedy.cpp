#include<bits/stdc++.h>
using namespace std;
struct node {
    int t, s, i;
    node(int t1, int s1, int i1) : t(t1), s(s1), i(i1) {}
};
int n, m, nGV = 0, numRes = 0;
bool assign[101][61] = {};
vector<pair<int, int>> c;
vector<node*> gv[101], res;
void greedy(){
    for(int teacher = 1; teacher <= nGV; ++teacher){
        bool teached[61] = {};
        for(node *subject : gv[teacher]){
            int t = subject->t;
            for(int room = subject->s; room <= m; ++room) {
                for(int tBegin = 1; tBegin <= 60; ++tBegin){
                    if((tBegin+t)/12 != tBegin/12) continue;
                    int check = 0;
                    for(int period = 0; period < t; ++period){
                        if(teached[tBegin+period] != 0 || assign[room][tBegin+period] != 0){
                            check = 1;
                            break;
                        }
                    }
                    if(check == 0){
                        ++numRes;
                        res.push_back(new node(tBegin, room, subject->i));
                        for(int period = 0; period < t; ++period){
                            teached[tBegin+period] = 1;
                            assign[room][tBegin+period] = 1;
                        }
                        goto nextSubject;
                    }
                }
            }
            nextSubject:;
        }
    }
}

int main(){
    freopen("D:\\Learning\\.vscode\\Hust-problem\\TULKH\\test case\\inp1.txt", "r", stdin);
    cin >> n >> m;
    for(int i = 1; i <= n; i++){
        int t, g, s;
        cin >> t >> g >> s;
        gv[g].push_back(new node(t, s, i));
        nGV = max(nGV, g);
    }
    for(int i = 1; i <= m; i++){
        int ci;
        cin >> ci;
        c.push_back({ci, i});
    }
    sort(c.begin(), c.end());
    for(int i = 1; i <= nGV; i++)
        for(auto &j : gv[i]){
            int k = 0;
            while(++k <= m && j->s > c[k].first);
            j->s = k;
        }
    for(int i = 1; i <= nGV; ++i) {
        sort(gv[i].begin(), gv[i].end(), [](node *a, node *b){
            if(a->t == b->t) return a->s < b->s;
            return a->t < b->t;
        });
    }
    greedy();
    cout << numRes << endl ;
    for(auto &j : res) {
        cout << j->i << " " << j->t << " " << j->s << endl;
    }
}