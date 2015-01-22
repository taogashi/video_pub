- Nodes
  - video\_pub\_node

    The video\_pub\_node reads video file with OpenCV api and publishes ROS sensor\_msgs::image via cv_bridge.
  - published topics

    /usb_cam/image_raw (compatiable with usb_cam)
  - parameters

    ~videosrc
    
      video file, full path.
      
    ~framerate
    
      frame rate of published topic.
      
    ~localdisplay
    
      bool value, choose whether to display the image locally.
      
- Example

  rosrun video\_pub video\_pub\_node \_videosrc:=<_full path to video file_> \_framerate:=30

- Hotkey

  Hit space to pause/replay.

- Tips
  - For many low-cost usb-camera, only mjpeg format can be output with hight rate. How to pack mjpeg stream from camera to video file?
  
    ffmpeg -f v4l2 -framerate 60 -input_format mjpeg -i /dev/video0 -codec copy out.avi
    
    This is lossless.


