#include <opencv2\opencv.hpp>
#include <fstream>

int main() {
	const std::string img_dir = "image";
	const std::string anno_dir = "annotation";
	const std::string res_dir = "result";

	std::ifstream fin(img_dir + "\\fileList.txt");
	if (!fin) {
		std::cout << "Fail to open " + img_dir + "\\fileList.txt\n";
		return -1;
	}
	std::ofstream fout1(res_dir + "\\pos.txt");
	std::ofstream fout2(res_dir + "\\neg.txt");
	
	std::string img_name = "";
	std::string anno_name = "";
	while (std::getline(fin, img_name) && img_name.substr(img_name.size() - 3, 3) == "jpg") {
		cv::Mat img = cv::imread(img_dir + "\\" + img_name);
		if (img.empty()) {
			std::cout << "Fail to read " + img_dir + "\\" + img_name + "\n";
			return -1;
		}
		anno_name = img_name.substr(0, img_name.size() - 3) + "txt";
		std::ifstream anno(anno_dir + "\\" + anno_name);
		if (!anno) {
			std::cout << "Fail to open " + anno_dir + "\\" + anno_name + "\n";
			return -1;
		}

		int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
		anno >> x1 >> y1 >> x2 >> y2;
		cv::Rect roi = cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2));

		cv::imwrite(res_dir + "\\pos_" + img_name, img);
		fout1 << "pos_" + img_name << " 1 " << x1 << " " << y1 << " " << x2 << " " << y2 << "\n";
		std::cout << "pos_" + img_name << " 1 " << x1 << " " << y1 << " " << x2 << " " << y2 << "\n";
		img(roi) = 0;
		cv::imwrite(res_dir + "\\neg_" + img_name, img);
		fout2 << "neg_" + img_name + "\n";
	}

	return 0;
}