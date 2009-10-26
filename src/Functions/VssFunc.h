#ifndef __VssFunc_H__
#define __VssFunc_H__
#pragma once


namespace vss
{
	void init_root_workfolder(LPCTSTR szWorkFolder);

	void list_all_files(LPCTSTR szOutputFile);
	void list_file_versions(LPCTSTR szFile, LPCTSTR szOutputFile);
};
//namespace vss


#endif //__VssFunc_H__
