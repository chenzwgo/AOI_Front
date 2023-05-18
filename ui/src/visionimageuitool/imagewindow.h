#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QLabel>
#include <QComboBox>
#include <QKeyEvent>
#include <QTimer>
#include <QUndoStack>
#include <QUndoView>
#include <QTranslator>
#include "OS_DEFINE.h"
#include "graphscene.h"

namespace Ui {
class ImageWindow;
}
class GraphScene;
class ImageWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ImageWindow(QWidget *parent = 0);
    ~ImageWindow();

    void showPictureInLabel(QString file_path);//ֻ��ʾһ��ͼƬ
    void showPictureInLabel(unsigned char *pixelsR, unsigned char *pixelsG, unsigned char *pixelsB,long width, long height);//��ʾ�Զ����Ƶ�ͼƬ

    void adjustPictureSize();
    void hiddenButton(int iHidden);//�������еİ�ť
    void showToolBar(int iHide);
    void showStatusBar(int iHide);
    void showActionItem(int iHide);

    bool init();
    bool deinit();
	bool showWindow(QWidget* parent, QRect& rect,bool bPopFlag = false);
    bool hideWindow();

    void fitSize(const QRect& rect);

    bool notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam);
    void SetLanguage(QString strlanguage);

    bool addShapeItem(const QVariant &param, double iWidth, bool bIsMoveEnable, int index);          //���ͼ��ʵ��
    QList<QVariant> queryShapeItem();                            //��ȡ����ͼ��ʵ��
    bool removeShapeItem(int index, bool clear = false);                //ɾ��ͼ��ʵ��, indexΪͼ�������� clearΪtrueʱ����б�

    void setRoiMaxCount(int iCount = -1);           //������������ROI������ Ĭ��-1Ϊ������

    void setManualWidget(QWidget *pWidget);
    void showCross(bool bShow, double dRadius = 0);
    void setItemSelect(int id);
    unsigned char *getImageLabelPtr();
    void UpdateImage();
    void setSaveImageFileName(QString strName);
signals:
    void sigItemMoveFinished();
    void sigItemRBtDoubleCliecked(QPointF &);
    void sigItemSelectChanged(int index);
protected slots:
    void on_actionOpenImage_triggered ( bool checked );  // ��ͼƬ
    void on_actionSaveImage_triggered ( bool checked );  // ��ͼƬ
    void on_actionZoomIn_triggered ( bool checked );     // �Ŵ�
    void on_actionZoomOut_triggered ( bool checked );    // ��С
    void on_actionFullView_triggered ( bool checked );   // ����
    void on_actionOriginalSize_triggered ( bool checked );   // ԭʼ��С
    void on_actionDeleteAll_triggered( bool checked );     // ɾ��ȫ��
    void on_actionDeleteSelect_triggered( bool checked );   // ɾ��ѡ����
    void on_actionSelectAll_triggered( bool checked );        // ȫѡ
    void on_actionMoveUp_triggered( bool checked );        // ����(ѡ��item)
    void on_actionMoveDown_triggered( bool checked );        // ����(ѡ��item)
    void on_actionMoveRight_triggered( bool checked );        // ����(ѡ��item)
    void on_actionMoveLeft_triggered( bool checked );        // ����(ѡ��item)
    void on_actionMoveUpPix_triggered( bool checked );        // ����һ������(ѡ��item)
    void on_actionMoveDownPix_triggered( bool checked );        // ����һ������(ѡ��item)
    void on_actionMoveRightPix_triggered( bool checked );        // ����һ������(ѡ��item)
    void on_actionMoveLeftPix_triggered( bool checked );        // ����һ������(ѡ��item)
    void on_actionClearImage_triggered( bool checked );        // ���ͼƬ
    void on_actionCopyShape_triggered( bool checked );              // ����
    void on_actionCutShape_triggered( bool checked );               // ����
    void on_actionPasteShape_triggered( bool checked );             // ճ��
    void on_actionSelectNext_triggered(bool checked);         // ѡ����һ��
    void on_actionSelectPre_triggered(bool checked);          // ѡ����һ��
    void on_actionCenterCross_triggered(bool checked);          // ��ʾʮ�ּ�
    void on_actionClearRoi_triggered(bool checked);          // ���ROI
    void on_action_triggered(QAction *);                 // ��ͼ�¼�
    void on_actionUndoStack_triggered(bool checked);        // ��������
    void slotStatusBarShowCoordinate(QPointF pos);       // ״̬����ʾ����
    void slotStatusBarShowGary(QPoint pos);              // ״̬����ʾ�Ҷ�ֵ
    void slotStatusBarScale(qreal);                      // ͼ�����
    void slotStatusBarImageSizeChangle(int, int);        // ͼ���С�ı�
    void slotStatusBarSelectCount();                      // ѡ�и���
    void slotTimerHandle();                              // ��ʱ������
    void on_checkBox_arrow_stateChanged( int state );    // ��ͷ��ʾ
    void on_checkBox_grid_stateChanged( int state );       // ������ʾ
    void on_toolButton_modifySize_clicked(); // �޸�ͼ������ߴ�
    void slot_showSelectItemData();      // ��ʾ��ǰͼ����Ϣ
    void on_action_color_triggered(bool checked);   // ����

    void slotItemMoveFinished();
    void slotItemRBtDoubleCliecked(QPointF &);
    void slotLineWidthChangle(const QString &strWidth);
    void slotFontSizeChangle(const QString &strWidth);
protected:
    void keyPressEvent(QKeyEvent *event);
    void showEvent(QShowEvent *e);
    void hideEvent(QShowEvent *e);
    void resizeEvent(QResizeEvent *e);
private:
    void initDrawAction();  // ��ʼ����ͼaction
    void moveItems(int dx, int dy);  // �ƶ�ѡ��items
    void updateActionStatus();     // ���¹�����״̬
    void initShapeDataFrame();     // ��ʼ��ͼ������
    void initUndoAction();         // ��ʼ������Action

    void showAllButton(bool show);
private:
    Ui::ImageWindow *ui;
    GraphScene *m_pScene; 
    QLabel m_statusBarCoordinate;       // ����
    QLabel m_statusBarGray;             // �Ҷ�ֵ
    QLabel m_statusBarScale;            // �Ŵ����
    QLabel m_statusBarSelectCount;      // ѡ�и���
    QLabel m_statusBarImageSize;      // ѡ�и���
    QLabel m_lineWidthTitle;         // �߿����
    QLabel m_FontSizeTitle;         // �����С����
    QComboBox m_comboLineWidth;         // �߿�����
    QComboBox m_comboFontSize;         // �����С
    QTimer m_timer;                     // ��ʱ��
    QMap<GraphicsShapeItem::DataType,QFrame*> m_shapeDataFrameList;  // ͼ�������б�
    QUndoStack *m_undoStack;   // ����������ջ
    QUndoView  *m_undoView;    // �����б�
    QTranslator m_translator;
    int      m_iMaxCount;
    QActionGroup *m_pDrawGroup; //��ͼ��
    QString m_save_file_path;
    QString m_save_file_name;

    int  m_iLineWidth;  //�߿�
    bool ReadConfig();
    void InitUI();
};

#endif // IMAGEWINDOW_H
