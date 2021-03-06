/*
 * FileReaderHelper.cpp
 *
 *  Created on: Sep 23, 2017
 *      Author: hungnv103
 */

#include "FileReaderHelper.h"
#include "cmmdefs.h"

FileReaderHelper::FileReaderHelper()
{
	this->meOffset = FileReaderHelper::UK31_OFFSET;
}
FileReaderHelper::FileReaderHelper(std::string fileName,FileReaderHelper::OFFSET_TYPE offset)
{
	this->mFilName = fileName;
	this->meOffset = offset;

	///Open File here

}
FileReaderHelper::FileReaderHelper(char *fileName,FileReaderHelper::OFFSET_TYPE offset)
{
	this->mFilName = std::string(fileName);
	this->meOffset = offset;
}

FileReaderHelper::~FileReaderHelper() {
	// TODO Auto-generated destructor stub
}

void FileReaderHelper::setFilName(std::string fileName)
{
	this->mFilName = fileName;
}

int FileReaderHelper::writeToFile( char *data, const int length, const int offset)
{
	int returnCode = ERR_SUCCESS;
	///First of all - open the file
	if( this->mFilName.empty() == true ){
		returnCode = ERR_FILE_PATH_NOT_SETTED;
		return returnCode;
	}

	FILE *filePtr = openFile( this->mFilName);
	if( filePtr == NULL ){
		returnCode = ERR_OPEN_FILE_FAILED;
		return returnCode;
	}

	int realPosision = this->meOffset + offset;
	///Go to right possion
	fseek(filePtr,realPosision,SEEK_SET);
	int writedSize = 0;
	writedSize = fwrite(data,length,1,filePtr);
	if( writedSize <= 0 ){
		returnCode = ERR_WRITE_FAILED;
	}
	fclose(filePtr);
	return returnCode;
}

int FileReaderHelper::readFromFile(char *data, const int length, const int offset)
{
	int returnCode = ERR_SUCCESS;
	///First of all - open the file
	if( this->mFilName.empty() == true ){
		returnCode = ERR_FILE_PATH_NOT_SETTED;
		return returnCode;
	}

	FILE *filePtr = openFile( this->mFilName);
	if( filePtr == NULL ){
		returnCode = ERR_OPEN_FILE_FAILED;
		fclose(filePtr);
		return returnCode;
	}
	int realPosision = this->meOffset + offset;
	///Go to right possion
	fseek(filePtr,realPosision,SEEK_SET);
	int readByte = 0;
	readByte = fread(data, length,1,filePtr);
	if( readByte <= 0 ){
		returnCode = ERR_READ_FAILED;
	}
	fclose(filePtr);

	return returnCode;
}


FileReaderHelper::OFFSET_TYPE FileReaderHelper::getMeOffset() const
{
	return meOffset;
}

void FileReaderHelper::setMeOffset(FileReaderHelper::OFFSET_TYPE meOffset)
{
	this->meOffset = meOffset;
}

FILE *FileReaderHelper::openFile(std::string filePath)
{
	FILE *filePtr;
	filePtr = fopen(filePath.c_str(),"rb+");///Open file in reading + writing mode
	if( filePtr == NULL){
		///Log here
	}
	return filePtr;
}
