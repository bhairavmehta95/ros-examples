# Implementation Plan

* [x] Camera Driver - strip everything out so it is **just the image publisher**
* [x] Change the framerate (~30 HZ) 
* [x] Take a video where it oscillates / flickers; usually happens when both the shadow and light are in the same frame (outside)
* [ ] Somehow, we need to timestamp the camera frame?
* [ ] Confirm that we can set to a fixed exposure
* [ ] Build dynamic_reconfigure options for framerate, fixed / autoexposure, maybe other things?
* [ ] Load onto Pelican Board

## Secondary Goals

* [ ] Remove the IMU data / where does it get inputted? -- Funnel it in through ROS
* [ ] Add stereo capabilities
* [ ] Can we change the image format, buffer size?
* [ ] Can we get it to work on the front camera
