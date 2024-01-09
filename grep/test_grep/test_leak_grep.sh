#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
RESULT=0
DIFF_RES=""

declare -a tests=(
"s test_grep/test_text_grep.txt VAR"
"for my_grep.c my_grep.h Makefile VAR"
"for my_grep.c VAR"
"-e for -e ^int my_grep.c my_grep.h Makefile VAR"
"-e for -e ^int my_grep.c VAR"
"-e regex -e ^print my_grep.c VAR -f test_grep/test_ptrn_grep.txt"
"-e while -e void my_grep.c Makefile VAR -f test_grep/test_ptrn_grep.txt"
"VAR no_file.txt"
)

declare -a extra=(
"-n for test_grep/test_1_grep.txt test_grep/test_2_grep.txt"
"-n for test_grep/test_1_grep.txt"
"-n -e ^\} test_grep/test_1_grep.txt"
"-ce ^int test_grep/test_1_grep.txt test_grep/test_2_grep.txt"
"-e ^int test_grep/test_1_grep.txt"
"-nivh = test_grep/test_1_grep.txt test_grep/test_2_grep.txt"
"-e"
"-ie INT test_grep/test_5_grep.txt"
"-echar test_grep/test_1_grep.txt test_grep/test_2_grep.txt"
"-ne = -e out test_grep/test_5_grep.txt"
"-iv int test_grep/test_5_grep.txt"
"-in int test_grep/test_5_grep.txt"
"-c -l aboba test_grep/test_1.txt test_grep/test_5_grep.txt"
"-v test_grep/test_1_grep.txt -e ank"
"-noe ) test_grep/test_5_grep.txt"
"-l for test_grep/test_1_grep.txt test_grep/test_2_grep.txt"
"-o -e int test_grep/test_4_grep.txt"
"-e = -e out test_grep/test_5_grep.txt"
"-noe ing -e as -e the -e not -e is test_grep/test_6_grep.txt"
"-e ing -e as -e the -e not -e is test_grep/test_6_grep.txt"
"-c -e . test_grep/test_1_grep.txt -e '.'"
"-l for no_file.txt test_grep/test_2_grep.txt"
"-f test_grep/test_3_grep.txt test_grep/test_5_grep.txt"
)

testing()
{
    t=$(echo "$@" | sed "s/VAR/$var/")
    valgrind --leak-check=yes ./my_grep $t > test_grep/test_my_grep.log 2>&1
    leak=$(grep -A100000 "ERROR SUMMARY:" test_grep/test_my_grep.log)
    (( COUNTER++ ))
    if [[ $leak == *"0 errors from 0 contexts"* ]]
    then
      (( SUCCESS++ ))
        echo -e "\e[31m$FAIL\e[0m/\e[32m$SUCCESS\e[0m/$COUNTER \e[32msuccess\e[0m grep $t"
    else
      (( FAIL++ ))
        echo -e "\e[31m$FAIL\e[0m/\e[32m$SUCCESS\e[0m/$COUNTER \e[31mfail\e[0m grep $t"
        echo "$leak"
    fi
    rm test_grep/test_my_grep.log
}

# специфические тесты
for i in "${extra[@]}"
do
    var="-"
    testing "$i"
done

# 1 параметр
for var1 in v c l n h o
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing "$i"
    done
done

# 2 параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
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
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        for var3 in v c l n h o
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

# 2 сдвоенных параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1$var2"
                testing "$i"
            done
        fi
    done
done

# 3 строенных параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        for var3 in v c l n h o
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1$var2$var3"
                    testing "$i"
                done
            fi
        done
    done
done

echo "======================================="
echo -e "\e[31mFAIL\e[0m: $FAIL"
echo -e "\e[32mSUCCESS\e[0m: $SUCCESS"
echo "ALL: $COUNTER"