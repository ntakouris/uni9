!#/bin/bash

#No args -> AM
if [ $# -eq 0 ]
  then
    echo "1054332-1054432"
    exit
fi

#parameter parsing
while getopts ":f:id:-firstnames:-lastnames:-born-since:-born-until:-socialmedia:-edit:" opt
   do
     case $opt in
        f ) file=$OPTARG;;
        id ) id=$OPTARG;;
        -firstnames ) firstnames=true;;
        -lastnames ) lastnames=true;;
        -born-since ) bornsince=$OPTARG;;
        -born-until ) bornultil=$OPTARG;;
        -socialmedia ) socialmedia=$OPTARG;;
        #figure how to parse <column> <value>
        -edit ) editid=$OPTARG;;
     esac
done

# id|lastName|firstName|gender|birthday|joinDate|IP|browserUsed|socialmedia

#if id -> $first $last $date
if [[ ! ${id+x} ]]; then
    # echo line that starts with given id
    line=$(awk '/^$id/p' $file) 
    #split into array by '|' delimeter
    cols=(${line//|/ })
    echo "${cols[2]} ${cols[1]} ${cols[5]}"
    exit
fi

#if lastnames/firstnames -> discrete names alphabetically sorted line-line
if [[ ! ${firstnames+x} ]]; then
    # replace first *|*| with '' and last |*|*|*|*|*|* to only print firstname
    sed -n -e 's/^.*|*|: //p' $file | sed -n -e 's/$.|*|*|*|*|*|*: //p' | sort | uniq
    exit
fi

if [[ ! ${lastnames+x} ]]; then

    exit
fi

# only lines that match
if [[ ! ${bornsince+x} ]] && [[ ! ${bornuntil+x} ]]; then
    # sed-out date, keep
    exit
fi

if [[ ! ${socialmedia+x} ]]; then
    sed -n -e 's/^.*|: //p' $file | sort | uniq -c | awk '{ print $2, $1 }' # yeet
    exit
fi

if [[ ! ${editid+x} ]] && [[ ! ${editcolumn+x} ]] && [[ ! ${editvalue+x} ]]; then

    exit
fi

#if only f -> all contents except lines that start with #
# "print lines that do not start with '#' "
cat $file | grep "^[^#]"