#!/bin/bash

read am pat subst

for  (( i = 0; i <= 10; i += 1)); do
    read string
    echo "$string ${string//$pat/$subst}"
done