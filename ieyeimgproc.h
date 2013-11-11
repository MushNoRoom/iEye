#ifndef IEYEIMGPROC_H
#define IEYEIMGPROC_H

#include <stdlib.h>
#include <iostream>
#include "windows.h"
#include "opencv.hpp"
namespace cv
{
	class Mat;
}
namespace iEye
{
	//template<typename T>
	class Point
	{
	public:
			Point(const int& x,const int& y)
				:x_m(x), y_m(y)
			{
			};

			int x()
			{
				return x_m;
			};
			int y()
			{
				return y_m;
			};
		private:
			int x_m, y_m;
	};

	double findThreshold(const cv::Mat &input, double initialVal = 90);

	void threshold(cv::Mat& input, double threshold);

  /*!
   * \brief windowScanMinSum gets the point of pupil by scanning the minimum local sum
   * \param input input data matrix
   * \param size size of sliding window
   * \return pupil location in the frame
   */
    const cv::Point2i windowScanMinSum(cv::Mat& input, cv::Size size = cv::Size(8, 8));

    /*!
     * \brief windowScanMin finds the minimum by comparing the local sum within the sliding window
     * \param input the input data matrix
     * \param size size of the sliding window
     * \return indicating where the pupil is
     */
    const cv::Point2i windowScanMin(cv::Mat& input, cv::Size size = cv::Size(8, 8));

    /*!
     * \brief windowScanMinStd finds the minimum comparing average and standard deviation
     * \param input the input data matrix
     * \param size size of the sliding window
     * \return point indicating where the pupil is
     */
    const cv::Point2i windowScanMinStdDev(cv::Mat& input, cv::Size size = cv::Size(8,8));

}

#endif
