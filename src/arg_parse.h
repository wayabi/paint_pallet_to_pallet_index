#ifndef __U_ARG_PARSE__
#define __U_ARG_PARSE__

#include <string>
#include <ostream>

#include <boost/program_options.hpp>

class arg_parse {

public:
	std::string path_img_label;
	std::string path_pallet_csv;
	std::string path_img_output;
public:
	std::string parse(int argc, const char** argv)
	{
		using namespace boost::program_options;
		options_description opt("opt");
		opt.add_options()
			("path_img_label", value<std::string>()->required(), "path_img_label")
			("path_pallet_csv", value<std::string>()->required(), "path_pallet_csv")
			("path_img_output", value<std::string>()->required(), "path_img_output")
		;
		variables_map vm;
		try{
			store(parse_command_line(argc, argv, opt), vm);
			notify(vm);
			path_img_label = vm["path_img_label"].as<std::string>();
			path_pallet_csv = vm["path_pallet_csv"].as<std::string>();
			path_img_output = vm["path_img_output"].as<std::string>();
		}catch(std::exception& e){
			return std::string(e.what());
		}
		return std::string();
	}

	void print(std::ostream& o)
	{
		o << "path_img_label[std::string]: " << path_img_label << std::endl;
		o << "path_pallet_csv[std::string]: " << path_pallet_csv << std::endl;
		o << "path_img_output[std::string]: " << path_img_output << std::endl;
	}
};
#endif
