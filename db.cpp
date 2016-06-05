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
	this->indexed = false;
}

void db::setTempFileDir(string dir){
	//All the files that created by your program should be located under this directory.
	mkdir(dir.c_str(), 0777);
	this->createFile = dir + string("/") + this->createFile;
	remove(this->createFile.c_str());
}

void db::import(string csvFile){
	//Inport csv files under this directory.

	//ifstream inFile(csvFile.c_str());
	FILE* inFile;
    inFile=fopen(csvFile.c_str(), "r");

	ofstream outFile(this->createFile, ios::binary | ios::app);
	//int limit = 5;

	char readline[512];
	fgets(readline,512,inFile);

	while(fgets(readline,100,inFile)){

		char* linedata = strtok(readline,",");
		//char Origin[4], Dest[4];
		string Origin, Dest;
		int flag = 1,count = 0, ArriDelay;

		while(flag && linedata!= NULL){
            if (count == 14){
            	if(isNA(linedata)){	
            		flag = 0;						// decide NA;
            	} else {
            		ArriDelay = atoi(linedata);
            	}
            }
            else if (count == 16) Origin = linedata;
            else if (count == 17) Dest = linedata;
            count++;
            linedata = strtok(NULL,",");
		}
		if(flag){
			//cout << ArriDelay << " | " << Origin << " | " << Dest << endl;
 			outFile.write((char*)&ArriDelay, ARRDELAY_4);
			outFile.write(Origin.c_str(), ORIGIN_3);
			outFile.write(Dest.c_str(), DEST_3);	
		}
	}

	fclose(inFile);
	outFile.close();
	printf("\tfinish!\n\n");
	return;
}

void db::createIndex(){
	//Create index.
	if(this->indexed == true)
		return;

	//init map STL.
	journey.clear();

	FILE* ReadFromDB;
    ReadFromDB=fopen(this->createFile.c_str(), "rb");

	//seek data & load to memory.
	int pos = 0;
	char onedata[ONEDATA_10+1],tmp_OD[ORIGIN_3+DEST_3+1];
	while(fread(&onedata,10,1,ReadFromDB)){

		for(int i = 4; i < 10; i++){
			tmp_OD[i-4] = onedata[i];
		}
		tmp_OD[ORIGIN_3+DEST_3] = '\0';

		this->journey[string(tmp_OD)].push_back(pos);
		pos+=ONEDATA_10;
	}

	fclose(ReadFromDB);
	this->indexed = true;

	return;
}

double db::query(string origin, string dest){
	//Do the query and return the average ArrDelay of flights from origin to dest.
	int resultNum = 0;
	double resultDelay = 0;
	char onedata[ONEDATA_10+1];
	fstream ReadFromDB(this->createFile, ios::in | ios::binary);

	if(this->indexed == true){
	    char tmp_OD[ORIGIN_3+DEST_3+1];
	    memcpy(tmp_OD, origin.c_str(), ORIGIN_3);
	    memcpy(tmp_OD+ORIGIN_3, dest.c_str(), DEST_3);
		tmp_OD[ORIGIN_3+DEST_3] = '\0';

		vector<int>& delayVector = this->journey[string(tmp_OD)];
		resultNum = delayVector.size();
		for(int i = 0; i < resultNum; i++){
			ReadFromDB.seekp(ios_base::beg + delayVector[i]);
			ReadFromDB.read(onedata, ONEDATA_10);
			double tmp_delay = (double)(*((int*)onedata));
			resultDelay += tmp_delay;
		}
	}
	else{
		while(ReadFromDB.read(onedata,ONEDATA_10)){
		    char tmp_Origin[4];
		    char tmp_Dest[4];
		    memcpy(tmp_Origin, onedata+ARRDELAY_4, ORIGIN_3);
		    memcpy(tmp_Dest, onedata+ARRDELAY_4+ORIGIN_3, DEST_3);
			tmp_Origin[3] = tmp_Dest[3] = '\0';

			if(strcmp(tmp_Origin,origin.c_str())==0 && strcmp(tmp_Dest,dest.c_str())==0){
				int tmp_ArrDelay = *((int*)onedata);
				resultDelay+= tmp_ArrDelay;
				resultNum++;
			}
		}
	}
	ReadFromDB.close();
	cout << "->" << resultNum << endl;
	return resultDelay / resultNum; //Remember to return your result.
}

void db::cleanup(){
	//Release memory, close files and anything you should do to clean up your db class.
}