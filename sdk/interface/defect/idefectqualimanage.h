#ifndef _I_DEFECTQUALIMANAGE_H
#define _I_DEFECTQUALIMANAGE_H

class IDefectQualification;
class IDefectQualimanage
{
public:
    enum
	{
		IDD_DEFECT_QULIFICATION_MANAGE = 0x244,
		IDD_DEFECT_LIST_MANAGE
	};
	
	virtual ~IDefectQualimanage(){}
	virtual IDefectQualification*   getDefectQualificationInstance(int iStation) = 0;
	virtual void ReleaseStation(int iStation) = 0;
};
#endif 