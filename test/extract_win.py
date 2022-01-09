file_path = "compare_3.0_and_3.1.7.txt"
file = open(file_path, "r")
target_file = open(file_path[:-4] + "_who_win.txt", "w")

ori_win = 0
now_win = 0
ori_win_list = []
now_win_list = []
for line in file:
    if line.find("origin win") != -1:
        ori_win += 1
        ori_win_list.append(line)
    elif line.find("now win") != -1:
        now_win += 1
        now_win_list.append(line)
        
target_file.write("origin win info:\n")
for item in ori_win_list:
    target_file.write(item + "\n")
target_file.write("total: " + str(ori_win) + "\n\n")

target_file.write("now win info:\n")
for item in now_win_list:
    target_file.write(item + "\n")
target_file.write("total: " + str(now_win) + "\n\n")
