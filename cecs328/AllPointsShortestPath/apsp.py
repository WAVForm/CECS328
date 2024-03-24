from os.path import exists as file_exists

def shortest_path(V):
    largest = 0
    for i in range(len(V)):
        if(V[i][0] > largest):
            largest = V[i][0]
        elif(V[i][1] > largest):
            largest = V[i][1]
    
    dist = [[float('inf')] * largest for _ in range(largest)]

    for i in range(len(V)):
        dist[V[i][0]-1][V[i][1]-1] = V[i][2]
        dist[V[i][1]-1][V[i][0]-1] = V[i][2]

    for i in range(largest):
        for j in range(largest):
            if i == j:
                dist[i][j] = 0
                dist[j][i] = 0


    for k in range(largest):
        for i in range(largest):
            for j in range(largest):
                p = min(dist[i][j], dist[i][k] + dist[k][j])
                dist[i][j] = p
                dist[j][i] = p
    
    return dist

def process_dist():
    V = []
    file_path = "graph.txt"
    file_str = ""
    with open(file_path, "r") as file:
        file_str = file.read()
    file.close()

    V = file_str.split("},")
    for i in range(len(V)):
        V[i] = [(int)(each) for each in V[i].replace(" ", "").replace("{", "").replace("}", "").split(",")]
    dist = shortest_path(V)
    dist_str = (str)(dist)
    with open("dist.out", "w") as dist_file:
        dist_file.write(dist_str)

def main():
    if not file_exists("dist.out"):
        process_dist()

    file_str = ""
    with open("dist.out", "r") as file:
        file_str = file.read()
    file.close()
    dist = file_str.split("],")
    for i in range(len(dist)):
        dist[i] = dist[i].replace(" ", "").replace("[", "").replace("]", "").split(",")
    
    print(dist[0][4])
    print(dist[969][373])
    print(dist[95][115])
    print(dist[792][941])
    print(dist[89][102])
    print(dist[369][315])
    print(dist[641][7])
    print(dist[68][373])
    print(dist[252][726])
    print(dist[373][115])
    print(dist[264][508])


main()