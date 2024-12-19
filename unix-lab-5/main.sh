#!/bin/bash

who > currently_logged

cut -f1 -d' ' currently_logged | tee currently_logged

cat currently_logged

while true; do

    sleep 2

    who | cut -f1 -d' ' > new_logged

    while read -r user
    do 
        if grep "$user" currently_logged; then
            continue
        else
            echo "$user" >> currently_logged
        fi

    done < new_logged

    cat currently_logged

done