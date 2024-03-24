def m(matrices): 
    table = [[None]*(len(matrices)) for _ in range(len(matrices))]

    for x in range(len(matrices)):
        for y in range(len(matrices)):
            i = y
            if y+x < len(matrices):
                j = y + x
            else:
                continue
            #print(f"@({i},{j})")
            if i == j:
                table[i][j] = 0
            else:
                #min_list = []
                #for k in range(i,j):
                #    print(f"K:{k}")
                #    print(f"{table[i][k]}+{table[k+1][j]}+({matrices[i][0]} * {matrices[k][1]} * {matrices[j][1]})")
                #    print(table[i][k] + table[k+1][j] + (matrices[i][0] * matrices[k][1] * matrices[j][1]))
                #    min_list.append(table[i][k] + table[k+1][j] + (matrices[i][0] * matrices[k][1] * matrices[j][1]))
                table[i][j] = min(table[i][k] + table[k+1][j] + (matrices[i][0] * matrices[k][1] * matrices[j][1]) for k in range(i,j))
    return table

def main():
    for num in range(1,11):
        path = "./probs/"+(str)(num)+".txt"
        file_str = ""
        with open(path, "r") as file:
            file_str = file.read()
        file.close()

        m_list = file_str.split("},")
        for i in range(len(m_list)):
            m_list[i] = m_list[i].replace("{", "").replace("}", "").replace(" ", "").split(",")
            for j in range(len(m_list[i])):
                m_list[i][j] = (int)(m_list[i][j])
        table = m(m_list)
        print(f"{num}:",table[0][len(m_list)-1])



main()