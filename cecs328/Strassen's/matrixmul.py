import time

pows_of_2 = [2**i for i in range(32)] 

def text_to_list(txt):
    rowsp = []
    rows = [each.strip('}{ ') for each in txt.split('},')]
    split = [each.split(',') for each in rows] 
    for row in split:
        r = []
        for each in row:
            r.append(int(each))
        rowsp.append(r)
    del rows, split, r, each
    return rowsp

def mat_str(rowsp):
    #for pretty printing
    mat_str = ""
    for row in rowsp:
        mat_str += str(row) + "\n"
    return mat_str

def rowsp_to_colsp(rowsp):
    colsp = []
    for j in range(len(rowsp[0])):
        col = []
        for i in range(len(rowsp)):
            col.append(rowsp[i][j])
        colsp.append(col)
    del j, i, col
    return colsp

def colsp_to_rowsp(colsp):
    rowsp = []
    for j in range(len(colsp[0])):
        row = []
        for i in range(len(colsp)):
            row.append(colsp[i][j])
        rowsp.append(row)
    del j, i, row
    return rowsp

def mat_add(a, b):
    if(len(a) == len(b)):
        sum_mat = [[a[i][j] + b[i][j] for j in range(len(a[i]))] for i in range(len(a))]
        return sum_mat
    
def mat_sub(a, b):
    return mat_add(a, mat_mul_var(b, -1)) 

def mat_mul_var(a, c):
    mul_array = []
    for i in range(len(a)):
        row = []
        for j in range(len(a[i])):
            row.append(a[i][j] * c)
        mul_array.append(row)
    del row
    return mul_array

def mat_mul(a, b):
    if len(rowsp_to_colsp(a)) == len(b):
        b_c = rowsp_to_colsp(b)
        mul_array = []
        for row in a:
            r = []
            for col in b_c:
                sum = 0
                for i in range(len(row)):
                    sum += row[i] * col[i]
                r.append(sum)
            mul_array.append(r)
        del r, sum, row, col
        return mul_array

def mat_strassen(mat_a, mat_b):
    if((len(mat_a) == len(mat_b)) and (len(mat_a) <= 128)):
        return mat_mul(mat_a, mat_b)
    
    _a = mat_a[:]
    _b = mat_b[:]

    while(len(_a) not in pows_of_2):
        for i in range(len(_a)):
            _a[i].append(0)
        _a.append([0]*len(_a[0]))
    while(len(_b) not in pows_of_2):
        for i in range(len(_b)):
            _b[i].append(0)
        _b.append([0]*len(_b[0]))

    a,b,c,d = split(_a) #split into 4 quarters
    e,f,g,h = split(_b)

    p1 = mat_strassen(a, mat_sub(f, h))
    p2 = mat_strassen(mat_add(a, b), h)  
    p3 = mat_strassen(mat_add(c, d), e) 
    p4 = mat_strassen(d, mat_sub(g, e))
    p5 = mat_strassen(mat_add(a, d), mat_add(e, h))
    p6 = mat_strassen(mat_sub(b, d), mat_add(g, h))
    p7 = mat_strassen(mat_sub(a, c),  mat_add(e, f))

    c11 = mat_sub(mat_add(p5, p4), mat_add(p2, p6))
    c12 = mat_add(p1,p2)          
    c21 = mat_add(p3,p4)            
    c22 = mat_sub(mat_add(p1,p5), mat_sub(p3, p7))  

    mat_c = stitch(c11,c12,c21,c22) #recombine the 4 corners into one matrix
    del a,b,c,d,e,f,g,h,p1,p2,p3,p4,p5,p6,p7,c11,c12,c21,c22,_a,_b #for memory :)
    return mat_c

def split(mat):
    row, col = len(mat)//2, len(mat[0])//2

    q11 = [[each for j,each in enumerate(r) if j<col] for i,r in enumerate(mat) if i<row]
    q12 = [[each for j,each in enumerate(r) if j>=col] for i,r in enumerate(mat) if i<row]
    q21 = [[each for j,each in enumerate(r) if j<col] for i,r in enumerate(mat) if i>=row]
    q22 = [[each for j,each in enumerate(r) if j>=col] for i,r in enumerate(mat) if i>=row]

    del row, col
    return q11,q12,q21,q22

def stitch(q11,q12,q21,q22):
    h1 = q11[:]
    h2 = q21[:]

    for i in range(len(h1)):
        h1[i].extend(q12[i])
    for i in range(len(h2)):
        h2[i].extend(q22[i])

    h1.extend(h2)
    del h2
    return h1

def fin_ans(mat):
    sum = 0
    for row in mat:
        for each in row:
            sum += each
    return sum

def main():
    start = time.time()
    a_rowsp = text_to_list(open("a.txt").read().replace("\n", ""))
    b_rowsp = text_to_list(open("b.txt").read().replace("\n", ""))

    print(fin_ans(mat_strassen(a_rowsp,b_rowsp)))
    print(f"Execution Time: {time.time()-start}s")
    start = time.time()

    print(fin_ans(mat_mul(a_rowsp, b_rowsp)))
    print(f"Execution Time: {time.time()-start}s")

main()