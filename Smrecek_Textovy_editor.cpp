#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

void logger(const std::string& message) 
{
	std::cout << "---------- " << message << std::endl;
}

void fails(const char* message, const char* context = "Unknown context")
{
	std::cerr << context << ": " << message << std::endl;
}

bool read_file(const std::string& path, std::vector<std::string>& all_lines)
{
	std::ifstream ifs{ path };
	if (!ifs.is_open())
	{
		fails("Not opened input", "read_file");
		return false;
	}

	std::string str_line;
	while (std::getline(ifs, str_line))
	{
		all_lines.push_back(str_line);
	}

	if (ifs.eof())
	{
		//ifs.close();
		//
		//if (ifs.fail())
		//{
		//	fails("Not closed", "read_file");
		//	return false;
		//}

		return true;
	}
	else
	{
		fails("Not eof", "read_file");
		return false;
	}
}

std::ofstream open_out_stram(const std::string& path)
{
	std::ofstream ofs{ path };

	return ofs;
}

void read_command()
{
	std::cout << "* ";
	std::string command;
	std::cin >> command;

	logger(command);
}

int main()
{
	//std::string path;
	std::string path{ "Example_1.txt" };

	//std::cin >> path;
	logger(path);

	std::vector<std::string> all_lines;

	std::ofstream ofs;

	if (!read_file(path, all_lines))
	{
		ofs = open_out_stram(path);
		if (!ofs.is_open())
		{
			fails("Not opened output", "main");
		}
	} 

	read_command();

	//for (size_t i = 0; i < all_lines.size(); i++)
	//{
	//	std::cout << all_lines[i] << std::endl;
	//}



	return 0;
}
