g++ $(find ./src | grep -E ".*.cpp$" | while read line; do printf "$line "; done) -lpthread -lallegro -lallegro_image 
