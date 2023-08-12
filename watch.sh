file_name=$1
ls examples/* src/* | entr sh -c "./run.sh ${file_name}"
