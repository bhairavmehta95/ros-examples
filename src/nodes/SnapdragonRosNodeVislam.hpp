/****************************************************************************
 *   Copyright (c) 2016 Ramakrishna Kintada. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name ATLFlight nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
#include <ros/ros.h>

#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>

#include <mvVISLAM.h>
#include "SnapdragonVislamManager.hpp"

// Custom Includes
#include <opencv2/core/core.hpp>

#include <dynamic_reconfigure/server.h>
#include <snap_ros_examples/SnapdragonConfig.h>


/**
 * Wrapper Ros Node to support VISLAM from Snapdragon flight platform.
 * VislamDynRec = Class to hold dynamic reconfigure parameters
 */

namespace Snapdragon {
  namespace RosNode {
    class Vislam;

    class VislamDynRec;
  }
}

/**
 * Ros Vislam Node that uses Snapdragon platform to get VISLAM pose data.
 */
class Snapdragon::RosNode::Vislam
{
public:
  /**
   * Constructor.
   * @param nh
   *   Ros Node handle to intialize the node.
   */
  Vislam( ros::NodeHandle nh );

  /**
   * Initialized the Ros Vislam Node. This does the initialization for the
   * Snapdragon VISLAM module as well.
   * @return int32_t
   *  0 = success
   * otherwise = failure;
   **/
  int32_t Initialize();

  /**
   * Start the VISLAM node processing.
   * @return int32_t
   *  0 = success
   * otherwise = false;
   **/
  int32_t Start();

  /**
   * Stops the VISLAM processing thread.
   * @return int32_t
   *  0 = success;
   * otherwise = false;
   **/
  int32_t Stop();

  /**
   * Callback to change FPS
   */

  void fpsCbFunction( snap_ros_examples::SnapdragonConfig &config, uint32_t level );  
  /**
   * Destructor for the node.
   */
  ~Vislam();
private: 

  // class methods
  int32_t PublishVislamData( mvVISLAMPose& vislamPose, int64_t vislamFrameId, uint64_t timestamp_ns );
  void PublishImageData( cv::Mat& image_mat ); 

  void ThreadMain();


  // data members;
  std::thread       vislam_process_thread_;
  std::atomic<bool> thread_started_;
  std::atomic<bool> thread_stop_;
  std::atomic<bool> vislam_initialized_;
  ros::NodeHandle  nh_;
  ros::Publisher   pub_vislam_pose_;
  ros::Publisher   pub_vislam_odometry_;
  ros::Publisher   pub_vislam_image_;

  dynamic_reconfigure::Server<snap_ros_examples::SnapdragonConfig> server;
  dynamic_reconfigure::Server<snap_ros_examples::SnapdragonConfig>::CallbackType fpsCb;

  Snapdragon::VislamManager vislam_manager_;
  Snapdragon::RosNode::VislamDynRec* vislam_dyn_rec_;
};

/**
 * Class to hold dynamic reconfig's current values
 */
class Snapdragon::RosNode::VislamDynRec
{

public:
  VislamDynRec();

  // public data members to make it accessible for all
  uint32_t current_fps_;

  bool manual_fps_;
};
