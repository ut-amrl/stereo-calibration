#!/bin/bash
rosservice call /stereo/left/set_camera_info "camera_info:
  header:
    seq: 0
    stamp:{secs: 0, nsecs: 0}
    frame_id: ''
  height: 1024
  width: 1224
  distortion_model: 'plumb_bob'
  D: [-0.04002572576778747, 0.2657238690260969, 0.00177228292204798, -0.00240932210140832, -0.1977982585991323]
  K: [831.8246508970198, 0, 619.3994418986066,
 0, 833.5998571169539, 513.1909876118189,
 0, 0, 1]
  R: [0.7448046019107144, -0.230312510919209, -0.6262764982711001,
 0.2319941972581232, 0.9693733827800504, -0.08058496879766698,
 0.6256554941908374, -0.08527245787269859, 0.7754249225535427]
  P: [802.381462833984, 0, 2046.755438089371, -288.8924728358938,
 0, 802.381462833984, 704.8193511962891, 0,
 0, 0, 1, 0]
  binning_x: 0
  binning_y: 0
  roi: {x_offset: 0, y_offset: 0, height: 0, width: 0, do_rectify: true}"