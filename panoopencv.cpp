#include "pano.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/stitching.hpp>
#include <opencv2/opencv.hpp>
#define GOOD_DIST 0.02


using namespace cv;

vector<Mat> GetImages(vector<string> paths);
vector<Point2f> Mat2Points(Mat mat);
Mat StitchImages(vector<Mat> images, vector<Mat> tforms, Size psize);
Mat stitch(Mat im1, Mat im2);
Mat RunPano(vector<string> paths);

int Panorama::exec(vector<string> paths, string save_path){
	Mat pano = RunPano(paths);
	// pyra down panorama if it is too big to display.
	while (pano.cols > 1500 || pano.rows > 700)
	{
		pyrDown(pano, pano);
	}
	imwrite(save_path, pano);
	waitKey(0);
	return 1;
}

Mat RunPano(vector<string> paths)
{
	Mat pano;

	// get images from command line arg
	vector<Mat> images = GetImages(paths);

	// num of images
	int noImages = paths.size();

	// handle exception
	if (images.empty())
	{
		return pano;
	}

	// Detect features and find descriptor for 1st image.
	Ptr<Feature2D> SURF = xfeatures2d::SURF::create();
	vector<KeyPoint> kpoints;
	SURF->detect(images[0], kpoints);
	Mat descriptors;
	SURF->compute(images[0], kpoints, descriptors);

	// used to match features
	FlannBasedMatcher matcher;

	// Initialize projective transform matrix.
	vector<Mat> tforms(noImages);

	// default tforms for 1st image.
	tforms[0] = Mat::eye(3, 3, CV_64F);

	for(size_t i = 1; i < noImages; i++)
	{
		// store keypoints and descriptor for images[i-1]
		vector<KeyPoint> preKpoints = kpoints;
		Mat preDescriptors = descriptors;

		// detect keypoints and compute descriptors for images[i]
		SURF->detect(images[i], kpoints);
		SURF->compute(images[i], kpoints, descriptors);
		// match descriptors using FlannMatcher.
		vector<DMatch> pairs;
		matcher.match(preDescriptors, descriptors, pairs);

		// handle exception
		if (pairs.empty()) 
		{//fix r
			return Mat(1,1, CV_64F, double(0));
		}

		// compute min distance in pairs.
		double minDist = pairs[0].distance;
		for (size_t j = 1; j < pairs.size(); j++)
		{
			if (pairs[j].distance < minDist)
			{
				minDist = pairs[i].distance;
			}
		}
		if (minDist > GOOD_DIST)
			minDist = GOOD_DIST;

		// get good natches from pairs.
		// good matches have distance < 3*minDist
		vector<DMatch> goodPairs;
		while(goodPairs.size() < 4)
		{
			goodPairs.clear();
			for (size_t j = 0; j < pairs.size(); j++)
			{
				if (pairs[j].distance < 3*minDist)
				{
					goodPairs.push_back(pairs[j]);
				}
			}
			minDist = minDist + 0.01;
		}

		// get keypoint from goood matches
		vector<Point2f> pre, curr;
		for (size_t j = 0; j < goodPairs.size(); j++)
		{
			pre.push_back(preKpoints[goodPairs[j].queryIdx].pt);
			curr.push_back(kpoints[goodPairs[j].trainIdx].pt);
		}

		// estimate the transformation between images[i] and images[i-1].
		tforms[i] = findHomography(curr, pre, CV_RANSAC);

		// estimate the transformation between images[i] and first images.
		tforms[i] = tforms[i - 1] * tforms[i];
	}

	// center image index
	int centerIdx = (noImages - 1) / 2;
	// rebuild tforms relative to the center image (instead of relative to the fist image) 
	// to get a better panorama
	Mat invTform;
	invert(tforms[centerIdx], invTform);
	for (int i = 0; i < tforms.size(); i++)
	{
		tforms[i] = invTform * tforms[i];
	}

	// compute xMin, xMax, yMin, yMax of result (panorama) image.
	float xmin = 0, xmax = 0, ymin = 0, ymax = 0;
	vector<Point2f> beforeTform = Mat2Points(images[0]);
	for (size_t i = 0; i < noImages; i++)
	{
		vector<Point2f> afterTform;
		perspectiveTransform(beforeTform, afterTform, tforms[i]);
		for (int i = 0; i < afterTform.size(); i++)
		{
			if (afterTform[i].x < xmin)
				xmin = afterTform[i].x;
			if (afterTform[i].x > xmax)
				xmax = afterTform[i].x;
			if (afterTform[i].y < ymin)
				ymin = afterTform[i].y;
			if (afterTform[i].y > ymax)
				ymax = afterTform[i].y;
		}
	}

	// compute vector translation (tx, ty)
	// translate by this vector to make the result be not outside the screen(tx < 0 || ty <0)
	float tx = -xmin, ty = -ymin;

	// compute size for panorama
	float pwidth = xmax - xmin, pheight = ymax - ymin;

	// build projective tfrom for vector translation (tx, ty)
	Mat e = Mat::eye(3, 3, CV_64F);
	e.at<double>(0, 2) = tx;
	e.at<double>(1, 2) = ty;

	// apply it for all image , all tformsa
	for(int i = 0; i < tforms.size(); i++)
	{
		tforms[i] = e * tforms[i];
	}

	// build panorama using tforms
	pano = StitchImages(images, tforms, Size(pwidth,pheight));
	
	return pano;
}


vector<Mat> GetImages(vector<string> paths)
{
	vector<Mat> images;
	for (int i = 0; i < paths.size(); i++)
	{
		Mat im = imread(paths[i]);
		//while (im.cols > 500 )
		//{
		//	pyrDown(im, im);
		//}
		images.push_back(im);
	}
	return images;// khong hieu loi~ lam 
}

Mat StitchImages(vector<Mat> images, vector<Mat> tforms, Size psize)
{
	if (images.empty())
		return Mat::zeros(500, 500, CV_64F);
	int centerIdx = images.size() / 2;
	vector<Mat> masks;
	for (int i = 0; i < images.size(); i++)
	{
		Mat currMask;
		warpPerspective(images[i], currMask, tforms[i], psize);
		masks.push_back(currMask);
	}
	Mat panorama = masks[centerIdx];
	for (int i = 0; i < images.size(); i++)
	{
		//addWeighted(panorama, 0.5, masks[i], 0.5, 0.0, panorama);
		panorama = stitch(panorama, masks[i]);
	}
	return panorama;
}

Mat stitch(Mat im1, Mat im2)
{
	Mat result = im1;
	for (size_t i = 0; i < result.rows; i++)
	{
		for (size_t j = 0; j < result.cols; j++)
		{
			if (im2.at<Vec3b>(i, j) != Vec3b(0, 0, 0))
			{
				result.at<Vec3b>(i, j) = im2.at<Vec3b>(i, j);
			}
		}
	}
	return result;
}

vector<Point2f> Mat2Points(Mat mat)
{
	vector<Point2f> pts;
	for (size_t row = 0; row < mat.rows; row++)
	{
		for (size_t col = 0; col < mat.cols; col++)
		{
			pts.push_back(Point2f(col, row));
		}
	}
	return pts;
}

