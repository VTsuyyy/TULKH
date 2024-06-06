from ortools.sat.python import cp_model
import sys 

def read_input_from_stdin():
    input_data = sys.stdin.read().strip().split('\n')

    first_line = input_data[0].strip()
    m, n = map(int, first_line.split())

    array1 = []
    array2 = []
    array3 = []

    for i in range(1, m + 1):
        line = input_data[i].strip().split()
        array1.append(int(line[0]))
        array2.append(int(line[1]))
        array3.append(int(line[2]))

    last_line = input_data[m + 1].strip()
    array_last = list(map(int, last_line.split()))

    return m, n, array1, array2, array3, array_last

def read_input_from_file(file_path):
    with open(file_path, 'r') as file:
        first_line = file.readline().strip()
        m, n = map(int, first_line.split())

        array1 = []
        array2 = []
        array3 = []

        for _ in range(m):
            line = file.readline().strip().split()
            array1.append(int(line[0]))
            array2.append(int(line[1]))
            array3.append(int(line[2]))

        last_line = file.readline().strip()
        array_last = list(map(int, last_line.split()))

    return m, n, array1, array2, array3, array_last

def solve(N, M, t, g, s, c):
    N_CLASSES = N
    N_ROOMS = M
    N_SESSIONS = 10
    N_PERIODS = 6

    model = cp_model.CpModel()
    x = {}
    for i in range(N_CLASSES):
        for j in range(N_ROOMS):
            for k in range(N_SESSIONS):
                for l in range(N_PERIODS):
                    x[i, j, k, l] = model.NewIntVar(0, 1, f'x[{i}, {j}, {k}, {l}]')
    
    for i in range(N_CLASSES):
        for j in range(N_ROOMS):
            for k in range(N_SESSIONS):
                for l in range(N_PERIODS):
                    model.Add(x[i, j, k, l]*s[i] <= c[j])

    for j in range(N_ROOMS):
        for k in range(N_SESSIONS):
            for l in range(N_PERIODS):
                model.Add(sum(x[i, j, k, l] for i in range(N_CLASSES)) <= 1)

    for i in range(N_CLASSES):
        for k in range(N_SESSIONS):
            for l in range(N_PERIODS):
                model.Add(sum(x[i, j, k, l] for j in range(N_ROOMS)) <= 1)

    for i in range(N_CLASSES):
        for j in range(N_ROOMS):
            for k in range(N_SESSIONS):
                    c = model.NewBoolVar(f'c[{i}, {j}, {k}]')
                    model.Add(sum(x[i, j, k, l] for l in range(N_PERIODS)) == t[i]).OnlyEnforceIf(c)
                    model.Add(sum(x[i, j, k, l] for l in range(N_PERIODS)) == 0).OnlyEnforceIf(c.Not())
    
    for i in range(N_CLASSES):
        for j in range(N_ROOMS):
            for k in range(N_SESSIONS):
                for l in range(N_PERIODS-t[i]):
                    c = model.NewBoolVar(f'c[{i}, {j}, {l}, {k}]')
                    model.Add(sum(x[i, j, k, m] for m in range(l, l+t[i])) == t[i]).OnlyEnforceIf(c)
                    model.Add(sum(x[i, j, k, m] for m in range(l, l+t[i])) != t[i]).OnlyEnforceIf(c.Not())
                    model.Add(x[i, j, k, l] == 1).OnlyEnforceIf(c)

    for i in range(N_CLASSES):
        for j in range(N_CLASSES):
            if i!=j:
                if g[i] == g[j]:
                    for r in range(N_ROOMS):
                        for k in range(N_SESSIONS):
                            for l in range(N_PERIODS):
                                model.Add(x[i, r, k, l]+x[j, r, k, l] <= 1)
    
    y = {}
    for i in range(N_CLASSES):  
        y[i] = model.NewIntVar(0, 1, f'y[{i}]')
        c = model.NewBoolVar('cy[{i}]')
        model.Add(y[i] == 1).OnlyEnforceIf(c)
        model.Add(y[i] == 0).OnlyEnforceIf(c.Not())
        model.Add(sum(x[i, j, k, l] for j in range(N_ROOMS) for k in range(N_SESSIONS) for l in range(N_PERIODS))==t[i]).OnlyEnforceIf(c)
        model.Add(sum(x[i, j, k, l] for j in range(N_ROOMS) for k in range(N_SESSIONS) for l in range(N_PERIODS))==0).OnlyEnforceIf(c.Not())    
    
    model.Maximize(sum(y[i] for i in range(N_CLASSES)))
    
    return model, x, y

def process_output(lines):
    tuples = [tuple(map(int, line.split())) for line in lines]
    sorted_tuples = sorted(tuples, key=lambda x: (x[0], x[1]))

    from itertools import groupby
    min_value_tuples = []
    for key, group in groupby(sorted_tuples, key=lambda x: x[0]):
        min_value_tuples.append(min(group, key=lambda x: x[1]))

    for item in min_value_tuples:
        print(f"{item[0]} {item[1]} {item[2]}")

if __name__ == "__main__":
    # N, M, t, g, s, c = read_input_from_file("inp.txt")
    N, M, t, g, s, c = read_input_from_stdin()
    model, x, y = solve(N, M, t, g, s, c)
    solver = cp_model.CpSolver()
    status = solver.Solve(model)
    lines = []
    if(status == cp_model.OPTIMAL):
        print(int(solver.ObjectiveValue()))
        for i in range(N):
            if solver.Value(y[i]) == 1:
                for j in range(M):
                    for k in range(10):
                        for l in range(6):
                            if solver.Value(x[i, j, k, l]) == 1:
                                lines.append(f'{i+1} {k*6+l+1} {j+1}')
 
    process_output(lines)
