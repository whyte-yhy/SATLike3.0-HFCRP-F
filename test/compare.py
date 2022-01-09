import sys


result_dict = dict()
file_path1 = "testResult_for_SATLike3.0_cleaned.txt"
#file_path1 = "testResult_for_SATLike_pray2_cleaned.txt"
file_path2 = sys.argv[1].split(".txt")[0] + "_cleaned.txt"
target_file = "compare_3.0new_and_" + sys.argv[1].split("SATLike")[1]
#target_file = "compare_3.8_and_" + sys.argv[1].split("SATLike")[1]

file1 = open(file_path1, "r")
file2 = open(file_path2, "r")
compared_file = open(target_file, "w")

class_list = ["MS18", "MS19", "MS20", "MS21"]
temp_class = "initial parameters:"

contents1 = file1.readlines()
contents2 = file2.readlines()
#assert len(contents1) == len(contents2), '实例规模不一致'

def classify(line):
    for _class in class_list:
        if line.find(_class) != -1:
            return _class

def write2file():
    compared_file.write("\n" + str(temp_class) + "\n")
    if unwt_num > 0:
        compared_file.write("unweighted statistic info:\n")
        #compared_file.write("#origin_alg\t\t#now_alg\t\t#equal\t\t#origin_solved\t\t#now_solved\n")
        compared_file.write('{0: <20}'.format("#origin_alg"))
        compared_file.write('{0: <20}'.format("#now_alg"))
        compared_file.write('{0: <20}'.format("#equal"))
        compared_file.write('{0: <20}'.format("#origin_solved"))
        compared_file.write('{0: <20}'.format("#now_solved"))
        compared_file.write('{0: <20}'.format("origin_ave_time"))
        compared_file.write('{0: <20}'.format("now_ave_time"))
        compared_file.write('{0: <20}'.format("origin_score"))
        compared_file.write('{0: <20}'.format("now_score"))
        compared_file.write("\n")
        #compared_file.write(str(ori_unweighted_win_time) + "\t\t\t\t" + str(now_unweighted_win_time) + "\t\t\t\t" + str(unweighted_equal_time) + "\t\t\t" + str(ori_unweighted_solved) + "\t\t\t\t\t" + str(now_unweighted_solved))
        compared_file.write('{0: <20}'.format(str(ori_unweighted_win_time)))
        compared_file.write('{0: <20}'.format(str(now_unweighted_win_time)))
        compared_file.write('{0: <20}'.format(str(unweighted_equal_time)))
        compared_file.write('{0: <20}'.format(str(ori_unweighted_solved)))
        compared_file.write('{0: <20}'.format(str(now_unweighted_solved)))
        compared_file.write('{0: <20}'.format(str(round(origin_unweighted_ave_time / unwt_num, 4))))
        compared_file.write('{0: <20}'.format(str(round(now_unweighted_ave_time / unwt_num, 4))))
        compared_file.write('{0: <20}'.format(str(round(origin_unweighted_score / unwt_num, 4))))
        compared_file.write('{0: <20}'.format(str(round(now_unweighted_score / unwt_num, 4))))
    if wt_num > 0:
        compared_file.write("\nweighted statistic info:\n")
        #compared_file.write("#origin_alg\t\t#now_alg\t\t#equal\t\t#origin_solved\t\t#now_solved\n")
        compared_file.write('{0: <20}'.format("#origin_alg"))
        compared_file.write('{0: <20}'.format("#now_alg"))
        compared_file.write('{0: <20}'.format("#equal"))
        compared_file.write('{0: <20}'.format("#origin_solved"))
        compared_file.write('{0: <20}'.format("#now_solved"))
        compared_file.write('{0: <20}'.format("origin_ave_time"))
        compared_file.write('{0: <20}'.format("now_ave_time"))
        compared_file.write('{0: <20}'.format("origin_score"))
        compared_file.write('{0: <20}'.format("now_score"))
        compared_file.write("\n")
        #compared_file.write(str(ori_weighted_win_time) + "\t\t\t\t" + str(now_weighted_win_time) + "\t\t\t\t" + str(weighted_equal_time) + "\t\t\t" + str(ori_weighted_solved) + "\t\t\t\t\t" + str(now_weighted_solved))
        compared_file.write('{0: <20}'.format(str(ori_weighted_win_time)))
        compared_file.write('{0: <20}'.format(str(now_weighted_win_time)))
        compared_file.write('{0: <20}'.format(str(weighted_equal_time)))
        compared_file.write('{0: <20}'.format(str(ori_weighted_solved)))
        compared_file.write('{0: <20}'.format(str(now_weighted_solved)))
        compared_file.write('{0: <20}'.format(str(round(origin_weighted_ave_time / wt_num, 4))))
        compared_file.write('{0: <20}'.format(str(round(now_weighted_ave_time / wt_num, 4))))
        compared_file.write('{0: <20}'.format(str(round(origin_weighted_score / wt_num, 4))))
        compared_file.write('{0: <20}'.format(str(round(now_weighted_score / wt_num, 4))))
        compared_file.write("\n")

temp_key = ""
flag = False
for line1 in contents1:
    if line1.startswith(".."):
        temp_key = line1
    elif line1.startswith("time limit:300s"):
        flag = True
    elif flag:
        flag = False
        key = temp_key.split('../')[1][:-1]
        result_dict[key] = line1

temp_key = ""
flag = False

ori_unweighted_win_time = 0
ori_weighted_win_time = 0
now_unweighted_win_time = 0
now_weighted_win_time = 0
unweighted_equal_time = 0
weighted_equal_time = 0

ori_unweighted_solved = 0
ori_weighted_solved = 0
now_unweighted_solved = 0
now_weighted_solved = 0

origin_unweighted_ave_time = 0
now_unweighted_ave_time = 0
origin_unweighted_score = 0
now_unweighted_score = 0
origin_weighted_ave_time = 0
now_weighted_ave_time = 0

best_unweighted_score = 0
best_weighted_score = 0
origin_weighted_score = 0
now_weighted_score = 0
unwt_num = 0
wt_num = 0

for line2 in contents2:
    if line2.startswith(".."):
        temp_key = line2.split('../')[1][:-1]

        if temp_class != classify(temp_key):
            print(classify(temp_key))
            if temp_class != "initial parameters:":
                write2file()
            ori_unweighted_win_time = 0
            ori_weighted_win_time = 0
            now_unweighted_win_time = 0
            now_weighted_win_time = 0
            unweighted_equal_time = 0
            weighted_equal_time = 0

            ori_unweighted_solved = 0
            ori_weighted_solved = 0
            now_unweighted_solved = 0
            now_weighted_solved = 0

            origin_unweighted_ave_time = 0
            now_unweighted_ave_time = 0
            origin_unweighted_score = 0
            now_unweighted_score = 0
            origin_weighted_ave_time = 0
            now_weighted_ave_time = 0

            best_unweighted_score = 0
            best_weighted_score = 0
            origin_weighted_score = 0
            now_weighted_score = 0
            unwt_num = 0
            wt_num = 0

            temp_class = classify(temp_key)
    elif line2.startswith("time limit:300s"):
        flag = True
    elif flag:
        flag = False

        if (not temp_key.find(".wcnf")) or (temp_key not in result_dict.keys()):
            continue
        ori_weight = eval(result_dict[temp_key].split("	")[0]) 
        now_weight = eval(line2.split("	")[0])
        ori_time = eval(result_dict[temp_key].split("	")[1])
        now_time = eval(line2.split("	")[1])
        ori_score = 1
        now_score = 1
        if ori_time == -1:
            ori_time = 600
        if now_time == -1:
            now_time = 600

        if (temp_class == "MS16" and temp_key.find("wpms") != -1) or (temp_class == "MS21" and temp_key.find("unwt") == -1) or (temp_class == "MS20" and temp_key.find("unweighted") == -1) or (temp_class == "MS19" and temp_key.find("unweighted") == -1 or (temp_class == "MS18" and temp_key.find("unweighted") == -1)):
            ori_weighted_solved += 1
            now_weighted_solved += 1
            origin_weighted_ave_time += ori_time
            now_weighted_ave_time += now_time

            if ori_weight == -1:
                ori_weight = sys.maxsize
                ori_weighted_solved -= 1
                ori_score = 0
            if now_weight == -1:
                now_weight = sys.maxsize
                now_weighted_solved -= 1
                ori_score = 0
            if ori_weight < now_weight:
                ori_weighted_win_time += 1
                compared_file.write(temp_key + "\t|\torigin win\n")
            elif ori_weight == now_weight:
                compared_file.write(temp_key + "\t|\tequal\n")
                weighted_equal_time += 1
            elif ori_weight > now_weight:
                compared_file.write(temp_key + "\t|\tnow win\n")
                now_weighted_win_time += 1
            best_weighted_score = min(ori_weight, now_weight)
            origin_weighted_score += (best_weighted_score + 1) / (ori_weight + 1) * ori_score
            now_weighted_score += (best_weighted_score + 1) / (now_weight + 1) * now_score
            wt_num += 1
        else:
            ori_unweighted_solved += 1
            now_unweighted_solved += 1
            origin_unweighted_ave_time += ori_time
            now_unweighted_ave_time += now_time

            if ori_weight == -1:
                ori_weight = sys.maxsize
                ori_unweighted_solved -= 1
                ori_score = 0
            if now_weight == -1:
                now_weight = sys.maxsize
                now_unweighted_solved -= 1
                now_score = 0
            if ori_weight < now_weight:
                ori_unweighted_win_time += 1
                compared_file.write(temp_key + "\t|\torigin win\n")
            elif ori_weight == now_weight:
                compared_file.write(temp_key + "\t|\tequal\n")
                unweighted_equal_time += 1
            elif ori_weight > now_weight:
                compared_file.write(temp_key + "\t|\tnow win\n")
                now_unweighted_win_time += 1
            best_unweighted_score = min(ori_weight, now_weight)
            origin_unweighted_score += (best_unweighted_score + 1) / (ori_weight + 1) * ori_score
            now_unweighted_score += (best_unweighted_score + 1) / (now_weight + 1) * now_score
            unwt_num += 1
            

write2file()

file1.close()
file2.close()
compared_file.close()
