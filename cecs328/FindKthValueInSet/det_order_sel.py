def fileToString(path:str):
	lines = []
	string = ""
	with open(path, "r") as file:
		lines = file.readlines()
	for line in lines:
		string += line
	return string

def stringToList(lst:str):
	k = 0
	numbers = []
	parans = 0
	buffer = ""
	for char in lst:
		if char == '{':
			parans += 1
		elif char == '}':
			if parans == 2:
				numbers.append((int)(buffer))
				buffer = ""
			parans -= 1
		elif char == ',':
			if parans == 1:
				k = (int)(buffer)
			else:
				numbers.append((int)(buffer))
			buffer = ""
		else:
			buffer += char
	return (k,numbers)

def baseCase(lst, k:int):
	lst.sort()
	return lst[k-1] #sort L and return kth position

def select(lst, k:int):
	if len(lst) <= 100:
		return baseCase(lst, k)

	#Partition L into groups of five
	partitions = [lst[i:i+5] for i in range(0, len(lst), 5)]
	
	#sort each group, median is in the middle ( ceil(len(group)/2) )
	medians = [sorted(group)[len(group)//2] for group in partitions]

	#M = SELECT({m_i}, n/10)
	M = select(medians, len(medians)//2)

	#do ifs first
	L1 = [x for x in lst if x < M]
	L2 = [x for x in lst if x == M]
	L3 = [x for x in lst if x > M]

	if k < len(L1): #if k is less than the middle, search the left partition
		return select(L1, k)
	elif k < len(L1) + len(L2): #if k is not less than the middle, but less than the sum of the amount of elements less than the middle and those equal to the middle, then k is in the middle, M
		return M
	else: #if k is larger than the sum of the amount of elements less than the middle and those equal to M, then k is larger than the middle, search the right portion
		return select(L3, k - len(L1) - len(L2))

def main():
	results = []
	for i in range(0,10,1):
		k,lst = stringToList(fileToString("probs/"+(str)(i+1)+".txt"))
		results.append(select(lst, k))
		#results.append(baseCase(lst, k))
	
	for i,answer in enumerate(results):
		print("i:",answer)
main()
