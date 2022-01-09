import sys

file_path = "reserve_pms_" + sys.argv[1].split("SATLike")[1]
#file_path = sys.argv[1]
target_file = sys.argv[1].split(".txt")[0] + "_cleaned.txt"

cleaned_file = open(target_file, "w")
origin_file = open(file_path, "r")

contents = origin_file.readlines()
flag = False
seg_fault = False

for content in contents:
    if content.startswith(".."):
        if seg_fault:
            cleaned_file.write("time limit:60s\n")
            cleaned_file.write("-1	-1\n")
            cleaned_file.write("time limit:300s\n")
            cleaned_file.write("-1	-1\n")
        cleaned_file.write(content)
        seg_fault = True
    elif content.startswith("time limit"):
        cleaned_file.write(content)
        flag = True
        seg_fault = False
        continue
    if flag:
        cleaned_file.write(content)
        flag = False
    # if content.startswith("closely_rate:"):
    #     cleaned_file.write(content)
    
        
cleaned_file.close()
origin_file.close()
