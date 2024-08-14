#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

#include "librealsense2/rs.hpp"

namespace rs2_systems {
class SingleRSInterface {
 public:
  SingleRSInterface();
  ~SingleRSInterface();

  void Start();
  void Stop();

 private:

  bool run_{false};

  void poll();
  void process_pc();

  rs2::pipeline pipeline_{};
  rs2::config config_{};
  rs2::align frame_aligner_{RS2_STREAM_COLOR};

  std::queue<rs2::frameset> latest_aligned_frames_{};
  std::queue<rs2::frame> latest_depth_{};
  rs2::points latest_pc_{};

  std::mutex points_mutex_;
  std::mutex frameset_mutex_;

  std::thread poll_thread_;
  std::thread pc_thread_;

  std::condition_variable has_new_frame_cond_var_;
};
}