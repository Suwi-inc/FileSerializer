
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <json/json.h>

using namespace std;
class antenStruct {
public:
	std::string name;
	int countElements;
	int referencePoint;
	float* f32_coordinatArray;

	antenStruct() {
		name = "";
		countElements = 0;
		referencePoint = 0;
		f32_coordinatArray = nullptr;
	}
};
class DeserializeArrayPointConfig
{
public:
	std::vector<antenStruct>  begin(std::string inputFile )
	{
		std::vector<antenStruct> antennas;


		std::ifstream input_file(inputFile);
		if (!input_file.is_open()) {
			std::cerr << "Failed to open input file" << std::endl;
			throw exception("error with file");
		}

		std::string jsonString;

		std::stringstream buffer;
		buffer << input_file.rdbuf();

		jsonString = buffer.str();

		input_file.close();


		antennas = deserializeAntennaArray(jsonString);

		return antennas;


	};
	
	antenStruct deserializeAntenna(const Json::Value& jsonAntenna)
	{
		antenStruct temp;
		temp.countElements = jsonAntenna["countElements"].asInt();
		temp.referencePoint = jsonAntenna["referenceAntenna"].asInt();
		temp.name = jsonAntenna["str_name"].asString();
		const Json::Value& jsonCoordinates = jsonAntenna["f32_coordinatArray"];
		temp.f32_coordinatArray = new float[3 * temp.countElements];

		for (int i = 0; i < jsonCoordinates.size(); i++) {
			temp.f32_coordinatArray[3 * i] = jsonCoordinates[i]["x"].asFloat();
			temp.f32_coordinatArray[3 * i + 1] = jsonCoordinates[i]["y"].asFloat();
			temp.f32_coordinatArray[3 * i + 2] = jsonCoordinates[i]["z"].asFloat();
		}
		return temp;
	}

	std::vector<antenStruct> deserializeAntennaArray(const std::string& jsonString)
	{

		std::vector<antenStruct> antennaArray{};

		Json::Value root;
		Json::Reader reader;
		bool parsingSuccessful = reader.parse(jsonString, root);
		if (!parsingSuccessful) {
			std::cerr << "Failed to parse JSON string." << std::endl;
			return antennaArray;
		}

		int size = root["count"].asInt();

		antennaArray.reserve(size);

		const Json::Value& jsonAntennas = root["Antenna Array"];

		for (int i = 0; i < jsonAntennas.size(); i++) {
			antenStruct antenna = deserializeAntenna(jsonAntennas[i]);
			antennaArray.push_back(antenna);
		}

		return antennaArray;
	}
	

};
class SerializeArrayPointConfig
{
public:
	void begin(std::vector<antenStruct> antennaArray, std::string outputFile)
	{
		std::string json = serializeAntennaArray(antennaArray);

		std::ofstream file(outputFile);

		if (file.is_open()) // check if the file was opened successfully
		{
			file << json; // write the string to the file
			file.close(); // close the file
			std::cout << "String written to file." << std::endl;
		}
		else
		{
			std::cerr << "Error: Could not open file." << std::endl;
		}
		std::cout << json << std::endl;
	}
	std::string serializeAntenna(const antenStruct& antenna) {
		std::stringstream ss;


		ss << "{\n";
		ss << "\"str_name\": \"" << antenna.name << "\",\n";
		ss << "\"countElements\": " << antenna.countElements << ",\n";
		ss << "\"referenceAntenna\": " << antenna.referencePoint << ",\n";

		ss << "\"f32_coordinatArray\": \n[\n";
		for (int i = 0; i < antenna.countElements; i++) {
			ss << "{\n";
			ss << "\"x\": " << antenna.f32_coordinatArray[3 * i] << ",\n";
			ss << "\"y\": " << antenna.f32_coordinatArray[3 * i + 1] << ",\n";
			ss << "\"z\": " << antenna.f32_coordinatArray[3 * i + 2];
			ss << "\n}\n";
			if (i != antenna.countElements - 1) {
				ss << ",\n";
			}
		}
		ss << "]\n";
		ss << "}\n";

		return ss.str();
	}

	std::string serializeAntennaArray(const std::vector<antenStruct>& antennaArray) {
		std::stringstream ss;

		ss << "{\n";
		ss << "\"count\": " << antennaArray.size() << ",\n";
		ss << "\"Antenna Array\": \n[\n";
		for (int i = 0; i < antennaArray.size(); i++) {

			ss << serializeAntenna(antennaArray[i]);
			if (i != antennaArray.size() - 1) {
				ss << ",\n";
			}
		}
		ss << "]\n";
		ss << "}\n";

		return ss.str();
	}


};


//these methods deserialize the json file
using namespace std;

int main()
{

	std::vector<antenStruct> antennaArray;

	antenStruct* a1 = new antenStruct();
	a1->countElements = 2;
	a1->name = "name1";
	a1->referencePoint = 1;
	a1->f32_coordinatArray = new float[3 * a1->countElements] {1, 1.1, 1.2, 2, 2.1, 2.2};
	antennaArray.push_back(*a1);

	antenStruct* a2 = new antenStruct();
	a2->countElements = 2;
	a2->name = "name2";
	a2->referencePoint = 2;
	a2->f32_coordinatArray = new float[3 * a2->countElements] {2, 2.1, 2.2, 3, 3.1, 3.2};
	antennaArray.push_back(*a2);

	antenStruct* a3 = new antenStruct();
	a3->countElements = 3;
	a3->name = "name3";
	a3->referencePoint = 2;
	a3->f32_coordinatArray = new float[3 * a3->countElements] {2, 2.1, 2.2, 3, 3.1, 3.2, 0.9, 2.7, 3.4};
	antennaArray.push_back(*a3);

	antenStruct* a4 = new antenStruct();
	a4->countElements = 4;
	a4->name = "name4";
	a4->referencePoint = 2;
	a4->f32_coordinatArray = new float[3 * a4->countElements] {2, 2.1, 2.2, 3, 3.1, 3.2, 0.9, 2.7, 3.4,0.8,0.35,1.22};
	antennaArray.push_back(*a4);




	cout << antennaArray[0].referencePoint << endl;
	cout << antennaArray[1].referencePoint << endl;
	cout << antennaArray[2].referencePoint << endl;

	///serialize
	cout << "started serialzing " << endl;
	SerializeArrayPointConfig fileOut;
	fileOut.begin(antennaArray,"jsonoutafterclass.json");

	cout << "started deserilzing " << endl;

	//deserialize
	DeserializeArrayPointConfig fileIn;

	std::vector<antenStruct> antennaArrayIn{};

	antennaArrayIn = fileIn.begin("jsonoutafterclass.json");

	std::cout << "Antenna Array Count: " << antennaArrayIn.size() << std::endl;

	for (int i = 0; i < antennaArrayIn.size(); i++) {
		antenStruct antenna = antennaArrayIn[i];
		std::cout << "Antenna Name: " << antenna.name << ", Count Elements: " << antenna.countElements << ",Reference Point : " << antenna.referencePoint << std::endl;
		for (int j = 0; j < antenna.countElements; j++) {
			std::cout << "  Coord " << j << ": (" << antenna.f32_coordinatArray[3 * j] << ", " << antenna.f32_coordinatArray[3 * j + 1] << ", " << antenna.f32_coordinatArray[3 * j + 2] << ")" << std::endl;
		}
	}




}


