#!/bin/bash


basic_cpp="#include <iostream>
#include <vector>
using namespace std;
using vvi = vector<vector<long>>;

int main(){

    return 0;
}"


test_code () {
    run_code() {
        if [[ $# -eq 4 ]]; then
            tmp="$(mktemp)"
                if [[ -e ./code.py ]]; then
                    python3 ./code.py < "$1" 2>>"$3" | tee "$tmp" && diff "$tmp" "$2" &> "$4"
                else
                    ./code  < "$1" 2>>"$3" | tee "$tmp" && diff "$tmp" "$2" &> "$4"
                fi
        else
            if [[ -e ./code.py ]]; then
                python3 ./code.py < "$1" 2>>"$3" | diff - "$2"
            else
                ./code < "$1" 2>>"$3" | diff - "$2"
            fi
        fi
        return $?
    }

    cd "$1" || exit 1
    local debugf="./debug_info.txt"
    declare -a results=()
    if [ -e code.py  ] || [ -x code ]; then # single bracket [] construct doesnt prevent globbing whereas double [[ ]] does
        echo -n "" > "$debugf"
        i=0
        testfiles=(./*.in)
        for testfile in "${testfiles[@]}"; do
            echo -e "\e[1mTesting $testfile\e[0m" | tee -a "$debugf"
            if [[ $# -eq 1 ]]; then
                run_code "$testfile" "${testfile%.in}.out" "$debugf"
                results[$i]=$?
            else
                run_code "$testfile" "${testfile%.in}.out" "$debugf" "/dev/null"
                results[$i]=$?
            fi
            let "i++"
            echo
        done
        for (( res=0; res < ${#results[@]}; res ++)); do
            success="Test ${testfiles[$res]} \e[1;32mSUCCESS\e[0m"
            failure="Test ${testfiles[$res]} \e[1;31mFAIL\e[0m"
            (( ${results[$res]} )) && echo -e "$failure" || echo -e "$success"
        done
    else
        echo "No runnable file found. Please use code.py or code"
        exit 1
    fi
}

build_dir () {
    mkdir -p "$1" || exit 1
    for (( i=1; i <= $2; i++ )); do
        touch "$1/test$i.in" "$1/test$i.out"
    done;
    # echo '#! /usr/bin/env python3' > "$1/code.py"
    echo "${basic_cpp}" > "$1/code.cpp"
}


SCRIPTNAME="`basename $0`"
USAGE="Usage: ${SCRIPTNAME} [test|build] (arg...)"

ARGS=2

if [[ $# -lt $ARGS ]]
then
    echo $USAGE
    exit 1
fi

cmd="$1"
shift

if [[ "$cmd" == "test" ]]; then
    if [[ $# -eq 1 ]]; then
        test_code "$1"
    elif [[ $# -eq 2 && "$2" == "nodiff" ]]; then
        test_code "$1" "$2"
    else
        echo "Usage: ${SCRIPTNAME} test DIRECTORY"
        exit 1
    fi
elif [[ "$cmd" == "build" ]]; then
    if [[ $# -eq 2 ]]; then
        build_dir "$1" "$2"
    else
        echo "Usage: ${SCRIPTNAME} build DIRECTORY NUM_TESTS"
        exit 1
    fi
else
    echo $USAGE
fi
