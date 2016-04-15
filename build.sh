clear
./bin/Debug/S3L programs/code.slll
if [ $? == 0 ]
	then llc check.ir
	if [ $? == 0 ]
		then gcc check.ir.s
		if [ $? == 0 ]
			then echo "finished"
			./a.out
			echo $?
		fi
	fi
fi