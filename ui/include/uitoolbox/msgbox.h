#ifndef _MSGBOX_H
#define _MSGBOX_H
#include <QDialog>
#include "dllexport.h"

class QPlainTextEdit;
class RT_API MsgBox : public QDialog
{
public:
	static MsgBox *getInstance(QWidget *parent = NULL);
	static void information(const QString &text, bool bShowTime=true);
	static void warning(const QString &text, bool bShowTime=true);
	static void critical(const QString &text, bool bShowTime=true);

private:
	MsgBox(QWidget *parent = NULL);
	~MsgBox(void);
	void showEvent(QShowEvent * event);
    void hideEvent(QHideEvent * event);
	void initDialog();

private:
	static MsgBox *m_pInstance;
	QPlainTextEdit *m_textEdit;
    static QString m_lastText;
};

#endif
