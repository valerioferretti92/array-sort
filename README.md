## Sorting algorithms with different languages for learning purposes

This repository hosts the implementation of array sort algorithms. Each folder contains the exact same program written with
different languages in order to be able to compare performances and for learning purposes. The program parses the command line
to determine the size of the array to be randomly initialized and what algorithm to apply to sort it.  

Currently, the implemented algorithms are the following:
- TrivialSort: O(n^2)
- MergeSort: O(nlog(n))

The same program is implemented in the following languages:
- C++
- NodeJs

Regardless of the languages choosen for implementation, the program accepts the following parameters:
- -a | --algorithm MergeSort | TrivialSort
- -s | --size SIZE_INT

### How to compile and run
- C++: 
```
cd c++
g++ Arrays.cpp -o Arrays
./Arrays -a MergeSort -s 100
```
- nodejs: 
```
cd nodejs
npm install
node arrays.js -a MergeSort -s 100
```
