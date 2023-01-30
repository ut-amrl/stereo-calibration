# Input Requirements:
#   <imgs_dir>/left<%d>.jpg
#   <imgs_dir>/right<%d>.jpg
#   where %d is an integer representing the image number
# 
# Output:
#   cam_left.yaml, cam_right.yml, opencv_cam_stereo.yml 
#   in the current directory
#

while getopts t:f:n:d: flag
do
    case "${flag}" in
        t) frame_text_file=${OPTARG};;
        f) raw_imgs_dir=${OPTARG};;
        n) num_imgs=${OPTARG};;
        d) imgs_dir=${OPTARG};;
    esac
done

python3 file_copier.py $frame_text_file $raw_imgs_dir $imgs_dir

# Calibrate left cam
./build/calibrate -w 10 -h 7 -n $num_imgs -s 0.1 -d $imgs_dir/1/ -i "left" -o "cam_left.yml" -e "jpg"

# # Calibrate right cam
./build/calibrate -w 10 -h 7 -n $num_imgs -s 0.1 -d $imgs_dir/2/ -i "right" -o "cam_right.yml" -e "jpg"

# Calibrate stereo
./build/calibrate_stereo -n $num_imgs -u cam_left.yml -v cam_right.yml -L $imgs_dir/1/ -R $imgs_dir/2/ -l left -r right -o opencv_cam_stereo.yml -e jpg
