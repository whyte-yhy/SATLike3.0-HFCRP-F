file = open("testResult_for_SATLike3.15_cleaned.txt", "r")
#file = open("testResult_for_SATLike3.0_cleaned.txt", "r")

key_list = []
duplicate_list = []
for line in file:
    if not line.startswith('..'):
        continue
    if line in key_list:
        duplicate_list.append(line)
    key_list.append(line)
    
print("重复的数量：" + str(len(duplicate_list)))
print(duplicate_list)
