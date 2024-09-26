# retrieving the input

with open("input.txt", "r") as filereader:
    lines = filereader.readlines()

n = int(lines[0])
del lines[0]

# formatting the input

sights = [line.split(",") for line in lines]

for i in range(n):
    sights[i][1], sights[i][3] = int(sights[i][1]), int(sights[i][3])
    if(sights[i][2] == " "):
        sights[i][2] = False
    else:
        sights[i][2] = True

result = [0]
events = {}

lastEssential = 0

indexLastSeen = {}

def calcDistance(i,j):
    return sights[j][3] - sights[i][3]

for i in range(n):

    if(sights[i][0] in indexLastSeen.keys()):
        # add to events, update indexLastSeen
        events[i] = indexLastSeen[sights[i][0]]
    indexLastSeen[sights[i][0]] = i

    if(sights[i][2] == True):

        print("SEGMENT DONE")
        for key, value in events.items():
            print(f"{key} --- {value}")

        # dp stuff
        dp = [0 for j in range(i-lastEssential+1)]
        for j in range(lastEssential+1, i+1, 1):
            if(j in events.keys()):
                dp[j-lastEssential] = dp[events[j]-lastEssential] + calcDistance(events[j], j)
            dp[j-lastEssential] = max(dp[j-lastEssential],dp[j-lastEssential-1])
        # backtracking to know which nodes were skipped in the dp solution
        
        print(dp)
        
        usedNodes = []
        cur = i
        while(cur > lastEssential):
            usedNodes.append(cur)
            if(dp[cur-lastEssential] == dp[cur-1-lastEssential]):
                cur -= 1
            else:
                cur = events[cur]
        
 
        # imporatnt, first NODE SKIPPED

        for j in range(len(usedNodes)-1,-1,-1):
            result.append(usedNodes[j])

        events = {}
        lastEssential = i
        indexLastSeen = {
            sights[i][0] : i
        }

print(result)

dp = [0 for j in range(i-lastEssential+1)]
for j in range(lastEssential+1, i+1, 1):
    if(j in events.keys()):
        dp[j-lastEssential] = dp[events[j]-lastEssential] + calcDistance(j, events[j])
    dp[j-lastEssential] = max(dp[j-lastEssential],dp[j-lastEssential-1])
# backtracking to know which nodes were skipped in the dp solution
usedNodes = []
cur = i
while(cur > lastEssential):
    usedNodes.append(cur)
    if(dp[cur-lastEssential] == dp[cur-1-lastEssential]):
        cur -= 1
    else:
        cur = events[cur]

for j in range(len(usedNodes)-1,-1,-1):
    result.append(usedNodes[j])

distanceSaved = 0

newSights = []

for i in range(len(result)-1):
    newSights.append([sights[result[i]][0], sights[result[i]][1], sights[result[i]][2], sights[result[i]][3]-distanceSaved])

    if(result[i]+1 != result[i+1]):
        # hier haben wir einen cycle gespart
        distanceSaved += sights[result[i+1]][3] - sights[result[i]][3]

newSights.append([sights[result[len(result)-1]][0], sights[result[len(result)-1]][1], sights[result[len(result)-1]][2], sights[result[len(result)-1]][3]-distanceSaved])

# handle output

print(f"We have saved a total distance of {distanceSaved} using the following path:")

for i in range(len(newSights)):
    print(f"{newSights[i][0]} during year {newSights[i][1]} with a total distance of {newSights[i][3]} --- {newSights[i][2]}")