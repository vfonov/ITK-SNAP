//MergableImageIOWizard.txx

#include "stdio.h"

template<typename ImageType>
MergableImageIOWizardLogic<ImageType>::MergableImageIOWizardLogic()
{
	this->m_AllowMergeSegmentation = true;
	this->editingLabel = -1;
}

template<typename ImageType>
void MergableImageIOWizardLogic<ImageType>::DoMerge()
{
	//Process Image and then save it;
	m_Flattener.SetImage(ImageIOWizardLogic<ImageType>::m_Image);
	m_Flattener.SetSegmentation(ImageIOWizardLogic<ImageType>::m_Segmentation);
	m_Flattener.ClearLabels();
	
	typename std::vector<LabelAssignment>::const_iterator iter;
	
	
	for (iter = m_Labels.begin(); iter != m_Labels.end(); ++iter)
	{
		//Only flatten labels that have been assigned, pass through all other voxels
		if (iter->valid)
			m_Flattener.AddLabel(iter->label, iter->assignment, iter->reassignment);
	}
		
	
	ImageIOWizardLogic<ImageType>::m_Image = m_Flattener.Flatten(NULL);
	ImageIOWizardLogic<ImageType>::m_Image->DisconnectPipeline();
	
	this->OnSaveFilePageSave();
}


template<typename ImageType>
void MergableImageIOWizardLogic<ImageType>::OnMergePageApplyLabel()
{
	LabelAssignment &assignment = m_Labels[editingLabel];
	
	size_t dest = (size_t)ImageIOWizardLogic<ImageType>::m_LabelAssignment->value();
	size_t remap = (size_t)ImageIOWizardLogic<ImageType>::m_OutputReassignment->value();
	
	assignment.assignment = dest;
	assignment.reassignment = remap;
}

template<typename ImageType>
void MergableImageIOWizardLogic<ImageType>::OnUnassignedLabelsChanged()
{
	int selected = ImageIOWizardLogic<ImageType>::m_UnassignedLabels->value();

	ImageIOWizardLogic<ImageType>::m_AssignedLabels->deselect();

	if (selected > 0)
	{
		LabelType value = (size_t)ImageIOWizardLogic<ImageType>::m_UnassignedLabels->data(selected);
		
		ImageIOWizardLogic<ImageType>::m_UnassignButton->deactivate();
		ImageIOWizardLogic<ImageType>::m_AssignButton->activate();
		EditLabel(value);
	}
	else
		ClearLabelEditor();
}

template<typename ImageType>
void MergableImageIOWizardLogic<ImageType>::OnAssignedLabelsChanged()
{
	int selected = ImageIOWizardLogic<ImageType>::m_AssignedLabels->value();
	
	ImageIOWizardLogic<ImageType>::m_UnassignedLabels->deselect();

	if (selected > 0)
	{
		LabelType value = (size_t)ImageIOWizardLogic<ImageType>::m_AssignedLabels->data(selected);
		
		ImageIOWizardLogic<ImageType>::m_AssignButton->deactivate();
		ImageIOWizardLogic<ImageType>::m_UnassignButton->activate();
		EditLabel(value);
	}
	else		
		ClearLabelEditor();
}

template<typename ImageType>
void MergableImageIOWizardLogic<ImageType>::OnAssignLabel()
{
	int selected = ImageIOWizardLogic<ImageType>::m_UnassignedLabels->value();
	
	if (selected > 0)
	{
		LabelType value = (size_t)ImageIOWizardLogic<ImageType>::m_UnassignedLabels->data(selected);
		
		m_Labels[value].valid = true;
	}
	
	UpdateBrowsers();
}
		

template<typename ImageType>
void MergableImageIOWizardLogic<ImageType>::OnUnassignLabel()
{
	int selected = ImageIOWizardLogic<ImageType>::m_AssignedLabels->value();
	
	if (selected > 0)
	{
		LabelType value = (size_t)ImageIOWizardLogic<ImageType>::m_AssignedLabels->data(selected);
		
		m_Labels[value].valid = false;
	}
	
	UpdateBrowsers();
}

template<typename ImageType>
void MergableImageIOWizardLogic<ImageType>::SetLabels(std::list<std::pair<std::string, LabelType> > labels)
{
	m_Labels.clear();
	
	std::list<std::pair<std::string, LabelType> >::const_iterator iter;
	
	for (iter = labels.begin(); iter != labels.end(); ++iter)
	{
		LabelAssignment assignment;
		
		assignment.name = iter->first;
		assignment.label = iter->second;
		assignment.assignment = 0;
		assignment.reassignment = 0;
		assignment.valid = false;
		
		m_Labels.push_back(assignment);
	}
	
	this->UpdateBrowsers();
}

template<typename ImageType>
void MergableImageIOWizardLogic<ImageType>::UpdateBrowsers()
{
	ImageIOWizardLogic<ImageType>::m_UnassignedLabels->clear();
	ImageIOWizardLogic<ImageType>::m_AssignedLabels->clear();
	
	typename std::vector<LabelAssignment>::const_iterator iter;
	
	for (iter = m_Labels.begin(); iter != m_Labels.end(); ++iter)
	{
		if (iter->valid)
			ImageIOWizardLogic<ImageType>::m_AssignedLabels->add(iter->name.c_str(), (void *)(iter->label));
		else
			ImageIOWizardLogic<ImageType>::m_UnassignedLabels->add(iter->name.c_str(), (void*)(iter->label));
	}
	
	ImageIOWizardLogic<ImageType>::m_UnassignedLabels->deselect();
	ImageIOWizardLogic<ImageType>::m_AssignedLabels->deselect();
	ImageIOWizardLogic<ImageType>::m_AssignButton->deactivate();
	ImageIOWizardLogic<ImageType>::m_UnassignButton->deactivate();
	ClearLabelEditor();
}

template<typename ImageType>
void MergableImageIOWizardLogic<ImageType>::ClearLabelEditor()
{
	ImageIOWizardLogic<ImageType>::m_LabelName->value("");
	ImageIOWizardLogic<ImageType>::m_LabelValue->value("");
	ImageIOWizardLogic<ImageType>::m_LabelAssignment->value(0.0);
	ImageIOWizardLogic<ImageType>::m_LabelAssignment->value(0.0);
	ImageIOWizardLogic<ImageType>::m_LabelEditorApplyButton->deactivate();
	
	editingLabel = -1;
}

template<typename ImageType>
void MergableImageIOWizardLogic<ImageType>::EditLabel(size_t l)
{
	const LabelAssignment &assignment = m_Labels[l];
	
	ImageIOWizardLogic<ImageType>::m_LabelName->value(assignment.name.c_str());
	
	//Possible (very slight chance) threading issue if someone reads from value while we're
	//updating below since FLTK does not make its own copy of the string
	ImageIOWizardLogic<ImageType>::m_LabelValue->value("");
	//Yes I know this does not bounds check and can heap/stack corrupt, but it's late.
	sprintf(m_LabelValueBuffer, "%d", assignment.label);
	
	ImageIOWizardLogic<ImageType>::m_LabelValue->value(m_LabelValueBuffer);
	ImageIOWizardLogic<ImageType>::m_LabelAssignment->value((double)assignment.assignment);
	ImageIOWizardLogic<ImageType>::m_OutputReassignment->value((double)assignment.reassignment);
	
	editingLabel = l;
	
	ImageIOWizardLogic<ImageType>::m_LabelEditorApplyButton->activate();
}
