#ifndef RUNPOINTBTN_H
#define RUNPOINTBTN_H

#include <QPushButton>

class RunPointBtn : public QPushButton
{
    Q_OBJECT

    Q_PROPERTY(QString stationName READ stationName WRITE setStationName)
    Q_PROPERTY(QString pointName READ pointName WRITE setPointName)
    Q_PROPERTY(QString mask READ mask WRITE setMask)
    Q_PROPERTY(bool relative READ relative WRITE setRelative)
    Q_PROPERTY(bool enablePrompt READ enablePrompt WRITE setEnablePrompt)
    Q_PROPERTY(QString prompt READ prompt WRITE setPrompt)
    Q_PROPERTY(QString slotPreProcess READ slotPreProcess WRITE setSlotPreProcess)

public:
    RunPointBtn(QWidget *parent = 0);

    void setStationName(const QString &stationName) { m_stationName = stationName; }
    QString stationName() const { return m_stationName; }

    void setPointName(const QString &pointName) { m_pointName = pointName;}
    QString pointName() const { return m_pointName; }

    void setMask(const QString &mask) { m_mask = mask;}
    QString mask() const { return m_mask; }

    void setRelative(bool relative) { m_bRelative = relative; }
    bool relative() const { return  m_bRelative; }

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
    QString m_stationName;
    QString m_pointName;
    QString m_mask;
    bool m_bRelative;
    bool m_bEnablePrompt;
    QString m_prompt;
	QString m_slotPreProcess;
};

#endif
