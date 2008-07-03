//MergeLabelsUILogic.h

#ifndef __MergeLabelsUILogic_h__
#define __MergeLabelsUILogic_h__

#include "MergeLabelsUI.h"
#include <map>
#include "ColorLabel.h"
#include "SNAPCommon.h"

class IRISApplication;
class GlobalState;
class UserInterfaceBase;
class BoundaryLayerSettings;

class MergeLabelsUILogic : public MergeLabelsUI
{
	private:
	UserInterfaceBase *m_Parent;
	GlobalState *m_GlobalState;
	IRISApplication *m_Driver;
	std::map<LabelType, ColorLabel> m_MergeLabels;
	
	void InitializeLabelBrowsers(void);
	void UpdateLabelBrowser(void);
	void UpdateLabelBrowsers(void);
	void UpdateMergeLabelsBrowser(void);
	
	void ProcessImage(LabelType outputLabel);
	
	public:
	MergeLabelsUILogic(void);
	virtual ~MergeLabelsUILogic();
	
	void DisplayWindow(void);
	
	void Register(UserInterfaceBase *m_Parent);
	
	virtual void OnAddLabelAction();
	virtual void OnRemoveLabelAction();
	virtual void OnCloseAction();
	virtual void OnApplyAction();
	virtual void OnAcceptAction();
};	

#endif
