#include <iostream>
#include <time.h>
#include "db.h"

using namespace std;

int main(int argc, char* argv[]){
	//declear db object
	db mydb;

	//init db
	mydb.init();

	//set temp directory
	mydb.setTempFileDir("temp");

	//Import data
	clock_t import_data = clock();
	for(int i = 2006; i < 2009; i++){
		cerr << "import data...";
		mydb.import("data/" + to_string(i) + ".csv");
	}

	//load without index
	clock_t no_index = clock();
	double result1_no = mydb.query("IAH", "JFK");
	double result2_no = mydb.query("IAH", "LAX");
	double result3_no = mydb.query("JFK", "LAX");
	double result4_no = mydb.query("JFK", "IAH");
	double result5_no = mydb.query("LAX", "IAH");

	//Create index on one or two columns.
	clock_t tIndex = clock();
	mydb.createIndex();

	//Start timing
	clock_t tQuery = clock();
	double result1 = mydb.query("IAH", "JFK");
	double result2 = mydb.query("IAH", "LAX");
	double result3 = mydb.query("JFK", "LAX");
	double result4 = mydb.query("JFK", "IAH");
	double result5 = mydb.query("LAX", "IAH");

	//End timing
	clock_t tEnd = clock();
	printf("----------------------- time records -----------------------\n\n");
	printf("Time taken for all data import: \t\t%.3fs\n", (double)(no_index - import_data)/CLOCKS_PER_SEC);
	printf("Time taken for making non-index queries: \t%.3fs\n", (double)(tIndex - no_index)/CLOCKS_PER_SEC);
	printf("Time taken for creating index: \t\t\t%.3fs\n", (double)(tQuery - tIndex) / CLOCKS_PER_SEC);
	printf("Time taken for making index queries: \t\t%.3fs\n", (double)(tEnd - tQuery) / CLOCKS_PER_SEC);
	printf("\n----------------------- time records -----------------------\n");
	cout << endl;

	// print result
	cout << "***query result 1***" << endl;
	cout << result1_no << "(without index)/" << result1 << "(index)" << endl << endl;
	cout << "***query result 2***" << endl;
	cout << result2_no << "(without index)/" << result2 << "(index)" << endl << endl;
	cout << "***query result 3***" << endl;
	cout << result3_no << "(without index)/" << result3 << "(index)" << endl << endl;
	cout << "***query result 4***" << endl;
	cout << result4_no << "(without index)/" << result4 << "(index)" << endl << endl;
	cout << "***query result 5***" << endl;
	cout << result5_no << "(without index)/" << result5 << "(index)" << endl << endl;

	//Cleanup db object
	mydb.cleanup();
}