package main

import (
	"errors"
	"flag"
	"fmt"
	"math"
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
	} else if algorithmName == "BubbleSort" {
		algorithm = bubbleSort
	} else if algorithmName == "HeapSort" {
		algorithm = heapSort
	} else if algorithmName == "QuickSort" {
		algorithm = quickSort
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
	algorithmName := flag.String("algorithm", "",
		"TrivialSort | MergeSort | InsertionSort | BubbleSort")
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

// Triavial Sort: best / worst case teta(n^2) in place
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

// BubbleSort: best / worst case teta(n^2)
func bubbleSort(myArray []uint) {
	for i := 0; i < len(myArray)-1; i++ {
		for j := len(myArray) - 1; j > i; j-- {
			if myArray[j] < myArray[j-1] {
				myArray[j], myArray[j-1] = myArray[j-1], myArray[j]
			}
		}
	}
}

// Merge Sort: best / worst case teta(nlog(n)) not in place
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

// Insertion Sort: best case teta(n), worst case teta(n^2) in place
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

// HeapSort: best / worst case O(nlog(n)) in place
func heapSort(myArray []uint) {
	buildMaxHeap(myArray)
	heapSize := len(myArray) - 1
	for i := len(myArray) - 1; i > 0; i-- {
		myArray[0], myArray[i] = myArray[i], myArray[0]
		heapSize = heapSize - 1
		maxHeapify(myArray, 0, heapSize)
	}
}

func buildMaxHeap(myArray []uint) {
	for i := parent(len(myArray) - 1); i >= 0; i-- {
		maxHeapify(myArray, i, len(myArray)-1)
	}
}

func maxHeapify(myArray []uint, root int, heapSize int) {
	var l, r, largest int

	l = leftChild(root)
	r = rightChild(root)
	largest = root

	if l <= heapSize && myArray[l] > myArray[root] {
		largest = l
	}
	if r <= heapSize && myArray[r] > myArray[largest] {
		largest = r
	}
	if largest != root {
		myArray[root], myArray[largest] = myArray[largest], myArray[root]
		maxHeapify(myArray, largest, heapSize)
	}
}

func parent(i int) int {
	if i%2 == 0 {
		return i/2 - 1
	}
	return int(math.Floor(float64(i) / float64(2)))
}

func leftChild(i int) int {
	return i*2 + 1
}

func rightChild(i int) int {
	return i*2 + 2
}

// QuickSort: best / worst case teta(nlog(n)) in place
func quickSort(myArray []uint) {
	rand.Seed(time.Now().UnixNano())
	var alg func([]uint, int, int)
	alg = func(myArray []uint, p int, r int) {
		if p < r {
			q := rpartition(myArray, p, r)
			alg(myArray, p, q-1)
			alg(myArray, q+1, r)
		}
	}

	alg(myArray, 0, len(myArray)-1)
}

func rpartition(myArray []uint, p, r int) int {
	i := uint(rand.Int())%uint(r-p+1) + uint(p)
	myArray[i], myArray[r] = myArray[r], myArray[i]
	return partition(myArray, p, r)
}

func partition(myArray []uint, p, r int) int {
	x := myArray[r]
	i := p - 1
	for j := p; j < r; j++ {
		if myArray[j] <= x {
			i = i + 1
			myArray[i], myArray[j] = myArray[j], myArray[i]
		}
	}
	myArray[i+1], myArray[r] = myArray[r], myArray[i+1]
	return i + 1
}
