
struct interestPoint {
  cv::Point2f currentFeature;
  cv::Point2f previousFeature;
  double MotionVectorAngle;
  bool isForeground;
  int clusterLabel;
};

struct cluster {
  int label;
  int kMeansLink;
  std::vector<interestPoint> interestPoints;
  interestPoint centroid;
};
