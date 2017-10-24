#include "stdafx.h"
#include "HighResCam.h"

using namespace cv;

void HighResCam::TransToLogic(QVector<cv::Point2f>& info)
{
	for (auto &pt : info)
	{
		pt = ccToLogic.cam2logic(pt);
	}
}

HighResCam::HighResCam(QObject *parent)
	:QAbstractVideoSurface(parent)
{
	locateMachine.InitCv();
	CoordinateConverter::Param theParam;
	theParam.CameraSize = Size2i(nCamWidth, nCamHeight);
	theParam.DisplaySize = Size2i(nViewWidth, nViewHeight);
	theParam.LogicSize = Size2i(nLogicWidth, nLogicHeight); 
	theParam.CornersInCamera[0] = Point2f(0, 0);
	theParam.CornersInCamera[1] = Point2f(nCamWidth, 0); 
	theParam.CornersInCamera[2] = Point2f(0, nCamHeight);
	theParam.CornersInCamera[3] = Point2f(nCamWidth, nCamHeight);
	ccToLogic.SetParam(theParam);
} 

HighResCam::~HighResCam()
{

}

QList<QVideoFrame::PixelFormat> HighResCam::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
	return (QList<QVideoFrame::PixelFormat>()<<QVideoFrame::Format_BGR24<<QVideoFrame::Format_RGB32);
}

bool HighResCam::present(const QVideoFrame & frame)
{
	if (frame.pixelFormat() != QVideoFrame::Format_RGB32) {
		setError(IncorrectFormatError);
		return false;
	}
	auto t = frame.handleType();
	auto handle = frame.handle().toUInt();
	QVideoFrame frametodraw(frame);

	QSize size = frame.size();

	if (!frametodraw.map(QAbstractVideoBuffer::ReadOnly))
	{
		setError(ResourceError);
		return false;
	}
	cv::Mat matToLocate = cv::Mat(frametodraw.height(),
		frametodraw.width(),
		CV_8UC4,
		frametodraw.bits(),
		frametodraw.bytesPerLine());
	locateMachine.Locate(matToLocate);
	QImage image = QImage(
		frametodraw.bits(),
		frametodraw.width(),
		frametodraw.height(),
		frametodraw.bytesPerLine(),
		QImage::Format_RGB32);
	auto info = locateMachine.GetLocation();
#ifdef PERS_DEBUG
	auto qstrCam = QString("Camera-Ball:(%1,%2)\n").arg(int(info[0].x)).arg(int(info[0].y));
	qstrCam += QString("Camera-CarA:(%1,%2)\n").arg(int(info[1].x)).arg((int)info[1].y);
	qstrCam += QString("Camera-CarB:(%1,%2)\n").arg(int(info[2].x)).arg((int)info[2].y);
#endif
	TransToLogic(info);
#ifdef PERS_DEBUG
	qstrCam += QString("Logic-Ball:(%1,%2)\n").arg(int(info[0].x)).arg(int(info[0].y));
	qstrCam += QString("Logic-CarA:(%1,%2)\n").arg(int(info[1].x)).arg(int(info[1].y));
	qstrCam += QString("Logic-CarB:(%1,%2)\n").arg(int(info[2].x)).arg(int(info[2].y));
	emit DebugPers(qstrCam);
#endif
	QPixmap pixSignal(QPixmap::fromImage(image));
	auto infCompressed = CameraInfo(info[0], info[1], info[2]);
	emit ImageArrived(infCompressed, pixSignal);
	frametodraw.unmap();
	return true;
}

void HighResCam::SetPerspective(const QVector<cv::Point2f>& pts)
{
	CoordinateConverter::Param theParam = ccToLogic.GetParam();
	theParam.CornersInCamera[0] = pts[0];
	theParam.CornersInCamera[1] = pts[1];
	theParam.CornersInCamera[2] = pts[2];
	theParam.CornersInCamera[3] = pts[3];
	ccToLogic.SetParam(theParam);
}

QVector<cv::Point2f> ImgProc::GetLocation()
{
	Point2f ptA(0,0), ptB(0,0), ptBall(0,0);
	if (!ball_centers.empty()) ptBall = ball_centers[0];
	if (!car1_centers.empty()) ptA = car1_centers[0];
	if (!car2_centers.empty()) ptB = car2_centers[0];
	return QVector<cv::Point2f>{ptBall, ptA, ptB};
}

void ImgProc::InitCv()
{
	config.ball_hue_lb = 0;
	config.ball_hue_ub = 0;
	config.car1_hue_lb = 0;
	config.car1_hue_ub = 0;
	config.car2_hue_lb = 0;
	config.car2_hue_ub = 0;
	config.car1_s_lb = 0;
	config.car2_s_lb = 0;
	config.ball_s_lb = 0;
	config.v_lb = 0;
	config.area_car_lb = 100;
	config.area_ball_lb = 100;
#ifdef CAMERA_DEBUG
	namedWindow("show");
	namedWindow("black");
	namedWindow("control", CV_WINDOW_NORMAL);
	createTrackbar("ball_hue_lb", "control", &config.ball_hue_lb, 180);
	createTrackbar("ball_hue_ub", "control", &config.ball_hue_ub, 180);
	createTrackbar("car1_hue_lb", "control", &config.car1_hue_lb, 180);
	createTrackbar("car1_hue_ub", "control", &config.car1_hue_ub, 180);
	createTrackbar("car2_hue_lb", "control", &config.car2_hue_lb, 180);
	createTrackbar("car2_hue_ub", "control", &config.car2_hue_ub, 180);
	createTrackbar("ball_s_lb", "control", &config.ball_s_lb, 255);
	createTrackbar("car1_s_lb", "control", &config.car1_s_lb, 255);
	createTrackbar("car2_s_lb", "control", &config.car2_s_lb, 255);
	createTrackbar("v_lb", "control", &config.v_lb, 255);
	createTrackbar("area_car_lb", "control", &config.area_car_lb, 1000);
	createTrackbar("area_ball_lb", "control", &config.area_ball_lb, 200);
#endif
}

void ImgProc::Locate(Mat& mat)
{
	src = mat;
	cvtColor(src, hsv, COLOR_BGR2HSV);
	cv::inRange(hsv,
		Scalar(0, config.ball_s_lb, config.v_lb),
		Scalar(180, 255, 255),
		mask_ball);
	cv::inRange(hsv,
		Scalar(0, config.car1_s_lb, config.v_lb),
		Scalar(180, 255, 255),
		mask_car1);
	cv::inRange(hsv,
		Scalar(0, config.car2_s_lb, config.v_lb),
		Scalar(180, 255, 255),
		mask_car2);
	int ch[] = { 0,0 };
	hue.create(src.size(), CV_8UC1);
	mixChannels(&hsv, 1, &hue, 1, ch, 1);
	inRange(hue, Scalar(config.ball_hue_lb), Scalar(config.ball_hue_ub), ball);
	cv::bitwise_and(ball, mask_ball, ball);
	inRange(hue, Scalar(config.car1_hue_lb), Scalar(config.car1_hue_ub), car1);
	cv::bitwise_and(car1, mask_car1, car1);
	inRange(hue, Scalar(config.car2_hue_lb), Scalar(config.car2_hue_ub), car2);
	cv::bitwise_and(car2, mask_car2, car2);
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
	imshow("show", dst);
	cv::merge(vector<Mat>{ car1, car2, ball }, merged);
	imshow("black", merged);
#endif
}

ImgProc::~ImgProc()
{
	
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
		if (nType == Types::BALL && area < area_ball_lb) continue;
		rett.push_back(center);		
	}
	return rett;
}
