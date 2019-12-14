export CPATH=./Dependencies
local_files=$(find ./src | grep -E ".*.cpp$" | while read line; do printf "$line "; done) 
dependencies=$(find ./Dependencies | grep -E ".*.cpp$" | grep -vE ".*main.cpp" | while read line; do printf "$line "; done)

echo "Local Files: $(echo $local_files | tr ' ' '\n')"
echo "Dependencies: $(echo $dependencies | tr ' ' '\n')"

g++ $local_files $dependencies -lpthread -lallegro -lallegro_image