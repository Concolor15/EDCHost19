#include "stdafx.h"
#include "HighResCam.h"

using namespace cv;

HighResCam::HighResCam(QObject *parent)
	:QAbstractVideoSurface(parent)
{
	locateMachine.InitCv();
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
	QPixmap pixSignal(QPixmap::fromImage(image.copy()));
	emit ImageArrived(info, pixSignal);
	frametodraw.unmap();

	return true;
}

CameraInfo ImgProc::GetLocation()
{
	Point ptA(0,0), ptB(0,0), ptBall(0,0);
	if (!ball_centers.empty()) ptBall = ball_centers[0];
	if (!car1_centers.empty()) ptA = car1_centers[0];
	if (!car2_centers.empty()) ptB = car2_centers[0];
	return CameraInfo(ptBall, ptA, ptB);
}

void ImgProc::InitCv()
{
	config.ball_hue_lb = 0;
	config.ball_hue_ub = 0;
	config.car1_hue_lb = 0;
	config.car1_hue_ub = 0;
	config.car2_hue_lb = 0;
	config.car2_hue_ub = 0;
	config.s_lb = 0;
	config.v_lb = 0;
	config.area_lb = 100;
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
	createTrackbar("s_lb", "control", &config.s_lb, 255);
	createTrackbar("v_lb", "control", &config.v_lb, 255);
	createTrackbar("area_lb", "control", &config.area_lb, 255);
#endif
}

void ImgProc::Locate(Mat& mat)
{
	src = mat;

	//static QElapsedTimer timer;
	//timer.start();

	cvtColor(src, hsv, COLOR_BGR2HSV);

	// 生成 mask 和提取 h 通道
#pragma region
	cv::inRange(hsv,
		Scalar(0, config.s_lb, config.v_lb),
		Scalar(180, 255, 255),
		mask);

	int ch[] = { 0,0 };
	hue.create(src.size(), CV_8UC1);
	mixChannels(&hsv, 1, &hue, 1, ch, 1);
#pragma endregion

	//printf("%lld\n", timer.elapsed());

	// 生成球和小车对应色块的二值化图像
#pragma region
	//Mat tmp;
	inRange(hue, Scalar(config.ball_hue_lb), Scalar(config.ball_hue_ub), ball);
	cv::bitwise_and(ball, mask, ball);

	inRange(hue, Scalar(config.car1_hue_lb), Scalar(config.car1_hue_ub), car1);
	cv::bitwise_and(car1, mask, car1);

	inRange(hue, Scalar(config.car2_hue_lb), Scalar(config.car2_hue_ub), car2);
	cv::bitwise_and(car2, mask, car2);

	//printf("%lld\n", timer.elapsed());
#pragma endregion

	//cvtColor(src, dst, COLOR_BGR2GRAY, 3);
	src.copyTo(dst);

	ball_centers = GetCenter(ball, config);
	car1_centers = GetCenter(car1, config);
	car2_centers = GetCenter(car2, config);
	//printf("%lld\n\n", timer.elapsed());
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
	// 调试输出
	imshow("show", dst);

	cv::merge(vector<Mat>{ car1, car2, ball }, merged);
	imshow("black", merged);
#endif
}

vector<Point2f> ImgProc::GetCenter(Mat src, const ProcConfig & cfg)
{
	float area_lb = cfg.area_lb;

	vector<Point2f> rett;
	vector<vector<Point>> contours;

	findContours(src, contours, cv::noArray(), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	for (auto const& contour : contours)
	{
		Point2f center;
		float radius;

		auto const moment = moments(contour);
		double area = moment.m00;
		center.x = moment.m10 / moment.m00;
		center.y = moment.m01 / moment.m00;

		if (area < area_lb)
			continue;

		rett.push_back(center);

		//mark the center of contours
		
	}

	return rett;
}
