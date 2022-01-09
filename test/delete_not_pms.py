import sys

def delete_not(filepath, target_file, pms_list, split_char):
    file = open(filepath, "r")
    write = False
    for line in file:
        line = line.replace("../SATSolvers/extracted_maxsat_instances/", "")
        if line.startswith('..'):
            if line.split(split_char)[1] in pms_list:
                write = True
                target_file.write(line)
            else:
                write = False
        elif write:
            target_file.write(line)


if __name__ == '__main__':
    pms_list = []
    pms_file = open("PMS_instances.txt", "r")
    for line in pms_file:
        pms_list.append(line.split("extracted_maxsat_instances/")[1])
    filepath = sys.argv[1]
    target_name = "reserve_pms_" + filepath.split("SATLike")[1]
    target_file = open(target_name, "w")
    delete_not(filepath, target_file, pms_list, '../')
    target_file.close()
