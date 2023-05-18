#include "uitoolbox/keyvaluebox.h"
KeyValueBox::KeyValueBox(QWidget *parent) : QGroupBox(parent)
{
	m_GridLayout = new QGridLayout();
	m_bHorizontal = true;
	this->setLayout(m_GridLayout);
	connect(this,SIGNAL(setTextSignal(QLabel*,QString)),this,SLOT(setTextSlot(QLabel*,QString)));
	connect(this, SIGNAL(addItemSignal(QString,QString)), this, SLOT(addItemSlot(QString,QString)));
	connect(this, SIGNAL(delItemSignal(QString)), this, SLOT(delItemSlot(QString)));
}

KeyValueBox::KeyValueBox( const QString title, S_Layout layout,QWidget *parent /*= 0*/ )
						: QGroupBox(parent)
{
	this->setTitle(title);

	m_GridLayout = new QGridLayout();
	if (layout == Horizontal)
	{
		m_bHorizontal = true;
	}
	else if (layout == Vertical)
	{
		m_bHorizontal = false;
	}
	this->setLayout(m_GridLayout);
	connect(this,SIGNAL(setTextSignal(QLabel*,QString)),this,SLOT(setTextSlot(QLabel*,QString)));
	connect(this, SIGNAL(addItemSignal(QString,QString)), this, SLOT(addItemSlot(QString,QString)));
	connect(this, SIGNAL(delItemSignal(QString)), this, SLOT(delItemSlot(QString)));
}

KeyValueBox::~KeyValueBox()
{
	try
	{
        vector<BoxItem*>::iterator iter = m_vecBoxItem.begin();
        for ( iter = m_vecBoxItem.begin(); iter != m_vecBoxItem.end();iter++)
        {
            if (*iter != NULL)
            {
                delete *iter;
                *iter = NULL;
            }
        }
	}
	catch (...)
	{
        qDebug( "Erro:throw exception '...' in destructor  ,%s ,%s", __FILE__,__LINE__);
	}
}

void KeyValueBox::addItem( const QString key,const QString value )
{
	emit addItemSignal(key,value);
}

bool KeyValueBox::setItemText( const QString key,const QString  value )
{
	vector<BoxItem*>::iterator iter = m_vecBoxItem.begin();
	for ( iter = m_vecBoxItem.begin(); iter != m_vecBoxItem.end();iter++)
	{
		if ((*iter)->keyLabel == NULL)
		{
			continue;
		}
		if ( (*iter)->keyLabel->text().compare(key) == 0)
		{
			emit setTextSignal((*iter)->valueLabel,value);
			return true;
		}
	}
	return false;
}

void KeyValueBox::delItem( const QString key )
{
	emit delItemSignal(key);
}

void KeyValueBox::setLayout( QLayout* layout )
{
	return QGroupBox::setLayout(layout);
}

void KeyValueBox::setTextSlot( QLabel* label,QString text )
{
	if (label != NULL)
	{
		label->setText(text);
	}
}

void KeyValueBox::addItemSlot( const QString key,const QString value )
{
	if (m_GridLayout == NULL)
	{
        return;
	}
    vector<BoxItem*>::iterator iter = m_vecBoxItem.begin();
	for ( iter = m_vecBoxItem.begin(); iter != m_vecBoxItem.end();iter++)
	{
		if ((*iter)->keyLabel == NULL)
		{
			continue;
		}
		if ( (*iter)->keyLabel->text().compare(key) == 0)
		{
			emit setTextSignal((*iter)->valueLabel,value);
			return;
		}
	}
	BoxItem* item = new BoxItem(key,value);
	if (m_bHorizontal)
	{
		m_GridLayout->addWidget(item->keyLabel,0,m_GridLayout->columnCount());
		m_GridLayout->addWidget(item->valueLabel,0,m_GridLayout->columnCount());
	}
	else
	{
		m_GridLayout->addWidget(item->keyLabel,m_GridLayout->rowCount(),0);
		m_GridLayout->addWidget(item->valueLabel,m_GridLayout->rowCount()-1,1);
	}
	m_vecBoxItem.push_back(item); 
}

void KeyValueBox::delItemSlot(QString key)
{
	vector<BoxItem*>::iterator iter = m_vecBoxItem.begin();
	for ( iter = m_vecBoxItem.begin(); iter != m_vecBoxItem.end();iter++)
	{	
		if ((*iter)->keyLabel == NULL)
		{
			continue;
		}
		if ( (*iter)->keyLabel->text().compare(key) == 0)
		{
			delete *iter;
			*iter = NULL;
			m_vecBoxItem.erase(iter);
			return ;
		}
	}
	return ;
}

