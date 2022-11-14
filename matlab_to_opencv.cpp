#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "popt_pp.h"
#include <iomanip>

using namespace std;
using namespace cv;

std::string generate_camcal_bash(int img_w, int img_h, Mat D, Mat K, Mat R, Mat P) {
  std::stringstream curr_fs;
  std::string curr_fs_str;
  curr_fs << "#!/bin/bash\nrosservice call /stereo/left/set_camera_info \"camera_info:\n  header:\n    seq: 0\n    stamp:{secs: 0, nsecs: 0}\n    frame_id: ''\n  height: ";
  curr_fs << img_h << "\n  width: " << img_w<< "\n  distortion_model: \'plumb_bob\'\n";
  curr_fs << std::fixed << std::setprecision(8)<< "  D: " << D << "\n";
  curr_fs << std::fixed << std::setprecision(8)<< "  K: " << K << "\n";
  curr_fs << std::fixed << std::setprecision(8)<< "  R: " << R << "\n";
  curr_fs << std::fixed << std::setprecision(8)<< "  P: " << P << "\n";
  curr_fs << "  binning_x: 0\n  binning_y: 0\n  roi: {x_offset: 0, y_offset: 0, height: 0, width: 0, do_rectify: true}\"";
  curr_fs_str = curr_fs.str();
  std::replace(curr_fs_str.begin(), curr_fs_str.end(), ';', ',');

  return curr_fs_str;
}

int main(int argc, char const *argv[])
{
  char* leftcalib_file;
  char* rightcalib_file;
  char* leftimg_dir;
  char* rightimg_dir;
  char* leftimg_filename;
  char* rightimg_filename;
  char* extension;
  char* leftout_file;
  char* rightout_file;
  int num_imgs;

  static struct poptOption options[] = {
    { "leftcalib_file",'u',POPT_ARG_STRING,&leftcalib_file,0,"Left camera calibration","STR" },
    { "rightcalib_file",'v',POPT_ARG_STRING,&rightcalib_file,0,"Right camera calibration","STR" },
    { "leftout_file",'l',POPT_ARG_STRING,&leftout_file,0,"Output calibration filename (YML)","STR" },
    { "rightout_file",'r',POPT_ARG_STRING,&rightout_file,0,"Output calibration filename (YML)","STR" },
    POPT_AUTOHELP
    { NULL, 0, 0, NULL, 0, NULL, NULL }
  };

  POpt popt(NULL, argc, argv, options, 0);
  int c;
  while((c = popt.getNextOpt()) >= 0) {}

  FileStorage fsl(leftcalib_file, FileStorage::READ);
  FileStorage fsr(rightcalib_file, FileStorage::READ);

  printf("Starting Calibration\n");
  Mat K1, K2, R, F, E;
  Vec3d T;
  Mat D1, D2;

  fsl["K"] >> K1;
  fsr["K"] >> K2;
  fsl["D"] >> D1;
  fsr["D"] >> D2;
  fsl["T"] >> T;
  fsl["R"] >> R;
  
  printf("Done Calibration\n");

  printf("Starting Rectification\n");

  cv::Mat R1, R2, P1, P2, Q;
  cv::Size img_size;
  img_size.height = 1024;
  img_size.width = 1224;
  stereoRectify(K1, D1, K2, D2, img_size, R, T, R1, R2, P1, P2, Q);

  // cv::FileStorage fs1(out_file, cv::FileStorage::FORMAT_YAML | cv::FileStorage::WRITE);

  std::stringstream nextMat;
  std::string procMat;

  // ofstream outfile_stream;
  // outfile_stream.open(out_file);

  // nextMat << std::fixed << std::setprecision(8) << "rectification_matrix1:\n  rows: 3\n  cols: 3\n  data: " << R1 << "\n";
  // nextMat << std::fixed << std::setprecision(8) << "projection_matrix1:\n  rows: 3\n  cols: 4\n  data: " << P1 << "\n";
  // nextMat << std::fixed << std::setprecision(8) << "rectification_matrix2:\n  rows: 3\n  cols: 3\n  data: " << R2 << "\n";
  // nextMat << std::fixed << std::setprecision(8) << "projection_matrix2:\n  rows: 3\n  cols: 4\n  data: " << P2 << "\n";
  // procMat = nextMat.str();
  // std::replace(procMat.begin(), procMat.end(), ';', ',');
  // outfile_stream << procMat;
  
  std::string curr_fs_str = generate_camcal_bash(img_size.width, img_size.height, D1, K1, R1, P1);

  ofstream leftbash_file;
  leftbash_file.open(leftout_file);
  leftbash_file << curr_fs_str;
  leftbash_file.close();

  curr_fs_str = generate_camcal_bash(img_size.width, img_size.height, D2, K2, R2, P2);
  ofstream rightbash_file;
  rightbash_file.open(rightout_file);
  rightbash_file << curr_fs_str;
  rightbash_file.close();

  printf("Done Rectification\n");

  return 0;
}