#ifndef __Data_H__
#define __Data_H__
#pragma once


struct SData
{
	std::string path;
	std::string date;
	std::string user;

	int version;
};


class SDataVect : public std::vector<SData*>
{
public:
	SDataVect();
	~SDataVect();
};


#endif //__Data_H__
