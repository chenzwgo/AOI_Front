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

    void showPictureInLabel(QString file_path);//只显示一张图片
    void showPictureInLabel(unsigned char *pixelsR, unsigned char *pixelsG, unsigned char *pixelsB,long width, long height);//显示以二进制的图片

    void adjustPictureSize();
    void hiddenButton(int iHidden);//隐藏所有的按钮
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

    bool addShapeItem(const QVariant &param, double iWidth, bool bIsMoveEnable, int index);          //添加图形实例
    QList<QVariant> queryShapeItem();                            //获取所有图形实例
    bool removeShapeItem(int index, bool clear = false);                //删除图形实例, index为图形索引， clear为true时清空列表

    void setRoiMaxCount(int iCount = -1);           //设置允许的最大ROI个数， 默认-1为不限制

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
    void on_actionOpenImage_triggered ( bool checked );  // 打开图片
    void on_actionSaveImage_triggered ( bool checked );  // 打开图片
    void on_actionZoomIn_triggered ( bool checked );     // 放大
    void on_actionZoomOut_triggered ( bool checked );    // 缩小
    void on_actionFullView_triggered ( bool checked );   // 铺满
    void on_actionOriginalSize_triggered ( bool checked );   // 原始大小
    void on_actionDeleteAll_triggered( bool checked );     // 删除全部
    void on_actionDeleteSelect_triggered( bool checked );   // 删除选择项
    void on_actionSelectAll_triggered( bool checked );        // 全选
    void on_actionMoveUp_triggered( bool checked );        // 上移(选中item)
    void on_actionMoveDown_triggered( bool checked );        // 下移(选中item)
    void on_actionMoveRight_triggered( bool checked );        // 右移(选中item)
    void on_actionMoveLeft_triggered( bool checked );        // 左移(选中item)
    void on_actionMoveUpPix_triggered( bool checked );        // 上移一个像数(选中item)
    void on_actionMoveDownPix_triggered( bool checked );        // 下移一个像数(选中item)
    void on_actionMoveRightPix_triggered( bool checked );        // 右移一个像数(选中item)
    void on_actionMoveLeftPix_triggered( bool checked );        // 左移一个像数(选中item)
    void on_actionClearImage_triggered( bool checked );        // 清除图片
    void on_actionCopyShape_triggered( bool checked );              // 复制
    void on_actionCutShape_triggered( bool checked );               // 剪切
    void on_actionPasteShape_triggered( bool checked );             // 粘贴
    void on_actionSelectNext_triggered(bool checked);         // 选择下一个
    void on_actionSelectPre_triggered(bool checked);          // 选择上一个
    void on_actionCenterCross_triggered(bool checked);          // 显示十字架
    void on_actionClearRoi_triggered(bool checked);          // 清除ROI
    void on_action_triggered(QAction *);                 // 绘图事件
    void on_actionUndoStack_triggered(bool checked);        // 撤销队列
    void slotStatusBarShowCoordinate(QPointF pos);       // 状态栏显示坐标
    void slotStatusBarShowGary(QPoint pos);              // 状态栏显示灰度值
    void slotStatusBarScale(qreal);                      // 图像比例
    void slotStatusBarImageSizeChangle(int, int);        // 图像大小改变
    void slotStatusBarSelectCount();                      // 选中个数
    void slotTimerHandle();                              // 定时器处理
    void on_checkBox_arrow_stateChanged( int state );    // 箭头显示
    void on_checkBox_grid_stateChanged( int state );       // 网格显示
    void on_toolButton_modifySize_clicked(); // 修改图像区域尺寸
    void slot_showSelectItemData();      // 显示当前图形信息
    void on_action_color_triggered(bool checked);   // 铺满

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
    void initDrawAction();  // 初始化绘图action
    void moveItems(int dx, int dy);  // 移动选中items
    void updateActionStatus();     // 更新工具栏状态
    void initShapeDataFrame();     // 初始化图形数据
    void initUndoAction();         // 初始化撤销Action

    void showAllButton(bool show);
private:
    Ui::ImageWindow *ui;
    GraphScene *m_pScene; 
    QLabel m_statusBarCoordinate;       // 坐标
    QLabel m_statusBarGray;             // 灰度值
    QLabel m_statusBarScale;            // 放大比例
    QLabel m_statusBarSelectCount;      // 选中个数
    QLabel m_statusBarImageSize;      // 选中个数
    QLabel m_lineWidthTitle;         // 线宽标题
    QLabel m_FontSizeTitle;         // 字体大小标题
    QComboBox m_comboLineWidth;         // 线宽属性
    QComboBox m_comboFontSize;         // 字体大小
    QTimer m_timer;                     // 定时器
    QMap<GraphicsShapeItem::DataType,QFrame*> m_shapeDataFrameList;  // 图形数据列表
    QUndoStack *m_undoStack;   // 撤销重做堆栈
    QUndoView  *m_undoView;    // 撤销列表
    QTranslator m_translator;
    int      m_iMaxCount;
    QActionGroup *m_pDrawGroup; //绘图组
    QString m_save_file_path;
    QString m_save_file_name;

    int  m_iLineWidth;  //线宽
    bool ReadConfig();
    void InitUI();
};

#endif // IMAGEWINDOW_H
