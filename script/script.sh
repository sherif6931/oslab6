#!/usr/bin/env bash

set -euo pipefail

errors=""

for input in 0 1; do
    outfile="out${input}.txt"
    timeout 10s ./bin/out <<< "$input" > "$outfile" 2>&1 || errors+="❎ Program Timeout for input $input"$'\n'
    output=$(<"$outfile")

    [[ "$output" =~ Child1\ PID:.*Parent\ PID: ]] || errors+="❎ Child1 PID/Parent Requirement Failed for input $input"$'\n'
    [[ "$output" =~ Child2\ PID:.*Parent\ PID: ]] || errors+="❎ Child2 PID/Parent Requirement Failed for input $input"$'\n'

    if [[ $input == 0 ]]; then
        [[ "$output" =~ "Child process resumed" ]] || errors+="❎ Child2 Should Have Been Resumed for input $input"$'\n'
    fi
    
    [[ "$output" =~ "[INTERRUPT=0]" ]] || errors+="❎ Child2 should have been killed by SIGKILL (interrupt=9) for input $input"$'\n'
done

if [[ -n "$errors" ]]; then
    echo "$errors"
    exit 1
fi

echo "✅ All checks passed"
