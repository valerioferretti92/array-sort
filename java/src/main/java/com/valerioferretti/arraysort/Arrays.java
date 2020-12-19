package com.valerioferretti.arraysort;

import picocli.CommandLine;
import picocli.CommandLine.Command;
import picocli.CommandLine.Option;

import java.time.Duration;
import java.time.Instant;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.concurrent.Callable;
import java.util.function.Function;

import static java.lang.System.exit;

@Command(name = "Arrays")
public class Arrays implements Callable<Integer> {

  @Option(names = {"-h", "--help"}, usageHelp = true, description = "Display this help message")
  private boolean help;
  
  @Option(names = {"-a", "--algorithm"}, required = true, 
          description = "TrivialSort | MergeSort | InsertionSort | BubbleSort")
  private static String algorithmName;
  
  @Option(names = {"-s", "--size"}, required = true, description = "Size of array to be sorted")
  private static Integer arraySize;
  
  public static void main(String... args) {
      exit(new CommandLine(new Arrays()).execute(args));
  }
  
  @Override
  public Integer call() {
    //Array initialization
    List<Integer> myArray = new ArrayList<>(arraySize);
    initializeArray(myArray);
    //Algorithm execution
    Function<List<Integer>, List<Integer>> algorithm;
    if (algorithmName.equals("TrivialSort")) {
        algorithm = this::trivialSort;
    } else if (algorithmName.equals("MergeSort")) {
        algorithm = this::mergeSort;
    } else if (algorithmName.equals("InsertionSort")) {
        algorithm = this::insertionSort;
    } else if (algorithmName.equals("BubbleSort")) {
        algorithm = this::bubbleSort;
    } else {
        System.out.println("ERROR: No valid algorithm specified.");
        return -1;
    }
    Instant t1 = Instant.now();
    algorithm.apply(myArray);
    Instant t2 = Instant.now();
    if(!checkArray(myArray)) {
        System.out.println("ERROR: The array has not been properly sorted");
        return -1;
    }
    //Printing results to stdout
    System.out.println("Algorithm: " + algorithmName);
    System.out.println("Problem Size: " + arraySize);
    System.out.println("Min Value: " + myArray.get(0));
    System.out.println("Max Value: " + myArray.get(myArray.size() - 1));
    long s = Duration.between(t1, t2).toSeconds();
    long ms = Duration.between(t1, t2).minusSeconds(s).toMillis();
    long us = Duration.between(t1, t2).minusSeconds(s).minusMillis(ms).toNanos() / 1000;
    System.out.println("Execution Time: " + s + "s " + ms + "ms " + us + "us");
    return 0;
  }
  
  private boolean checkArray(List<Integer> myArray){
    for(int i = 1; i < arraySize; i++){
        if(myArray.get(i) < myArray.get(i - 1)){
            return false;
        }
    }
    return true;
  }
  
  private void initializeArray(List<Integer> myArray) {
    Random random = new Random();
    for (int i = 0; i < arraySize; i++) {
        myArray.add(random.nextInt(Integer.MAX_VALUE));
    }
  }

  // BubbleSort: best / worst case teta(n^2)
  private List<Integer> bubbleSort(List<Integer> myArray) {
    for (int i = 0; i < arraySize-1; i++) {
      for (int j = arraySize - 1; j > i; j--) {
        if (myArray.get(j) < myArray.get(j-1)) {
          Integer tmp = myArray.get(j);
          myArray.set(j, myArray.get(j-1));
          myArray.set(j-1, tmp);
        }
      }
    }
    return myArray;
  }

  // TrivialSort: best / worst case teta(n^2)
  private List<Integer> trivialSort(List<Integer> myArray) {
    for(int i = 0; i < arraySize; i++){
      int indexMin = 0, temp = 0, min = Integer.MAX_VALUE;
      for(int j = i; j < arraySize; j++){
        if(myArray.get(j) < min){
          min = myArray.get(j);
          indexMin = j;
        }
      }
      temp = myArray.get(i);
      myArray.set(i, min);
      myArray.set(indexMin, temp);
    }
    return myArray;
  }
  
  // MergeSort: best / worst case teta(nlog(n))
  private List<Integer> mergeSort(List<Integer> myArray){
    divide(myArray, 0, arraySize - 1);
    return myArray;
  }
  
  private void divide(List<Integer> myArray, int low, int high){
    if(low == high) return;
    int mid = (low + high)/2;
    divide(myArray, low, mid);
    divide(myArray, mid + 1, high);
    merge(myArray, low, mid, mid + 1, high);
  }
  
  private void merge(List<Integer> myArray, int left_low, int left_high, int right_low, int right_high){
    int length = right_high - left_low + 1;
    int left = left_low;
    int right = right_low;
    List<Integer> tempArray = new ArrayList<Integer>(length);
    for(int i = 0; i < length; i++){
      if (left > left_high) {
        tempArray.add(myArray.get(right++));
      } else if (right > right_high) {
        tempArray.add(myArray.get(left++));
      } else if (myArray.get(left) <= myArray.get(right)) {
        tempArray.add(myArray.get(left++));
      } else if (myArray.get(right) <= myArray.get(left))
        tempArray.add(myArray.get(right++));
    }
    for(int i = 0; i < length; i++){
      myArray.set(left_low++, tempArray.get(i));
    }
  }
  
  // InsertionSort: best case teta(n), worst case teta(n^2)
  List<Integer> insertionSort(List<Integer> myArray){
  	for (int i = 1; i < arraySize; i++) {
      int val = myArray.get(i);
      int j = 0;
  		for (j = i - 1; j >= 0 && myArray.get(j) > val; j--) {
  			myArray.set(j+1, myArray.get(j));
  		}
  		myArray.set(j+1, val);
    }
    return myArray;
  }
}
  