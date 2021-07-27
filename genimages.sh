if [[ ! -d xpm ]]
then
	echo "xpm folder not found!"
else
	pushd xpm
	> ../images.h
	for dir in `find . -type d`
	do
		if [[ $dir != "." ]]
		then
			echo "Directory:" $dir
			echo "#ifdef "`echo $dir | sed 's/\.\///g'` >> ../images.h
			for file in $dir/*.xpm
			do
				str=`cat $file | grep _xpm`
				str=${str#*\*}
				str=${str%[*}
				if grep -q $str ../*.cpp
				then
					cat $file | sed 's/char/const char/g' >> ../images.h
					echo "Progress:" $file
				fi
			done
			echo "#endif" >> ../images.h
		fi
	done
	popd
	echo
	echo "Generating images.h done!"
fi