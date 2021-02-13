#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <climits>
#include <string>
#include <math.h>
#include "cxxopts-2.1.2/include/cxxopts.hpp"
using namespace std;
using namespace std::chrono;

void initializeArray(unsigned int myArray[]);
bool checkArray(unsigned int myArray[]);
void trivialSort(unsigned int myArray[]);
void bubbleSort(unsigned int myArray[]);
void insertionSort(unsigned int myArray[]);
void divide(unsigned int myArray[], int low, int high);
void merge(unsigned int myArray[], int left_low, int left_high, int right_low, int right_high);
void mergeSort(unsigned int myArray[]);
void heapSort(unsigned int myArray[]);
void buildMaxHeap(unsigned int myArray[]);
void maxHeapify(unsigned int myArray[], int root, int heapSize);
int parent(int i);
int leftChild(int i);
int rightChild(int i);
void quickSort(unsigned int myArray[]);
unsigned int partition(unsigned int myArray[], int p, int r);
unsigned int rpartition(unsigned int myArray[], int p, int r);

unsigned int arraySize;
string algorithmName;
void (*algorithm)(unsigned int[]);
unsigned int* myArray;

int main(int argc, char** argv){
  cxxopts::Options options(argv[0], "Implementation and comparison of sorting algorithms");

  //Command lie parsing code
  try{
    options.add_options()
      ("h,help","print help")
      ("a,algorithm", 
        "sorting algorithm (TrivialSort | MergeSort | InsertionSort | BubbleSort)", 
        cxxopts::value<string>())
      ("s,size", "array size", cxxopts::value<int>())
      ;
    auto result = options.parse(argc, argv);

    if (result.count("h") || !result.count("a") || !result.count("s")){
      cout << options.help({"", "Group"}) << std::endl;
      exit(0);
    }

    algorithmName = result["a"].as<string>();
    arraySize = result["s"].as<int>();
  }catch(const cxxopts::OptionException& e){
    cout << "error parsing options: " << e.what() << endl;
    cout << options.help({"", "Group"}) << std::endl;
    exit(-1);
  }

  //Vector initilization
  myArray = new unsigned int[arraySize];
  initializeArray(myArray);

  //Algorithm execution
  if(algorithmName.compare("TrivialSort") == 0){
    algorithm = trivialSort;
    algorithmName = "Trivial Sort";
  }else if(algorithmName.compare("MergeSort") == 0){
    algorithm = mergeSort;
    algorithmName = "Merge Sort";
  }else if(algorithmName.compare("InsertionSort") == 0){
    algorithm = insertionSort;
    algorithmName = "Insertion Sort";
  }else if(algorithmName.compare("BubbleSort") == 0){
    algorithm = bubbleSort;
    algorithmName = "Bubble Sort";
  }else if(algorithmName.compare("HeapSort") == 0){
    algorithm = heapSort;
    algorithmName = "Heap Sort";
  }else if(algorithmName.compare("QuickSort") == 0){
    algorithm = quickSort;
    algorithmName = "Quick Sort";
  }else{
    cout << "ERROR: No valid algorithm specified" << endl;
    exit(-1);
  }
  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  (*algorithm)(myArray);
  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  bool isOrdered = checkArray(myArray);
  if(!isOrdered) {
    cout << "ERROR: The array has not been properly sorted" << endl;
    exit(-1);
  }

  //Printing results to stdout
  cout << "Algorithm: " << algorithmName << endl;
  cout << "Problem Size: " << arraySize << endl;
  cout << "Min Value: " << myArray[0] << endl;
  cout << "Max Value: " << myArray[arraySize - 1] << endl;
  auto s = duration_cast<seconds>((t2 - t1)).count();
  auto ms = duration_cast<milliseconds>((t2 - t1) % seconds(1)).count();
  auto us = duration_cast<microseconds>((t2 - t1) % milliseconds(1)).count();
  cout << "Execution Time: " << s << "s " << ms << "ms " << us << "us" << endl;

  //Cleaning out memory
  delete [] myArray;
}

void initializeArray(unsigned int myArray[]){
  srand(time(NULL));
  for(int i = 0; i < arraySize; i++){
    myArray[i] = rand();
  }
}

bool checkArray(unsigned int myArray[]){
  for(int i = 1; i < arraySize; i++){
    if(myArray[i] < myArray[i - 1]){
      return false;
    }
  }
  return true;
}

// BubbleSort: best / worst case teta(n^2) in place
void bubbleSort(unsigned int myArray[]) {
	for (int i = 0; i < arraySize-1; i++) {
		for (int j = arraySize - 1; j > i; j--) {
			if (myArray[j] < myArray[j-1]) {
        unsigned int tmp = myArray[j];
				myArray[j] = myArray[j-1];
        myArray[j-1] = tmp;
			}
		}
	}
}

// MergeSort: best / worst case teta(nlog(n)) not in place
void mergeSort(unsigned int myArray[]){
  divide(myArray, 0, arraySize - 1);
}

void divide(unsigned int myArray[], int low, int high){
  if(low == high) return;
  int mid = (low + high)/2;
  divide(myArray, low, mid);
  divide(myArray, mid + 1, high);
  merge(myArray, low, mid, mid + 1, high);
}

void merge(unsigned int myArray[], int left_low, int left_high, int right_low, int right_high){
  int length = right_high - left_low + 1;
  int left = left_low;
  int right = right_low;
  int* tempArray = new int[length];
  for(int i = 0; i < length; i++){
    if(left > left_high)
      tempArray[i] = myArray[right++];
    else if (right > right_high)
      tempArray[i] = myArray[left++];
    else if(myArray[left] <= myArray[right])
      tempArray[i] = myArray[left++];
    else if(myArray[right] <= myArray[left])
      tempArray[i] = myArray[right++];
  }

  for(int i = 0; i < length; i++){
    myArray[left_low++] = tempArray[i];
  }

  delete [] tempArray;
}

// TrivialSort: best / worst case teta(n^2) in place
void trivialSort(unsigned int myArray[]){
  for(int i = 0; i < arraySize; i++){
    int indexMin = 0, temp = 0, min = UINT_MAX;
    for(int j = i; j < arraySize; j++){
      if(myArray[j] < min){
        min = myArray[j];
        indexMin = j;
      }
    }
    temp = myArray[i];
    myArray[i] = min;
    myArray[indexMin] = temp;
  }
}

// InsertionSort: best case teta(n), worst case O(n^2) in place
void insertionSort(unsigned int myArray[]){
	for (int i = 1; i < arraySize; i++) {
		unsigned int val = myArray[i];
    int j = 0;
		for (j = i - 1; j >= 0 && myArray[j] > val; j--) {
			myArray[j+1] = myArray[j];
		}
		myArray[j+1] = val;
	}
}

// HeapSort: best / worst case O(nlog(n)) in place
void heapSort(unsigned int myArray[]) {
	buildMaxHeap(myArray);
	int heapSize = arraySize - 1;
	for (int i = arraySize - 1; i > 0; i--) {
    int tmp = myArray[0];
    myArray[0] = myArray[i];
		myArray[i] = tmp;
		heapSize = heapSize - 1;
		maxHeapify(myArray, 0, heapSize);
	}
}

void buildMaxHeap(unsigned int myArray[]) {
	for (int i = parent(arraySize - 1); i >= 0; i--) {
		maxHeapify(myArray, i, arraySize-1);
	}
}

void maxHeapify(unsigned int myArray[], int root, int heapSize) {
	int l = 0, r = 0, largest = 0;

	l = leftChild(root);
	r = rightChild(root);
	largest = root;

	if (l <= heapSize && myArray[l] > myArray[root]) {
		largest = l;
	}
	if (r <= heapSize && myArray[r] > myArray[largest]) {
		largest = r;
	}
	if (largest != root) {
    int tmp = myArray[root];
    myArray[root] = myArray[largest];
		myArray[largest] = tmp;
		maxHeapify(myArray, largest, heapSize);
	}
}

int parent(int i) {
  return (i%2 == 0) ? i/2 - 1 : (int) floor(i / (float) 2);
}

int leftChild(int i) {
	return i*2 + 1;
}

int rightChild(int i) {
	return i*2 + 2;
}

// QuickSort: best / worst case teta(nlog(n)) in place
void quickSort(unsigned int myArray[]) {
  srand(time(NULL));
  function<void(unsigned int[], int, int)> alg;
	alg = [&alg](unsigned int myArray[], int p, int r) {
		if (p < r) {
			unsigned int q = rpartition(myArray, p, r);
			alg(myArray, p, q-1);
			alg(myArray, q+1, r);
		}
	};

	alg(myArray, 0, arraySize-1);
}

unsigned int rpartition(unsigned int myArray[], int p, int r) {
	int i = rand()%uint(r-p+1) + uint(p);
  unsigned int tmp = myArray[i];
  myArray[i] = myArray[r];
  myArray[r] = tmp;
	return partition(myArray, p, r);
}

unsigned int partition(unsigned int myArray[], int p, int r) {
	unsigned int x = myArray[r];
	int i = p - 1;
	for (int j = p; j < r; j++) {
		if (myArray[j] <= x) {
			i = i + 1;
			unsigned int tmp = myArray[i];
      myArray[i] = myArray[j];
      myArray[j] = tmp;
		}
	}
	unsigned int tmp = myArray[i+1];
  myArray[i+1] = myArray[r];
  myArray[r] = tmp;
	return i + 1;
}