#ifndef STATICSHADINGCORRECTIONPAGE_H
#define STATICSHADINGCORRECTIONPAGE_H
#include "abstractpagewidget.h"
#include "ui_staticcorrection.h"

class ExtendStackWidget;
class stImageInfo;
class StaticShadingCorrectionpage :  public AbstractPageWidget
{  
    Q_OBJECT

public:
    StaticShadingCorrectionpage(QWidget *parent = 0);
    void  onEnable(bool enable);
    void  onSave();
    void  onCancel();
    void  onUpdate();
signals:

private slots:
    void on_currentChanged(int index);

private:
	ExtendStackWidget *m_pExtendStatic;
};

class CurveGraph;
class QImage;
class IArithmeticStatic;
class StaticPage : public AbstractPageWidget
{
    Q_OBJECT
public:

	StaticPage(QString strKey,QWidget *parent = 0);
	~StaticPage();
    void  onEnable(bool enable);
    void  onSave();
    void  onCancel();
    void  onUpdate();

private slots:

    void on_clearTextEdit();
    void on_saveFactors();
    void on_runTeach();

    void on_loadFactors();
    void on_showChart();
    void on_loadPic();
    void on_runCorrection();
    void on_saveCorImage();

    void on_spinBox_current_line_valueChanged();
    void on_spinBox_grey_valueChanged();
    void on_spinBox_min_col_valueChanged();
    void on_spinBox_max_col_valueChanged();
    void on_spinBox_min_grey_valueChanged();
    void on_spinBox_max_grey_valueChanged();
    void on_checkBox_source_clicked (bool checked);
    void on_checkBox_correction_clicked (bool checked);
	void on_loadPicSetFlag();
    void on_pushButton_get_image_clicked(bool checked);
    void on_pushButton_get_check_image_clicked(bool checked);
private:
    bool saveImage(unsigned char *buff,int width,int height,QString  strPath);
    bool updateVideo(int);
	void reInitSDKFlag();
private:
    //QString m_strProductName;
    IArithmeticStatic*  m_pIArithmeticStatic;
	QString m_strChannelName;
	 Ui::StaticCorrectionForm  *ui;
     CurveGraph* m_pStaticCurveGraph;
     int m_imageWidth;
     int m_imageHeight;
     float m_standardGray;
     int m_iStartOffset;
     int m_iEndOffset;
     unsigned char *m_rawImage;
     unsigned char *m_correctImage;
	 float *m_pFfactor;
     int  m_ifactorLength;
     QImage *m_qImg;
     QImage *m_qcorrectImage;
     stImageInfo *m_imageInfoBuffer;
     bool m_bflag;
};
#endif // STATICSHADINGCORRECTIONPAGE_H
