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
    void processBatch(int batch, float y);
    void processBatchEpilog(float y);

    std::unique_ptr<int[]> m_data;              // final destination
    std::unique_ptr<float[]> m_realData;        // intermediate values of real component
    std::unique_ptr<float[]> m_startRealData;   // intermediate values of real component at 0th iteration
    std::unique_ptr<float[]> m_imagData;        // intermediate values of imaginary component
    std::unique_ptr<bool[]> m_resultMask;       // array mask
    std::unique_ptr<int[]> m_results;           // intermediary results
    int* m_rawData = nullptr;                   // raw pointer to m_data
    int* m_rawRealData = nullptr;               // raw pointer to m_realData

    static constexpr int BATCH_SIZE = 256;

};

#endif
