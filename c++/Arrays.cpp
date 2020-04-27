#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <climits>
#include <string>
#include "cxxopts-2.1.2/include/cxxopts.hpp"
using namespace std;
using namespace std::chrono;

void initializeArray(unsigned int myArray[]);
bool checkArray(unsigned int myArray[]);
void trivialSort(unsigned int myArray[]);
void divide(unsigned int myArray[], int low, int high);
void merge(unsigned int myArray[], int left_low, int left_high, int right_low, int right_high);
void mergeSort(unsigned int myArray[]);

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
      ("a,algorithm", "sorting algorithm (TrivialSort|MergeSort)", cxxopts::value<string>())
      ("s,size", "array size", cxxopts::value<int>())
      ;
    auto result = options.parse(argc, argv);

    if (result.count("help")){
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
  }else{
    cout << "No valid algorithm specified." << endl;
    exit(0);
  }
  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  (*algorithm)(myArray);
  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  bool isOrdered = checkArray(myArray);
  if(!isOrdered) cout << "An error occured" << endl;

  //Printing results to stdout
  cout << "Algorithm: " << algorithmName << endl;
  cout << "Problem Size: " << arraySize << endl;
  cout << "Min Value: " << myArray[0] << endl;
  cout << "Max Value: " << myArray[arraySize - 1] << endl;
  auto s = duration_cast<seconds>((t2 - t1) % minutes(1)).count();
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

void trivialSort(unsigned int myArray[]){
  for(int i = 0; i < arraySize; i++){
    int indexMin = 0;
    unsigned int min = UINT_MAX;
    int temp = 0;
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
