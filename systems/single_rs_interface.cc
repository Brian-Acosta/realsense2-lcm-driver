#include "single_rs_interface.h"

namespace rs2_systems {

SingleRSInterface::SingleRSInterface() {
  config_.enable_stream(
      RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 30);
  config_.enable_stream(
      RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 30);
}

void SingleRSInterface::Start() {
  run_ = true;
  pipeline_.start(config_);
  poll_thread_ = std::thread(&SingleRSInterface::poll, this);
  pc_thread_ = std::thread(&SingleRSInterface::process_pc, this);
}

void SingleRSInterface::Stop() {
  run_ = false;

  if (poll_thread_.joinable()) {
    poll_thread_.join();
  }

  if (pc_thread_.joinable()) {
    pc_thread_.join();
  }

  try {
    pipeline_.stop();
  } catch (rs2::wrong_api_call_sequence_error& e) {
    // make stop() essentially a no-op if pipeline is stopped
  }

}

void SingleRSInterface::poll() {
  rs2::frameset frames;

  while (run_) {
    std::scoped_lock<std::mutex> lock(frameset_mutex_);
    frames = pipeline_.wait_for_frames();
    latest_depth_.push(frames.get_depth_frame());
    latest_aligned_frames_.push(frame_aligner_.process(frames));
    has_new_frame_cond_var_.notify_all();
  }
}

void SingleRSInterface::process_pc() {
  rs2::decimation_filter decimation;
  rs2::frame depth;
  rs2::pointcloud pc;
  decimation.set_option(RS2_OPTION_FILTER_MAGNITUDE, 4.0f);

  while (run_) {
    std::unique_lock<std::mutex> frame_lock(frameset_mutex_);
    has_new_frame_cond_var_.wait(
        frame_lock, [this] {return not latest_depth_.empty();}
    );

    depth = latest_depth_.front();
    latest_depth_.pop();

    frame_lock.unlock();

    depth = decimation.process(depth);
    std::unique_lock<std::mutex> pc_lock(points_mutex_);
    latest_pc_ = pc.calculate(depth);
    pc_lock.unlock();
  }
}


SingleRSInterface::~SingleRSInterface() {
 this->Stop();
}



}