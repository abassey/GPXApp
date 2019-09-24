#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "TestHarness.h"
#include "ListTestCases.h"

#define TESTS 11
#define DEBUG 0
#define OUT stdout

static int testNo;
static testRec * * testRecords;
static int studentScore;  //globals  required to handle segfaults gracefully
static void addTestResult(testRec* tmpRec){
    testRecords[testNo-1] = tmpRec;
    studentScore = studentScore + getScore(testRecords[testNo-1]);
    testNo++;
}

static float calcGrade(void){
    float weights[] = {	10,10,10,10,10,10,10,10,10,10,10 };
    float totalScore = 0;
    int i = 0;
    for (i = 0; i < TESTS; i++){
        totalScore += weights[i]*(float)getScore(testRecords[i]);
               
    }
    return totalScore;
}

int main(void)
{
    studentScore = 0;
    testNo = 1;
    testRec* tmpRec = NULL;

    //record keeping array
    testRecords =  malloc(sizeof(testRec *)* TESTS);
    
    if(DEBUG) fprintf(OUT, "************** Testing Details ********************\n\n");
    
    //Create list
    if (DEBUG) fprintf(OUT,"Testing list creation\n");
    tmpRec = initListTest(testNo);
    addTestResult(tmpRec);
    
    //Node Creation
    if (DEBUG) fprintf(OUT, "Testing Node Creation\n");
    tmpRec = initNodeTest(testNo);
    addTestResult(tmpRec);
    
    //Testing getLength
    if (DEBUG) fprintf(OUT, "Testing getLength\n");
    tmpRec = getLengthTest(testNo);
    addTestResult(tmpRec);
    
    //Insert Front and Back
    if (DEBUG) fprintf(OUT, "Testing insertion\n");
    tmpRec = insertTest(testNo);
    addTestResult(tmpRec);
    
    //Getter functions test
    if (DEBUG) fprintf(OUT, "Testing getter functions\n");
    tmpRec = getTest(testNo);
    addTestResult(tmpRec);
    
    //Insert Sorted
    if (DEBUG) fprintf(OUT, "Testing sorted insertion\n");
    tmpRec = insertSortedTest(testNo);
    addTestResult(tmpRec);
    
    //clear List
    if (DEBUG) fprintf(OUT, "Testing clearing functionality");
    tmpRec = clearTest(testNo);
    addTestResult(tmpRec);

    //delete List
    if (DEBUG) fprintf(OUT, "Testing deletion functionality");
    tmpRec = deleteTest(testNo);
    addTestResult(tmpRec);
    
    //Iterator tests
    if (DEBUG) fprintf(OUT, "Testing list iterator functionality");
    tmpRec = iteratorTest(testNo);
    addTestResult(tmpRec);

    //Printing
    if (DEBUG) fprintf(OUT, "Testing list printing");
    tmpRec = printTest(testNo);
    addTestResult(tmpRec);

    //search List
    if (DEBUG) fprintf(OUT, "Testing search list functionality");
    //runTest(&deleteTest);
    tmpRec = searchTest(testNo);
    addTestResult(tmpRec);
    
    int j;
    for(j=0; j<TESTS; j++)
    {
        
        if (j == 0) {
            printf("\n\nLinked List API:\n");
        }
        printRecord(testRecords[j]);
    }
    
    printf("Score: %.0f\n", calcGrade());
    destroyRecords(testRecords, TESTS);
    
    return 0;
    
}
