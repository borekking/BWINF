from pulp import *
from functools import reduce
import time

# For some string containing spaces, tabs and line brackets and digits, 
# return a list of integers coressponding to the string
def get_all_nums(s: str): 
        return list(map(lambda x: int(x), s.split()))

# Read in the problem from a file given by its path:
# Return as a tuple: amount of types, amount of styles, matrix A and relation K 
def read_problem(path):
    # --- Read file by lines ---
    file = open(path, "r")
    lines = file.readlines()
    file.close()

    # 1) Read amount of types and amount of styles from the first line
    numbers0 = get_all_nums(lines[0])
    types = numbers0[0]
    styles = numbers0[1]

    # 2) Read the relation K as a matrix (0-indexed!)
    K = [ [False for _ in range(styles) ] for _ in range(styles)]

    for i in range(0, styles):
        K[i][i] = True

    index = 1
    while not lines[index].isspace():
        numbers1 = get_all_nums(lines[index])
        x = numbers1[0] - 1
        y = numbers1[1] - 1
        K[x][y] = True
        K[y][x] = True
        index += 1

    index += 1 # Skip the empty line

    # 3) Read the rest of the file:
    # Matrix A indicating the amount of clothes per Type and Style
    A = [ [0 for _ in range(styles) ] for _ in range(types)]

    while index < len(lines):
        numbers2 = get_all_nums(lines[index])
        style = numbers2[0] - 1 # 0-indexed!
        type = numbers2[1] - 1 # 0-indexed!
        amount = numbers2[2]
        A[style][type] = amount
        index += 1

    return (types, styles, A, K)

def get_cliques(K, styles):
    list = []
    queue = []

    for i in range(styles):
        queue.append({i})
    
    while len(queue) != 0:
        clique = queue.pop()
        max = True

        for i in range(styles):
            if i in clique:
                continue
            # Try to add i to clique. Check if it remains a clique
            is_clique = True
            for x in clique:
                if K[i][x] == False:
                    is_clique = False
                    break
            
            if is_clique:
                max = False
                new_clique = set()
                for x in clique:
                    new_clique.add(x)
                new_clique.add(i)
                queue.append(new_clique)
        if max:
            sorted(clique)
            if not clique in list:
                list.append(clique)

    return list

def ilp(A, cliques, types, styles, maxi, left, right, message=0, time=None):
    # --- Init ILP Solver ---
    # 1) Create Variables
    #    For each clique create some amount k of boxes only using that clique 
    #    Also, create decision variables indicating of the box is empty
    # boxes = [] # 4D array: [clique index][box index][box row][box col]
    boxes = [] # 3D array: [box index][box row][box col]
    # empty = [] # 2D array: [clique index][box index]
    clique_index = [] # 1D array: Map box index to clique index
    empty = [] # 1D array: [box index]
    cnt = 0 # Box index over all boxes

    for ii in range(len(cliques)):
        clique = cliques[ii]
        # Define the amount of boxes only using current clique to be k
        # Where k = minimum over all rows (clique cols) of row sum
        k = 100000
        for i in range(types):
            sum = 0
            for j in clique:
                sum += A[i][j]
            k = min(k, sum)
        k = k // left
        
        # For every box: 
        for index in range(k):
            clique_index.append(ii)

            # 1.1) Create box of current type
            box = [ [None for _ in range(styles)] for _ in range(types)]
            for i in range(types):
                for j in clique:
                    name = "Box-" + str(cnt) + "-" + str(i) + "-" + str(j)
                    x = LpVariable(name, lowBound=0, upBound=3, cat="Integer")
                    box[i][j] = x
            boxes.append(box)

            # 1.2) For each box create a variable indicating that that box is all zero
            name =  "Empty-" + str(cnt)
            x = LpVariable(name, lowBound=0, cat="Binary")
            empty.append(x)

            # All box counter
            cnt += 1
    
    # 2) Define Problem and objective
    PROBLEM = LpProblem("Problem-1", LpMaximize)
    objective = lpSum(boxes)
    PROBLEM += objective

    # 3) Add constraints 
    # 3.1) Sum per (type, style) over all boxes 
    for i in range(types):
        for j in range(styles): 
            sum = 0
            for index in range(cnt):
                if boxes[index][i][j] is None:
                    continue
                sum += boxes[index][i][j]
            PROBLEM += sum <= A[i][j]

    # 3.2) Row constraints considering empty boxes 
    D2 = left + 1
    for index in range(cnt):
        for i in range(types):
            sum = 0
            for j in range(styles): 
                if boxes[index][i][j] is None:
                    continue
                sum += boxes[index][i][j]
            PROBLEM += sum <= right
            PROBLEM += sum + D2 * (empty[index]) >= left
            
    # 3.3) Empty boxes
    D3 = 10000 # Make sure D3 > Sum of all element of A
    for index in range(cnt):
        sum = 0
        for i in range(types):
            for j in range(styles): 
                if boxes[index][i][j] is None:
                    continue
                sum += boxes[index][i][j]
        PROBLEM += sum - D3 * (1 - empty[index]) <= 0

    # 3.4) New Constaint: Make sure the amount of used boxes is <= maxi
    sum = 0
    for index in range(cnt): 
        sum += 1 - empty[index]
    PROBLEM += sum <= maxi

    # 4) Solve 
    solver = pulp.PULP_CBC_CMD(msg=message, timeLimit=time) # HERE ---------------------------
    PROBLEM.setSolver(solver)
    PROBLEM.solve()

    return (PROBLEM, boxes, empty, cnt)
    
def solve(types, styles, maxi, left_bound, right_bound, A, K):
    cliques = get_cliques(K, styles)

    s = 0
    for i in range(types):
        for j in range(styles):
            s += A[i][j]

    print("---------- Start: " + path + "----------")
    print("Cliques:", cliques)

    msg = 0
    timeLimit = None
    start = time.time()

    PROBLEM, boxes, empty, k = ilp(A, cliques, types, styles, maxi, left_bound, right_bound, msg, timeLimit)

    end = time.time()
    print("Time:", end - start)

    print("Boxes:", k)

    print("constraints:", len(PROBLEM.constraints))
    print("Variables:", len(PROBLEM.variables()))

    print("Status:", LpStatus[PROBLEM.status])
    print("Status:", PROBLEM.status)
    print("Objective", value(PROBLEM.objective))
    print("Sum:", s)

    print("---------- Stats ----------")

    # Print Boxes to a file 
    file_name = "results.txt"
    print("---------- Starting to write results to", file_name, "----------")
    used_boxed = 0

    with open(file_name, "w") as f:
        f.truncate()

        for index in range(k):
            if empty[index].value() == 0:
                used_boxed += 1
                for i in range(types):
                    for j in range(styles):
                        if boxes[index][i][j] is None:
                            f.write("0 ")
                        else:
                            f.write(str(int(boxes[index][i][j].value())) + " ")
                    f.write("\n")
                f.write("\n")
    f.close()
    
    print("Used boxes:", used_boxed)
    print("---------- Done writing results ----------")

# Get user inputs from command line and 
if __name__ == '__main__':
    left_bound = int(input("Please enter the left bound (B) for the problem: "))
    right_bound = int(input("Please enter the right bound (C) for the problem: "))
    path = input("Please enter an relativ-path to the file containing the problem instance: ")
    maxi = int(input("Please enter the maximum amount of boxes: "))

    types, styles, A, K  = read_problem(path)
    solve(types, styles, maxi, left_bound, right_bound, A, K)

    print("A:")
    for i in range(types):
        for j in range(styles):
            print(str(A[i][j]), end=" ")
        print("")

    print("The programm has ended!")