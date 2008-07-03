//BoundaryEditorUILogic.h

#ifndef __BoundaryEditorUILogic_h__
#define __BoundaryEditorUILogic_h__

#include "BoundaryEditorUI.h"
#include "BoundaryLayerSettings.h"
#include <vector>

class IRISApplication;
class GlobalState;
class UserInterfaceBase;
class BoundaryLayerSettings;

class BoundaryEditorUILogic : public BoundaryEditorUI
{
	private:
	UserInterfaceBase *m_Parent;
	GlobalState *m_GlobalState;
	IRISApplication *m_Driver;
	BoundaryLayerSettings settings;
		
	protected:
	void UpdateLabelBrowsers(void);
	
	public:
	BoundaryEditorUILogic();
	virtual ~BoundaryEditorUILogic();
	
	void Register(UserInterfaceBase *parent);
	void DisplayWindow(void);
	
	//UI Actions 
	virtual void OnCloseAction(void);
	virtual void OnAcceptAction(void);
	virtual void OnAddToBoundaryAction(void);
	virtual void OnRemoveFromBoundaryAction(void);
};

#endif
