//FlattenSegmentationManager.h

#ifndef __FlattenSegmentationManager_h__
#define __FlattenSegmentationManager_h__

#include <map>
#include "FlattenSegmentationFunctor.h"
#include "itkBinaryFunctorImageFilter.h"
#include "itkCommand.h"

template<class ImageType, class LabelImageType>
class FlattenSegmentationManager
{
	public:
	typedef typename ImageType::PixelType ImagePixelType;
	typedef typename LabelImageType::PixelType LabelPixelType;
	typedef typename itk::BinaryFunctorImageFilter<ImageType, LabelImageType, ImageType, FlattenSegmentationFunctor<ImagePixelType, LabelPixelType> > FilterType;
	
	private:
	typename std::map<LabelPixelType, std::pair<ImagePixelType, ImagePixelType> > labelMap;
	
	typename ImageType::ConstPointer inputImage;
	typename LabelImageType::ConstPointer inputSegmentation;
	
	FlattenSegmentationFunctor<ImagePixelType, LabelPixelType> functor;
	typename FilterType::Pointer binaryFilter;
	
	public:	
	//The function assigns a mapping for the label 'label'.  Output pixels with that label
	//are mapped to outValue, and pixels that were originally outValue are remapped to otherValue
	//to avoid confusion.
	void AddLabel(LabelPixelType label, ImagePixelType outValue, ImagePixelType otherValue);
	void RemoveLabel(LabelPixelType label);
	void ClearLabels();
	
	void SetImage(const ImageType *image);
	void SetSegmentation(const LabelImageType *segmentation);
	
	ImageType* Flatten(itk::Command *progressCommand);
};

#include "FlattenSegmentationManager.txx"

#endif
