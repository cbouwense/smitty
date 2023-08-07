file_name=$1
ls *.c *.h | entr sh -c "./run.sh ${file_name}" 
