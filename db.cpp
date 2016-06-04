#include <string>
#include "db.h"
using namespace std;

#define ARRDELAY_4 4
#define ORIGIN_3 3
#define DEST_3 3
#define ONEDATA_10 10

int isNA(string s){
	if(s.size()==0){
		return 1;
	}
	return 0;
}
void db::init(){
	//Do your db initialization.
	this->createFile = "travelData";
}

void db::setTempFileDir(string dir){
	//All the files that created by your program should be located under this directory.
	mkdir(dir.c_str(), 0777);
	this->createFile = dir + string("/") + this->createFile;
	remove(this->createFile.c_str());
	// this->createFile = dir + "/diskdata";
}

void db::import(string csvFile){
	//Inport csv files under this directory.
	cout << "import data: " << csvFile;

	ifstream inFile(csvFile.c_str());
	ofstream outFile(this->createFile, ios::binary | ios::app);

	string readline;
	getline(inFile, readline);
	int limit = 30;

	while(getline(inFile, readline) && limit--){
		
		istringstream iss(readline);
		string data, Origin, Dest;
		int flag = 1,count = 0, ArriDelay;

		while(flag && getline(iss, data, ',')){
            if (count == 14){
            	if(isNA(data)){	
            		flag = 0;						// decide NA;
            	} else {
            		ArriDelay = atoi(data.c_str());
            	}
            }
            else if (count == 16){
                Origin = data;
                // cout << Origin << endl;
            }
            else if (count == 17){
                Dest = data;
                // cout << Dest << endl;
            }
            count++;
		}
		if(flag){
			//cout << "read: " << Origin << "|" << Dest << "|" << ArriDelay << endl;
 			outFile.write((char*)&ArriDelay, ARRDELAY_4);
			outFile.write(Origin.c_str(), ORIGIN_3);
			outFile.write(Dest.c_str(), DEST_3);	
		}

	}

	inFile.close();
	outFile.close();
	cout << "\tfinish" << endl;
	cout << endl;

	return;
}

void db::createIndex(){
	//Create index.
	if(this->indexed == true){
		return;
	}

	//init map STL.
	journey.clear();
	ifstream ReadFromDB(this->createFile, ios::binary);

	//seek data to memory.
	int pos = 0;
	char onedata[ONEDATA_10+1];
	while(ReadFromDB.read(onedata,ONEDATA_10)){
		//this->journey[].push_back();
		pos+=ONEDATA_10;
	}
			// string road = Origin+Dest;
	  //       map<string, vector<int> >:: iterator it;
	  //       it = journey.find(road);

	  //       if (it != journey.end()){
	  //           journey[road].push_back(ArriDelay);
	  //       } else {
	  //           vector<int> v;
	  //           v.push_back(ArriDelay);
	  //           journey[road] = v;
	  //       }	
	ReadFromDB.close();
	this->indexed = true;

	return;
}

double db::query(string origin, string dest){
	//Do the query and return the average ArrDelay of flights from origin to dest.
	//This method will be called multiple times.
	double resultDelay = 0;
	int resultNum = 0;
	char onedata[ONEDATA_10+1];

	ifstream ReadFromDB(this->createFile, ios::binary);
	if(this->indexed == true){

	}
	else{
		while(ReadFromDB.read(onedata,ONEDATA_10)){

		}
	}
	return 0;//resultDelay / resultNum; //Remember to return your result.
}

void db::cleanup(){
	//Release memory, close files and anything you should do to clean up your db class.
}