//BoundaryLayerSettings.cxx

#include "BoundaryLayerSettings.h"

using namespace std;

void BoundaryLayerSettings::Add(LabelType label)
{
	m_BoundaryLabels[label] = true;
}

void BoundaryLayerSettings::Remove(LabelType label)
{
	m_BoundaryLabels.erase(label);
}

void BoundaryLayerSettings::Clear(void)
{
	m_BoundaryLabels.clear();
}

list<LabelType> BoundaryLayerSettings::GetLabels(void)
{
	map<LabelType, bool>::const_iterator	iter;
	list<LabelType>		labels;
	
	for (iter = m_BoundaryLabels.begin(); iter != m_BoundaryLabels.end(); ++iter)
		labels.push_back(iter->first);
		
	return labels;
}
