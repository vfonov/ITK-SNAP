//BoundaryEditorUIBase.h

#ifndef __BoundaryEditorUIBase_h__
#define __BoundaryEditorUIBase_h__

class BoundaryEditorUIBase
{
	public:
	virtual ~BoundaryEditorUIBase() {};
	
	virtual void OnCloseAction(void) = 0;
	virtual void OnAcceptAction(void) = 0;
	virtual void OnAddToBoundaryAction(void) = 0;
	virtual void OnRemoveFromBoundaryAction(void) = 0;
};

#endif
