from sys import maxsize

def maxSubArraySum(a, size):
    max_so_far = -maxsize - 1
    max_ending_here = 0
    start = 0
    end = 0
    s = 0
 
    for i in range(0, size):
 
        max_ending_here += a[i]
 
        if max_so_far < max_ending_here:
            max_so_far = max_ending_here
            start = s
            end = i
 
        if max_ending_here < 0:
            max_ending_here = 0
            s = i+1
 
    print("Maximum contiguous sum is %d" % (max_so_far))
    print("Starting Index %d" % (start))
    print("Ending Index %d" % (end))

    return a[start:end+1]

def formatArray(file):
    arr = []
    file = file.strip("{ ")
    buffer = ""
    for each in file:
        if each == ',' or each == '}':
            arr.append(int(buffer))
            buffer = ""
        else:
            buffer += each
    return arr

def main():
    for i in range(10):
        path = "probs/"+str(i+1)+".txt"
        with open(path) as file:
            arr = formatArray(file.read())
            maxSubArraySum(arr, len(arr))
        input("Press to continue: ")
main()