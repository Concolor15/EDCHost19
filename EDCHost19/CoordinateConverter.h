#pragma once
#include <opencv2/core.hpp>
#include <eigen3/Eigen/Eigen>
class CoordinateConverter
{
public:
	struct Param {
		cv::Size2i CameraSize;
		cv::Point2f CornersInCamera[4];

		cv::Size2f LogicSize;

		cv::Size2i DisplaySize;
		//cv::Point2f CornersInDisplay[4];
	};

	void TransformImage(const cv::Mat &src, cv::Mat& dst);

	void SetParam(const Param &param)
	{
		_param = param;
		updateParam();
	}
	Param GetParam() const { return _param; }
	cv::Point2f cam2logic(const cv::Point2f &);
	cv::Point2f logic2cam(const cv::Point2f &);

private:
	Param _param;
	Eigen::Matrix3d _cam2logic;
	Eigen::Matrix3d _logic2cam;

	cv::Mat transformMatrix;

	void updateParam();
};
