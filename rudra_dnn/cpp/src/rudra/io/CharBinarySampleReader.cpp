/*
 * CharBinarySampleReader.cpp
 */

#include "rudra/io/CharBinarySampleReader.h"
#include "rudra/util/MatrixContainer.h"
#include "rudra/MLPparams.h"
#include "rudra/util/RudraRand.h"
#include <vector>
#include <algorithm>

namespace rudra {
CharBinarySampleReader::CharBinarySampleReader(std::string sampleFileName,
				       std::string labelFileName, RudraRand rr) :
		sizePerImg(MLPparams::_numInputDim), tdFile(sampleFileName), tlFile(
										    labelFileName),rr(rr) {
    this->setLabelDim();
    this->sizePerLabel = MLPparams::_labelDim;
   
}

CharBinarySampleReader::~CharBinarySampleReader() {
}

/**
 * Read a chosen number of samples into matrix X and the corresponding labels
 * into matrix Y.
 */
void CharBinarySampleReader::readLabelledSamples(size_t numSamples,
		MatrixContainer<float> &X, MatrixContainer<float>& Y) {
	std::vector<size_t> idx(numSamples);
	for (size_t i = 0; i < numSamples; ++i) {
	    idx[i] = rr.getLong() % MLPparams::_numTrainSamples;
	}
	std::sort(idx.begin(), idx.end());

	// binary format is transposed
	MatrixContainer<uint8> tX(MLPparams::_batchSize, MLPparams::_numInputDim);
	MatrixContainer<float> tY(MLPparams::_batchSize, MLPparams::_labelDim); // May 15, 2015, labelDim
	for (size_t i = 0; i < numSamples; ++i) {
	    readBinMat<uint8>(tX.buf + i * sizePerImg, idx[i], 1, sizePerImg, tdFile);
	    readBinMat<float>(tY.buf + i * sizePerLabel, idx[i], 1, sizePerLabel, tlFile); // June 30, 2015 label file always read as floats
	}
	X = tX;
	Y = tY;

//	tX.transposeTo(X);
//	tY.transposeTo(Y);
}

  
    void CharBinarySampleReader::setLabelDim(){
	int rows, columns;
        MLPparams::readBinHeader(tlFile, rows, columns);
	MLPparams::setLabelDim(columns);
    }
   

} /* namespace rudra */
