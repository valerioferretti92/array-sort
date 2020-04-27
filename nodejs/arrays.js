const argv = require('minimist')(process.argv.slice(2));
const randomInt = require('random-int');

const TRIVIAL_SORT = "TrivialSort";
const MERGE_SORT = "MergeSort";
const algorithms = [TRIVIAL_SORT,MERGE_SORT];

function main(){
  let parameters = parseCommandLineInput();
  let myArray = populateArray(parameters.size);

  let algorithm;
  let algorithmName;
  if(parameters.algorithm === TRIVIAL_SORT){
    algorithm = trivialSort;
    algorithmName = "Trivial Sort";
  }else if(parameters.algorithm === MERGE_SORT){
    algorithm = mergeSort;
    algorithmName = "Merge Sort";
  }

  let t0 = process.hrtime();
  myArray = algorithm(myArray);
  let t1 = process.hrtime();
  var executionTime = calculateExcutionTime(t1, t0);

  if(!checkArray(myArray)){
    console.log("ERROR: The array has not been properly sorted.");
    return;
  }

  console.log(`Algorithm: ${algorithmName}.`);
  console.log(`Problem size: ${parameters.size}.`);
  console.log(`Min value: ${myArray[0]}.`);
  console.log(`Max value: ${myArray[parameters.size - 1]}.`);
  console.log(`Execution Time: ${executionTime.seconds}s ${executionTime.milliseconds}ms ${executionTime.microseconds}us.`);
}

function mergeSort(myArray){
  return divide(myArray, 0, myArray.length - 1);
}

function divide(myArray, low, high){
  if(low == high) return myArray;
  let mid = Math.floor((low + high)/2);
  myArray = divide(myArray, low, mid);
  myArray = divide(myArray, mid + 1, high);
  myArray = merge(myArray, low, mid, mid + 1, high);
  return myArray;
}

function merge(myArray, left_low, left_high, right_low, right_high){
  let length = right_high - left_low + 1;
  let left = left_low;
  let right = right_low;
  let tempArray = [];
  let i;
  for(i = 0; i < length; i++){
    if(left > left_high)
      tempArray[i] = myArray[right++];
    else if (right > right_high)
      tempArray[i] = myArray[left++];
    else if(myArray[left] <= myArray[right])
      tempArray[i] = myArray[left++];
    else if(myArray[right] <= myArray[left])
      tempArray[i] = myArray[right++];
  }
  for(i = 0; i < length; i++){
    myArray[left_low++] = tempArray[i];
  }

  return myArray;
}

function trivialSort(myArray){
  for(i = 0; i < myArray.length; i++){
    indexMin = 0;
    min = Number.MAX_SAFE_INTEGER;
    temp = 0;
    for(j = i; j < myArray.length; j++){
      if(myArray[j] < min){
        min = myArray[j];
        indexMin = j;
      }
    }
    temp = myArray[i];
    myArray[i] = min;
    myArray[indexMin] = temp;
  }
  return myArray;
}

function checkArray(myArray){
  let minInteger = 0;
  let isOrdered = true;
  myArray.every((element) => {
    if(minInteger > element){
      isOrdered = false;
      return false;
    }else{
      minInteger = element;
      return true;
    }
  });
  return isOrdered;
}

function populateArray(dimension){
  let myArray = [];
  for(i = 0; i < dimension; i++){
    myArray.push(randomInt(0, Number.MAX_SAFE_INTEGER));
  }
  return myArray;
}

function calculateExcutionTime(t1, t0){
  let timestamp1 = t1[0] * Math.pow(10,9) + t1[1];
  let timestamp0 = t0[0] * Math.pow(10,9) + t0[1];
  let executionTime = timestamp1 - timestamp0;
  let ns = executionTime % Math.pow(10,3);
  let usByNs = (executionTime - ns) % Math.pow(10,6);
  let msByNs = (executionTime - ns - usByNs) % Math.pow(10,9);
  let sByNs = executionTime - ns - usByNs - msByNs;
  return {
    seconds: sByNs / Math.pow(10,9),
    milliseconds: msByNs / Math.pow(10,6),
    microseconds: usByNs / Math.pow(10,3),
  }
}

function parseCommandLineInput(){
  if(argv.help || argv.h){
    printHelp();
  }
  if((!argv.algorithm && !argv.a) || (!argv.size && !argv.s)){
    printHelp();
  }
  if(!algorithms.includes(argv.algorithm) && !algorithms.includes(argv.a)){
    console.log("ERROR: Invalid algorithm specified");
    printHelp();
  }
  return {
    algorithm: argv.algorithm || argv.a,
    size: argv.size || argv.s
  }
}

function printHelp(){
  console.log("\nUsage: node arrays.js <parameters>");
  console.log("\nParameters:");
  console.log("-h|--help, -a|--algorith TrivialSort|MergeSort, -s|--size\n");
  process.exit(1);
}

main();
