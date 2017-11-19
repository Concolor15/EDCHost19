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

    if (!ball_centers.empty())
    {
        r->ball_succeeded = true;
        cv::Point2f p = ball_centers[0];
        r->ball_center = cvToQ(p);
        r->logic_ball_center = cvt->cam2logic(p);
    }

    r->cars_succeeded = !car1_centers.empty() && !car2_centers.empty();

    cv::Point2f ps[2] = {
        !car1_centers.empty() ? car1_centers[0] : cv::Point2f(0, 0),
        !car2_centers.empty() ? car2_centers[0] : cv::Point2f(0, 0),
    };

    for (int i: {0,1})
    {
        r->cars_center[i]=cvToQ(ps[i]);
        r->logic_cars_center[i] = cvt->cam2logic(ps[i]);
    }

    return r;
}

void ImgProc::Locate(Mat& yuv)
{
    auto const& config = *this->config;

    cv::inRange(yuv,
                Scalar(config.ball_Y_lb, config.ball_U_lb, config.ball_V_lb),
                Scalar(config.ball_Y_ub, config.ball_U_ub, config.ball_V_ub),
                ball);
    cv::inRange(yuv,
                Scalar(config.car1_Y_lb, config.car1_U_lb, config.car1_V_lb),
                Scalar(config.car1_Y_ub, config.car1_U_ub, config.car1_V_ub),
                car1);
    cv::inRange(yuv,
                Scalar(config.car2_Y_lb, config.car2_U_lb, config.car2_V_lb),
                Scalar(config.car2_Y_ub, config.car2_U_ub, config.car2_V_ub),
                car2);

    if (debugEnabled)
    {
        cv::merge(vector<Mat>{ ball, car1, car2}, merged);
    }

    ball_centers = GetCenter(ball, Types::BALL);
    car1_centers = GetCenter(car1, Types::CARA);
    car2_centers = GetCenter(car2, Types::CARB);

    if (debugEnabled)
    {
        yuv.copyTo(dst);
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

        // Black Magic to invoke cv::imshow in main thread

        QTimer::singleShot(0, qApp, [p_dst=&dst, p_merged=&merged](){
            imshow("show", *p_dst);
            imshow("balck", *p_merged);
        });
    }
}

vector<Point2f> ImgProc::GetCenter(Mat v2, int nType)
{
    auto max_sqr = [](double x, double y){double r=x>y?x:y; return r*r;};

    QRectF logicBound(QPointF(0, 0), cvt->getParam().LogicSize);

    double area_car_lb = config->area_car_lb;
    double area_ball_ub = config->area_ball_ub;
    double area_ball_r_lb = config->area_ball_r_lb / 100.0;
    double area_ball_lb = config->area_ball_lb;
	vector<Point2f> rett;
	vector<vector<Point>> contours;
    findContours(v2, contours, cv::noArray(), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	for (auto const& contour : contours)
	{
		Point2f center;
		auto const moment = moments(contour);
		double area = moment.m00;
		center.x = moment.m10 / moment.m00;
		center.y = moment.m01 / moment.m00;

        QPointF logicCenter = cvt->cam2logic(center);
        if (!logicBound.contains(logicCenter))
            continue;

		if ((nType == Types::CARA || nType == Types::CARB) && area < area_car_lb) continue;
        if (nType == Types::BALL)
        {
            if (area < area_ball_lb) continue;
            if (area > area_ball_ub) continue;

            Rect bound = boundingRect(contour);

            if (area < area_ball_r_lb*max_sqr(bound.width, bound.height))
                continue;
        }
        else
        {
            if (area < area_car_lb) continue;
        }
		rett.push_back(center);		
	}
	return rett;
}
