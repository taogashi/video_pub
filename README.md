## Nodes
### video\_pub\_node
The video\_pub\_node reads video file with OpenCV api and publishes ROS sensor\_msgs::image via cv_bridge.
- published topics
    /usb_cam/image_raw (compatiable with usb_cam)
- parameters
    *~videosrc* video file, full path. 
    *~framerate* frame rate of published topic.
    *~localdisplay* bool value, choose whether to display the image locally.
    *~skip* number of frames to skip
      
- Example
```
  rosrun video_pub video_pub_node _videosrc:=<full path to video file> _framerate:=30
```
```
  rosrun video_pub video_pub_node _videosrc:=<full path to the video file> _framerate:=60 _skip:=1000
```
- Hotkey

  Hit space to pause/replay.

- Tips
  - For many low-cost usb-camera, only mjpeg format can be output with hight rate. How to pack mjpeg stream from camera to video file?
  
    ffmpeg -f v4l2 -framerate 60 -input_format mjpeg -i /dev/video0 -codec copy out.avi
    
    This is lossless.


