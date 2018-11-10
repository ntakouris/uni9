#!/bin/bash

#No args -> AM
if [ $# -eq 0 ]
  then
    echo "1054332-1054432"
    exit
fi

#parameter parsing
while getopts ":f:i:s:" opt
   do
     case $opt in
        f ) file=$OPTARG;;
        i ) id=$OPTARG;;
        s ) socialmedia='true';;
     esac
done
socialmedia='asdf'
echo $socialmedia

# id|lastName|firstName|gender|birthday|joinDate|IP|browserUsed|socialmedia

columnsep='\t'


# only lines that match
if [[ ! ${bornsince+x} ]] && [[ ! ${bornuntil+x} ]]; then
    # sed-out date, keep
    exit
fi

if [[ ! ${editid+x} ]] && [[ ! ${editcolumn+x} ]] && [[ ! ${editvalue+x} ]]; then

    exit
fi

#ignore lines that start with #
#keep only column of last name
#sort alphabetically, print only discrete rows
if [[ ! ${lastnames+x} ]]; then
    grep "^[^#]" $file |  awk -F "$columnsep" '{print $2}' | sort | uniq
    exit
fi

#ignore lines that start with #
#keep only column of first name
#sort alphabetically, print only discrete rows
if [[ ! ${firstnames+x} ]]; then
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
    awk "/^$id/" $file | awk -F "$columnsep" '{print $3 " " $2 " " $6}'
    exit
fi

#print lines that do not start with '#'
grep "^[^#]" $file