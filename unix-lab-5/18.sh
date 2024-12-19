#!/bin/bash

traitors=($( ps -e -o user,pid,start,command | grep -e '^.*T' | cut -d' ' -f1 | tail -n +2 ))

for user in "${traitors[@]}"; do
    echo "ACHTUNG!" | write -u "$user" 
done

sleep $(( 2 ))

new_traitors=($( ps -e -o user,pid,start,command | grep -e '^.*T' | cut -d' ' -f1 | tail -n +2 ))

for user in "${new_traitors[@]}"; do

    echo "$user" >> traitors_log

done