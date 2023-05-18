#include <QAxObject>
#include <QFile>
#include <QStringList>
#include <QDebug>
#include "uitoolbox/qexcel.h"
#define SAFE_DEL(_X_) \
    do \
    { \
        if(_X_ != NULL) \
        {   \
            delete _X_; \
            _X_ = NULL; \
        }   \
    }while(0);

QExcel::QExcel(QString xlsFilePath, QObject *parent):
    m_pexcel(NULL),
    m_pworkBooks(NULL),
    m_pworkBook(NULL),
    m_psheets(NULL),
    m_psheet(NULL)
{
    m_pexcel = new QAxObject("Excel.Application", parent);
    m_pworkBooks = m_pexcel->querySubObject("Workbooks");
    QFile file(xlsFilePath);
    if (file.exists())
    {
        m_pworkBooks->dynamicCall("Open(const QString&)", xlsFilePath);
        m_pworkBook = m_pexcel->querySubObject("ActiveWorkBook");
        m_psheets = m_pworkBook->querySubObject("WorkSheets");
    }
}

QExcel::~QExcel()
{
    close();
}

void QExcel::close()
{
    m_pexcel->dynamicCall("Quit()");
    SAFE_DEL(m_psheet);
    SAFE_DEL(m_psheets);
    SAFE_DEL(m_pworkBook);
    SAFE_DEL(m_pworkBooks) ;
    SAFE_DEL(m_pexcel);
}

QAxObject *QExcel::getWorkBooks()
{
    return m_pworkBooks;
}

QAxObject *QExcel::getWorkBook()
{
    return m_pworkBook;
}

QAxObject *QExcel::getWorkSheets()
{
    return m_psheets;
}

QAxObject *QExcel::getWorkSheet()
{
    return m_psheet;
}

void QExcel::selectSheet(const QString& sheetName)
{
    m_psheet = m_psheets->querySubObject("Item(const QString&)", sheetName);
}

void QExcel::deleteSheet(const QString& sheetName)
{
    if(m_psheets == NULL)
    {
        return;
    }
    QAxObject * a = m_psheets->querySubObject("Item(const QString&)", sheetName);
    a->dynamicCall("delete");
}

void QExcel::deleteSheet(int sheetIndex)
{
    if(m_psheets == NULL)
    {
        return;
    }
    QAxObject * a = m_psheets->querySubObject("Item(int)", sheetIndex);
    a->dynamicCall("delete");
}

void QExcel::selectSheet(int sheetIndex)
{
    if(m_psheet == NULL)
    {
        return;
    }
    m_psheet = m_psheets->querySubObject("Item(int)", sheetIndex);
}

void QExcel::setCellString(int row, int column, const QString& value)
{
    if(m_psheet == NULL)
    {
        return;
    }
    QAxObject *range = m_psheet->querySubObject("Cells(int,int)", row, column);
    range->dynamicCall("SetValue(const QString&)", value);
}

void QExcel::setCellFontBold(int row, int column, bool isBold)
{
    QString cell;
    cell.append(QChar(column - 1 + 'A'));
    cell.append(QString::number(row));

    if(m_psheet == NULL)
    {
        return;
    }
    QAxObject *range = m_psheet->querySubObject("Range(const QString&)", cell);
    range = range->querySubObject("Font");
    range->setProperty("Bold", isBold);
}

void QExcel::setCellFontSize(int row, int column, int size)
{
    QString cell;
    cell.append(QChar(column - 1 + 'A'));
    cell.append(QString::number(row));

    QAxObject *range = m_psheet->querySubObject("Range(const QString&)", cell);
    range = range->querySubObject("Font");
    range->setProperty("Size", size);
}

void QExcel::mergeCells(const QString& cell)
{
    QAxObject *range = m_psheet->querySubObject("Range(const QString&)", cell);
    range->setProperty("VerticalAlignment", -4108);//xlCenter
    range->setProperty("WrapText", true);
    range->setProperty("MergeCells", true);
}

void QExcel::mergeCells(int topLeftRow, int topLeftColumn, int bottomRightRow, int bottomRightColumn)
{
    QString cell;
    cell.append(QChar(topLeftColumn - 1 + 'A'));
    cell.append(QString::number(topLeftRow));
    cell.append(":");
    cell.append(QChar(bottomRightColumn - 1 + 'A'));
    cell.append(QString::number(bottomRightRow));

    QAxObject *range = m_psheet->querySubObject("Range(const QString&)", cell);
    range->setProperty("VerticalAlignment", -4108);//xlCenter
    range->setProperty("WrapText", true);
    range->setProperty("MergeCells", true);
}

QVariant QExcel::getCellValue(int row, int column)
{
    QAxObject *range = m_psheet->querySubObject("Cells(int,int)", row, column);
    return range->property("Value");
}

void QExcel::save()
{
     m_pworkBook->dynamicCall("Save()");
}

int QExcel::getSheetsCount()
{
    return m_psheets->property("Count").toInt();
}

QString QExcel::getSheetName()
{
    return m_psheet->property("Name").toString();
}

QString QExcel::getSheetName(int sheetIndex)
{
    QAxObject * a = m_psheets->querySubObject("Item(int)", sheetIndex);
    return a->property("Name").toString();
}

void QExcel::getUsedRange(int *topLeftRow, int *topLeftColumn, int *bottomRightRow, int *bottomRightColumn)
{
    QAxObject *usedRange = m_psheet->querySubObject("UsedRange");
    *topLeftRow = usedRange->property("Row").toInt();
    *topLeftColumn = usedRange->property("Column").toInt();

    QAxObject *rows = usedRange->querySubObject("Rows");
    *bottomRightRow = *topLeftRow + rows->property("Count").toInt() - 1;

    QAxObject *columns = usedRange->querySubObject("Columns");
    *bottomRightColumn = *topLeftColumn + columns->property("Count").toInt() - 1;
}

void QExcel::setColumnWidth(int column, int width)
{
    QString columnName;
    columnName.append(QChar(column - 1 + 'A'));
    columnName.append(":");
    columnName.append(QChar(column - 1 + 'A'));

    QAxObject * col = m_psheet->querySubObject("Columns(const QString&)", columnName);
    col->setProperty("ColumnWidth", width);
}

void QExcel::setCellTextCenter(int row, int column)
{
    QString cell;
    cell.append(QChar(column - 1 + 'A'));
    cell.append(QString::number(row));

    QAxObject *range = m_psheet->querySubObject("Range(const QString&)", cell);
    range->setProperty("HorizontalAlignment", -4108);//xlCenter
}

void QExcel::setCellTextWrap(int row, int column, bool isWrap)
{
    QString cell;
    cell.append(QChar(column - 1 + 'A'));
    cell.append(QString::number(row));

    QAxObject *range = m_psheet->querySubObject("Range(const QString&)", cell);
    range->setProperty("WrapText", isWrap);
}

void QExcel::setAutoFitRow(int row)
{
    QString rowsName;
    rowsName.append(QString::number(row));
    rowsName.append(":");
    rowsName.append(QString::number(row));

    QAxObject * rows = m_psheet->querySubObject("Rows(const QString &)", rowsName);
    rows->dynamicCall("AutoFit()");
}

void QExcel::insertSheet(QString sheetName)
{
    m_psheets->querySubObject("Add()");
    QAxObject * a = m_psheets->querySubObject("Item(int)", 1);
    a->setProperty("Name", sheetName);
}

void QExcel::mergeSerialSameCellsInAColumn(int column, int topRow)
{
    int a,b,c,rowsCount;
    getUsedRange(&a, &b, &rowsCount, &c);

    int aMergeStart = topRow, aMergeEnd = topRow + 1;

    QString value;
    while(aMergeEnd <= rowsCount)
    {
            value = getCellValue(aMergeStart, column).toString();
            while(value == getCellValue(aMergeEnd, column).toString())
            {
                    clearCell(aMergeEnd, column);
                    aMergeEnd++;
            }
            aMergeEnd--;
            mergeCells(aMergeStart, column, aMergeEnd, column);

            aMergeStart = aMergeEnd + 1;
            aMergeEnd = aMergeStart + 1;
    }
}

void QExcel::clearCell(int row, int column)
{
    QString cell;
    cell.append(QChar(column - 1 + 'A'));
    cell.append(QString::number(row));

    QAxObject *range = m_psheet->querySubObject("Range(const QString&)", cell);
    range->dynamicCall("ClearContents()");
}

void QExcel::clearCell(const QString& cell)
{
    QAxObject *range = m_psheet->querySubObject("Range(const QString&)", cell);
    range->dynamicCall("ClearContents()");
}

int QExcel::getUsedRowsCount()
{
    QAxObject *usedRange = m_psheet->querySubObject("UsedRange");
    int topRow = usedRange->property("Row").toInt();
    QAxObject *rows = usedRange->querySubObject("Rows");
    int bottomRow = topRow + rows->property("Count").toInt() - 1;
    return bottomRow;
}

void QExcel::setCellString(const QString& cell, const QString& value)
{
    QAxObject *range = m_psheet->querySubObject("Range(const QString&)", cell);
    range->dynamicCall("SetValue(const QString&)", value);
}

void QExcel::setCellFontSize(const QString &cell, int size)
{
    QAxObject *range = m_psheet->querySubObject("Range(const QString&)", cell);
    range = range->querySubObject("Font");
    range->setProperty("Size", size);
}

void QExcel::setCellTextCenter(const QString &cell)
{
    QAxObject *range = m_psheet->querySubObject("Range(const QString&)", cell);
    range->setProperty("HorizontalAlignment", -4108);//xlCenter
}

void QExcel::setCellFontBold(const QString &cell, bool isBold)
{
    QAxObject *range = m_psheet->querySubObject("Range(const QString&)", cell);
    range = range->querySubObject("Font");
    range->setProperty("Bold", isBold);
}

void QExcel::setCellTextWrap(const QString &cell, bool isWrap)
{
    QAxObject *range = m_psheet->querySubObject("Range(const QString&)", cell);
    range->setProperty("WrapText", isWrap);
}

void QExcel::setRowHeight(int row, int height)
{
    QString rowsName;
    rowsName.append(QString::number(row));
    rowsName.append(":");
    rowsName.append(QString::number(row));

    QAxObject * r = m_psheet->querySubObject("Rows(const QString &)", rowsName);
    r->setProperty("RowHeight", height);
}
