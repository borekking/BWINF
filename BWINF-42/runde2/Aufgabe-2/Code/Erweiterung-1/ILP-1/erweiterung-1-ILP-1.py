from pulp import *
import math
import time

# For some string containing spaces, tabs, line brackets and digits:
# Return a list of integers coressponding to the string
def get_all_nums(s: str): 
        return list(map(lambda x: int(x), s.split()))

def ilp(A, K, types, styles, k, left, right, message=0, time=600):
    # Maximum element of A
    m = max(list(map(lambda l: max(l), A)))

    boxes = []
    columns = []
    empty = []

    # 1) Create Variables
    # 1.1) Boxes
    for index in range(k):    
        box = [ [] for _ in range(types)]
        for i in range(types):
            for j in range(styles):
                name = "Box-" + str(index) + "-" + str(i) + "-" + str(j) 
                x = LpVariable(name, lowBound=0, upBound=right, cat="Integer")
                box[i].append(x)
        boxes.append(box)

    # 1.2) Decistion Variables for cols and empty (k + k*styles)
    for index in range(k):
        empt = LpVariable("Empty-" + str(index), lowBound=0, cat="Binary")
        empty.append(empt)

        col = []
        for j in range(styles):
            name = "Column-" + str(index) + "-" + str(j)
            x = LpVariable(name, lowBound=0, cat="Binary")
            col.append(x)
        columns.append(col)

    # 2) Define Problem and setup objective
    PROBLEM = LpProblem("Problem-1", LpMaximize)
    objective = lpSum(boxes)
    PROBLEM += objective

    # 3) Add constraints
    # 3.1) Set columns that cannot be used together 
    # Make sure to only use upper diag of matrix K
    for index in range(k):
        for i in range(styles):
            for j in range(i+1, styles):
                if K[i][j] == False:
                    PROBLEM += columns[index][i] + columns[index][j] <= 1

    # 3.2) Set sum over all boxes of x_ij <= a_ij
    for i in range(types):
        for j in range(styles):
            sum = 0
            for index in range(k):
                sum += boxes[index][i][j]
            PROBLEM += sum <= A[i][j]

    # 3.3) For each col: IF decison variable for col is 0 => column will be empty; If its 1 we dont care if the column is used
    D1 = types * m + 1 # Set D1 such that D1 >= sum of column
    for index in range(k):
        for j in range(styles):
            sum = 0
            for i in range(types):
                sum += boxes[index][i][j]
            PROBLEM += sum - D1 * columns[index][j] <= 0

    # 3.4) If any column is used we want the row-constrains to be ensured
    D2 = styles * m + left + 1 # Set D2 such that D2 >= LEFT + sum of row

    for index in range(k):
        sum = 0 # Sum over descion Variables of current box
        for j in range(styles):
            sum += columns[index][j]
        # Define empty[index] as the or of the decison variables of current box
        # E.i., the box is empty if empty[index] is 0
        PROBLEM += styles * empty[index] - sum >= 0
        PROBLEM += styles * empty[index] - sum - styles + 1 <= 0

        # If the box is empty (empty[index] is 0) make sure LOWER BOUND constraint is always satisfied
        for i in range(types):
            row = 0
            for j in range(styles):
                row += boxes[index][i][j]

            # 1) Lower Bound (A)
            PROBLEM += row + D2 * (1 - empty[index]) >= left

            # 2) Upper bound (B)
            PROBLEM += row <= right

    # 4) Solve 
    solver = pulp.PULP_CBC_CMD(msg=message, timeLimit=time)
    PROBLEM.setSolver(solver)
    PROBLEM.solve()

    return (PROBLEM, boxes, empty, columns)

# Function computing the 
def solve(types, styles, left, right, A, K):
    # Sum of all elements in A
    sum_a = 0
    for i in range(types):
        for j in range(styles):
            sum_a += A[i][j]

    # Minimum of all sums of rows of A
    k = -1
    for i in range(types):
        sum = 0
        for j in range(styles):
            sum += A[i][j]
        if k == -1 or sum < k:
            k = sum
    
    # Set k to ceiled k / left
    # Now, k is the upper limit for the amount of boxes!
    k = math.ceil(float(k) / float(left))

    print("---------- Start computations ----------")

    msg = 0
    timeLimit = None # time limit in seconds
    start = time.time()

    PROBLEM, boxes, empty, columns = ilp(A, K, types, styles, k, left, right, msg, timeLimit)

    end = time.time()
    print("Time:", end - start)

    print("---------- Done computating ----------")

    # Print stats
    print("---------- Stats ----------")

    print("Boxes:", k)
    print("constraints:", len(PROBLEM.constraints))
    print("Variables:", len(PROBLEM.variables()))
    print("Status:", LpStatus[PROBLEM.status], "(", PROBLEM.status, ")")
    print("Objective:", value(PROBLEM.objective))
    print("Sum of A:", sum_a)

    print("---------- Stats ----------")
    
    # Print Boxes to a file 
    file_name = "results.txt"
    print("---------- Starting to write results to", file_name, "----------")
    used_boxed = 0

    with open(file_name, "w") as f:
        f.truncate()

        for index in range(k):
            if empty[index].value() == 1:
                used_boxed += 1
                for i in range(types):
                    for j in range(styles):
                         f.write(str(int(boxes[index][i][j].value())) + " ")
                    f.write("\n")
                f.write("\n")
    f.close()
    
    print("Used boxes:", used_boxed)
    print("---------- Done writing results ----------")

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

# Get user inputs from command line and 
if __name__ == '__main__':
    left_bound = int(input("Please enter the left bound (B) for the problem: "))
    right_bound = int(input("Please enter the right bound (C) for the problem: "))
    path = input("Please enter an relativ-path to the file containing the problem instance: ")

    types, styles, A, K  = read_problem(path)

    solve(types, styles, left_bound, right_bound, A, K)

    print("The programm has ended!")