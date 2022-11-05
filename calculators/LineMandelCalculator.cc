/**
 * @file LineMandelCalculator.cc
 * @author Samuel Repka <xrepka07@stud.fit.vutbr.cz>
 * @brief Implementation of Mandelbrot calculator that uses SIMD paralelization over lines
 * @date 04/11/2022
 */
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <stdlib.h>

#include "LineMandelCalculator.h"

LineMandelCalculator::LineMandelCalculator(unsigned matrixBaseSize, unsigned limit)
    : BaseMandelCalculator(matrixBaseSize, limit, "LineMandelCalculator")
    , m_data(new int[height * width]())
    , m_realData(new float[width]())
    , m_startRealData(new float[width]())
    , m_imagData(new float[width]())
    , m_resultMask(new bool[width]())
    , m_results(new int[width]())
    , m_rawData(m_data.get())
{
}

LineMandelCalculator::~LineMandelCalculator()
{
}

int* LineMandelCalculator::calculateMandelbrot()
{
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

        // calculate an iteration
        for (int iters = 0; iters < limit; ++iters) {

            #pragma omp simd
            for (int j = 0; j < width; j++) {
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
                }

                // calculate next iteration
                m_imagData[j] = 2.0f * zReal * zImag + y;
                m_realData[j] = r2 - i2 + m_startRealData[j];
            }

            int sum = 0;
            #pragma omp simd reduction(+:sum)
            for (int j = 0; j < width; j++) {
                sum += m_resultMask[j];
            }

            // check if everything was calculated
            if (sum == width) {
                break;
            }
        }

        // save to result array, account for symmetry
        void* where = m_rawData + width * i;
        void* where2 = m_rawData + width * (height - i - 1);
        memcpy(where, m_results.get(), width * sizeof(int));
        memcpy(where2, m_results.get(), width * sizeof(int));
    }
    return m_rawData;
}
