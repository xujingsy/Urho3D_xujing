#include "stdafx.h"
#include "ButtonsPanel.h"

ButtonsPanel::ButtonsPanel(QWidget* parent)
{
	m_pCurrentTool = NULL;
	m_pCurrentButton = NULL;

	vBoxLayout = new QVBoxLayout();
	vBoxLayout->setAlignment(Qt::AlignTop);
	vBoxLayout->setMargin(0);

	m_ButtonPanel = new QFrame();
	m_ButtonPanel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
	m_ButtonPanel->setLineWidth(1);
	m_ButtonPanel->setLayout(new QBoxLayout(QBoxLayout::LeftToRight));

	m_ContentPanel = new QFrame();
	m_ContentPanel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
	m_ContentPanel->setLineWidth(1);
	m_ContentPanel->setLayout(new QBoxLayout(QBoxLayout::LeftToRight));

	QScrollArea* scrollButtons = new QScrollArea(this);
	scrollButtons->setWidget(m_ButtonPanel);
	scrollButtons->setWidgetResizable(true);

	QScrollArea* scrollContent = new QScrollArea(this);
	scrollContent->setWidget(m_ContentPanel);
	scrollContent->setWidgetResizable(true);

	vBoxLayout->addWidget(scrollButtons);
	vBoxLayout->addWidget(scrollContent);
	vBoxLayout->setStretch(0,0);
	vBoxLayout->setStretch(1,1);

	this->setLayout(vBoxLayout);
}

void ButtonsPanel::AddButton(const char* buttonName,EditTool* pPanel)
{
	SButton button;
	button.Name = buttonName;
	button.pPanel = pPanel;

	m_buttons.push_back(button);
}

void ButtonsPanel::OnInitDialog()
{
	for(int i = 0;i < m_buttons.size();i ++)
	{
		SButton& btn = m_buttons[i];
		btn.pButton = new QPushButton(QString::fromLocal8Bit(btn.Name.c_str()));
		btn.pButton->resize(120,30);
		btn.pButton->setCheckable(true);
		
		//¼Óµ½ButtonPanel
		m_ButtonPanel->layout()->addWidget(btn.pButton);

		connect(btn.pButton,SIGNAL(clicked(bool)),this,SLOT(buttonClicked(bool)));
	}

	RecalcButtonsLayout();
}

void ButtonsPanel::buttonClicked(bool checked)
{
	QPushButton* who = (QPushButton*)sender();

	bool isCheck = who->isChecked();
	if(isCheck == true)
	{
		for(int i = 0;i < m_buttons.size();i ++)
		{
			if(m_buttons[i].pButton == who)
			{
				EditTool* pTool = m_buttons[i].pPanel;

				if(m_pCurrentTool != NULL)
				{
					m_pCurrentTool->setParent(NULL);
					m_pCurrentTool = NULL;
				}
				if(m_pCurrentButton != NULL && m_pCurrentButton != who)
				{
					m_pCurrentButton->setChecked(false);
				}

				m_pCurrentTool = pTool;
				m_pCurrentButton = who;

				m_ContentPanel->layout()->addWidget(m_pCurrentTool);
				//m_pCurrentTool->BeginEditParams(NULL);	//todo
				return;
			}
		}
	}
	else
	{
		if(m_pCurrentTool != NULL)
		{
			m_pCurrentTool->setParent(NULL);
			//m_pCurrentTool->parentWidget()->layout()->removeWidget(m_pCurrentTool);
			m_pCurrentTool = NULL;
		}
	}
}

void ButtonsPanel::RecalcButtonsLayout()
{
	for(int i = 0;i < m_buttons.size();i ++)
	{
		QPushButton* pButton = m_buttons[i].pButton;
		pButton->move((i % 2) * 100 + 30,(i / 2) * 40 + 10);
	}
}
