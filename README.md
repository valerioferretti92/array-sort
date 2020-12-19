## Sorting algorithms with different languages for learning purposes

This repository hosts the implementation of array sort algorithms. Each folder contains the exact same program written with different languages in order to be able to compare performances and for learning purposes. The program parses the command line to determine the size of the array to be randomly initialized and what algorithm to apply to sort it.  

Currently, the implemented algorithms are the following:
- TrivialSort: best / worst case Θ(n^2)
- MergeSort: best / worst case Θ(nlog(n))
- InsertionSort: best case Θ(n), worst case Θ(n^2)
- BubbleSort: best / worst case Θ(n^2)

The same program is implemented in the following languages:
- C++
- NodeJs
- Java
- GO

Regardless of the languages choosen for implementation, the program accepts the following parameters:
- -a | --algorithm MergeSort | TrivialSort | InsertionSort | BubbleSort
- -s | --size SIZE_INT

### How to compile and run
- C++: 
```
cd c++
g++ Arrays.cpp -o Arrays
./Arrays -a MergeSort -s 100
```
- NodeJs: 
```
cd nodejs
npm install
node Arrays.js -a MergeSort -s 100
```
- Java: 
```
cd java
mvn clean package
./java/target/appassembler/bin/Arrays -a MergeSort -s 100
```
- GO (No short parameters provided): 
```
cd go/Arrays
go build
./Arrays -algorithm MergeSort -size 100
```
