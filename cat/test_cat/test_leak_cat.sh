#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
RESULT=0
DIFF_RES=""

declare -a tests=(
"VAR test_case_cat.txt"
"VAR no_file.txt"
)

declare -a extra=(
"-s test_1_cat.txt"
"-b -e -n -s -t -v test_1_cat.txt"
"-t test_3_cat.txt"
"-n test_2_cat.txt"
"no_file.txt"
"-n -b test_1_cat.txt"
"-s -n -e test_4_cat.txt"
"test_1_cat.txt -n"
"-n test_1_cat.txt"
"-n test_1_cat.txt test_2_cat.txt"
"-v test_5_cat.txt"
)

testing()
{
    t=$(echo "$@" | sed "s/VAR/$var/")
    valgrind --leak-check=yes ../s21_cat $t > test_s21_cat.log 2>&1
    leak=$(grep -A100000 "in use at exit:" test_s21_cat.log)
    (( COUNTER++ ))
    if [[ $leak == *"in use at exit: 0 bytes in 0 blocks"* ]]
    then
      (( SUCCESS++ ))
        echo -e "\e[31m$FAIL\e[0m/\e[32m$SUCCESS\e[0m/$COUNTER \e[32msuccess\e[0m cat $t"
    else
      (( FAIL++ ))
        echo -e "\e[31m$FAIL\e[0m/\e[32m$SUCCESS\e[0m/$COUNTER \e[31mfail\e[0m cat $t"
#        echo "$leak"
    fi
    rm test_s21_cat.log
}

# специфические тесты
for i in "${extra[@]}"
do
    var="-"
    testing "$i"
done

# 1 параметр
for var1 in b e n s t v
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing "$i"
    done
done

# 2 параметра
for var1 in b e n s t v
do
    for var2 in b e n s t v
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                testing "$i"
            done
        fi
    done
done

# 3 параметра
for var1 in b e n s t v
do
    for var2 in b e n s t v
    do
        for var3 in b e n s t v
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    testing "$i"
                done
            fi
        done
    done
done

# 4 параметра
for var1 in b e n s t v
do
    for var2 in b e n s t v
    do
        for var3 in b e n s t v
        do
            for var4 in b e n s t v
            do
                if [ $var1 != $var2 ] && [ $var2 != $var3 ] \
                && [ $var1 != $var3 ] && [ $var1 != $var4 ] \
                && [ $var2 != $var4 ] && [ $var3 != $var4 ]
                then
                    for i in "${tests[@]}"
                    do
                        var="-$var1 -$var2 -$var3 -$var4"
                        testing "$i"
                    done
                fi
            done
        done
    done
done

echo "======================================="
echo -e "\e[31mFAIL\e[0m: $FAIL"
echo -e "\e[32mSUCCESS\e[0m: $SUCCESS"
echo "ALL: $COUNTER"