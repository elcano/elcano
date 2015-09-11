
// -------------------------------------------------------------------------------------------------
//Sorting function (Array, clock postion, 0, numer of samples)

//Pass by Reference
// void quicksort(int (&array)[RANGE_DATA_SIZE][SAMPLE_DATA_SIZE], int clockPos, int start, int end)

void quicksort(int array[][SAMPLE_DATA_SIZE], int clockPos, int start, int end)
{
    if (end - start > 1)
    {
        int up = start, down = end;
        int pivot = array[clockPos][(start + end) / 2];

        while (up <= down)  //Loop until they cross
        {
            
            while (array[clockPos][up] < pivot) { up++; }     // Walks the up right
            while (array[clockPos][down] > pivot) { down--; } // Walks the down left
            
            if (up <= down)
            {
                //Swap
                int temp = array[clockPos][up];
                array[clockPos][up] = array[clockPos][down];
                array[clockPos][down] = temp;
                
                up++;
                down--;
            }
        }
        quicksort(array, clockPos, start, down);
        quicksort(array, clockPos, up, end);
    }
}


// -------------------------------------------------------------------------------------------------
//Mode function, returning the mode or median.
// - Finds the MODE in the given data set
// - If there isn't a MODE, the MEDIAN is returned
// - if there are two or more MODES (bimodal), the MEDIAN is returned
// - Equal and less than Zero values are not included when finding the MODE or MEDIAN
// -------------------------------------------------------------------------------------------------
int findMode(int array[][SAMPLE_DATA_SIZE], int clockPos, int arraySize)
{
    int modeValue = 0;    //Known Mode/Medium Value
    bool bimodal = false; //Bimodal Distribution
    int index = 0;        //Sample Index
    int shiftSize = 0;    //Shift Array Size
    
    int count = 0,    prevCount = 0;
    int maxCount = 0, prevMaxCount = 0;
    
    //Checks the value for anything equal and less than zero
    while (array[clockPos][index] <= 0) { index++; }
    shiftSize = index;
    
    //checks if you reached the end of the array
    while (index < arraySize)
    {
        prevCount = count;  //copy the previous count value
        count = 0;          //reset the count value
        
        //counts the number of duplications values
        while (array[clockPos][index] == array[clockPos][index + 1])
        {
            count++;
            index++;
        }
        
        if (count > prevCount && count >= maxCount) //Found the MODE. Higher than max and prev
        {
            modeValue = array[clockPos][index]; //set the new MODE
            prevMaxCount = maxCount;
            maxCount = count;
            bimodal = false;
        }
        
        //Moves to the next sample if no MODE was found
        if (count == 0) { index++; }
        //If the sample dataset has 2 or more MODES.
        else if (maxCount == prevMaxCount) { bimodal = true; }
        
        //Return the MEDIAN if there is no MODE or there are more than two MODES.
        if (modeValue == 0 || bimodal)
        {
            modeValue = array[clockPos][((arraySize + shiftSize ) / 2)];
            bimodal = true;
        }
        else { bimodal = false; }
    }
    if (VERBOSE_DEBUG && DEBUG_MODE)
    {
        Serial.print((bimodal ? "MEAN\t": "MODE\t"));
    }
    return modeValue;
}