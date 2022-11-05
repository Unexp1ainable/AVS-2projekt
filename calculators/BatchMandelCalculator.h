/**
 * @file BatchMandelCalculator.h
 * @author Samuel Repka <xrepka07@stud.fit.vutbr.cz>
 * @brief Implementation of Mandelbrot calculator that uses SIMD paralelization over small batches
 * @date 04/11/2022
 */
#ifndef BATCHMANDELCALCULATOR_H
#define BATCHMANDELCALCULATOR_H

#include <BaseMandelCalculator.h>
#include <memory>

class BatchMandelCalculator: public BaseMandelCalculator
{
public:
    BatchMandelCalculator(unsigned matrixBaseSize, unsigned limit);
    ~BatchMandelCalculator();
    int* calculateMandelbrot();

private:
    std::unique_ptr<int[]> m_data;
    std::unique_ptr<float[]> m_realData;
    std::unique_ptr<float[]> m_startRealData;
    std::unique_ptr<float[]> m_imagData;
    std::unique_ptr<bool[]> m_resultMask;
    std::unique_ptr<int[]> m_results;
    int* m_rawData = nullptr;
    int* m_rawRealData = nullptr;

    static constexpr int BATCH_SIZE = 64;
};

#endif
