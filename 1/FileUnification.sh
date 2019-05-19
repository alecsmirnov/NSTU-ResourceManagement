#!/bin/bash
IFS=$'\n'			# Split words starting with a new line

# Script input parameters
DIRECTORY="$1"		# Home directory
FILE_TYPE="$2"		# File type to unification

# Inpput validation codes
CORRECT=0			# ОК!
NO_ARG_ERR=1		# No input parameters specified
FILE_TYPE_ERR=2		# File type not specified
DIRECTORY_ERR=3		# Incorrectly specified directory


# Input parameter check function.
# Returns code that indicates the status of the check
function inputCheckStatus {
	local status=$CORRECT;

	if [ -z "$FILE_TYPE" ] && [ -z "$DIRECTORY" ]; then
		status=$NO_ARG_ERR
	else 
		if [ -z "$FILE_TYPE" ]; then
			status=$FILE_TYPE_ERR
		else 
			if [ ! -d "$DIRECTORY" ]; then
				status=$DIRECTORY_ERR
			fi
		fi
	fi

	echo "$status"
}

# Function of displaying errors on the screen by their code.
# The output is carried out in stderr
function printErrorByStatus {
	err_code="$1"

	>&2 echo -n "Error: "
	case $err_code in 
		$NO_ARG_ERR) >&2 echo "directory and filetype not specified!";;
		$FILE_TYPE_ERR) >&2 echo "filetype not specified!";;
		$DIRECTORY_ERR) >&2 echo "\"$DIRECTORY\" is not a directory!";;
	esac
}

# Function of search and print combined files
function fileSearch {
	cur_dir="$1"
	echo -n "\"$cur_dir\":"

	if [ -z "$(/bin/ls -A $cur_dir)" ]; then
   		echo " is empty folder!"
	else
		file_list="$(/usr/bin/find $cur_dir -maxdepth 1 -name '*.'$FILE_TYPE)"

		if [ -z "$file_list" ]; then
			echo " no files with \"$FILE_TYPE\" type!"
		else
			echo ""
			for file in $file_list; do
				echo -e -n "\tFile \"$(basename $file)\":"
				if [ -s "$file" ]; then 
					echo ""
					cat "$file" | sed 's/^/\t/'
					echo -e "\n\tEnd of \"$(basename $file)\" file"
				else
					echo " is empty!"
				fi
			done
		fi

		dir_list="$(/usr/bin/find $cur_dir -mindepth 1 -maxdepth 1 -type d)"
		for dir in $dir_list; do
			fileSearch "$dir"
		done
	fi 
}


# Main programm
input_status=$(inputCheckStatus)				# Getting inputs status

if [ "$input_status" -eq "$CORRECT" ]; then		# Status is correct
	fileSearch "$DIRECTORY"						# Start the main function
else 											# Error status
	$(printErrorByStatus "$input_status")		# Display the error 
fi