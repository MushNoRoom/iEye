//#include "stdafx.h"
#include "ieyeimgproc.h"
#include "opencv.hpp"
#include <stdlib.h>
#include <assert.h>
#include <QVector>
#include <QPoint>
#include <math.h>

//#ifdef _DEBUG
#include <QDebug>
//#endif



//typedef QVector<cv::Scalar> ScalarQueue;

namespace iEye
{
	double findThreshold(const cv::Mat &input, double initialVal)
	{
		//printf("%d %d \n", input.size().height, input.size().width);
		double th(initialVal);
		double th_prev(0.0);
		double sum_greater, sum_less;
		int amount_greater_th = 0;
		int amount_less_th = 0;
		while(std::abs(th - th_prev) > 1)
		{
			
			sum_greater = 0;
			sum_less = 0;
			amount_greater_th = 0;
			amount_less_th = 0;
			for (int i = 0; i < input.size().height; ++i)
			{
				const uchar* row_ptr = input.ptr(i);
				//assert(row_ptr != NULL);
				for(int j = 0; j < input.size().width; ++j)
				{

					int pxVal(static_cast<int>(row_ptr[j]));
					
					if (pxVal > th)
					{
						sum_greater += pxVal;
						amount_greater_th++;
					}
					else
					{
						sum_less += pxVal;
						amount_less_th++;
					}
					
				}
				double avg_grt_th(sum_greater / amount_greater_th);
				double avg_ls_th(sum_less / amount_less_th);
				th_prev = th;
				th = (avg_grt_th + avg_ls_th) / 2;
			}
		}
		return th;
	}

	void threshold(cv::Mat& input, double threshold)
	{
			for (int i = 0; i < input.size().height; ++i)
			{
				//assert(input.size() == output.size());
				uchar* row_ptr = input.ptr(i);
				//assert(row_ptr != NULL);
				for(int j = 0; j < input.size().width; ++j)
				{
					double pxVal(static_cast<double>(row_ptr[j]));

					if (pxVal > threshold)
					{
						row_ptr[j] = 255;
					}
					
				}
			}
    }

    /*!
     * Function that computes the sum of pixels in a window.
     * The sum of each column is stored in the columnSumVec
     * This funtion simply just sums the elements in the queue.
     */
    cv::Scalar windowSum(QVector<cv::Scalar> &columnSumVec)
    {
            cv::Scalar windowSum(0);

            for (int i = 0; i < columnSumVec.size(); ++i)
            {
                    windowSum += columnSumVec.at(i);
            }
            return windowSum;
    }

	cv::Mat buildSubMat(int row, int col, int width, int height, const cv::Mat& input)
	{
		using namespace cv;
		cv::Size size = input.size();
		if (height > size.height || width > size.width)
			return input.rowRange(0, 0).colRange(0, 0);

		// Reset the row and col values if the dimension cannot be realized.
		if (row < 0)
			row = 0;
		if (col < 0)
			col = 0;
		if ((row + height) > size.height)
		{
			qDebug() << "Height too much";
			row = size.height - height;
		}
		if ((col + width) > size.width)
		{
			qDebug() << "Width too much.";
			col = size.width - height;
		}

		return input.rowRange(row, row + height).colRange(col, col + width);
		
	}

    /*!
     * \brief variance computes the variance of the column
     * \param input matrix
     * \param totalPixel total number of pixels in the window
     * \return variance value
     */
    int variance(const cv::Mat& input, int mean)
    {
        int result(0);
        for(int i = 0; i < input.rows; ++i)
            for(int j = 0; j < input.cols; ++j)
            {
                int temp = static_cast<int>(pow(double(input.at<unsigned char>(i, j) - mean),
                          2));
                result += temp;
            }
        return result;
    }

    const cv::Point2i windowScanMinSum(cv::Mat& input, cv::Size size)
    {
        using namespace cv;
        const int wnd_width(size.width);
        const int wnd_height(size.height);

        int min_rb, min_cb;
        int rb_count;
        int cb_count;

        QVector<Scalar> sum_queue;
        int min_sum;
        Scalar window_sum;



        for (rb_count = 0; rb_count < input.rows - wnd_height ; ++ rb_count)
        {
            Mat sub_mat(input.rowRange(rb_count, rb_count + wnd_height));
            sum_queue.clear(); // Clear the sum queue when change line
            for (cb_count = 0; cb_count < input.cols - wnd_width; ++cb_count)
            {
                Mat sub_sub_mat = sub_mat.colRange(cb_count, (cb_count + wnd_width ));
                if (cb_count == 0)
                {
                    for(int block_cc = 0; block_cc < size.width; ++block_cc)
                    {
                        cv::Scalar col_sum(cv::sum(sub_sub_mat.col(block_cc)));
                        sum_queue.push_back(col_sum); // Sum all the column in the window
                                                                                                                                                // Construct a queue of the column sums at first window of each line
                    }
                    window_sum = windowSum(sum_queue);
                    if (rb_count == 0)
                    {
                        min_sum = (int)window_sum[0];
                    }

                }
                else
                {
                    sum_queue.pop_front();
                    sum_queue.push_back(cv::sum(sub_sub_mat.col(size.width - 1)));
                    window_sum = windowSum(sum_queue);

                }

                // Check if the local sum is the minimum so far
                if ((int)window_sum[0] <= min_sum)
                {
                    min_sum = window_sum[0];
                    min_rb = rb_count;
                    min_cb = cb_count;
                }

            }
        }
        // Calculate the point according to the window block location
        int center_y(min_rb + wnd_height / 2);
        int center_x(min_cb + wnd_width / 2);


        return cv::Point(center_x, center_y);
    }

	const cv::Point2i windowScanMin(cv::Mat &input, cv::Size size)
	{

//        qDebug() << "Global Sum is" << static_cast<int>((cv::sum(input))[0]);
		using namespace cv;
		const int wnd_width(size.width);
		const int wnd_height(size.height);

		int rb_count;
		int cb_count;
		
		int min_rb1(0), min_cb1(0);
        int min_rb2(0), min_cb2(0);
        int min_rb3(0), min_cb3(0);
        int min_sum, min_variance;
		int min_sum2, min_sum3;
        Scalar window_sum;
        QVector<Scalar> sum_queue;

        int min_queue_size = 10;

        QVector<int> min_queue(min_queue_size); // Vector storing the minimum average sum
        QVector<QPoint> min_queue_bc(min_queue_size); // The vector storing the block counters corresponding to the local sum candidates
        min_queue_bc.fill(QPoint(0,0));

        int number_of_pixels(wnd_height * wnd_width);


		for (rb_count = 0; rb_count < input.size().height - wnd_height ; ++ rb_count)
		{
			Mat sub_mat(input.rowRange(rb_count, rb_count + wnd_height));
            sum_queue.clear(); // Clear the sum queue when change line
			for (cb_count = 0; cb_count < input.size().width - wnd_width; ++cb_count)
			{
				Mat sub_sub_mat = sub_mat.colRange(cb_count, (cb_count + wnd_width ));
				if (cb_count == 0)
				{
								for	(int block_cc = 0; block_cc < size.width; ++block_cc)
								{
									cv::Scalar col_sum(cv::sum(sub_sub_mat.col(block_cc)));
									sum_queue.push_back(col_sum); // Sum all the column in the window
																																							// Construct a queue of the column sums at first window of each line
								}
								window_sum = windowSum(sum_queue);
								if (rb_count == 0)
								{
										min_sum = (int)window_sum[0];
                                        // Fill up the min_sum vector
                                        min_queue.fill(min_sum);
								}

				}
                else
                {
                        sum_queue.pop_front();
                        sum_queue.push_back(cv::sum(sub_sub_mat.col(size.width - 1)));
                        window_sum = windowSum(sum_queue);

                }

				if ((int)window_sum[0] <= min_queue.back())
                {
					min_queue.pop_front(); // Remove the largest among the three
                    min_queue.push_back((int)window_sum[0]); // Push the new minimum into the queue
                    // Shift the block counters to front
                    min_queue_bc.pop_front();
                    min_queue_bc.push_back(QPoint(rb_count, cb_count));
                }
		/*
				if ((int)window_sum[0] <= min_sum)
                {
					min_rb1 = rb_count;
					min_cb1 = cb_count;
					min_sum = (int)window_sum[0];
                }*/
		
			}
		}


        int eyeBlockIndex(0); // index of the eye block in the vector
        for (int i = 0; i < min_queue_size; ++i)
        {

            QPoint block = min_queue_bc.at(i);
            //Gets the candiate matrix
            Mat candidate = iEye::buildSubMat(block.x(), block.y(), wnd_width, wnd_height, input);
            int var = iEye::variance(candidate, min_queue.at(i)/number_of_pixels);
            if (i == 0)
            {
                min_variance = var; // Initialize the minimum variance
            }
            else
            {
                if (var < min_variance)
                {
                    eyeBlockIndex = i;
                    min_variance = var; // update the minimum variance
                }
            }

        }


        int center_y(min_queue_bc.at(eyeBlockIndex).x() + wnd_height / 2);
        int center_x(min_queue_bc.at(eyeBlockIndex).y() + wnd_width / 2);

		
		/*int center_y(min_rb1 + wnd_height / 2);
		int center_x(min_cb1 + wnd_width / 2);*/
		//qDebug() << center_x << center_y;
		return cv::Point(center_x, center_y);
		
	}



    const cv::Point2i windowScanMinStdDev(cv::Mat& input, cv::Size size)
    {
        using namespace cv;
        const int wnd_width(size.width);
        const int wnd_height(size.height);

        int rb_count;
        int cb_count;

        int min_rb1(0), min_cb1(0);
        int min_rb2(0), min_cb2(0);
        int min_rb3(0), min_cb3(0);
        int min_ave, min_variance;
        int min_sum2, min_sum3;
        Scalar window_sum;
        QVector<Scalar> sum_queue;
        QVector<int> min_queue;
//        Scalar mean, std;

        int num_of_pixels(wnd_width * wnd_height);



        for (rb_count = 0; rb_count < input.rows - wnd_height ; ++ rb_count)
        {
            Mat sub_mat(input.rowRange(rb_count, rb_count + wnd_height));
            sum_queue.clear(); // Clear the sum queue when change line
            for (cb_count = 0; cb_count < input.cols - wnd_width; ++cb_count)
            {
                Mat sub_sub_mat = sub_mat.colRange(cb_count, (cb_count + wnd_width ));
               // cv::meanStdDev(input, mean, std);
                if (cb_count == 0)
                {
                        for	(int block_cc = 0; block_cc < size.width; ++block_cc)
                        {
                            cv::Scalar col_sum(cv::sum(sub_sub_mat.col(block_cc)));
                            sum_queue.push_back(col_sum); // Sum all the column in the window
                                                                                                                                                    // Construct a queue of the column sums at first window of each line
                        }
                        window_sum = windowSum(sum_queue);
                        if (rb_count == 0)
                        {
                                min_ave = (int)window_sum[0] / num_of_pixels;
                                min_variance = iEye::variance(sub_sub_mat, min_ave);
                                min_rb1 = rb_count;
                                min_cb1 = cb_count;
                        }

                }
                else
                {
                        sum_queue.pop_front();
                        sum_queue.push_back(cv::sum(sub_sub_mat.col(size.width - 1)));
                        window_sum = windowSum(sum_queue);

                }
                //  Calculate the mean of the window.
                int mean = (int)window_sum[0] / num_of_pixels;


                // Compare mean with the minimum mean
                if (mean < min_ave)
                {
                    min_rb1 = rb_count;
                    min_cb1 = cb_count;
                    min_ave = mean;
                }
                else if(mean == min_ave) // Check variance if mean is equal
                {
                    int var(iEye::variance(sub_sub_mat, mean));
                    if (var < min_variance)
                    {
                        min_rb1 = rb_count;
                        min_cb1 = cb_count;
                        min_variance = var;
                    }
                }

            }
        }



        int center_y(min_rb1 + wnd_height / 2);
        int center_x(min_cb1 + wnd_width / 2);
        //qDebug() << center_x << center_y;
        return cv::Point(center_x, center_y);
    }


    void ieye_keybd_event(BYTE bVk, BYTE bScan)
    {
        keybd_event(bVk,
                    bScan,
                    KEYEVENTF_EXTENDEDKEY | 0,
                    0);

        keybd_event(bVk,
                    bScan,
                    KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
                    0);
    }

}
