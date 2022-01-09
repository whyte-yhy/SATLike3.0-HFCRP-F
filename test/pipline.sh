#!/bin/bash

python ./delete_not_pms.py $1
python ./clean.py $1
python ./compare.py $1
#python ./statistics.py
