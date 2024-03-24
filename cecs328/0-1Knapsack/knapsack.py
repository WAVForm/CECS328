import os
import time

cwd = cwd = os.path.abspath(os.path.join(os.path.realpath(__file__), os.pardir))

def knapsack_table(i,c, bricks):
    #This program will populate a table of the maximum profit given the amount of bricks i, the maximum weight capacity c, and the weight and value of each brick bricks which stores value in index 0 and weight in index 1

    table = [[0] * (c+1) for row  in range(i+1)] #start by making a 2D array. This array will go from 0 to c horizontally (meaning it will be c+1 units wide), and from 0 to i (meaning it will be i+1 units tall)
    #since the table starts off being filled with 0's, we don't need the base case: if i==0 or c==0: table[i][c] = 0

    for x in range(1,i+1): #since we don't need the base case, iterate from 1 to i (the stop in range is exclusive so i+1 really stops at i)
        for y in range(1,c+1): #for each row, iterate from 1 to c
            #x will store current available items {1...x}
            #y will store capacity remaining
            if bricks[x-1][1] > y:
                table[x][y] = table[x-1][y]
            else:
                #store in the table whichever is largest
                #the previous max profit
                #the max profit found from considering the value with respect to the previous items weight, the remaining capacity left, and the current items weight?
                table[x][y] = max(table[x-1][y], table[x-1][y-bricks[x-1][1]]+bricks[x-1][0])

    #return the max profit
    return table[i][c]
    

def P(i,c, bricks):
    if i == 0 or c == 0:
        return 0
    if(bricks[i-1][1] > c):
        return P(i-1, c, bricks)
    else:
        return max(P(i-1,c, bricks), P(i-1, c-bricks[i-1][1], bricks)+bricks[i-1][0])
    
def main():
    files = [(str)(i)+".txt" for i in range(1,11,1)]
    bricks_list = []
    capacities = []
    for file in files:
        file_str = ""
        with open(cwd+"/probs/"+file) as file:
            file_str = file.read()
        file.close()

        buffer = ""
        capacity = 0
        for char in file_str:
            if char != ',':
                buffer += char
            else:
                capacity = (int)(buffer.replace("{", ""))
                begin_size = len(buffer)+1
                break
        capacities.append(capacity)
        buffer = ""
        file_str = file_str[begin_size:]
        bricks = []
        for i,char in enumerate(file_str):
            if i != len(file_str)-1:
                if char == ',' and file_str[i-1] == '}':
                    bricks.append([(int)(number) for number in buffer.replace("{", "").replace("}", "").split(",")])
                    buffer = ""
                elif char == '}' and file_str[i-1] == '}':
                    bricks.append([(int)(number) for number in buffer.replace("{", "").replace("}", "").split(",")])
                    buffer = ""
                else:
                    buffer += char
        bricks_list.append(bricks)
    
    for i,file in enumerate(files):
        start = time.time()
        #print(f"#{i+1}: "+(str)(P(len(bricks_list[i]), capacities[i], bricks_list[i])),(str)(round(time.time()-start, 4))+"s") 
        #print(f"#{i+1}:",(str)(knapsack(bricks_list[i], capacities[i])),(str)(round(time.time()-start, 4))+"s")
        print(f"#{i+1}:",(str)(knapsack_table(len(bricks_list[i]), capacities[i], bricks_list[i])),(str)(round(time.time()-start, 4))+"s")

main()