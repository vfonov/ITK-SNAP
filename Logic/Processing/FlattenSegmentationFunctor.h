//FlattenSegmentationFunctor.h

#ifndef __FlattenSegmentationFunctor_h__
#define __FlattenSegmentationFunctor_h__

#include <map>
#include <iostream>

template<class ImagePixel, class LabelPixel>
class FlattenSegmentationFunctor
{
	private:
	//Use two maps, one for labels and one for greyscale
	std::map<ImagePixel, ImagePixel> imageMap;
	std::map<LabelPixel, ImagePixel> labelMap;
	
	typedef std::pair<ImagePixel, ImagePixel> PairType;
	
	public:
	void SetMapping(std::map<LabelPixel, std::pair<ImagePixel, ImagePixel> > mapping)
	{
		imageMap.clear();
		labelMap.clear();
		
		typename std::map<LabelPixel, PairType>::const_iterator iter;
		
		for (iter = mapping.begin(); iter != mapping.end(); ++iter)
		{
			labelMap[iter->first] = iter->second.first; //Pair is in format {outPixel, otherPixel}
			imageMap[iter->second.first] = iter->second.second;
		}
	}
	
	ImagePixel operator()(const ImagePixel &image, const LabelPixel &label)
	{
		typename std::map<LabelPixel, ImagePixel>::const_iterator labelMapping = labelMap.find(label);
		
		//Check if we have a label that we should map
		if (labelMapping != labelMap.end())
			return labelMapping->second;

		//Check if we should switch the value away from a label value
		typename std::map<ImagePixel, ImagePixel>::const_iterator imageMapping = imageMap.find(image);
		if (imageMapping != imageMap.end())
			return imageMapping->second;
			
		//Otherwise pass it through
		return image;
	}
		
	bool operator==(const FlattenSegmentationFunctor<ImagePixel, LabelPixel> &rhs)
	{
		return (imageMap == rhs.imageMap) && (labelMap == rhs.labelMap);
	}
	
	bool operator !=(const FlattenSegmentationFunctor<ImagePixel, LabelPixel> &x)
    { return !((*this) == x); }
};		


#endif
