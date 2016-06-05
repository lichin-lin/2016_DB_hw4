#include <map>
#include <vector>
#include <string>
#include <cstdio>
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <algorithm>
#include <sys/stat.h>

using namespace std;

class db{
	public:
		void init();                            // Do your db initialization.

		void setTempFileDir(string);            // All the files that created by your program should be located under this directory.

		void import(string);                    // Inport csv files under this directory.

		void createIndex();                     // Create index on one or two columns.

		double query(string, string);          	// Do the query and return the average ArrDelay of flights from origin to dest.

		void cleanup();                         // Release memory, close files and anything you should do to clean up your db class.

		bool indexed;							// check indexed or not
		string createFile;
		map<string, vector<int> > journey;	    // Origin+dest, time delay
};
