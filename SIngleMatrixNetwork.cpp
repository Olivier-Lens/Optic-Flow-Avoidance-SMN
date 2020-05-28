// SIngleMatrixNetwork.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Network.h"
#include <math.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
using namespace std;
using namespace cv;

//void doLearning(Network fred, string input_file_name, string prefix);

/*
 * Convert values to strings. Needed because std::to_string is apparently missing
 * from <string>.
 */
template <typename T>
string to_string(T value)
{
	//create an output string stream
	ostringstream os ;

	//throw the value into the string stream
	os << value ;

	//convert the string stream into a string and return
	return os.str() ;
}

/*
 * Usage: SingleMatrixNetwork <config_file> <input_file> <weight_directory>
 */
int main(int argc, char* argv[])
{
    double inp[2];
    inp[0] = 0.5;
    inp[1] = 0.5;
    int t;
	string network_file_name;
    if (argc < 2) {
		network_file_name = "ganglia5.txt";
	} else {
		network_file_name = argv[1];
	}

	string input_file_name;
	if (argc > 2) {
		input_file_name = argv[2];
		cout << "Reading inputs from " << input_file_name << endl;
	}

	string filename = "OpticFlowVideo.mp4";

	VideoCapture capture(filename);
    if (!capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        return 0;
    }

    Mat img(9,9,CV_32FC2,Scalar(1,3));

    vector<Mat> frames;

    vector<double> array;
    if (img.isContinuous()) {
          // array.assign((float*)mat.datastart, (float*)mat.dataend); // <- has problems for sub-matrix like mat = big_mat.row(i)
          array.assign((double*)img.data, (double*)img.data + img.total());
    } else {
          for (int i = 0; i < img.rows; ++i) {
            array.insert(array.end(), img.ptr<double>(i), img.ptr<double>(i)+img.cols);
            }
    }

	Network fred(network_file_name);


	//cycleNetwork

	//fred.PrintNetworkState(); //checks the state of the network
	//printf("\n");

	fred.resetNeuronOutputs();

    for(t = 0; t < 100; t++){
        fred.printNetworkOuput();
        fred.setNetworkInput(array);
        fred.cycleNetwork();
    }
	//doLearning(fred, "bilateral-inputs.txt", network_file_name);

	return 0;
}

/*
 * Process each line in the input_file_name through the network
 */
/*void doLearning(Network fred, string input_file_name, string prefix) {
	const string output_file_name = prefix + "-output_squash.txt";

	cout << "Writing output state to " << output_file_name << endl;

	int t = 0;

	printf("*** Begin network learning ***\n");

	string input_line;
	ifstream input_file(input_file_name);
	if (!input_file.is_open()) {
		cerr << "Error opening input file " << input_file_name << endl;
	} else {
		while (getline(input_file, input_line)) {

			istringstream iss(input_line);
			vector<string> input_values_str;
			copy(istream_iterator<string>(iss),
				 istream_iterator<string>(),
				 back_inserter(input_values_str));

			// Transform the string values to doubles
			vector<double> input_values(input_values_str.size());
			transform(input_values_str.begin(), input_values_str.end(), input_values.begin(), [](const std::string& val)
			{
				return std::stod(val);
			});

			fred.setNetworkInput( input_values );

			printf("t=%03d: ", t);
			fprintf(fred.getLogFile(), "-- t=%03d --\n", t);

			fred.cycleNetwork();

			fred.cycleNetworkNormalizeHebbianLearning();

			fred.printNetworkOutputState( );

			fred.writeNetworkOutputStateToFile(output_file_name);

			fred.writeNetworkToFile(prefix + "-out.txt");

			t++;
		}
	}
	printf("*** End network learning ***\n");
}
*/
