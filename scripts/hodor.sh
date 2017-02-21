#!/bin/bash
rm -rf test_range/*
./parser $@
if [[ $? -eq 0 ]]; then
echo "Compiling $@.cpp..."
mv $@.cpp test_range
g++ -o test_range/sim -Ofast -O4 test_range/$@.cpp runtime/HD*.cpp -framework Accelerate &> test_range/error.log
if [[ $? -ne 0 ]]; then
  echo "Compile error, see error.log" 
else
  echo "Running ./sim"
  cp gify.sh test_range
  cd test_range
  ./sim 
 # ./gify.sh
fi
fi
