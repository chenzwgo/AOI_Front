#pragma once
#ifndef _I_DEFECTDEFECTLISTMANAGE_H
#define _I_DEFECTDEFECTLISTMANAGE_H

class iDefectlist;
class IDefectListManage
{
public:
	enum
	{
		IDD_DEFECT_DEFECTLIST_MANAGE = 0x245
	};

	virtual ~IDefectListManage() {}
	virtual iDefectList*   getDefectListInstance(int iStation) = 0;
	virtual void ReleaseStation(int iStation) = 0;
};
#endif 