#include "stdafx.h"
#include "config.h"

#include "imgproc.h"
#include <vector>

using namespace std;
using namespace cv;

QPointF cvToQ(cv::Point2f p)
{
    return {p.x, p.y};
}

LocateResult* ImgProc::GetResult() {
    LocateResult* r = new LocateResult;

    r->ball_succeeded = false;
    r->cars_succeeded = false;

    if (!ball_centers.empty())
    {
        r->ball_succeeded = true;
        cv::Point2f p = ball_centers[0];
        r->ball_center = cvToQ(p);
        r->logic_ball_center = cvt->cam2logic(p);
    }

    if (!car1_centers.empty() && !car2_centers.empty())
    {
        r->cars_succeeded = true;
        cv::Point2f ps[2] = {car1_centers[0], car2_centers[0]};

        for (int i: {0,1})
        {
            r->cars_center[i]=cvToQ(ps[i]);
            r->logic_cars_center[i] = cvt->cam2logic(ps[i]);

        }
    }

    return r;
}

void ImgProc::Locate(Mat& mat)
{
	src = mat;
    hsv = mat;

    auto const& config = *this->config;

    cv::inRange(hsv,
                Scalar(config.ball_Y_lb, config.ball_U_lb, config.ball_V_lb),
                Scalar(config.ball_Y_ub, config.ball_U_ub, config.ball_V_ub),
                ball);
    cv::inRange(hsv,
                Scalar(config.car1_Y_lb, config.car1_U_lb, config.car1_V_lb),
                Scalar(config.car1_Y_ub, config.car1_U_ub, config.car1_V_ub),
                car1);
    cv::inRange(hsv,
                Scalar(config.car2_Y_lb, config.car2_U_lb, config.car2_V_lb),
                Scalar(config.car2_Y_ub, config.car2_U_ub, config.car2_V_ub),
                car2);

	src.copyTo(dst);
	ball_centers = GetCenter(ball, config, Types::BALL);
	car1_centers = GetCenter(car1, config, Types::CARA);
	car2_centers = GetCenter(car2, config, Types::CARB);
	for (auto & cts : ball_centers)
	{
		circle(dst, cts, 10, Scalar(255, 0, 0),-1);
	}
	for (auto & cts : car1_centers)
	{
		circle(dst, cts, 10, Scalar(0, 255, 0), -1);
	}
	for (auto & cts : car2_centers)
	{
		circle(dst, cts, 10, Scalar(0, 0, 255), -1);
	}
#ifdef CAMERA_DEBUG
    //imshow("show", dst);
    cv::merge(vector<Mat>{ ball, car1, car2}, merged);
    //imshow("black", merged);
    //QEventLoop::processEvents();
#endif
}

vector<Point2f> ImgProc::GetCenter(Mat src, const ProcConfig & cfg, int nType)
{
	float area_car_lb = cfg.area_car_lb;
	float area_ball_lb = cfg.area_ball_lb;
	vector<Point2f> rett;
	vector<vector<Point>> contours;
	findContours(src, contours, cv::noArray(), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	for (auto const& contour : contours)
	{
		Point2f center;
		auto const moment = moments(contour);
		double area = moment.m00;
		center.x = moment.m10 / moment.m00;
		center.y = moment.m01 / moment.m00;
		if ((nType == Types::CARA || nType == Types::CARB) && area < area_car_lb) continue;
        if (nType == Types::BALL)
        {
            if (area < area_ball_lb) continue;
            if (area > 500) continue;
        }
		rett.push_back(center);		
	}
	return rett;
}
