#include "stdafx.h"
#include "CoordinateConverter.h"

#include <opencv2/imgproc.hpp>
#include <eigen3/Eigen/Eigen>

using namespace cv;
using namespace Eigen;

static void getPerspectiveTransform(Vector2d(&src)[4], Vector2d(&dst)[4], Matrix3d& res)
{
	// Copy from OpenCv Source Code

	Matrix<double, 8, 8> A;
	Matrix<double, 8, 1> B;
	for (int i = 0; i < 4; ++i)
	{
        A(i, 0) = A(i + 4, 3) = src[i].x();
        A(i, 1) = A(i + 4, 4) = src[i].y();
		A(i, 2) = A(i + 4, 5) = 1;

		A(i, 3) = A(i, 4) = A(i, 5) =
			A(i + 4, 0) = A(i + 4, 1) = A(i + 4, 2) = 0.0;

        A(i, 6) = -src[i].x()*dst[i].x();
        A(i, 7) = -src[i].y()*dst[i].x();
        A(i + 4, 6) = -src[i].x()*dst[i].y();
        A(i + 4, 7) = -src[i].y()*dst[i].y();

        B[i] = dst[i].x();
        B[i + 4] = dst[i].y();
	}

    Matrix<double, 8, 1> X = A.jacobiSvd(ComputeFullU | ComputeFullV).solve(B);

	res(0, 0) = X[0];
	res(0, 1) = X[1];
	res(0, 2) = X[2];
	res(1, 0) = X[3];
	res(1, 1) = X[4];
	res(1, 2) = X[5];
	res(2, 0) = X[6];
	res(2, 1) = X[7];
	res(2, 2) = 1.0;
}

void CoordinateConverter::updateParam()
{
    Array2d vlogicSize(_param.LogicSize.width(),_param.LogicSize.height());
    Array2d vcamSize(_param.CameraSize.width(), _param.CameraSize.height());

    Vector2d logicCorners[4] = {
        {0, 0},
        {1.0, 0.0},
        {0.0, 1.0},
        {1.0, 1.0},
	};

    for (Vector2d& x : logicCorners) x.array() *= vlogicSize;

    auto const& corners = _param.CornersInCamera;
    Vector2d cornersInCamera[4] = {
        {corners[0].x(), corners[0].y()},
        {corners[1].x(), corners[1].y()},
        {corners[2].x(), corners[2].y()},
        {corners[3].x(), corners[3].y()},
    };

    for (Vector2d& x: cornersInCamera) x.array() *= vcamSize;

    getPerspectiveTransform(cornersInCamera, logicCorners, _cam2logic);
	_logic2cam = _cam2logic.inverse();
}

QPointF CoordinateConverter::cam2logic(const Point2f& p) const
{
    Vector3d v = _cam2logic*Vector3d(p.x, p.y, 1.0);
    return {v[0]/v[2], v[1]/v[2]};
}

Point2f CoordinateConverter::logic2cam(const QPointF& p) const
{
    Vector3d v = _logic2cam*Vector3d(p.x(), p.y(), 1.0);
    return {float(v[0]/v[2]), float(v[1]/v[2])};
}

