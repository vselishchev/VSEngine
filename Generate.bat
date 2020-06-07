@echo OFF

mkdir Output
cd Output
set BuildType=Debug

echo "Loading dependencies"
conan install .. -s build_type=%BuildType%
echo "Generating solution"
cmake -DCMAKE_BUILD_TYPE=%BuildType% ..
PAUSE