package main

import (
	"errors"
	"flag"
	"fmt"
	"math/rand"
	"time"
)

const maxUint = ^uint(0)

func main() {
	algorithmName, arraySize, err := parseCmdlArgs()
	if err != nil {
		fmt.Println(err.Error())
		flag.Usage()
		return
	}

	//Array initialisation
	myArray := make([]uint, arraySize, arraySize)
	initialize(myArray)

	//Algorithm execution
	var algorithm func([]uint)
	if algorithmName == "TrivialSort" {
		algorithm = trivialSort
	} else if algorithmName == "MergeSort" {
		algorithm = mergeSort
	} else if algorithmName == "InsertionSort" {
		algorithm = insertionSort
	} else {
		fmt.Println("ERROR: invalid algorithm specified")
		return
	}
	t1 := time.Now()
	algorithm(myArray)
	d := time.Since(t1)
	if !checkArray(myArray) {
		fmt.Println("ERROR: The array has not been properly sorted")
		return
	}

	//Printing results to stdout
	fmt.Printf("Algorithm: %s\n", algorithmName)
	fmt.Printf("Problem Size: %d\n", arraySize)
	fmt.Printf("Min Value: %d\n", myArray[0])
	fmt.Printf("Max Value: %d\n", myArray[len(myArray)-1])
	s := int(d.Seconds())
	ms := d.Milliseconds() % 1000
	us := d.Microseconds() % 1000
	fmt.Printf("Execution Time: %vs %vms %vus\n", s, ms, us)
}

func checkArray(myArray []uint) bool {
	for i := 1; i < len(myArray); i++ {
		if myArray[i] < myArray[i-1] {
			return false
		}
	}
	return true
}

func initialize(myArray []uint) {
	rand.Seed(time.Now().UnixNano())
	for i := range myArray {
		myArray[i] = uint(rand.Int())
	}
	return
}

func parseCmdlArgs() (string, int, error) {
	algorithmName := flag.String("algorithm", "", "TrivialSort | MergeSort | InsertionSort")
	arraySize := flag.Int("size", 0, "Size of the array to be sorted")
	flag.Parse()

	if *algorithmName == "" {
		return *algorithmName, *arraySize, errors.New("flag is mandatory: -algorithm")
	}
	if *arraySize == 0 {
		return *algorithmName, *arraySize, errors.New("falg is mandatory: -size")
	}

	return *algorithmName, *arraySize, nil
}

// Triavial Sort: O(n^2)
func trivialSort(myArray []uint) {
	for i, temp := range myArray {
		var indexMin int
		var min uint = maxUint
		for j := i; j < len(myArray); j++ {
			if myArray[j] < min {
				min = myArray[j]
				indexMin = j
			}
		}
		myArray[i] = min
		myArray[indexMin] = temp
	}
}

// Merge Sort: O(nlog(n))
func mergeSort(myArray []uint) {
	divide(myArray, 0, len(myArray)-1)
}

func divide(myArray []uint, low, high int) {
	if low == high {
		return
	}
	mid := (low + high) / 2
	divide(myArray, low, mid)
	divide(myArray, mid+1, high)
	merge(myArray, low, mid, mid+1, high)
}

func merge(myArray []uint, leftLow, leftHigh, rightLow, rightHigh int) {
	length := rightHigh - leftLow + 1
	left := leftLow
	right := rightLow
	tempArray := make([]uint, length, length)
	for i := range tempArray {
		if left > leftHigh {
			tempArray[i] = myArray[right]
			right++
		} else if right > rightHigh {
			tempArray[i] = myArray[left]
			left++
		} else if myArray[left] <= myArray[right] {
			tempArray[i] = myArray[left]
			left++
		} else if myArray[right] <= myArray[left] {
			tempArray[i] = myArray[right]
			right++
		}
	}

	for i := range tempArray {
		myArray[leftLow] = tempArray[i]
		leftLow++
	}
}

// Insertion Sort: O(n^2)
func insertionSort(myArray []uint) {
	for i := 1; i < len(myArray); i++ {
		val := myArray[i]
		var j int
		for j = i - 1; j >= 0 && myArray[j] > val; j-- {
			myArray[j+1] = myArray[j]
		}
		myArray[j+1] = val
	}
}
