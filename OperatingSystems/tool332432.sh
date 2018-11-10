#!/bin/bash

#No args -> AM
if [ $# -eq 0 ]
  then
    echo "1054332-1054432"
    exit
fi

# id|lastName|firstName|gender|birthday|joinDate|IP|browserUsed|socialmedia

columnsep='\t'
dateformat='%d/%m/%y'

#parameter parsing
while [ -n "$1" ]; do # iterates every option until no option left
    case "$1" in
 
    -f) file="$2" # gets file name from next parameter
        shift;;
 
    -id) id="$2"
        shift;;
 
    --socialmedia) socialmedia='true';; 
 
    --firstnames) firstnames='true';;
    
    --lastnames) lastnames'true';;
    
    --born-since) bornsince="$2"
            shift;;
    
    --born-until) bornuntil="$2"
            shift;;
            
    --edit) editid="$2"
            editcolumn="$3"
            editvalue="$4"
            shift 3;;
 
    *) echo "Option $1 not recognized";;
    esac
    shift # next parameter for parsing
done

#ignore lines that start with #
#compare birthdates and if check pass, print line (5th column is birthday date)
if [[ ${bornsince+x} ]] && [[ ${bornuntil+x} ]]; then
    awk -F "$columnsep" -v dateA="$bornsince" -v dateB="$bornuntil" '{if (FNR>1 && dateA<=$5 && dateB>=$5) {print}}' $file
    exit
fi

if [[ ${bornsince+x} ]]; then
    awk -F "$columnsep" -v dateA="$bornsince" '{if (FNR>1 && dateA<=$5) {print}}' $file
    exit
fi

if [[ ${bornuntil+x} ]]; then
    awk -F "$columnsep" -v dateB="$bornuntil" '{if (FNR>1 && dateB>=$5) {print}}' $file
    exit
fi

if [[ ${editid+x} ]] && [[ ${editcolumn+x} ]] && [[ ${editvalue+x} ]]; then

    exit
fi

#ignore lines that start with #
#keep only column of last name
#sort alphabetically, print only discrete rows
if [[ ${lastnames+x} ]]; then
    grep "^[^#]" $file |  awk -F "$columnsep" '{print $2}' | sort | uniq
    exit
fi

#ignore lines that start with #
#keep only column of first name
#sort alphabetically, print only discrete rows
if [[ ${firstnames+x} ]]; then
    grep "^[^#]" $file |  awk -F "$columnsep" '{print $3}' | sort | uniq
    exit
fi

#ignore lines that start with #
#keep only last column (social media name)
#sort alphabetically
#group by social media name and count
# uniq -c needs output columns swap to match $name $count specification
if [[ ${socialmedia+x} ]]; then
    grep "^[^#]" $file |  awk -F "$columnsep" '{print $9}' | sort | uniq -c | awk '{ print $2 " " $1 }'
    exit
fi

# echo line that starts with given id | split with column separator and print correct columns with spacing
if [[ ${id+x} ]]; then
    awk "/^$id/" $file | awk -F "$columnsep" '{print $3 " " $2 " " $5}'
    exit
fi

#print lines that do not start with '#'
grep "^[^#]" $file