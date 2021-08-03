#include <iostream>
#include <unistd.h>
#include <stdio.h>

#include <opencv2/core.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <sstream>
#include <memory>

#include "boost_util.h"
#include "arg_parse.h"

using namespace std;
using namespace cv;

//30 colors
const int SizePaintdotnetPallet = 30;
const int PaintdotnetPallet[][3] = {
	{64, 64, 64},
	{128, 128, 128},
	{255, 0, 0},
	{127, 0, 0},
	{255, 106, 0},
	{127, 51, 0},
	{255, 216, 0},
	{127, 106, 0},
	{182, 255, 0},
	{91, 127, 0},
	{76, 255, 0},
	{38, 127, 0},
	{0, 255, 33},
	{0, 127, 14},
	{0, 255, 144},
	{0, 127, 70},
	{0, 255, 255},
	{0, 127, 127},
	{0, 148, 255},
	{0, 74, 127},
	{0, 38, 255},
	{0, 19, 127},
	{72, 0, 255},
	{33, 0, 127},
	{178, 0, 255},
	{87, 0, 127},
	{255, 0, 220},
	{127, 0, 110},
	{255, 0, 110},
	{127, 0, 55},
};

int get_pallet_idx(uint8_t* src)
{
	for(int i=0;i<SizePaintdotnetPallet;++i){
		if(
			*(src+2) == PaintdotnetPallet[i][0] &&
			*(src+1) == PaintdotnetPallet[i][1] &&
			*(src+0) == PaintdotnetPallet[i][2]
			){
			return i;
		}
	}
	return -1;
}

vector<vector<int>> get_masked_label_average(Mat* m_color, Mat* m_mask)
{
	int w = m_color->cols;
	int h = m_color->rows;
	uint8_t* p_color = (uint8_t*)m_color->ptr();
	uint8_t* p_mask = (uint8_t*)m_mask->ptr();
	vector<vector<int>> buf;
	for(int i=0;i<SizePaintdotnetPallet;++i){
		buf.push_back(vector<int>());
	}
	for(int y=0;y<h;++y){
		for(int x=0;x<w;++x){
			int idx = y*w*3+x*3;
			int pallet = get_pallet_idx(p_mask+idx);
			if(pallet >= 0){
				vector<int> color;
				buf[pallet].push_back(*(p_color+idx+0));
				buf[pallet].push_back(*(p_color+idx+1));
				buf[pallet].push_back(*(p_color+idx+2));
			}
		}
	}
	vector<vector<int>> ave;
	for(int i=0;i<SizePaintdotnetPallet;++i){
		if(buf[i].size() == 0){
			ave.push_back(vector<int>());
			continue;
		}
		int r = 0;
		int g = 0;
		int b = 0;
		printf("%02d num_pixel = %d\n", i, buf[i].size()/3);
		for(int j=0;j<buf[i].size()/3;++j){
			b += buf[i][j*3+0];
			g += buf[i][j*3+1];
			r += buf[i][j*3+2];
		}
		vector<int> rgb;
		rgb.push_back((int)(r / (buf[i].size()/3)));
		rgb.push_back((int)(g / (buf[i].size()/3)));
		rgb.push_back((int)(b / (buf[i].size()/3)));
		ave.push_back(rgb);
	}
	return ave;
}

int main(int argc, const char** argv)
{
  arg_parse args;
  string arg_error = args.parse(argc, argv);
  if(arg_error.length() > 0){
    _le << "arg parse error:" << arg_error;
    return 1;
  }
  stringstream ss_args;
  args.print(ss_args);
  _li << ss_args.str();

  Mat m_color = imread(args.path_color.c_str());
  Mat m_mask = imread(args.path_mask.c_str());
  if(m_color.cols == 0){
    _le << "invalid path:" << args.path_color;
    return 1;
  }
  if(m_mask.cols == 0){
    _le << "invalid path:" << args.path_mask;
    return 1;
  }

	vector<vector<int>> label_rgb;
	label_rgb = get_masked_label_average(&m_color, &m_mask);
	for(int i=0;i<SizePaintdotnetPallet;++i){
		if(label_rgb[i].size() == 0){
			printf("%02d\n", i);
		}else{
			printf("%02d % 3d, % 3d, % 3d\n", i, label_rgb[i][0], label_rgb[i][1], label_rgb[i][2]);
		}
	}

	if(args.path_output.size() > 0){
		//imwrite(args.path_output, *(m_send.get()));
	}
	
  return 0;
}
