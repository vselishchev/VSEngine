@echo OFF

mkdir Solution
cd Solution
set BuildType=Debug

echo "Loading dependencies"
conan install .. -s build_type=%BuildType%
echo "Generating solution"
cmake -DCMAKE_BUILD_TYPE=%BuildType% ..
PAUSE