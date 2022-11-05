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
    std::unique_ptr<int[]> m_data;
    std::unique_ptr<float[]> m_realData;
    std::unique_ptr<float[]> m_startRealData;
    std::unique_ptr<float[]> m_imagData;
    std::unique_ptr<bool[]> m_resultMask;
    std::unique_ptr<int[]> m_results;
    int* m_rawData = nullptr;
    int* m_rawRealData = nullptr;
};
