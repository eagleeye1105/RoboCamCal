//
// Created by yongqi on 18-1-3.
//

#ifndef ROBOCAMCAL_BOARDDETECTOR_HPP
#define ROBOCAMCAL_BOARDDETECTOR_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include "CalibrationCommon.hpp"

namespace Robocamcal {

class BoardDetector {
 public:
  BoardDetector(const std::string &cfg_file);

  /**
   * Detect calibration board from frame
   * @param frame input frame
   * @return is board detected
   */
  bool Detect(const cv::Mat &frame);

  /**
   * Get calibration board points in image coordinate
   * @return image points
   */
  inline const std::vector<cv::Point2f> &GetImagePoints() const {
    return image_points_;
  }

  /**
   * Get calibration board points in camera coordinate
   * @return object points
   */
  inline const std::vector<cv::Point3f> &GetObjectPoints() const {
    return object_points_;
  }

  /**
   * Get calibration board rotation and translation vectors in Rodrigues form
   * @param camera_matrix
   * @param dist_coeffs
   * @param rotation rotation vector
   * @param translation translation vector
   */
  inline void CalculateBoardPose(const cv::Mat &camera_matrix, const cv::Mat& dist_coeffs,
                                 cv::Vec3d& rotation, cv::Vec3d &translation) const {
    cv::solvePnPRansac(object_points_, image_points_, camera_matrix, dist_coeffs, rotation, translation);
  }

  /**
   * Draw board points in frame
   * @param frame
   */
  void DrawImagePoints(cv::Mat &frame) const;

  /**
   * Draw 3d-axis based on board pose in frame
   * @param frame
   * @param rotation
   * @param translation
   * @param axis_length
   */
  inline static void DrawBoardPose(cv::Mat &frame, const cv::Mat &camera_matrix,
                                   const cv::Mat &dist_coeffs, const cv::Vec3d& rotation,
                                   const cv::Vec3d &translation, float axis_length) {
    cv::aruco::drawAxis(frame, camera_matrix, dist_coeffs, rotation, translation, axis_length);
  }

 private:
  bool DetectChessBoard(const cv::Mat &frame);
  bool DetectAcircleGrid(const cv::Mat &frame);
  bool DetectArucoBoard(const cv::Mat &frame);

  // results
  bool board_found_;
  std::vector<cv::Point2f> image_points_;
  std::vector<cv::Point3f> object_points_;
  // params
  BoardType board_type_;
  cv::Size board_size_;
  float board_len_;
};

}

#endif //ROBOCAMCAL_BOARDDETECTOR_HPP
