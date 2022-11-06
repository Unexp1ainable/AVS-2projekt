/**
 * @file BatchMandelCalculator.cc
 * @author Samuel Repka <xrepka07@stud.fit.vutbr.cz>
 * @brief Implementation of Mandelbrot calculator that uses SIMD paralelization over small batches
 * @date 04/11/2022
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cmath>

#include <stdlib.h>
#include <stdexcept>

#include "BatchMandelCalculator.h"

BatchMandelCalculator::BatchMandelCalculator(unsigned matrixBaseSize, unsigned limit):
	BaseMandelCalculator(matrixBaseSize, limit, "BatchMandelCalculator")
	, m_data(new int[height * width]())
	, m_realData(new float[width]())
	, m_startRealData(new float[width]())
	, m_imagData(new float[width]())
	, m_resultMask(new bool[width]())
	, m_results(new int[width]())
	, m_rawData(m_data.get())
{
}

BatchMandelCalculator::~BatchMandelCalculator() {
}


void BatchMandelCalculator::processBatch(int batch, float y) {
	int batchOffset = batch * BATCH_SIZE;
	int fastEnd = BATCH_SIZE;

	// calculate an iteration
	for (int iters = 0; iters < limit; ++iters) {

		int j = batchOffset;
		#pragma omp simd
		for (int jj = 0; jj < BATCH_SIZE; jj++) {
			j++;
			// skip calculation if result achieved
			if (m_resultMask[j])
				continue;
			float zImag = m_imagData[j];
			float zReal = m_realData[j];
			float r2 = zReal * zReal;
			float i2 = zImag * zImag;
			// check results
			if (r2 + i2 > 4.0f) {
				m_resultMask[j] = true;
				m_results[j] = iters;
				fastEnd--;
			}

			// calculate next iteration
			m_imagData[j] = 2.0f * zReal * zImag + y;
			m_realData[j] = r2 - i2 + m_startRealData[j];
		}

		// check if everything was calculated
		if (fastEnd == 0) {
			break;
		}
	}
}


void BatchMandelCalculator::processBatchEpilog(float y) {
	const int count = width % BATCH_SIZE;
	if (count == 0) {
		return;
	}
	int fastEnd = count;
	int batchOffset = width - count;
	// calculate an iteration
	for (int iters = 0; iters < limit; ++iters) {

		int j = batchOffset;
		#pragma omp simd
		for (int jj = 0; jj < count; jj++) {
			j++;
			// skip calculation if result achieved
			if (m_resultMask[j])
				continue;
			float zImag = m_imagData[j];
			float zReal = m_realData[j];
			float r2 = zReal * zReal;
			float i2 = zImag * zImag;
			// check results
			if (r2 + i2 > 4.0f) {
				m_resultMask[j] = true;
				m_results[j] = iters;
				fastEnd--;
			}

			// calculate next iteration
			m_imagData[j] = 2.0f * zReal * zImag + y;
			m_realData[j] = r2 - i2 + m_startRealData[j];
		}

		// check if everything was calculated
		if (fastEnd == 0) {
			break;
		}
	}
}


int* BatchMandelCalculator::calculateMandelbrot() {
	int* pdata = m_rawData;
	int* prdata = m_rawRealData;

	const int itercount = std::lround(height / 2.);
	for (int i = 0; i < itercount; i++) {
		float y = y_start + i * dy; // current imaginary value

		// initialize intermediate arrays
		#pragma omp simd
		for (int j = 0; j < width; j++) {
			m_imagData[j] = y;
			m_realData[j] = x_start + j * dx;
			m_startRealData[j] = m_realData[j];
			m_results[j] = limit;
			m_resultMask[j] = false;
		}

		int batches = (int)(width / (double)BATCH_SIZE);
		for (int batch = 0; batch < batches; ++batch) {
			processBatch(batch, y);
		}
		processBatchEpilog(y);

		// save to result array, account for symmetry
		void* where = m_rawData + width * i;
		void* where2 = m_rawData + width * (height - i - 1);
		memcpy(where, m_results.get(), width * sizeof(int));
		memcpy(where2, m_results.get(), width * sizeof(int));
	}
	return m_rawData;
}
