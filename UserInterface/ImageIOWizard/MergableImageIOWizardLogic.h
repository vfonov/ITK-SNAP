//MergableImageIOWizardLogic.h
//This class defines a subclass of ImageIOWizardLogic that is capable of merging segmentation
//images into greyscale values.

#ifndef __MergableImageIOWizardLogic_h__
#define __MergableImageIOWizardLogic_h__

#include "SNAPCommonUI.h"
#include "RestrictedImageIOWizardLogic.h"



template<typename PixelType>
class MergableImageIOWizardLogic : public ImageIOWizardLogic<PixelType>
{
	typedef struct
	{
		std::string		name;
		LabelType		label;
		PixelType		assignment;
		PixelType		reassignment;
		bool 			valid;
	} LabelAssignment;
	
	private: 	
	/** Object to handle flattening segmentation into greyscale image */
  	FlattenSegmentationManager<typename ImageIOWizardLogic<PixelType>::ImageType, itk::Image<LabelType,3> > m_Flattener;
  	std::vector<LabelAssignment> m_Labels;
  	char 						m_LabelValueBuffer[256];
  	int 						editingLabel;
  	
  	void EditLabel(size_t l);
  	void ClearLabelEditor();
  	void UpdateBrowsers();
  	
  	public:
  	MergableImageIOWizardLogic();
  	
  	virtual void DoMerge();
  	virtual void SetLabels(std::list<std::pair<std::string, LabelType> >);
  	
	virtual void OnMergePageApplyLabel();
	virtual void OnUnassignedLabelsChanged();
	virtual void OnAssignedLabelsChanged();
	virtual void OnAssignLabel();
	virtual void OnUnassignLabel();
};

#include "MergableImageIOWizardLogic.txx"

#endif
