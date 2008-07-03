/*BoundaryLayerSettings.h

This class stores the settings for what labels, if any are present in the boundary layer and should
be set to 0 (for edge preprocessing) or -1 (for region preprocessing).
*/

#ifndef __BoundaryLayerSettings_h__
#define __BoundaryLayerSettings_h__

#include <map>
#include <list>
#include "SNAPCommon.h"

class BoundaryLayerSettings
{
	private:
	std::map<LabelType, bool>	m_BoundaryLabels;
	
	public:
	//Use default constructor, destructor and copy constructor, we have no complex types or pointers
	//to manage ourselves.
	
	void Clear(void);
	void Add(LabelType label);
	void Remove(LabelType label); //If label is not present, this will silently succeed

	//Inline this because it can be called many times while looping over an image
	inline bool LabelInBoundary(LabelType label) const
	{
		return (m_BoundaryLabels.find(label) != m_BoundaryLabels.end());
	}
	
	std::list<LabelType> GetLabels(void);
};	

#endif
