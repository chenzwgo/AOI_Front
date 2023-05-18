#ifndef DEFECTHEATMAPPER_H
#define DEFECTHEATMAPPER_H
#include <QObject>
#include <QWidget>
#include <QString>
#include "maindefine.h"
#include "common_data.h"
#include "g12globalenv.h"
#include "maindefine.h"
#include <stack>
#include "ProductUnion.h"
using namespace std;
class QImage;
class HeatMapper;
class GradientPalette;


struct stBatchData
{
    //unsigned int dvdefectCnt[DEFECT_TYPE_SIZE];								// H算法缺陷类型分类，每一种缺陷的个数
    vector<int> dvdefectVec;
    stBatchData()
	{
       // memset(dvdefectVec, 0, sizeof(dvdefectVec));
	}
	void clear()
	{
        dvdefectVec.clear();
        //memset(dvdefectVec, 0, sizeof(dvdefectVec));
	}

	stBatchData(const stBatchData& other)
	{
        //memcpy(dvdefectVec, other.dvdefectVec, sizeof(dvdefectVec));
	}
	stBatchData& operator =(const stBatchData& other)
	{
		if (&other == this)
		{
			return *this;
		}
        //memcpy(dvdefectVec, other.dvdefectVec, sizeof(dvdefectVec));
		return *this;
	}

	stBatchData& operator +=(const stBatchData& other)
	{

        for (int i = 0; i < dvdefectVec.size(); i++)
		{
            dvdefectVec[i] += other.dvdefectVec[i];
		}
		return *this;
	}

	stBatchData& operator +(const stBatchData& other)
	{

        for (int i = 0; i < dvdefectVec.size();i++)
		{
            dvdefectVec[i] += other.dvdefectVec[i];
		}
		return *this;
	}

	stBatchData& operator -=(const stBatchData& other)
	{
        for (int i = 0; i < dvdefectVec.size(); i++)
		{
            dvdefectVec[i] -= other.dvdefectVec[i];
		}
		return *this;
	}

	stBatchData& operator -(const stBatchData& other)
	{
        for (int i = 0; i < dvdefectVec.size(); i++)
		{
            dvdefectVec[i] -= other.dvdefectVec[i];
		}
		return *this;
	}
};
struct stRODData
{
	int ibatchRowCnt;
	int ibatchColumnCnt;
	stBatchData **pDefectCnt;
	stRODData(int iRowCnt, int iColumnCnt)
	{
		ibatchRowCnt = iRowCnt;
		ibatchColumnCnt = iColumnCnt;
		if (ibatchRowCnt > 0 && ibatchColumnCnt > 0)
		{
			pDefectCnt = new stBatchData*[ibatchRowCnt];
			for (int i = 0; i < ibatchRowCnt; ++i)
			{
				pDefectCnt[i] = new stBatchData[ibatchColumnCnt];
			}
		}
	}
	void clear()
	{
		for (int i = 0; i < ibatchRowCnt; i++)
		{
			for (int j = 0; j < ibatchColumnCnt; j++)
			{
				pDefectCnt[i][j].clear();
			}
		}
	}
	void release()
	{
		for (int i = 0; i < ibatchRowCnt; ++i)
		{
			delete pDefectCnt[i];
		}
		delete pDefectCnt;
		pDefectCnt = NULL;
	}

	void copyto(const stRODData& other)
	{
		if (ibatchRowCnt != other.ibatchRowCnt || ibatchColumnCnt != other.ibatchColumnCnt)
		{
			return;
		}

	}
	stRODData(const stRODData& other)
	{
		ibatchRowCnt = other.ibatchRowCnt;
		ibatchColumnCnt = other.ibatchColumnCnt;
		pDefectCnt = other.pDefectCnt;
	}
	stRODData& operator =(const stRODData& other)
	{
		if (&other == this)
		{
			return *this;
		}
		ibatchRowCnt = other.ibatchRowCnt;
		ibatchColumnCnt = other.ibatchColumnCnt;
		pDefectCnt = other.pDefectCnt;
		return *this;
	}


	stRODData& operator+(const stRODData& other)
	{
		if (ibatchRowCnt != other.ibatchRowCnt || ibatchColumnCnt != other.ibatchColumnCnt)
		{
			return *this;
		}

		for (int i = 0; i < ibatchRowCnt; i++)
		{
			for (int j = 0; j < ibatchColumnCnt; j++)
			{
				pDefectCnt[i][j] += other.pDefectCnt[i][j];
			}
		}

		return *this;
	}

	stRODData& operator-(const stRODData& other)
	{
		if (ibatchRowCnt != other.ibatchRowCnt || ibatchColumnCnt != other.ibatchColumnCnt)
		{
			return *this;
		}

		for (int i = 0; i < ibatchRowCnt; i++)
		{
			for (int j = 0; j < ibatchColumnCnt; j++)
			{
				pDefectCnt[i][j] -= other.pDefectCnt[i][j];
			}
		}

		return *this;
	}
};


struct stRODCfg
{
	struct StSearchEnable
	{
		bool bdefectenable;
		int  idefectype;
		int  idefectcnt;
	};
	bool bEnableAll;
	int iSearchWidth;
	int iSearchHeight;
	int iContiueCnt;
    //int dvdefectWarningCnt[DEFECT_TYPE_SIZE];
    vector<int> dvdefectWarningVec;
	StSearchEnable SearchEnable[AREA_WARNING_ARRAY_CNT];
	int ibatchSize;
	stack<stRODData*> m_stRODDataStack;
	stRODData* m_totalData;
};


class DefectheatmapperWidget:public QWidget
{
	Q_OBJECT

public:
	enum {
		CANVAS_WIDTH = 610,
		CANVAS_HEIGHT = 702,
		DEFAULT_RADIUS = 10,
		DEFAULT_OPACITY = 128,
		DEFAULT_WIDTH = 255
	};

	explicit DefectheatmapperWidget(QWidget * parent = NULL);
    ~DefectheatmapperWidget();

	void initHeatmap(int RangeX, int RangeY, vector<STPoint>& vecPos);
    void clearUI();
    void updateUI();
	void addPoint(double dposx, double dposy);

	void addRegionOfDefectPoint(CImageProduct* pProduct);
	void clearRegionOfDefectPoint();
public :signals:
	void sigdefectWarning(QString);

protected:
	void paintEvent(QPaintEvent *);
	void mouseMoveEvent(QMouseEvent *event);
	void readRODCfg();
	void testROD(CImageProduct* pProduct);

	//void mouseReleaseEvent(QMouseEvent *e);
private:
	// 绘图对象指针
	HeatMapper *mapper_;
	// 用于显示输出的图像
	QImage *canvas_;
	// 调色板
	GradientPalette *palette_;
	bool m_bInit;
	double m_xRadio;
	double m_yRadio;
	stRODCfg m_stRODCfg;

	vector<string> m_defectNameVec;

};

#endif
