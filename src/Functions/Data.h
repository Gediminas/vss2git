#ifndef __Data_H__
#define __Data_H__
#pragma once


struct SData
{
	SData() : version(0) { }
	CString path;
	CString date;
	CString user;
	int     version;
};


class SDataVect : public std::vector<SData*>
{
public:
	SDataVect();
	~SDataVect();
};



#endif //__Data_H__
