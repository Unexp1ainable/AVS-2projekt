/**
 * @file LineMandelCalculator.h
 * @author Samuel Repka <xrepka07@stud.fit.vutbr.cz>
 * @brief Implementation of Mandelbrot calculator that uses SIMD paralelization over lines
 * @date 04/11/2022
 */

#include <BaseMandelCalculator.h>
#include <cmath>
#include <cstring>
#include <memory>

class LineMandelCalculator: public BaseMandelCalculator {
public:
    LineMandelCalculator(unsigned matrixBaseSize, unsigned limit);
    ~LineMandelCalculator();
    int* calculateMandelbrot();

private:
    void processLine(float y);

    std::unique_ptr<int[]> m_data;              // final destination
    std::unique_ptr<float[]> m_realData;        // intermediate values of real component
    std::unique_ptr<float[]> m_startRealData;   // intermediate values of real component at 0th iteration
    std::unique_ptr<float[]> m_imagData;        // intermediate values of imaginary component
    std::unique_ptr<bool[]> m_resultMask;       // array mask
    std::unique_ptr<int[]> m_results;           // intermediary results
    int* m_rawData = nullptr;                   // raw pointer to m_data
    int* m_rawRealData = nullptr;               // raw pointer to m_realData
};
