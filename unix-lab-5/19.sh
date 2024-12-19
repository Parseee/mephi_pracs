#!/bin/bash

who >currently_logged

while [ true ]; do
    sleep 5
    who >new_logged

    for user in new_logged; do
        if [[ (( (( grep "$user" new_logged | wc -l | cut -d -f1 )) >= 2 )) && (( who | grep "$user" )) ]] then
            echo "$1" | write "$user"
        else
            echo "$1" | mail "$user"
        fi
    done
done