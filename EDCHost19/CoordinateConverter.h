#pragma once
#include <QtCore>
#include <opencv2/core.hpp>
#include <eigen3/Eigen/Eigen>
class CoordinateConverter
{
public:
	struct Param {
        QSize CameraSize;
        QPointF CornersInCamera[4];

        QSizeF LogicSize;
	};

    CoordinateConverter()
    {
        _cam2logic.setIdentity();
        _logic2cam.setIdentity();
    }

	void SetParam(const Param &param)
	{
		_param = param;
		updateParam();
	}
	Param GetParam() const { return _param; }
    Param* GetParamAddress() { return &_param; }

    void updateParam();

    QPointF cam2logic(const cv::Point2f &);
    cv::Point2f logic2cam(const QPointF &);

private:
	Param _param;
	Eigen::Matrix3d _cam2logic;
	Eigen::Matrix3d _logic2cam;
};
