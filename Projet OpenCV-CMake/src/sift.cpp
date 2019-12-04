//
// Created by xizheng on 11/28/19.
//

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "sift.h"
#include <vector>
#include <opencv2/xfeatures2d/nonfree.hpp>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

void performSift() {
    surfTest();
    return;
//    Mat queryImg = imread("../Images/query.png", 0);    // Load as gray scale
//    Mat trainImg = imread("../Images/train.png", 0);    // Load as gray scale
    Mat queryImg = imread("../Images/a.jpeg", 0);    // Load as gray scale
    Mat trainImg = imread("../Images/a.jpeg", 0);    // Load as gray scale

    imshow("query image", queryImg);
    imshow("train gray", trainImg);

    // Detect keypoints in both images.
//    xfeatures2d::SiftFeatureDetector detector;
//    vector<KeyPoint> queryKeypoints, trainKeypoints;
//    detector.detect(queryImg, queryKeypoints);
//    detector.detect(trainImg, trainKeypoints);

    Ptr<cv::xfeatures2d::SIFT> detector = SiftFeatureDetector::create();
    std::vector<cv::KeyPoint> keypoints;
    detector->detect(trainImg, keypoints);
    printf("Found %dkeypoints.\n", keypoints.size());



    // Print how many keypoints were found in each image.
//    printf("Found %d and %d keypoints.\n", queryKeypoints.size(), trainKeypoints.size());
}

void surfTest() {
    Mat img1 = imread("../Images/query.png", 0);    // Load as gray scale
    Mat img2= imread("../Images/train.png", 0);    // Load as gray scale

    int minHessian = 400;
    Ptr<SURF> detector = SURF::create( minHessian );
    std::vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    detector->detectAndCompute( img1, noArray(), keypoints1, descriptors1 );
    detector->detectAndCompute( img2, noArray(), keypoints2, descriptors2 );

    //-- Step 2: Matching descriptor vectors with a FLANN based matcher
    // Since SURF is a floating-point descriptor NORM_L2 is used
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
    std::vector< std::vector<DMatch> > knn_matches;
    matcher->knnMatch( descriptors1, descriptors2, knn_matches, 2 );

    //-- Filter matches using the Lowe's ratio surfTest
    const float ratio_thresh = 0.7f;
    std::vector<DMatch> good_matches;
    for (size_t i = 0; i < knn_matches.size(); i++) {
        if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
            good_matches.push_back(knn_matches[i][0]);
    }

    //-- Draw matches
    Mat img_matches;
    drawMatches( img1, keypoints1, img2, keypoints2, good_matches, img_matches, Scalar::all(-1),
                 Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    //-- Show detected matches
    imshow("Good Matches", img_matches );

}


