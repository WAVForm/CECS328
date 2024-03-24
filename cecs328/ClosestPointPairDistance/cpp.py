'''

                            Online Python Compiler.
                Code, Compile, Run and Debug python program online.
Write your code in this editor and press "Run" button to execute it.

'''

import random

def dist(p1,p2):
    #print("Distance between",p1, p2)
    return (((p1[0]-p2[0])**2) + ((p1[1]-p2[1])**2))**(1/2)#sqrt((x_1 - x_2)^2 + (y_1 - y_2)^2)

def cpp(S):
    if (abs(len(S)) <= 3): #base case
        return brute_force(S)
    
    LEFT = S[:len(S)//2:]
    RIGHT = S[(len(S)//2)::]
    
    p1,p2 = cpp(LEFT)
    q1,q2 = cpp(RIGHT)
    
    d = None
    pair = (None,None)
    if(dist(p1,p2) < dist(q1,q2)):
        d = dist(p1,p2)
        pair = (p1,p2)
    else:
        d = dist(q1,q2)
        pair = (q1,q2)
        
    mid = (LEFT[-1][0] + RIGHT[0][0]) / 2 #l's right-most point's x-coord + r's left-most points' x-coord / 2 = mid point x coord

    strip = []
    for p in LEFT:
        if p[0] >= mid - d:
            strip.append(p)
    for p in RIGHT:
        if p[0] <= mid + d:
            strip.append(p)
    
    for index, p1 in enumerate(strip):
        for p2 in [p for i,p in enumerate(strip) if i!=index]:
            if(dist(p1,p2) < d):
                pair = (p1,p2)
    
    return pair
    

def brute_force(S):
    min_d = float('inf') #sentinel, d should always be > 0
    cp = (0,0)
    for index, p1 in enumerate(S):
        S_p = [p for i,p in enumerate(S) if i!=index] #new list without point1 in it
        for p2 in S_p:
            d = dist(p1,p2)
            #print(p1, "<->", p2, "=", d) #debug
            if d < min_d: #otherwise check if d pis the lowest and set according
                min_d = d
                cp = (p1, p2)
                #print("LOWEST") #debug
    return cp

def str_to_point_arr(datastr):
    points = []
    for each in datastr.split('},'): #seperate each point
        coords = []
        for coord in each.strip('{} ').split(','): 
            coords.append(float(coord)) #for each coordinate append to coords and convert to float
        points.append(coords) #append point to points
    return points

def random_points():
    amt = random.randint(2,1000)
    points = []
    for i in range(amt):
        coords = []
        for i in range(2):
            coords.append(float(random.random()))
        points.append(coords)
    return points

def main():
    datastr = ""
    with open(input("Enter the name of the txt file (ex. 10.txt): ")) as f:
        for line in f:
            datastr += line.strip('\n')
    #print(datastr)
    points = str_to_point_arr(datastr) #convert input to list
    
    #points = random_points()
    
    points.sort() #sort points
    #print("Points:", points)
    #print()
    p1,p2 = cpp(points) #closest point pair
    print("Answer:",round(dist(p1,p2), 3)) #round d to 3 decimal

main()
