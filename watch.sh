file_name=$1
ls src/* | entr sh -c "./run.sh ${file_name}" 
