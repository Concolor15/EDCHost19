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

void ImgProc::genResult(LocateResult* r, QTime const& timestamp)
{
    if (!ball_centers.empty())
    {
        cv::Point2f rawP = ball_centers[0];
        QPointF p = cvt->cam2logic(rawP);
        ball_tracker.update(p, cvToQ(rawP), timestamp);
    }
    else
    {
        ball_tracker.update_failure(timestamp);
    }

    for (int i: {0,1})
    {
        if (!cars_centers[i].empty())
        {
            cv::Point2f rawP = cars_centers[i][0];
            QPointF p = cvt->cam2logic(rawP);
            car_tracker[i].update(p, cvToQ(rawP), timestamp);
        }
        else
        {
            car_tracker[i].update_failure(timestamp);
        }
    }

    r->timestamp = timestamp;
    ball_tracker.genReport(&r->ball);
    car_tracker[0].genReport(&r->cars[0]);
    car_tracker[1].genReport(&r->cars[1]);
}

void ImgProc::binarize(const Mat &yuv)
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
}

LocateResult* ImgProc::Locate(Mat const& yuv, QTime const& timestamp)
{
    binarize(yuv);

    if (debugEnabled)
    {
        cv::merge(vector<Mat>{ ball, car1, car2}, merged);
    }

    ball_centers = GetCenter(ball, Types::BALL);
    cars_centers[0] = GetCenter(car1, Types::CARA);
    cars_centers[1] = GetCenter(car2, Types::CARB);

    if (debugEnabled)
    {
        yuv.copyTo(dst);
        for (auto & cts : ball_centers)
        {
            circle(dst, cts, 10, Scalar(255, 0, 0),-1);
        }
        for (auto & cts : cars_centers[0])
        {
            circle(dst, cts, 10, Scalar(0, 255, 0), -1);
        }
        for (auto & cts : cars_centers[1])
        {
            circle(dst, cts, 10, Scalar(0, 0, 255), -1);
        }

        // Black Magic to invoke cv::imshow in main thread

        QTimer::singleShot(0, qApp, [p_dst=&dst, p_merged=&merged](){
            imshow("show", *p_dst);
            imshow("black", *p_merged);
        });
    }

    LocateResult* r = new LocateResult();
    genResult(r, timestamp);
    return r;
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
