#ifndef __Data_H__
#define __Data_H__
#pragma once


struct SData
{
	SData() : version(0) { }
	CString file;
	CString time;
	CString user;
	CString comment;
	CString label;
	CString label_comment;
	int     version;
};


class SDataVect : public std::vector<SData*>
{
public:
	SDataVect();
	~SDataVect();
};



#endif //__Data_H__
