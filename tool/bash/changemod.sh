#!/bin/bash
# author : Fu Fenghai
# date : 2019/
# brief : 
#!/bin/sh  
list_alldir(){  
	for file2 in `ls -a $1`  
	do  
		if [ x"$file2" != x"." -a x"$file2" != x".."  ]; then  
			if [ -d "$1/$file2"  ];then  
				chmod 0777 "$1/$file2"
				list_alldir "$1/$file2"  
			elif [ -f "$1/$file2" ]; then
				if [ "${file2##*.}"x = "h"x ] || [ "${file2##*.}"x = "c"x ]; then
					chmod 0666 "$1/$file2"
				fi
			fi
		fi  
	done  
}  

list_alldir "$1"
