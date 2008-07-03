//MergeLabelsUILogic.cxx

#include "MergeLabelsUILogic.h"

#include "UserInterfaceBase.h"
#include "GlobalState.h"
#include "IRISApplication.h"
#include "IRISImageData.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "LabelImageWrapper.h"

#include <iostream>

using namespace itk;

MergeLabelsUILogic::MergeLabelsUILogic(void) : 
	m_Parent(NULL), m_Driver(NULL), m_GlobalState(NULL)
{
}

MergeLabelsUILogic::~MergeLabelsUILogic(void)
{
}

void MergeLabelsUILogic::Register(UserInterfaceBase *parent)
{
  m_Parent = parent;
  m_Driver = parent->GetDriver();
  m_GlobalState = m_Driver->GetGlobalState();
}

void MergeLabelsUILogic::DisplayWindow(void)
{
	this->InitializeLabelBrowsers();
	this->m_MainWin->show();
}

void MergeLabelsUILogic::UpdateLabelBrowser(void)
{
	m_LabelsBrowser->clear();
	
	for (size_t i = 0; i < MAX_COLOR_LABELS; ++i)
	{
		ColorLabel label = m_Driver->GetColorLabelTable()->GetColorLabel(i);
		if (label.IsValid() && m_MergeLabels.find(i) == m_MergeLabels.end())
		{
			m_LabelsBrowser->add(label.GetLabel(), (void *)i);
		}
	}
}

void MergeLabelsUILogic::UpdateLabelBrowsers(void)
{
	UpdateLabelBrowser();
	UpdateMergeLabelsBrowser();
}

void MergeLabelsUILogic::UpdateMergeLabelsBrowser(void)
{
	m_MergeLabelsBrowser->clear();
	
	for(std::map<LabelType, ColorLabel>::iterator iter = m_MergeLabels.begin(); iter != m_MergeLabels.end(); ++iter)
		m_MergeLabelsBrowser->add(iter->second.GetLabel(), (void *)iter->first);
}

void MergeLabelsUILogic::InitializeLabelBrowsers(void)
{
	m_LabelsBrowser->clear();
	m_OutputLabelChooser->clear();
	this->m_MergeLabels.clear();
	
	for (size_t i = 0; i < MAX_COLOR_LABELS; ++i)
	{
		ColorLabel label = m_Driver->GetColorLabelTable()->GetColorLabel(i);
		if (label.IsValid() && m_MergeLabels.find(i) == m_MergeLabels.end())
		{
			m_LabelsBrowser->add(label.GetLabel(), (void *)i);
			m_OutputLabelChooser->add(label.GetLabel(), 0, NULL, (void *)i, 0);
		}
	}

	UpdateMergeLabelsBrowser();
}

void MergeLabelsUILogic::OnAddLabelAction(void)
{
	int selected = m_LabelsBrowser->value();
	
	if (selected != 0)
	{
		LabelType labelID = (size_t)m_LabelsBrowser->data(selected);
		m_MergeLabels[labelID] = m_Driver->GetColorLabelTable()->GetColorLabel(labelID);
		this->UpdateLabelBrowsers();
	}
}

void MergeLabelsUILogic::OnRemoveLabelAction(void)
{
	int selected = m_MergeLabelsBrowser->value();
	
	if (selected != 0)
	{
		LabelType labelID = (size_t)m_MergeLabelsBrowser->data(selected);
		m_MergeLabels.erase(labelID);
		this->UpdateLabelBrowsers();
	}
}

void MergeLabelsUILogic::OnCloseAction(void)
{
	m_MainWin->hide();
}

void MergeLabelsUILogic::ProcessImage(LabelType outputLabel)
{
	LabelImageWrapper::ImageType::Pointer imgNewLabel = 
    m_Driver->GetIRISImageData()->GetSegmentation()->GetImage();

  
  	typedef ImageRegionIterator<LabelImageWrapper::ImageType> IteratorType;  
  	IteratorType itLabel(imgNewLabel,imgNewLabel->GetBufferedRegion());  

	size_t numProcessed = 0;
	
	//Save an Undo Point
	m_Driver->StoreUndoPoint("Before Merging");
	
  	while(!itLabel.IsAtEnd())
   	{
    	LabelType voxel = itLabel.Value();
    	
    	if (m_MergeLabels.find(voxel) != m_MergeLabels.end())
    	{	
    		itLabel.Value() = outputLabel;
    		++numProcessed;
    	}
    		
    	++itLabel;
    }
    
    //Since we made some voxels to be the output label, make sure it is marked as valid 
    if (numProcessed > 0)
    {
	    m_Driver->GetColorLabelTable()->SetColorLabelValid(outputLabel, true);
	    imgNewLabel->Modified();
	}
	    
	std::cout << "Merged " << numProcessed << " voxels into label " << outputLabel << std::endl; 
}

void MergeLabelsUILogic::OnApplyAction(void)
{
	const Fl_Menu_Item *outputSelection = m_OutputLabelChooser->mvalue();
	
	if (outputSelection != NULL)
	{
		LabelType outputLabel = (size_t)outputSelection->user_data();
		
		ProcessImage(outputLabel);
	}
	else
		std::cout << "No output label selected.  Doing nothing..." << std::endl;
}

void MergeLabelsUILogic::OnAcceptAction(void)
{
	OnApplyAction();
	OnCloseAction();
}
