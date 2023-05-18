#include "imagewindow.h"
#include "ui_imagewindow.h"
#include <QFileDialog>
#include "graphwidget.h"
#include "graphscene.h"
#include <QDebug>
#include <qmath.h>
#include <QClipboard>
#include "shapemimedata.h"
#include "commands.h"
#include "uicommon/commonutils.h"
#include "controlsdef.h"
#include "uitoolbox/visionimagetooldlg.h"
#include <QColorDialog>

#define PLUGIN_PATH "plugins/visionimagetoolplugin"

static const char *pluginText[] = {
    QT_TRANSLATE_NOOP( "VisionimagetoolDlg", "PluginName" ),
};
ImageWindow::ImageWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageWindow),
    m_undoStack(NULL),
    m_undoView(NULL),
    m_pDrawGroup(NULL)
{
    ui->setupUi(this);

    m_save_file_path = ".";
    m_lineWidthTitle.setText(QString::fromLocal8Bit("线宽"));
    ui->toolBar_draw->addWidget(&m_lineWidthTitle);
    m_comboLineWidth.setEditable(true);
    m_comboFontSize.setEditable(true);
    QStringList widthList;
    widthList <<"0.1"<< "1" << "5" << "12" << "30" << "60";
    m_comboLineWidth.addItems(widthList);
    for (int i = 8; i < 32; i = i+2)
    {
        m_comboFontSize.addItem(QString().setNum(i));
    }
    for (int i = 32; i < 128; i = i + 4)
    {
        m_comboFontSize.addItem(QString().setNum(i));
    }
    for (int i = 128; i <= 256; i = i + 8)
    {
        m_comboFontSize.addItem(QString().setNum(i));
    }
    ui->toolBar_draw->addWidget(&m_comboLineWidth);
    m_FontSizeTitle.setText(QString::fromLocal8Bit("字体大小"));
    ui->toolBar_draw->addWidget(&m_FontSizeTitle);
    ui->toolBar_draw->addWidget(&m_comboFontSize);
    this->addToolBar(Qt::TopToolBarArea,ui->toolBar_imageZoom);
    this->addToolBar(Qt::TopToolBarArea,ui->toolBar_manual);
    this->addToolBar(Qt::TopToolBarArea,ui->toolBar_draw);
    this->addToolBar(Qt::TopToolBarArea,ui->toolBar_undo);        
    this->addToolBarBreak();
    ui->toolBar_size->addWidget(ui->frame_size);
    this->addToolBar(Qt::BottomToolBarArea,ui->toolBar_size);
    ui->toolBar_property->addWidget(ui->frame_property);
    ui->toolBar_property->setMinimumSize(50,30);
    this->addToolBar(Qt::BottomToolBarArea,ui->toolBar_property);
    //this->addToolBarBreak();
    //this->addToolBar(ui->toolBar_edit);
    //this->addToolBar(ui->toolBar_itemMove);
    
    ui->actionSelectNext->setShortcut(Qt::Key_Tab);
    ui->actionSelectPre->setShortcut(QApplication::translate("ImageWindow", "Shift+Tab", 0));
    ui->graphicsView->setBackgroundRole(QPalette::Dark);

    initUndoAction();
    initShapeDataFrame();
    initDrawAction(); // 绘图动作
    statusBar()->addWidget(&m_statusBarImageSize);
    statusBar()->addWidget(&m_statusBarCoordinate);
    statusBar()->addWidget(&m_statusBarGray);
    statusBar()->addWidget(&m_statusBarScale);
    statusBar()->addWidget(&m_statusBarSelectCount);
    m_pScene = qobject_cast<GraphScene*>(ui->graphicsView->scene());
    m_pScene->setUndoStack(m_undoStack);

    showAllButton(false);
    connect(m_pScene, SIGNAL(sigCoordinate(QPointF)), this, SLOT(slotStatusBarShowCoordinate(QPointF)));
    connect(m_pScene, SIGNAL(selectionChanged()), this, SLOT(slotStatusBarSelectCount()));
    connect(m_pScene, SIGNAL(sig_items_moved(QList<QGraphicsItem*>, QList<QPointF>)), this, SLOT(slot_items_moved(QList<QGraphicsItem*>, QList<QPointF>)));
    connect(m_pScene, SIGNAL(sigUpdateGray(QPoint)), this, SLOT(slotStatusBarShowGary(QPoint)));
    connect(ui->graphicsView, SIGNAL(sigScaleChanged(qreal)), this, SLOT(slotStatusBarScale(qreal)));

    connect(&m_timer,SIGNAL(timeout()),this,SLOT(slotTimerHandle()));
    connect(m_pScene, SIGNAL(sigItemMoveFinished()), this, SLOT(slotItemMoveFinished()));
    connect(m_pScene, SIGNAL(sigItemRBtDoubleCliecked(QPointF &)), this, SLOT(slotItemRBtDoubleCliecked(QPointF &)));

    connect(&m_comboLineWidth, SIGNAL(currentIndexChanged(const QString & )), this, SLOT(slotLineWidthChangle(const QString &)));
    connect(&m_comboLineWidth, SIGNAL(editTextChanged(const QString & )), this, SLOT(slotLineWidthChangle(const QString &)));

    connect(&m_comboFontSize, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(slotFontSizeChangle(const QString &)));
    connect(&m_comboFontSize, SIGNAL(editTextChanged(const QString &)), this, SLOT(slotFontSizeChangle(const QString &)));
}


ImageWindow::~ImageWindow()
{
    QMimeData *mp = const_cast<QMimeData *>(QApplication::clipboard()->mimeData());
    ShapeMimeData *data = dynamic_cast<ShapeMimeData*>( mp );
    if (data)
    {
        QApplication::clipboard()->clear();
    }

    delete ui;
}

void ImageWindow::initUndoAction()
{
    m_undoStack = new QUndoStack(this);
    m_undoStack->setUndoLimit(100);
    m_undoView = new QUndoView(m_undoStack);
    m_undoView->setWindowTitle(tr("Undo List"));
    m_undoView->hide();
    m_undoView->setAttribute(Qt::WA_QuitOnClose, false);

    QAction *pUndoAction = m_undoStack->createUndoAction(this, tr("&Undo"));
    pUndoAction->setShortcuts(QKeySequence::Undo);
    pUndoAction->setToolTip(QString::fromLocal8Bit("撤销"));
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/image/icons/undo.svg"), QSize(), QIcon::Normal, QIcon::Off);
    pUndoAction->setIcon(icon);

    QAction *pRedoAction = m_undoStack->createRedoAction(this, tr("&Redo"));
    pRedoAction->setShortcuts(QKeySequence::Redo);
    pRedoAction->setToolTip(QString::fromLocal8Bit("重做"));
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/image/icons/redo.svg"), QSize(), QIcon::Normal, QIcon::Off);
    pRedoAction->setIcon(icon1);
    ui->toolBar_undo->addAction(pUndoAction);
    ui->toolBar_undo->addAction(pRedoAction);
    connect(pUndoAction, SIGNAL(triggered(bool)), this, SLOT(on_actionUndo_triggered(bool)));
    connect(pRedoAction, SIGNAL(triggered(bool)), this, SLOT(on_actionRedo_triggered(bool)));
}

void ImageWindow::initDrawAction()
{
    // 绘图组
    if (NULL == m_pDrawGroup)
    {
        m_pDrawGroup = new QActionGroup(this);
    }
    m_pDrawGroup->addAction(ui->actionLine2point);    
    m_pDrawGroup->addAction(ui->actionArc3point);
    m_pDrawGroup->addAction(ui->actionArcCenter2point);
    m_pDrawGroup->addAction(ui->actionCircle3point);
    m_pDrawGroup->addAction(ui->actionCircleCenterPoint);
    m_pDrawGroup->addAction(ui->actionCircleCenterRadius);
    m_pDrawGroup->addAction(ui->actionPolyline);
    m_pDrawGroup->addAction(ui->actionRect);
    m_pDrawGroup->addAction(ui->actionRectangle);
    m_pDrawGroup->addAction(ui->actionText);
    m_pDrawGroup->addAction(ui->actionCurve);
    m_pDrawGroup->addAction(ui->actionErase);

    m_pDrawGroup->addAction(ui->actionSelect);     // 选取
    m_pDrawGroup->addAction(ui->actionMovePic);    // 平移
    m_pDrawGroup->addAction(ui->actionZoomInRect); // 局部放大

    ui->actionLine2point->setData(QSize(OT_Draw,EnShape_Line));
    ui->actionArc3point->setData(QSize(OT_Draw,EnShape_Arc));
    ui->actionArcCenter2point->setData(QSize(OT_Draw,EnShape_Arc_Center));
    ui->actionCircle3point->setData(QSize(OT_Draw,EnShape_Circle));
    ui->actionCircleCenterPoint->setData(QSize(OT_Draw,EnShape_Circle_Center));
    ui->actionPolyline->setData(QSize(OT_Draw,EnShape_Polygon));
    ui->actionRect->setData(QSize(OT_Draw, EnShape_Rect));
    ui->actionRectangle->setData(QSize(OT_Draw, EnShape_Rect1));
    ui->actionErase->setData(QSize(OT_Draw, EnShape_Erase_Path));
    ui->actionText->setData(QSize(OT_Draw, QGraphicsTextItem::Type));
    //ui->selectcolor->setData(QSize(OT_None, EnShape_Erase_Path));

    ui->actionSelect->setData(QSize(OT_Select,EnShape_None));
    ui->actionMovePic->setData(QSize(OT_Move,EnShape_None));
    ui->actionZoomInRect->setData(QSize(OT_ZoomRect,EnShape_None));

    connect(m_pDrawGroup, SIGNAL(triggered(QAction*)), this, SLOT(on_action_triggered(QAction*)));
    ui->actionSelect->setChecked(true);
}

void ImageWindow::on_actionOpenImage_triggered( bool checked )
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), ".", tr("Image Files (*.png *.jpg *jpeg *.bmp)"));
    if (!fileName.isEmpty())
    {
        ui->graphicsView->setBackgroundImage(fileName);
        ui->spinBox_width->setValue(m_pScene->sceneRect().toRect().width());
        ui->spinBox_height->setValue(m_pScene->sceneRect().toRect().height());
        slotStatusBarImageSizeChangle(m_pScene->sceneRect().toRect().width(), m_pScene->sceneRect().toRect().height());
        ui->spinBox_width->setEnabled(false);
        ui->spinBox_height->setEnabled(false);
        ui->toolButton_modifySize->hide();
    }
}

void ImageWindow::on_actionSaveImage_triggered( bool checked )
{
    QString strFilter;
    if (!m_save_file_name.isEmpty())
    {
        if (m_save_file_path.isEmpty())
        {
            m_save_file_path = m_save_file_name;
        }
        else
        {
            QFileInfo info(m_save_file_path);
            m_save_file_path.replace(info.fileName(), m_save_file_name);
        }
    }
    m_save_file_path = QFileDialog::getSaveFileName(this, "Save As", m_save_file_path, tr("JPEG (*.jpg);;BMP (*.bmp)"), &strFilter);
    if (!m_save_file_path.isEmpty())
    {
        if (strFilter == "BMP (*.bmp)" && m_save_file_path.indexOf(".bmp") == -1)
        {
            m_save_file_path += ".bmp";
        }
        else if (strFilter == "JPEG (*.jpg)" && m_save_file_path.indexOf(".jpg") == -1)
        {
            m_save_file_path += ".jpg";
        } 
        ui->graphicsView->saveImage(m_save_file_path);
    }
}

void ImageWindow::on_actionZoomIn_triggered( bool checked )
{
    ui->graphicsView->slot_zoomIn();
}

void ImageWindow::on_actionZoomOut_triggered( bool checked )
{
    ui->graphicsView->slot_zoomOut();
}

void ImageWindow::on_actionOriginalSize_triggered( bool checked )
{
    ui->graphicsView->setTransform(QTransform());
    // 状态栏显示比列
    slotStatusBarScale(ui->graphicsView->transform().m11());
}

void ImageWindow::on_actionFullView_triggered( bool checked )
{
    qDebug()<<ui->graphicsView->sceneRect();
    ui->graphicsView->fitInView(ui->graphicsView->sceneRect(), Qt::KeepAspectRatio);
    // 状态栏显示比列
    slotStatusBarScale(ui->graphicsView->transform().m11());
}

void ImageWindow::on_action_triggered( QAction * pAction)
{
    if (NULL == pAction || !pAction->isChecked())
    {
        return;
    }

    OPERATE_TYPE operateType = (OPERATE_TYPE)(pAction->data().toSize().width());
    ShapeType pathType = (ShapeType)(pAction->data().toSize().height());
    m_pScene->setOperateType(operateType);
    m_pScene->setDrawType(pathType);
}

void ImageWindow::on_actionSelectAll_triggered( bool checked )
{
    QPainterPath path;
    path.addRect(ui->graphicsView->scene()->sceneRect());
    ui->graphicsView->scene()->setSelectionArea(path);
}

void ImageWindow::slotStatusBarShowCoordinate( QPointF pos )
{
    QString str(QString::fromLocal8Bit("坐标 (%1, %2) "));
    str = str.arg(pos.x(),0,'f', 2).arg(pos.y(),0,'f', 2);
    m_statusBarCoordinate.setText(str);
}

void ImageWindow::slotStatusBarShowGary( QPoint pos )
{
    QString strGray(QString::fromLocal8Bit("阈值 : %1"));
    strGray = strGray.arg(ui->graphicsView->getImageGray(pos));
    m_statusBarGray.setText(strGray);
}

void ImageWindow::keyPressEvent( QKeyEvent *event )
{
    switch (event->key())
    {
    case Qt::Key_Up:
        break;
    case Qt::Key_Down:
        break;
    case Qt::Key_Left:
        break;
    case Qt::Key_Right:
        break;
    case Qt::Key_Plus:
        on_actionZoomIn_triggered(true);
        break;
    case Qt::Key_Minus:
        on_actionZoomOut_triggered(true);
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
        on_actionFullView_triggered(true);
        break;
    case Qt::Key_Period:
        on_actionOriginalSize_triggered(true);
        break;
    case Qt::Key_Tab:
        break;
    case Qt::Key_Backtab:
        break;
    case Qt::Key_Delete:
        on_actionDeleteSelect_triggered(true);
        break;
    default:
        QMainWindow::keyPressEvent(event);
        break;
    }
}

void ImageWindow::slotStatusBarScale( qreal scale)
{
    QString strScale = QString::number(scale*100);
    m_statusBarScale.setText(QString::fromLocal8Bit("比例:")+strScale+"%");
}

void ImageWindow::slotStatusBarImageSizeChangle( int width, int height)
{
    QString strImageSize = QString(QString::fromLocal8Bit("图像大小( %1 × %2 )")).arg(width).arg(height);
    m_statusBarImageSize.setText(strImageSize);
}

void ImageWindow::on_actionDeleteAll_triggered( bool checked )
{
    if (m_pScene->items().isEmpty())
    {
        return;
    }

    // 加入撤销队列
    QUndoCommand *addCommand = new DeleteCommand(m_pScene->items());
    m_undoStack->push(addCommand);
}

void ImageWindow::on_actionDeleteSelect_triggered( bool checked )
{
    if (m_pScene->selectedItems().isEmpty())
    {
        return;
    }

    // 加入撤销队列
    QUndoCommand *addCommand = new DeleteCommand(m_pScene->selectedItems());
    m_undoStack->push(addCommand);
}

void ImageWindow::updateActionStatus()
{
    static OPERATE_TYPE operateType = OT_None;
    if (operateType == m_pScene->operateType())
    {
        return;
    }
    operateType = m_pScene->operateType();
    switch(m_pScene->operateType())
    {
    case OT_Select:
        {
            ui->actionSelect->setChecked(true);
            // 多加这句,解决鼠标形状不更新的bug
            ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
            ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
            ui->graphicsView->setCursor(Qt::ArrowCursor);
            break;
        }
    case OT_Move:
        {
            ui->actionMovePic->setChecked(true);
            ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
            break;
        }
    case OT_ZoomRect:
        {
            ui->actionZoomInRect->setChecked(true);
            ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
            break;
        }
    case OT_Draw:
        {
            // 多加这句,解决鼠标形状不更新的bug
            ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
            ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
            ui->graphicsView->setCursor(Qt::CrossCursor);
            switch (m_pScene->drawShape())
            {
            case EnShape_Line:
                ui->actionLine2point->setChecked(true);
                break;
            case EnShape_Rect:
                ui->actionRect->setChecked(true);
                break;
            case EnShape_Rect1:
                ui->actionRectangle->setChecked(true);
                break;
            case EnShape_Polygon:
                ui->actionPolyline->setChecked(true);
                break;
            case EnShape_Arc:
                ui->actionArc3point->setChecked(true);
                break;
            case EnShape_Arc_Center:
                ui->actionArcCenter2point->setChecked(true);
                break;
            case EnShape_Circle:
                ui->actionCircle3point->setChecked(true);
                break;
            case EnShape_Circle_Center:
                ui->actionCircleCenterPoint->setChecked(true);
                break;
            default:
                break;
            }
            break;
        }
    default:
        break;
    }
}

void ImageWindow::on_actionMoveUp_triggered( bool checked )
{
    moveItems(0,-MoveDistance);
}

void ImageWindow::on_actionMoveDown_triggered( bool checked )
{
    moveItems(0,MoveDistance);
}

void ImageWindow::on_actionMoveRight_triggered( bool checked )
{
    moveItems(MoveDistance, 0);
}

void ImageWindow::on_actionMoveLeft_triggered( bool checked )
{
    moveItems(-MoveDistance, 0);
}


void ImageWindow::on_actionMoveUpPix_triggered( bool checked )
{
    moveItems(0, -MovePix);
}

void ImageWindow::on_actionMoveDownPix_triggered( bool checked )
{
    moveItems(0, MovePix);
}

void ImageWindow::on_actionMoveRightPix_triggered( bool checked )
{
    moveItems(MovePix,0);
}

void ImageWindow::on_actionMoveLeftPix_triggered( bool checked )
{
    moveItems(-MovePix,0);
}

void ImageWindow::moveItems( int dx, int dy )
{
    QList<QGraphicsItem*> selectItems = ui->graphicsView->scene()->selectedItems();
    if (selectItems.isEmpty())
    {
        return;
    }

    QList<QPointF> oldPosList;
    foreach (QGraphicsItem *item, selectItems)
    {
        oldPosList.push_back(item->scenePos());
        GraphicsShapeItem *pItem = dynamic_cast<GraphicsShapeItem*>(item);
        pItem->moveOffset(dx, dy);
    }

    // 磁性连接
    m_pScene->joinedShape();

    // 加入撤销队列
    QUndoCommand *addCommand = new MoveCommand(selectItems, oldPosList);
    m_undoStack->push(addCommand);
}

void ImageWindow::slotTimerHandle()
{
    if (!isActiveWindow())
    {
        return;
    }
    if (m_iMaxCount > m_pScene->items().size() || m_iMaxCount < 0)
    {
        if (!m_pDrawGroup->isEnabled())
        {
            m_pDrawGroup->setEnabled(true);
        }
    }
    else
    {
        if (m_pDrawGroup->isEnabled())
        {
            m_pDrawGroup->setEnabled(false);
        }
    }

    updateActionStatus();

    // 显示选中图形信息
    slot_showSelectItemData();
}

void ImageWindow::slot_showSelectItemData()
{
    QList<QGraphicsItem*> selectItems = ui->graphicsView->scene()->selectedItems();
    if (selectItems.size() == 1)
    {
        foreach (GraphicsShapeItem::DataType type, m_shapeDataFrameList.keys())
        {
            m_shapeDataFrameList[type]->hide();
        }

        //D_BeginPoint=0, D_EndPoint, D_Center, D_Radius, D_Length, D_Angle, D_Width, D_Height
        QGraphicsItem *pGItem = selectItems.first();
        if (pGItem->type() < pGItem->UserType)
        {
            return;
        }
        GraphicsShapeItem* pItem = dynamic_cast<GraphicsShapeItem*>(selectItems.first());
        StShapeType t;
        ui->lineEdit_shapeType->setText(t.typeMap[pItem->type()]);
        QPointF beginPoint = pItem->shapeData(GraphicsShapeItem::D_BeginPoint).toPointF();
        QPointF endPoint = pItem->shapeData(GraphicsShapeItem::D_EndPoint).toPointF();
        QPointF center = pItem->shapeData(GraphicsShapeItem::D_Center).toPointF();
        double radius = pItem->shapeData(GraphicsShapeItem::D_Radius).toDouble();
        double length = pItem->shapeData(GraphicsShapeItem::D_Length).toDouble();
        double angle = pItem->shapeData(GraphicsShapeItem::D_Angle).toDouble();
        double height = pItem->shapeData(GraphicsShapeItem::D_Height).toDouble();
        double width = pItem->shapeData(GraphicsShapeItem::D_Width).toDouble();

        QString strBeginPos = QString("%1, %2").arg(beginPoint.x(),0,'f',2).arg(beginPoint.y(),0,'f',2);
        QString strEndPos = QString("%1, %2").arg(endPoint.x(),0,'f',2).arg(endPoint.y(),0,'f',2);
        QString strCenterPos = QString("%1, %2").arg(center.x(),0,'f',2).arg(center.y(),0,'f',2);
        ui->lineEdit_beginPoint->setText(strBeginPos);
        ui->lineEdit_endPoint->setText(strEndPos);
        ui->lineEdit_center->setText(strCenterPos);
        ui->doubleSpinBox_angle->setValue(angle);
        ui->doubleSpinBox_len->setValue(length);
        ui->doubleSpinBox_radius->setValue(radius);
        ui->doubleSpinBox_height->setValue(height);
        ui->doubleSpinBox_width->setValue(width);
        slotStatusBarImageSizeChangle(width, height);

        // 选中的图形
        switch (pItem->type())
        {
        case EnShape_Line:
            {
                ui->frame_beginPoint->show();
                ui->frame_endPoint->show();
                ui->frame_Angle->show();
                ui->frame_length->show();
                break;
            }
        case EnShape_Arc:
        case EnShape_Arc_Center:
            {
                ui->frame_beginPoint->show();
                ui->frame_endPoint->show();
                ui->frame_Angle->show();
                ui->frame_center->show();
                ui->frame_length->show();
                ui->frame_radius->show();
                break;
            }
        case EnShape_Circle:
            {
                ui->frame_beginPoint->show();
                ui->frame_endPoint->show();
                ui->frame_center->show();
                ui->frame_length->show();
                ui->frame_radius->show();
                break;
            }
        case EnShape_Circle_Center:
            {
                ui->frame_endPoint->show();
                ui->frame_center->show();
                ui->frame_length->show();
                ui->frame_radius->show();
                break;
            }
        case EnShape_Rect:
        case EnShape_Rect1:
            {
                ui->frame_beginPoint->show();
                ui->frame_endPoint->show();
                ui->frame_center->show();
                ui->frame_width->show();
                ui->frame_height->show();
                break;
            }
        case EnShape_Polygon:
            {
                ui->frame_beginPoint->show();
                ui->frame_endPoint->show();
                ui->frame_length->show();;
                break;
            }
            
        }
    }
    else
    {
        ui->frame_type->hide();
        ui->frame_beginPoint->hide();
        ui->frame_endPoint->hide();
        ui->frame_Angle->hide();
        ui->frame_center->hide();
        ui->frame_length->hide();
        ui->frame_radius->hide();
        ui->frame_width->hide();
        ui->frame_height->hide();
    }
}

void ImageWindow::on_action_color_triggered(bool checked)
{
    QColorDialog dlg;
    dlg.exec();
    QColor color = dlg.selectedColor();
    m_pScene->setShapeColor(color);
}

void ImageWindow::on_checkBox_arrow_stateChanged(int state)
{
    QList<QGraphicsItem*> selectItems = ui->graphicsView->scene()->items();
    foreach (QGraphicsItem *pItem, selectItems)
    {
        GraphicsShapeItem *pShape = dynamic_cast<GraphicsShapeItem*>(pItem);
        if (pShape)
        {
            pShape->showArrow(state == Qt::Checked);
        }
    }

    m_pScene->update();
}


void ImageWindow::on_checkBox_grid_stateChanged( int state )
{
    ui->graphicsView->showBackGroundGrid(state == Qt::Checked);
}

void ImageWindow::on_actionClearImage_triggered( bool checked )
{
    ui->graphicsView->clearBackgroundImage();
    ui->spinBox_width->setValue(m_pScene->sceneRect().toRect().width());
    ui->spinBox_height->setValue(m_pScene->sceneRect().toRect().height());
    slotStatusBarImageSizeChangle(m_pScene->sceneRect().toRect().width(), m_pScene->sceneRect().toRect().height());
    ui->spinBox_width->setEnabled(true);
    ui->spinBox_height->setEnabled(true);
    ui->toolButton_modifySize->show();
}

void ImageWindow::showEvent( QShowEvent *e )
{
    if (!m_timer.isActive())
    {
        m_timer.start(100);
    }

    ui->spinBox_width->setValue(m_pScene->sceneRect().toRect().width());
    ui->spinBox_height->setValue(m_pScene->sceneRect().toRect().height());
    slotStatusBarImageSizeChangle(m_pScene->sceneRect().toRect().width(), m_pScene->sceneRect().toRect().height());
}

void ImageWindow::hideEvent( QShowEvent *e )
{
    if (m_timer.isActive())
    {
        m_timer.stop();
    }
}

void ImageWindow::on_toolButton_modifySize_clicked()
{
    int width = ui->spinBox_width->value();
    int height = ui->spinBox_height->value();
    slotStatusBarImageSizeChangle(width, height);
    m_pScene->setSceneRect(0,0,width, height);
}

void ImageWindow::initShapeDataFrame()
{
    //    enum DataType{ D_BeginPoint=0, D_EndPoint, D_Center, D_Radius, D_Length, D_Angle, D_Width, D_Height };
    m_shapeDataFrameList[GraphicsShapeItem::D_BeginPoint] = ui->frame_beginPoint;
    m_shapeDataFrameList[GraphicsShapeItem::D_EndPoint] = ui->frame_endPoint;
    m_shapeDataFrameList[GraphicsShapeItem::D_Center] = ui->frame_center;
    m_shapeDataFrameList[GraphicsShapeItem::D_Radius] = ui->frame_radius;
    m_shapeDataFrameList[GraphicsShapeItem::D_Length] = ui->frame_length;
    m_shapeDataFrameList[GraphicsShapeItem::D_Angle] = ui->frame_Angle;
    m_shapeDataFrameList[GraphicsShapeItem::D_Width] = ui->frame_width;
    m_shapeDataFrameList[GraphicsShapeItem::D_Height] = ui->frame_height;
}

void ImageWindow::slotStatusBarSelectCount()
{
    int count = m_pScene->selectedItems().count();
    m_statusBarSelectCount.setText(QString::fromLocal8Bit("选中个数:")+QString::number(count));
    sigItemSelectChanged(m_pScene->getItemSelect());
}

void ImageWindow::on_actionCopyShape_triggered( bool checked )
{
    ShapeMimeData * data = new ShapeMimeData( m_pScene->selectedItems() );
    QApplication::clipboard()->setMimeData(data);
}

void ImageWindow::on_actionCutShape_triggered( bool checked )
{
    ShapeMimeData * data = new ShapeMimeData( m_pScene->selectedItems() );
    QApplication::clipboard()->setMimeData(data);
    foreach (QGraphicsItem *pItem, m_pScene->selectedItems())
    {
        ui->graphicsView->scene()->removeItem(pItem);
    }
}

void ImageWindow::on_actionPasteShape_triggered( bool checked )
{
    QMimeData *mp = const_cast<QMimeData *>(QApplication::clipboard()->mimeData());
    ShapeMimeData *data = dynamic_cast<ShapeMimeData*>( mp );
    if(NULL == data) return;
    if (data->items().isEmpty()) return;

    m_pScene->clearSelection();
    QList<QGraphicsItem*> addItems;
    foreach (QGraphicsItem * item , data->items())
    {  
        GraphicsShapeItem *sp = qgraphicsitem_cast<GraphicsShapeItem*>(item);  
        QGraphicsItem * copy = sp->copy();
        if ( copy )
        {
            addItems.push_back(copy);
            m_pScene->addItem(copy);
            copy->setPos(item->pos());
            copy->setSelected(true);
            copy->moveBy(20,20);
        }
    }

    QUndoCommand *addCommand = new AddCommand(addItems);
    m_undoStack->push(addCommand);
}

void ImageWindow::on_actionSelectNext_triggered( bool checked )
{
    m_pScene->focusNext(true);
}

void ImageWindow::on_actionSelectPre_triggered( bool checked )
{
    m_pScene->focusNext(false);
}

void ImageWindow::on_actionCenterCross_triggered( bool checked )
{
    showCross(checked);
}

void ImageWindow::on_actionUndoStack_triggered( bool checked )
{
    if (m_undoView)
    {
        m_undoView->show();
    }
}

bool ImageWindow::init()
{
    return true;
}

bool ImageWindow::deinit()
{
    return true;
}

bool ImageWindow::showWindow(QWidget* parent, QRect& rect,bool bPopFlag)
{
    if (parent != NULL)
    {
        this->setParent(parent);

    }
    if (bPopFlag)
    {
        QWidget::setWindowFlags(Qt::Dialog);
        if(parent != NULL)
        {
            QPoint point (parent->geometry().center() - QPoint (this->width() / 2,
                                                                this->height() / 2));
            move(point);
        }
    }
    else
    {
        resize(rect.width(), rect.height());
        move(rect.x(), rect.y());
    }
    show();
    return true;
}

bool ImageWindow::hideWindow()
{
    hide();
    return true;
}


void ImageWindow::fitSize(const QRect& rect)
{
    resize(rect.width(), rect.height());
}

bool ImageWindow::notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam)
{
    qDebug("%s:%d(%s) event is %s, wparam is %d, lparam is %d",
           __FILE__, __LINE__, __FUNCTION__, qPrintable(event), wparam, lparam);


    return true;
}



void ImageWindow::InitUI()
{
}

bool ImageWindow::ReadConfig()
{
    return true;
}

void ImageWindow::SetLanguage(QString strlanguage)
{
    QString strModule = CommonUtils::getPluginLibraryPath();
    QString strLangFilePath  = strModule + '/' + PLUGIN_PATH+ '/'+strlanguage+LANG_POSTFIX;
    qDebug("%s:%d(%s) LangFile is \"%s\" ",
           __FILE__, __LINE__, __FUNCTION__, qPrintable(strLangFilePath));


    if (m_translator.load(strLangFilePath))
    {
        qApp->installTranslator(&m_translator);

        ui->retranslateUi(this);
    }
    else
    {
        qWarning("%s:%d(%s) translator load LangFile\"%s\" failed, or is not exist",
                 __FILE__, __LINE__, __FUNCTION__, qPrintable(strLangFilePath));
    }
}

void ImageWindow::showPictureInLabel( QString file_path )
{
    if (!file_path.isEmpty())
    {
        ui->graphicsView->setBackgroundImage(file_path);
        ui->spinBox_width->setValue(m_pScene->sceneRect().toRect().width());
        ui->spinBox_height->setValue(m_pScene->sceneRect().toRect().height());
        slotStatusBarImageSizeChangle(m_pScene->sceneRect().toRect().width(), m_pScene->sceneRect().toRect().height());
        ui->spinBox_width->setEnabled(false);
        ui->spinBox_height->setEnabled(false);
        ui->toolButton_modifySize->hide();
        ui->graphicsView->update();
    }
}

void ImageWindow::showPictureInLabel( unsigned char *pixelsR, unsigned char *pixelsG, unsigned char *pixelsB,long width, long height )
{
    if (pixelsR == NULL || width <= 0 || height <= 0)
    {
        on_actionClearImage_triggered(true);
        return;
    }
    int iWidth = ui->spinBox_width->value();
    int iHeight = ui->spinBox_height->value();
    //int iWidth = ui->graphicsView->width();
    //int iHeight = ui->graphicsView->height();
    ui->graphicsView->showPictureInLabel(pixelsR, pixelsG, pixelsB, width, height);
    ui->spinBox_width->setValue(m_pScene->sceneRect().toRect().width());
    ui->spinBox_height->setValue(m_pScene->sceneRect().toRect().height());
    ui->spinBox_width->setEnabled(false);
    ui->spinBox_height->setEnabled(false);
    ui->toolButton_modifySize->hide();
    if (iWidth != width || iHeight != height)
    {
        slotStatusBarImageSizeChangle(width, height);
        if (iWidth > 0 && iHeight > 0)
        {
            on_actionFullView_triggered(true);
        }
    }
    QList<QRectF> listRect;
    listRect.push_back(ui->graphicsView->sceneRect());
    ui->graphicsView->updateScene(listRect);
}

void ImageWindow::adjustPictureSize()
{

}

void ImageWindow::hiddenButton( int iHidden )
{  
    if (iHidden == VS_IMAGE_SHOW_SHOW_SHOW_ALL)
    {
        bool ShowAll = true;
        ui->toolBar_itemMove->setVisible(ShowAll);
        ui->toolBar_edit->setVisible(ShowAll);
        //ui->checkBox_cross->setVisible(ShowAll);
        ui->toolBar_imageZoom->setVisible(ShowAll);
        ui->toolBar_draw->setVisible(ShowAll);
        ui->toolBar_undo->setVisible(ShowAll);
        ui->toolBar_size->setVisible(ShowAll);
        ui->statusbar->setVisible(ShowAll);
        ui->toolBar_property->setVisible(ShowAll);
    }
    else
    {
        if (iHidden & VS_IMAGE_SHOW_SHOW_ITEM_EDIT)
        {
            ui->toolBar_edit->setVisible(true);
            //ui->toolBar_property->setVisible(true);
        }
        if (iHidden & VS_IMAGE_SHOW_SHOW_ITEM_MOVE)
        {
            ui->toolBar_itemMove->setVisible(true);
        }
        if (iHidden & VS_IMAGE_SHOW_SHOW_ITEM_UNDO)
        {
            ui->toolBar_undo->setVisible(true);
        }
        if (iHidden & VS_IMAGE_SHOW_SHOW_ITEM_DRAW)
        {
            ui->toolBar_draw->setVisible(true);
        }
        else
        {
            bool isHaveItem = false;
            if ((iHidden & VS_IMAGE_SHOW_SHAPE_LINE))
            {
                ui->actionLine2point->setVisible(true);
                isHaveItem = true;
            }
            else
            {
                ui->actionLine2point->setVisible(false);
            }
            if (iHidden & VS_IMAGE_SHOW_SHAPE_POLYLINE)
            {
                ui->actionPolyline->setVisible(true);
                isHaveItem = true;
            }
            else
            {
                ui->actionPolyline->setVisible(false);
            }
            
            if (iHidden & VS_IMAGE_SHOW_SHAPE_ARC2)
            {
                ui->actionArcCenter2point->setVisible(true);
                isHaveItem = true;
            }
            else
            {
                ui->actionArcCenter2point->setVisible(false);
            }
            if (iHidden & VS_IMAGE_SHOW_SHAPE_ARC3)
            {
                ui->actionArc3point->setVisible(true);
                isHaveItem = true;
            }
            else
            {
                ui->actionArc3point->setVisible(false);
            }
            if (iHidden & VS_IMAGE_SHOW_SHAPE_RECT)
            {
                ui->actionRect->setVisible(true);
                isHaveItem = true;
            }
            else
            {
                ui->actionRect->setVisible(false);
            }
            if (iHidden & VS_IMAGE_SHOW_SHAPE_RECT1)
            {
                ui->actionRectangle->setVisible(true);
                isHaveItem = true;
            }
            else
            {
                ui->actionRectangle->setVisible(false);
            }
            if (iHidden & VS_IMAGE_SHOW_SHAPE_CIRCLE3)
            {
                ui->actionCircle3point->setVisible(true);
                isHaveItem = true;
            }
            else
            {
                ui->actionCircle3point->setVisible(false);
            }
            if (iHidden & VS_IMAGE_SHOW_SHAPE_CIRCLECENTER)
            {
                ui->actionCircleCenterPoint->setVisible(true);
                isHaveItem = true;
            }
            else
            {
                ui->actionCircleCenterPoint->setVisible(false);
            }
            if (iHidden & VS_IMAGE_SHOW_SHAPE_ERASE)
            {
                ui->actionErase->setVisible(true);
                isHaveItem = true;
            }
            else
            {
                ui->actionErase->setVisible(false);
            }
            if (isHaveItem)
            {
                ui->toolBar_draw->setVisible(true);
            }
        }
        if (iHidden & VS_IMAGE_SHOW_SHOW_ITEM_ATTRI)
        {
            ui->toolBar_size->setVisible(true);//隐藏属性，只保留状态栏
        }
        if (iHidden & VS_IMAGE_SHOW_SHOW_IMAGE_ZOOM)
        {
            ui->toolBar_imageZoom->setVisible(true);
        }
        if (iHidden & VS_IMAGE_SHOW_SHOW_STATUS)
        {
            ui->statusbar->setVisible(true);//隐藏所有,包括状态栏;
        }
    }
}

void ImageWindow::showToolBar(int iHide)
{
    if (iHide & XTV_IMAGE_SHOW::VS_TOOLBAR_EDIT)
    {
        ui->toolBar_edit->setVisible(true);
    }
    else
    {
        ui->toolBar_edit->setVisible(false);
    }
    if (iHide & XTV_IMAGE_SHOW::VS_TOOLBAR_ITEMMOVE)
    {
        ui->toolBar_itemMove->setVisible(true);
    }
    else
    {
        ui->toolBar_itemMove->setVisible(false);
    }
    if (iHide & XTV_IMAGE_SHOW::VS_TOOLBAR_UNDO)
    {
        ui->toolBar_undo->setVisible(true);
    }
    else
    {
        ui->toolBar_undo->setVisible(false);
    }
    if (iHide & XTV_IMAGE_SHOW::VS_TOOLBAR_DRAW)
    {
        ui->toolBar_draw->setVisible(true);
    }
    else
    {
        ui->toolBar_draw->setVisible(false);
    }
    if (iHide & XTV_IMAGE_SHOW::VS_TOOLBAR_SIZE)
    {
        ui->toolBar_size->setVisible(true);
    }
    else
    {
        ui->toolBar_size->setVisible(false);
    }
    if (iHide & XTV_IMAGE_SHOW::VS_TOOLBAR_IMAGEZOOM)
    {
        ui->toolBar_imageZoom->setVisible(true);
    }
    else
    {
        ui->toolBar_imageZoom->setVisible(false);
    }
    if (iHide & XTV_IMAGE_SHOW::VS_TOOLBAR_PROPERTY)
    {
        ui->toolBar_property->setVisible(true);
    }
    else
    {
        ui->toolBar_property->setVisible(false);
    }
    if (iHide & XTV_IMAGE_SHOW::VS_TOOLBAR_MANUAL)
    {
        ui->toolBar_manual->setVisible(true);
    }
    else
    {
        ui->toolBar_manual->setVisible(false);
    }
}

void ImageWindow::showStatusBar(int iHide)
{
    if (iHide == XTV_IMAGE_SHOW::VS_STATUS_HIDDEN_ALL)
    {
        ui->statusbar->setVisible(false);
    }
    else
    {
        ui->statusbar->setVisible(true);
        if (iHide & XTV_IMAGE_SHOW::VS_STATUSBARIMAGESIZE)
        {
            m_statusBarImageSize.setVisible(true);
        }
        else
        {
            m_statusBarImageSize.setVisible(false);
        }
        if (iHide & XTV_IMAGE_SHOW::VS_STATUSBARCOORDINATE)
        {
            m_statusBarCoordinate.setVisible(true);
        }
        else
        {
            m_statusBarCoordinate.setVisible(false);
        }
        if (iHide & XTV_IMAGE_SHOW::VS_STATUSBARGRAY)
        {
            m_statusBarGray.setVisible(true);
        }
        else
        {
            m_statusBarGray.setVisible(false);
        }
        if (iHide & XTV_IMAGE_SHOW::VS_STATUSBARSCALE)
        {
            m_statusBarScale.setVisible(true);
        }
        else
        {
            m_statusBarScale.setVisible(false);
        }
        if (iHide & XTV_IMAGE_SHOW::VS_STATUSBARSELECTCOUNT)
        {
            m_statusBarSelectCount.setVisible(true);
        }
        else
        {
            m_statusBarSelectCount.setVisible(false);
        }
    }
}

void ImageWindow::showActionItem(int iHide)
{
    if (iHide == XTV_IMAGE_SHOW::VS_ACTION_HIDDEN_ALL)
    {
        ui->toolBar_draw->setVisible(false);
    }
    else
    {
        ui->toolBar_draw->setVisible(true);
        if (iHide & XTV_IMAGE_SHOW::VS_ACTION_LINE_2_POINT)
        {
            ui->actionLine2point->setVisible(true);
        }
        else
        {
            ui->actionLine2point->setVisible(false);
        }
        if (iHide & XTV_IMAGE_SHOW::VS_ACTIONPOLYLINE)
        {
            ui->actionPolyline->setVisible(true);
        }
        else
        {
            ui->actionPolyline->setVisible(false);
        }
        if (iHide & XTV_IMAGE_SHOW::VS_ACTIONARCCENTER2POINT)
        {
            ui->actionArcCenter2point->setVisible(true);
        }
        else
        {
            ui->actionArcCenter2point->setVisible(false);
        }
        if (iHide & XTV_IMAGE_SHOW::VS_ACTIONARC3POINT)
        {
            ui->actionArc3point->setVisible(true);
        }
        else
        {
            ui->actionArc3point->setVisible(false);
        }
        if (iHide & XTV_IMAGE_SHOW::VS_ACTIONRECT)
        {
            ui->actionRect->setVisible(true);
        }
        else
        {
            ui->actionRect->setVisible(false);
        }
        if (iHide & XTV_IMAGE_SHOW::VS_ACTIONRECTANGLE)
        {
            ui->actionRectangle->setVisible(true);
        }
        else
        {
            ui->actionRectangle->setVisible(false);
        }
        if (iHide & XTV_IMAGE_SHOW::VS_ACTIONCIRCLE3POINT)
        {
            ui->actionCircle3point->setVisible(true);
        }
        else
        {
            ui->actionCircle3point->setVisible(false);
        }
        if (iHide & XTV_IMAGE_SHOW::VS_ACTIONCIRCLECENTERPOINT)
        {
            ui->actionCircleCenterPoint->setVisible(true);
        }
        else
        {
            ui->actionCircleCenterPoint->setVisible(false);
        }
        if (iHide & XTV_IMAGE_SHOW::VS_ACTIONERASE)
        {
            ui->actionErase->setVisible(true);
        }
        else
        {
            ui->actionErase->setVisible(false);
        }
    }
}

bool ImageWindow::addShapeItem(const QVariant &param, double dWidth, bool bIsMoveEnable, int index)
{
    m_pScene->setLineWidth(dWidth);
    //if ((m_iMaxCount <= m_pScene->items().size() && m_iMaxCount > 0))
    //{
    //    return false;
    //}
    return m_pScene->addShapeItem(param, bIsMoveEnable, index);
}

QList<QVariant> ImageWindow::queryShapeItem()
{
    QList<QVariant> GraphList;
    QVariant value;
    QList<QGraphicsItem *> items = m_pScene->queryShapeItem();
    for (QList<QGraphicsItem *>::iterator it = items.begin(); it != items.end(); it++)
    {
        if ((*it)->type() >= (*it)->UserType)
        {
            GraphicsShapeItem *pItem = dynamic_cast<GraphicsShapeItem*>(*it);
            GraphList.push_back(pItem->shapeData());
        }
    }
    return GraphList;
}

bool ImageWindow::removeShapeItem( int index, bool clear /*= false*/ )
{
    return m_pScene->removeShapeItem(index, clear);
}

void ImageWindow::setRoiMaxCount( int iCount /*= -1*/ )
{
    m_iMaxCount = iCount;
}

void ImageWindow::slotItemMoveFinished()
{
    emit sigItemMoveFinished();
}

void ImageWindow::slotItemRBtDoubleCliecked( QPointF &point)
{
    emit sigItemRBtDoubleCliecked(point);
}

void ImageWindow::slotLineWidthChangle( const QString &strWidth )
{
    m_pScene->setLineWidth(strWidth.toFloat());
}

void ImageWindow::slotFontSizeChangle(const QString &strWidth)
{
    m_pScene->setFontSize(strWidth.toInt());
}

void ImageWindow::showAllButton(bool show)
{
    ui->actionLine2point->setVisible(show);
    ui->actionArcCenter2point->setVisible(show);
    ui->actionArc3point->setVisible(show);
    ui->actionCircle3point->setVisible(show);
    ui->actionOpenImage->setVisible(show);
    ui->actionClearImage->setVisible(show);
    ui->actionItemMoveLimitH->setVisible(show);
    ui->actionItemMoveLimitV->setVisible(show);
    ui->actionItemMoveLimitHV->setVisible(show);
    ui->actionMoveLeft->setVisible(show);
    ui->actionMoveUp->setVisible(show);
    ui->actionMoveRight->setVisible(show);
    ui->actionMoveDown->setVisible(show);

    ui->toolBar_itemMove->setVisible(show);
    ui->toolBar_edit->setVisible(show);
    ui->checkBox_arrow->setVisible(show);
    ui->checkBox_grid->setVisible(show);
    //ui->checkBox_cross->setVisible(show);
    ui->toolBar_imageZoom->setVisible(show);
    ui->toolBar_draw->setVisible(show);
    ui->toolBar_undo->setVisible(show);
    ui->toolBar_size->setVisible(show);
    ui->statusbar->setVisible(show);
    ui->toolBar_property->setVisible(show);
    ui->toolBar_manual->setVisible(show);
}

void ImageWindow::setManualWidget( QWidget *pWidget )
{
    if (NULL != pWidget)
    {
        ui->toolBar_manual->addWidget(pWidget);
        //this->addToolBar(Qt::TopToolBarArea,ui->toolBar_manual);
    }
}

void ImageWindow::on_actionClearRoi_triggered( bool checked )
{
    m_pScene->removeShapeItem(0, true);
    emit sigItemMoveFinished(); //item有变化， 通知更新
    m_pScene->update();
}

void ImageWindow::showCross( bool bShow, double dRadius /*= 0*/ )
{
    ui->graphicsView->showBackGroundCross(bShow, dRadius);
}

void ImageWindow::setItemSelect(int id)
{
    m_pScene->setItemSelect(id);
}

unsigned char * ImageWindow::getImageLabelPtr()
{
    return ui->graphicsView->getImageLabelPtr();
}

void ImageWindow::UpdateImage()
{
    ui->graphicsView->UpdateImage();
}

void ImageWindow::setSaveImageFileName(QString strName)
{
    m_save_file_name = strName;
}

void ImageWindow::resizeEvent(QResizeEvent *e)
{
    on_actionFullView_triggered(true);
}


