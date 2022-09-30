#include <iostream>
#include <unistd.h>
#include <stdio.h>

#include <iostream>
#include <sstream>
#include <memory>
#include <map>

#include <opencv2/core.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "boost_util.h"
#include "arg_parse.h"
#include "Util.h"

using namespace std;
using namespace cv;

map<int,int> load_pallet(const string& path_pallet_csv)
{
	map<int, int> ret;
	vector<vector<string>> ss = Util::load_csv(path_pallet_csv.c_str());
	if(ss.size() == 0){
		return ret;
	}
	int count = 0;
	for(auto ite = ss.begin();ite != ss.end();++ite){
		int r = atoi((*ite)[0].c_str());
		int g = atoi((*ite)[1].c_str());
		int b = atoi((*ite)[2].c_str());
		int idx = b*256*256 + g*256 + r;
		ret[idx] = count++;
	}
	return ret;
}

int get_pallet_idx(uint8_t* src, map<int,int>& pallet_map)
{
	//BGR
	int idx = *(src+0)*256*256 + *(src+1)*256 + *(src+2);
	if(pallet_map.count(idx) > 0){
		return pallet_map[idx];
	}else{
		return 255;
	}
}

void convert2pallet_idx(const Mat& m_src, Mat& m_dst, map<int,int> pallet)
{
	int w = m_src.cols;
	int h = m_src.rows;
	if(m_src.channels() != 3)
	{
		_le << "label_img channels must be 3.";
		return;
	}
	uint8_t* p_label = (uint8_t*)m_src.ptr();
	uint8_t* p_out= (uint8_t*)m_dst.ptr();
	
	for(int y=0;y<h;++y){
		for(int x=0;x<w;++x){
			int pallet_idx = get_pallet_idx(p_label, pallet);
			*(p_out+0) = pallet_idx;
			*(p_out+1) = pallet_idx;
			*(p_out+2) = pallet_idx;
			p_label += 3;
			p_out += 3;
		}
	}
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

  Mat m_label = imread(args.path_img_label.c_str());
  if(m_label.cols == 0){
    _le << "invalid path:" << args.path_img_label;
    return 1;
  }

	map<int,int> pallet_map = load_pallet(args.path_pallet_csv);

	Mat m_out(m_label.rows, m_label.cols, CV_8UC3);
	m_out.setTo(Scalar(255, 255, 255));
	convert2pallet_idx(m_label, m_out, pallet_map);

	imwrite(args.path_img_output, m_out);
	_li << "done.";

  return 0;
}
