from pulp import *
import time

def get_points(fileName):
    # Read file's content line by line
    file = open(fileName, 'r')
    lines = file.readlines()

    # Read amount of points (first line)
    # And create list of that size containing lists of size 2 
    # that will present the points
    size = int(lines[0])
    array = [[0]*2 for _ in range(size)]

    # Fill the list with the points from the file
    for i in range(0, size):
        # Read the current point
        line = lines[i+1]
        array[i][0] = int(float(line.split(" ")[0]) * 1000000)
        array[i][1] = int(float(line.split(" ")[1]) * 1000000)

    return array

def ilp(Z, points, A, B, R, message=0, time=60):
    # --- Init ILP Solver ---
    # 1) Create Variables
    variables = []

    for p in points:
        name = "Point:" + str(p[0]) + ":" + str(p[1])
        x = LpVariable(name, lowBound=0, cat="Binary")
        variables.append(x)
    
    # 2) Define Problem and objective
    PROBLEM = LpProblem("Problem-1", LpMaximize)
    objective = lpSum(variables)
    PROBLEM += objective

    # 3) Add constraints 
    n = len(points)
    cnt = 0

    for i in range(0, n):
        for j in range(i+1, n):
            if (((points[i][0] - Z[0])**2 + (points[i][1] - Z[1])**2) <= R**2 or ((points[j][0] - Z[0])**2 + (points[j][1] - Z[1])**2) <= R**2) and ((points[i][0] - points[j][0])**2 + (points[i][1] - points[j][1])**2) < A**2:
                PROBLEM += ((variables[i] + variables[j]) <= 1)
                cnt += 1
            elif (((points[i][0] - Z[0])**2 + (points[i][1] - Z[1])**2) > R**2 and ((points[j][0] - Z[0])**2 + (points[j][1] - Z[1])**2) > R**2) and ((points[i][0] - points[j][0])**2 + (points[i][1] - points[j][1])**2) < B**2:
                PROBLEM += ((variables[i] + variables[j]) <= 1)
                cnt += 1

    print("Anzahl constraints:", cnt)

    solver = pulp.PULP_CBC_CMD(msg=message, timeLimit=time)
    PROBLEM.setSolver(solver)
    PROBLEM.solve()

    result = []
    for i in range(n):
        if variables[i].value() == 1:
            result.append(points[i])

    print("Anzahl Punkte:", len(result))
    return result

def solve(G, T, A, B, R):
    best_set = []
    best_Z = None

    for Z in G:
        current_set = ilp(Z, T, A, B, R)

        if len(current_set) > len(best_set):
            best_set = current_set
            best_Z = Z

    return (best_Z, best_set)

if __name__ == "__main__":
    R = 85*1000000
    A = 10*1000000
    B = 20*1000000
   	 
    # Expect files to have format: <amount of points>,
    # for each line: x y  
    file0 = input("Input file name containg points in set G: ")
    G = get_points(file0)
    file1 = input("Input file name containg points in set T: ")
    T = get_points(file1)

    start = time.time()

    Z, best_set = solve(G, T, A, B, R)

    end = time.time()
    print("Zeit:", end - start)

    print("Gesundheitszentrum:", Z[0] / 1000000.0, Z[1] / 1000000.0)    
    print("Punkte:")

    for X in best_set:
        print(X[0] / 1000000.0, X[1] / 1000000.0)
