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
    // foreach teacher
    for(int teacher = 1; teacher <= nGV; ++teacher){
        bool teached[61] = {}; // mark this teacher has been assigned at this period yet 
        // foreach subject this teacher can teach 
        for(node *subject : gv[teacher]){
            int t = subject->t;
            // choose the best fit room
            for(int room = subject->s; room <= m; ++room) {
                // check all possible periods 
                for(int tBegin = 1; tBegin+t <= 60; ++tBegin){
                    int check = 0;
                    for(int period = 0; period < t; ++period){
                        // if not value
                        if(teached[tBegin+period] != 0 || assign[room][tBegin+period] != 0){
                            check = 1;
                            break;
                        }
                    }
                    // if valid
                    if(check == 0){
                        ++numRes;
                        res.push_back(new node(tBegin, room, subject->i));
                        for(int period = 0; period < t; ++period){
                            // assign it
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
    cin >> n >> m;
    for(int i = 1; i <= n; i++){
        int t, g, s;
        cin >> t >> g >> s;
        // input each subject and save it to a node (number periods, number students, index of subject)
        gv[g].push_back(new node(t, s, i));
        nGV = max(nGV, g);
    }
    for(int i = 1; i <= m; i++){
        int ci;
        cin >> ci;
        // save each room c[i] and index of room
        c.push_back({ci, i});
    }
    // sort room by c[i]
    sort(c.begin(), c.end());
    for(int i = 1; i <= nGV; i++)
        for(auto &j : gv[i]){
            int k = 0;
            // for each subject find the room smallest can assign
            while(++k <= m && j->s > c[k].first);
            j->s = k;
        }
    for(int i = 1; i <= nGV; ++i) {
        // sort subject by compare periods, if equal then compare number of students.
        sort(gv[i].begin(), gv[i].end(), [](node *a, node *b){
            if(a->t == b->t) return a->s < b->s;
            return a->t < b->t;
        });
    }

    greedy();

    // output
    cout << numRes << endl ;
    for(auto &j : res) {
        cout << j->i << " " << j->t << " " << j->s << endl;
    }
}
