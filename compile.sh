export CPATH=./Dependencies
local_files=$(find ./src | grep -E ".*.cpp$" | while read line; do printf "$line "; done) 
dependencies=$(find ./Dependencies/AllegroCPPWrappers/src | grep -E ".*.cpp$" | while read line; do printf "$line "; done) 

g++ $local_files $dependencies -lpthread -lallegro -lallegro_image