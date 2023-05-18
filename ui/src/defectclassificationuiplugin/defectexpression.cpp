#include "defectexpression.h"
#include "ui_defectexpression.h"
#include "scriptLexer.h"
#include "Features.h"

#include <QTextBlock>
#include <QMessageBox>
#include <QStandardItemModel>

DefectExpression::DefectExpression(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DefectExpression)
{
    ui->setupUi(this);
    initUI();
}

DefectExpression::~DefectExpression()
{
    delete ui;
}

void DefectExpression::initUI()
{
    const map<string, stFeaturesInfo>* pBaseMap = CFeatures::getInstance()->getBaseFeaturesMap();

    QStandardItemModel *model = new QStandardItemModel();  
    ui->tableView_baseFeaturews->setModel(model); 
    ui->tableView_baseFeaturews->horizontalHeader()->setResizeContentsPrecision(QHeaderView::ResizeToContents);
    ui->tableView_baseFeaturews->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_baseFeaturews->verticalHeader()->hide();
    ui->tableView_baseFeaturews->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_baseFeaturews->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_baseFeaturews->setSelectionMode ( QAbstractItemView::SingleSelection);

    model->setHorizontalHeaderLabels(QStringList()<< tr("Name")<<tr("Unit")<<tr("Default")<<tr("Description"));
       
    map<string, stFeaturesInfo>::const_iterator it = pBaseMap->begin();
    while(it != pBaseMap->end())
    {
        QList<QStandardItem*> aitems;
        aitems.push_back(new QStandardItem(QString::fromStdString(it->second.strFeaturesName)));
        aitems.push_back(new QStandardItem(QString::fromStdString(it->second.strFeaturesUnit)));
        aitems.push_back(new QStandardItem(QString::fromStdString(it->second.strFeaturesDefaultValue)));
        aitems.push_back(new QStandardItem(QString::fromStdString(it->second.strFeaturesDescription)));
        model->appendRow(aitems);
        it++;
    }

}

void DefectExpression::on_pushButton_Expression_clicked()
{
    int icount = ui->plainTextEdit_Expression->blockCount();
    vector<Token>  Tokens;
    string strErr;
    double dRes = 0.0;
    for(int index =0 ; index <icount ;index++)
    {
        QString strText = ui->plainTextEdit_Expression->document()->findBlockByNumber(index).text();
        if(strText.length() == 0)
        {
            continue;
        }
        bool bflag = ScriptLexer::GetTokensOneLine(strText.append("\n").toStdString(),Tokens,strErr);
        if (!bflag)
        {
            QMessageBox::warning(this,"warning",QString::fromStdString(strErr));
        }
    }
    ScriptLexer* pScriptLexer = new ScriptLexer(Tokens);
    int iRet = pScriptLexer->parser();
    if (iRet != NOERROR)
    {
        string strerr = "parser return false  is " +pScriptLexer->getErrString();
        QMessageBox::warning(this,"warning",QString::fromStdString(strerr));
    }
    iRet = pScriptLexer->evaluation(dRes);
    if (iRet != NOERROR)
    {
        string strerr = "evaluation return false  is " +pScriptLexer->getErrString();
        QMessageBox::warning(this,"warning",QString::fromStdString(strerr));
    }
    ui->label_Result->setText(QString::number(dRes,'f',4));
}
