//FlattenSegmentationManager.txx

template<typename ImageType, typename LabelImageType>
void FlattenSegmentationManager<ImageType, LabelImageType>::AddLabel(LabelPixelType label, ImagePixelType outValue, ImagePixelType otherValue)
{
	labelMap[label] = std::pair<ImagePixelType,ImagePixelType>(outValue, otherValue);
}
	
template<typename ImageType, typename LabelImageType>
void FlattenSegmentationManager<ImageType, LabelImageType>::RemoveLabel(LabelPixelType label)
{
	labelMap.erase(label);
}

template<typename ImageType, typename LabelImageType>
void FlattenSegmentationManager<ImageType, LabelImageType>::ClearLabels(void)
{
	labelMap.clear();
}

template<typename ImageType, typename LabelImageType>
void FlattenSegmentationManager<ImageType, LabelImageType>::SetImage(const ImageType *image)
{
	this->inputImage = image;
}

template<typename ImageType, typename LabelImageType>
void FlattenSegmentationManager<ImageType, LabelImageType>::SetSegmentation(const LabelImageType *segmentation)
{
	this->inputSegmentation = segmentation;
}

template<typename ImageType, typename LabelImageType>
ImageType* FlattenSegmentationManager<ImageType, LabelImageType>::Flatten(itk::Command *progressCommand)
{
	if (binaryFilter.IsNull())
		binaryFilter = FilterType::New();
	
	functor.SetMapping(labelMap);
	
	//Initialize the filter
	binaryFilter->SetFunctor(functor);
	binaryFilter->SetInput1(inputImage);
	binaryFilter->SetInput2(inputSegmentation);
	
	if (progressCommand)
		binaryFilter->AddObserver(itk::AnyEvent(),progressCommand);
	
	binaryFilter->Update();
	
	return binaryFilter->GetOutput();
}
