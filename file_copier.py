import sys
import os
import pdb
import shutil
import numpy as np

# output folder
# left images folder
# right images folder
# images to choose text file path

def main(imgframepath, imgpath, outdir):
    if not os.path.exists(outdir):
        os.makedirs(outdir)
        print("Created new outdir at %s" % outdir)

    imgframenp = np.loadtxt(imgframepath, dtype=np.int32)

    for idx, frame in enumerate(imgframenp):
        img_idx = idx+1
        leftinfile = os.path.join(imgpath, "left%04d.jpg" % frame)
        rightinfile= os.path.join(imgpath, "right%04d.jpg" % frame)

        #Save left
        leftoutpath = os.path.join(outdir, "1")
        if not os.path.exists(leftoutpath):
            os.makedirs(leftoutpath)

        leftoutfile = os.path.join(leftoutpath, "left%0d.jpg" % img_idx)
        if not os.path.exists(leftoutpath):
            os.makedirs(leftoutpath)
        shutil.copy(leftinfile, leftoutfile)
        
        #Save right
        rightoutpath = os.path.join(outdir, "2")
        if not os.path.exists(rightoutpath):
            os.makedirs(rightoutpath)
        rightoutfile = os.path.join(rightoutpath, "right%0d.jpg" % img_idx)
        if not os.path.exists(rightoutpath):
            os.makedirs(rightoutpath)
        shutil.copy(rightinfile, rightoutfile)
        print("Copied frame %04d" % frame)

if __name__ == "__main__":
    arguments = sys.argv
    if len(arguments) != 4:
        print("Syntax: python file_copier.py [frame_text_file] [images_folder] [output_folder]")
    else:
        imgframepath = arguments[1]
        imgpath = arguments[2]
        outdir = arguments[3]
        main(imgframepath, imgpath, outdir)




