//MergeLabelsUIBase.h

#ifndef __MergeLabelsUIBase_h__
#define __MergeLabelsUIBase_h__

class MergeLabelsUIBase
{
	public:
	virtual ~MergeLabelsUIBase() {};
	
	virtual void OnAddLabelAction() = 0;
	virtual void OnRemoveLabelAction() = 0;
	virtual void OnCloseAction() = 0;
	virtual void OnApplyAction() = 0;
	virtual void OnAcceptAction() = 0;
};

#endif
