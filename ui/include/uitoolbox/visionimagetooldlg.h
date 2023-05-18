#ifndef _VISIONIMAGETOOL_DLG_H_
#define _VISIONIMAGETOOL_DLG_H_

#include <QDialog>
#include <QTranslator>
#include <QGridLayout>
#include "OS_DEFINE.h"
#include "dllexport.h"
#include "uidefine.h"

class XTV_IMAGE_SHOW
{
public:
    enum XTV_LAYOUT_SHOW_ITEM
    {
        XTV_LAYOUT_FRAME_PROPERTY,
        XTV_LAYOUT_IMAGE_ZOOM,
        XTV_LAYOUT_MANUAL,
        XTV_LAYOUT_DRAW,
        XTV_LAYOUT_UNDO,
        XTV_LAYOUT_BAR_SIZE,
    };

    enum XTV_BUTTON_SHOW_TYPE
    {
        VS_IMAGE_SHOW_HIDDEN_ALL         =  0x0000,
        VS_IMAGE_SHOW_SHOW_STATUS        =  0x0001,
        VS_IMAGE_SHOW_SHOW_IMAGE_ZOOM    =  0x0002,
        VS_IMAGE_SHOW_SHOW_ITEM_ATTRI    =  0x0004,
        VS_IMAGE_SHOW_SHOW_ITEM_DRAW     =  0x0008,
        VS_IMAGE_SHOW_SHOW_ITEM_UNDO     =  0x0010,
        VS_IMAGE_SHOW_SHOW_ITEM_MOVE     =  0x0020,
        VS_IMAGE_SHOW_SHOW_ITEM_EDIT     =  0x0040,
        VS_IMAGE_SHOW_SHAPE_RECT1        =  0x0080,
        VS_IMAGE_SHOW_SHAPE_LINE         =  0x0100,
        VS_IMAGE_SHOW_SHAPE_POLYLINE     =  0x0200,
        VS_IMAGE_SHOW_SHAPE_ARC2         =  0x0400,
        VS_IMAGE_SHOW_SHAPE_ARC3         =  0x0800,
        VS_IMAGE_SHOW_SHAPE_RECT         =  0x1000,
        VS_IMAGE_SHOW_SHAPE_CIRCLE3      =  0x2000,
        VS_IMAGE_SHOW_SHAPE_CIRCLECENTER =  0x4000,
        VS_IMAGE_SHOW_SHAPE_ERASE        =  0x8000,
        VS_IMAGE_SHOW_SHOW_SHOW_ALL      =  0xFFFF
    };

    enum XTV_ACTION_ITEM_SHOW_TYPE
    {
        VS_ACTION_HIDDEN_ALL       = 0x0000,
        VS_ACTION_LINE_2_POINT     = 0x0001,
        VS_ACTIONPOLYLINE          = 0x0002,
        VS_ACTIONARCCENTER2POINT   = 0x0004,
        VS_ACTIONARC3POINT         = 0x0008,
        VS_ACTIONRECT              = 0x0010,
        VS_ACTIONRECTANGLE         = 0x0020,
        VS_ACTIONCIRCLE3POINT      = 0x0040,
        VS_ACTIONCIRCLECENTERPOINT = 0x0080,
        VS_ACTIONERASE             = 0x0100,
        VS_ACTION_SHOW_ALL         = 0xFFFF
    };

    enum XTV_TOOLBAR_SHOW_TYPE
    {
        VS_TOOLBAR_HIDDEN_ALL      = 0x0000,
        VS_TOOLBAR_EDIT            = 0x0001,
        VS_TOOLBAR_ITEMMOVE        = 0x0002,
        VS_TOOLBAR_UNDO            = 0x0004,
        VS_TOOLBAR_DRAW            = 0x0008,
        VS_TOOLBAR_SIZE            = 0x0010,
        VS_TOOLBAR_IMAGEZOOM       = 0x0020,
        VS_TOOLBAR_PROPERTY        = 0x0040,
        VS_TOOLBAR_MANUAL          = 0x0080,
        VS_TOOLBAR_SHOW_ALL        = 0xFFFF
    };
    enum XTV_STATUSBAR_SHOW_TYPE
    {
        VS_STATUS_HIDDEN_ALL       = 0x0000,
        VS_STATUSBARIMAGESIZE      = 0x0001,
        VS_STATUSBARCOORDINATE     = 0x0002,
        VS_STATUSBARGRAY           = 0x0004,
        VS_STATUSBARSCALE          = 0x0008,
        VS_STATUSBARSELECTCOUNT    = 0x0010,
        VS_STATUS_SHOW_ALL         = 0xFFFF
    };
};
#define VS_IMAGE_SHOW_HIDDEN_ALL           XTV_IMAGE_SHOW::VS_IMAGE_SHOW_HIDDEN_ALL
#define VS_IMAGE_SHOW_SHOW_STATUS          XTV_IMAGE_SHOW::VS_IMAGE_SHOW_SHOW_STATUS    
#define VS_IMAGE_SHOW_SHOW_IMAGE_ZOOM      XTV_IMAGE_SHOW::VS_IMAGE_SHOW_SHOW_IMAGE_ZOOM
#define VS_IMAGE_SHOW_SHOW_ITEM_ATTRI      XTV_IMAGE_SHOW::VS_IMAGE_SHOW_SHOW_ITEM_ATTRI
#define VS_IMAGE_SHOW_SHOW_ITEM_DRAW       XTV_IMAGE_SHOW::VS_IMAGE_SHOW_SHOW_ITEM_DRAW 
#define VS_IMAGE_SHOW_SHOW_ITEM_UNDO       XTV_IMAGE_SHOW::VS_IMAGE_SHOW_SHOW_ITEM_UNDO 
#define VS_IMAGE_SHOW_SHOW_ITEM_MOVE       XTV_IMAGE_SHOW::VS_IMAGE_SHOW_SHOW_ITEM_MOVE 
#define VS_IMAGE_SHOW_SHOW_ITEM_EDIT       XTV_IMAGE_SHOW::VS_IMAGE_SHOW_SHOW_ITEM_EDIT 
//#define VS_IMAGE_SHOW_SHOW_SHOW_ALL        0x0080

#define VS_IMAGE_SHOW_SHAPE_LINE           XTV_IMAGE_SHOW::VS_IMAGE_SHOW_SHAPE_LINE        
#define VS_IMAGE_SHOW_SHAPE_POLYLINE       XTV_IMAGE_SHOW::VS_IMAGE_SHOW_SHAPE_POLYLINE    
#define VS_IMAGE_SHOW_SHAPE_ARC2           XTV_IMAGE_SHOW::VS_IMAGE_SHOW_SHAPE_ARC2        
#define VS_IMAGE_SHOW_SHAPE_ARC3           XTV_IMAGE_SHOW::VS_IMAGE_SHOW_SHAPE_ARC3        
#define VS_IMAGE_SHOW_SHAPE_RECT           XTV_IMAGE_SHOW::VS_IMAGE_SHOW_SHAPE_RECT          
#define VS_IMAGE_SHOW_SHAPE_RECT1           XTV_IMAGE_SHOW::VS_IMAGE_SHOW_SHAPE_RECT1     
#define VS_IMAGE_SHOW_SHAPE_CIRCLE3        XTV_IMAGE_SHOW::VS_IMAGE_SHOW_SHAPE_CIRCLE3     
#define VS_IMAGE_SHOW_SHAPE_CIRCLECENTER   XTV_IMAGE_SHOW::VS_IMAGE_SHOW_SHAPE_CIRCLECENTER
#define VS_IMAGE_SHOW_SHAPE_ERASE          XTV_IMAGE_SHOW::VS_IMAGE_SHOW_SHAPE_ERASE       
#define VS_IMAGE_SHOW_SHOW_SHOW_ALL        XTV_IMAGE_SHOW::VS_IMAGE_SHOW_SHOW_SHOW_ALL     
class QRegExpValidator;
class ImageWindow;
class RT_API CVisionimagetoolDlg : public QWidget
{
    Q_OBJECT

protected:
    void keyPressEvent( QKeyEvent *e );
public:
    CVisionimagetoolDlg(QWidget *parent = 0);
    ~CVisionimagetoolDlg();
    bool init();
    bool deinit();
    bool showWindow(QWidget* parent, QRect& rect,bool bPopFlag = false);
    bool hideWindow();

    void fitSize(const QRect& rect);

    bool notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam);
    void SetLanguage(QString strlanguage);

    void showPictureInLabel(QString file_path);//只显示一张图片
    void showPictureInLabel(unsigned char *pixelsR, unsigned char *pixelsG, unsigned char *pixelsB,long width, long height);//显示以二进制的图片

    void adjustPictureSize();
    void showButton(int iHidden);//隐藏所有的按钮 0, 隐藏所有， 1，显示状态栏; 2，+属性； 3+显示部分控制；4 显示所有
    void showToolBar(int iHidden);
    void showStatusBar(int iHidden);
    void showActionItem(int iHidden);
    void clearPicture();

    bool addShapeItem(const QVariant &param, double dWidth = 0.001, bool bIsMoveEnable = true, int id = -1);          //添加图形实例
    QList<QVariant> queryShapeItem();                            //获取所有图形实例
    bool removeShapeItem(int index, bool clear = false);                //删除图形实例, index为图形索引， clear为true时清空列表

    void setRoiMaxCount(int iCount = -1);           //设置允许的最大ROI个数， 默认-1为不限制

    void addManualToolBar(QWidget *pWidget);
    void setParamValue(QString strKey, double dValue = 0.0, int iValue = 0);
    void setItemSelect(int id);
    unsigned char *getImageLabelPtr();   //该接口需确保内存已分配
    void UpdateImage();                  //更新图像显示
    void setSaveImageFileName(QString strName);
public slots:
    void slotItemMoveFinished();
    void slotItemRBtDoubleCliecked(QPointF &);
    void slotItemSelectChanged(int id);
signals:
    void sigItemMoveFinished();
    void sigItemRBtDoubleCliecked(QPointF &);
    void sigItemSelectChanged(int id);
    
private:
    QTranslator m_translator;

    bool ReadConfig();
    void InitUI();

    ImageWindow *m_pImageWindow;
};

//封装一个专用于显示图片几行几列的容器
class RT_API ShowPictureWidget : public QWidget
{
    Q_OBJECT

public:
    //ShowPictureWidget(int row,int column,QWidget *parent = 0);
    ShowPictureWidget(QWidget *parent = 0);
    ~ShowPictureWidget();
    void setShowPicture(int row,int column);
    CVisionimagetoolDlg* getSpecifyWidget(int row,int column);
    void hiddenSpecifyWidget(int row,int column,bool isHidden);
    void resizeWindowsSize();

private:
    QGridLayout *mainLayout;
    QList<CVisionimagetoolDlg*> m_allWidget;
    int m_row;
    int m_column;
};

#endif // _VISIONIMAGETOOL_DLG_H_

