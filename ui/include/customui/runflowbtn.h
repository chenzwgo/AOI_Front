#ifndef RUNFLOWBTN_H
#define RUNFLOWBTN_H

#include <QPushButton>

class RunFlowBtn : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QString groupName READ groupName WRITE setGroupName)
    Q_PROPERTY(QString flowName READ flowName WRITE setFlowName)
    Q_PROPERTY(QString beginStep READ beginStep WRITE setBeginStep)
    Q_PROPERTY(QString endStep READ endStep WRITE setEndStep)
    Q_PROPERTY(bool enablePrompt READ enablePrompt WRITE setEnablePrompt)
    Q_PROPERTY(QString prompt READ prompt WRITE setPrompt)
    Q_PROPERTY(QString slotPreProcess READ slotPreProcess WRITE setSlotPreProcess)

public:
    RunFlowBtn(QWidget *parent = 0);

    void setGroupName(const QString &groupName) { m_groupName = groupName;}
    QString groupName() const { return m_groupName; }

    void setFlowName(const QString &flowName) { m_flowName = flowName; setToolTip(m_flowName);}
    QString flowName() const { return m_flowName; }

    void setBeginStep(const QString &beginStep) { m_beginStep = beginStep;}
    QString beginStep() const { return m_beginStep; }

    void setEndStep(const QString &endStep) { m_endStep = endStep;}
    QString endStep() const { return m_endStep; }

    void setEnablePrompt(bool enablePrompt) { m_bEnablePrompt = enablePrompt; }
    bool enablePrompt() const { return  m_bEnablePrompt; }

    void setPrompt(const QString &prompt) { m_prompt = prompt;}
    QString prompt() const { return m_prompt; }
    QString slotPreProcess() {return m_slotPreProcess;}
    void setSlotPreProcess(const QString text) {m_slotPreProcess = text;}

    // 预处理
    bool preProcess();

protected:
    void showEvent(QShowEvent *);

signals:
    void sigPreProcess(bool&); // 预处理

private:
    QString m_groupName;
    QString m_flowName;
    QString m_beginStep;
    QString m_endStep;
    bool m_bEnablePrompt;
    QString m_prompt;
	QString m_slotPreProcess;
};

#endif
