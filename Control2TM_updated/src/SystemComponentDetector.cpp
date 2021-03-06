/*
 * SystemComponentDetector.cpp
 *
 *  Created on: Sep 25, 2017
 *      Author: hungnv103
 */

#include "SystemComponentDetector.h"

std::string uk31path;
std::string uk51path;
std::string uk61path;
std::string uk43path;

SystemComponentDetector::SystemComponentDetector() {
	// TODO Auto-generated constructor stub

}

SystemComponentDetector::~SystemComponentDetector() {
	// TODO Auto-generated destructor stub
}

std::string SystemComponentDetector::getUk31Path(){
	return uk31path;
}
std::string SystemComponentDetector::getUk43Path(){
	//	std::string path("/net/uk43/dev/shmem/2tm");
	return uk43path;
}
std::string SystemComponentDetector::getUk51Path(){

	return uk51path;
}
std::string SystemComponentDetector::getUk61Path(){
	return uk61path;
}
std::string SystemComponentDetector::getUk42SimFilePath(){
	std::string path("/net/uk42/2tm/raid/I");
	std::string filepath = path + std::string(".txt");
	int fileExist = access(filepath.c_str(), F_OK);
	if (0 == fileExist) {
		return path;
	} else {
		path = std::string("/net/uk42./2tm/raid/I");
		filepath = path + std::string(".txt");
		fileExist = access(filepath.c_str(), F_OK);
		if (0 == fileExist) {
			return path;
		} else {
			return std::string("");
		}
	}
}

void SystemComponentDetector::run(){
	static pthread_t system_tracking;
	pthread_create(&system_tracking,NULL,SystemComponentDetector::targetChecking,NULL);
}

void *SystemComponentDetector::targetChecking(void *args){
	std::string sharedMemoryLocation("/dev/shmem/2tm");
	std::string getProcess("pidin -f N -n ");
	std::string filterCommand(" | grep -i \"uk\"");
	std::string uk51Process("uk51");
	std::string uk31Process("uk31");
	std::string uk61Process("uk61");
	std::string uk43Process("uk43");
//	std::cout << "here" << std::endl;
	while(1){
		FILE* fp = popen("ls -a /net/", "r");
		if(fp != NULL ){
			char tmp[128];
			std::string qnetProcess;
			while( fgets(tmp, sizeof(tmp)-1, fp) != NULL){
				tmp[strlen(tmp)-1]=0;
			if(strcmp(tmp,qnetProcess.c_str()) != 0 ){
				qnetProcess = std::string(tmp);

				///Get Uk31 process
				{
					std::stringstream getuk31Stream;
					getuk31Stream<<getProcess<<qnetProcess<<" -p "<<uk31Process<<filterCommand;
					//						printf("-------%s------\n",getuk31Stream.str().c_str());
					FILE* pp31 = popen(getuk31Stream.str().c_str(),"r");
					if(pp31!=NULL){
						char processName[32];
						while (fgets(processName, sizeof(processName)-1, pp31) != NULL) {
							std::string process(processName);
							//								std::cout<<"process 31: "<<process<<std::endl;
							if(process.find(uk31Process) != std::string::npos){
								std::stringstream path;
								path<<"/net/"<<qnetProcess<<sharedMemoryLocation;
								uk31path = path.str();
								//									printf("uk31 path: %s\n",uk31path.c_str());
							}
						}
					}
					pclose(pp31);
				}
				{
					///Get Uk43 process
					std::stringstream getuk43Stream;
					getuk43Stream<<getProcess<<qnetProcess<<" -p "<<uk43Process<<filterCommand;
					//						printf("-------%s------\n",getuk51Stream.str().c_str());/
					FILE* pp43 = popen(getuk43Stream.str().c_str(),"r");
					if(pp43!=NULL){
						char processName[32];
						while (fgets(processName, sizeof(processName)-1, pp43) != NULL) {
							std::string process(processName);
							//								std::cout<<"process 51: "<<process<<std::endl;
							if(process.find(uk43Process) != std::string::npos){
								std::stringstream path;
								path<<"/net/"<<qnetProcess<<sharedMemoryLocation;
								uk43path = path.str();
//								printf("uk43 path: %s\n",uk43path.c_str());
							}
						}
					}
					pclose(pp43);
				}
				{
					///Get Uk51 process
					std::stringstream getuk51Stream;
					getuk51Stream<<getProcess<<qnetProcess<<" -p "<<uk51Process<<filterCommand;
					//						printf("-------%s------\n",getuk51Stream.str().c_str());/
					FILE* pp51 = popen(getuk51Stream.str().c_str(),"r");
					if(pp51!=NULL){
						char processName[32];
						while (fgets(processName, sizeof(processName)-1, pp51) != NULL) {
							std::string process(processName);
							//								std::cout<<"process 51: "<<process<<std::endl;
							if(process.find(uk51Process) != std::string::npos){
								std::stringstream path;
								path<<"/net/"<<qnetProcess<<sharedMemoryLocation;
								uk51path = path.str();
								//									printf("uk51 path: %s\n",uk51path.c_str());
							}
						}
					}
					pclose(pp51);
				}
				{
					///Get Uk61 process
					std::stringstream getuk61Stream;
					getuk61Stream<<getProcess<<qnetProcess<<" -p "<<uk61Process<<filterCommand;
					//						printf("-------%s------\n",getuk61Stream.str().c_str());
					FILE* pp61 = popen(getuk61Stream.str().c_str(),"r");
					if(pp61!=NULL){
						char processName[32];
						while (fgets(processName, sizeof(processName)-1, pp61) != NULL) {
							std::string process(processName);
							//								std::cout<<"process 61: "<<process<<std::endl;
							if(process.find(uk61Process) != std::string::npos){
								std::stringstream path;
								path<<"/net/"<<qnetProcess<<sharedMemoryLocation;
								uk61path = path.str();
//								printf("uk61 path: %s\n",uk61path.c_str());
							}
						}
					}
					pclose(pp61);
				}
			}
			}
		}
		pclose(fp);
		sleep(1);
	}
}
