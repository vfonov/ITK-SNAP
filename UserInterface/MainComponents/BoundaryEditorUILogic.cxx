//BoundaryEditorUILogic.cxx

#include "BoundaryEditorUILogic.h"
#include "UserInterfaceBase.h"
#include "GlobalState.h"
#include "IRISApplication.h"
#include "IRISImageData.h"
#include <iostream>

using namespace std;

BoundaryEditorUILogic::BoundaryEditorUILogic(void) : 
	m_Parent(NULL), m_Driver(NULL), m_GlobalState(NULL)
{
}

BoundaryEditorUILogic::~BoundaryEditorUILogic()
{
}

void BoundaryEditorUILogic::Register(UserInterfaceBase *parent)
{
  m_Parent = parent;
  m_Driver = parent->GetDriver();
  m_GlobalState = m_Driver->GetGlobalState();
}

void BoundaryEditorUILogic::DisplayWindow(void)
{
	settings = m_GlobalState->GetBoundaryLayerSettings(); //Reinitialize settings

	this->UpdateLabelBrowsers();
	m_MainWin->show();
}

void BoundaryEditorUILogic::UpdateLabelBrowsers(void)
{	
	//Iterate through all the valid labels and put them in their homes
	m_NormalLabelBrowser->clear();
	m_BoundaryLabelBrowser->clear();
	
	for (size_t i = 0; i < MAX_COLOR_LABELS; ++i)
	{
		ColorLabel label = m_Driver->GetColorLabelTable()->GetColorLabel(i);
		if (label.IsValid())
		{
			if(settings.LabelInBoundary(i))
				m_BoundaryLabelBrowser->add(label.GetLabel(), (void *)i);
			else
				m_NormalLabelBrowser->add(label.GetLabel(), (void *)i);
		}
	}
}
				
	
void BoundaryEditorUILogic::OnCloseAction(void)
{
	m_MainWin->hide();
}

void BoundaryEditorUILogic::OnAcceptAction(void)
{
	m_GlobalState->SetBoundaryLayerSettings(settings); //Save our settings back to the global state
	this->OnCloseAction();
}

void BoundaryEditorUILogic::OnAddToBoundaryAction(void)
{
	int selected = m_NormalLabelBrowser->value();
	
	if (selected != 0)
	{
		LabelType labelID = (size_t)m_NormalLabelBrowser->data(selected);
		settings.Add(labelID);
		this->UpdateLabelBrowsers();
	}
}

void BoundaryEditorUILogic::OnRemoveFromBoundaryAction(void)
{
	int selected = m_BoundaryLabelBrowser->value();
	
	if (selected != 0)
	{
		LabelType labelID = (size_t)m_BoundaryLabelBrowser->data(selected);
		settings.Remove(labelID);
		this->UpdateLabelBrowsers();
	}
}
