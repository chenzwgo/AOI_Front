#ifndef DEFECTTABLEVIEW_H
#define DEFECTTABLEVIEW_H
#include "uidefine.h"
#include <QWidget>
#include "maindefine.h"
#include "jmutex.h"
#include "../toolbox/defectimage.h"
namespace Ui {
class defectWidget;
}
class  QStandardItemModel;
class  QItemSelectionModel;
class QModelIndex;
class QStandardItem;
class QMenu;
class IDefectRecordInterface;
class defectTableView : public QWidget
{
    Q_OBJECT

public:
    explicit defectTableView(QWidget *parent = 0);
    ~defectTableView();
    void initDefectTable();
    void dataSort();
	/*新ui修改*/
	void updateTextColor(const QColor& textColor);
	
public slots:
    void selePresTabelViw(const QModelIndex& intdex,const QModelIndex& index2);
    void slotsinitTable(QMap<QString, QVector<MarkStrut *> *> *vecmark);
	void slotClearTable();
	//增加单个缺陷
	void slotAddMarkStrut(stDefectFeature  Defect, int iSene);
	void slotAddImageSrc(ImageSrcStrut* Image,int istation);
	void seleIamge(const QModelIndex& iIntdex);
    void slotUiShow(int ichoose);
    void slotSelectDefect(const QModelIndex& iIntdex);
    void slotSelectDefect(int index);
    void slotcustomContextMenuRequested(const QPoint &pos);
    void slotInsert();
    void slotHeaderPress(int index);
signals:
    void sigSelectDefect(const QRectF pos,int iSence);
    void sigSelectCamera(QString strName,int iSence);
    void sigRightButtonClicked();
public:
private:
	void saveDefectImage(const stDefectFeature  *Defect);
	void saveChannelDefectImage(const stDefectFeature  *Defect, string strChannelName);
	unsigned char *getCutImageRect(const stDefectFeature  *Defect, const ImageSrcStrut *pImageSrc, int &out_width, int &out_height);
	void setTableBackground(int DefectLevel, int iLevelIndex);
private:
    Ui::defectWidget *ui;
    QStandardItemModel *m_model;
    QItemSelectionModel *m_selectionModel;
	int m_iRow;
	QList<stDefectFeature> mListQata;
	QMap<QString, ImageSrcStrut*> m_MapImageData;
	DefectImage m_showimg;
	int m_iCurChooseRow;
    int m_iShow;
    QMenu*   m_pContextMenu;
    QAction* m_pActInsert;
    IDefectRecordInterface *m_pDefectRecord;

    //数据排序
    int  m_sortIndex;
    bool m_bIsAscending;
    /*新ui修改*/
	QColor m_textColor;

	vector<string> m_defectNameVec;

};

#endif // DEFECTTABLEVIEW_H
