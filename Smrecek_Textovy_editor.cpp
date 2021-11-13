#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <regex>

/// <summary>
/// https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/
/// </summary>
/// <param name="s"></param>
/// <returns></returns>
std::string ltrim(const std::string& s) 
{
	return std::regex_replace(s, std::regex("^\\s+"), std::string(""));
}

/// <summary>
/// https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/
/// </summary>
/// <param name="s"></param>
/// <returns></returns>
std::string rtrim(const std::string& s) 
{
	return std::regex_replace(s, std::regex("\\s+$"), std::string(""));
}

/// <summary>
/// https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/
/// </summary>
/// <param name="s"></param>
/// <returns></returns>
std::string trim(const std::string& s) 
{
	return ltrim(rtrim(s));
}

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


	if (ifs.fail() && !ifs.eof())
	{
		fails("Fail", "read_file");
	}

	if (ifs.eof())
	{
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

bool dummy_range_check(const int& first_line, const int& last_line)
{
	bool simple_check = ((first_line != -2) && (last_line != -2) && (first_line != 0) && (last_line != 0));
	if (last_line == -1)
	{
		return simple_check;
	}
	else
	{
		return simple_check && (first_line <= last_line);
	}
}

int dummy_to_int_convertor(const std::string& str_num)
{
	int number = -2;
	try
	{
		number = std::stoi(str_num);
		return number;
	}
	catch (const std::exception&)
	{
		return -2;
	}
}

bool line_number_extractor(const std::string& token, int& first_line, int& last_line)
{
	std::regex case5(R"([0-9]+)");

	if (regex_match(token, case5))
	{
		//std::cout << token << std::endl;
		first_line = dummy_to_int_convertor(token);
		last_line = dummy_to_int_convertor(token);
		return first_line >= 0 && first_line == last_line;
	}

	first_line = -2;
	last_line = -2;
	return false;
}

bool range_extractor(const std::string& token, int& first_line, int& last_line)
{
	// What about negative numbers?

	std::regex case1(R"(,)");
	std::regex case2(R"(,[0-9]+)");
	std::regex case3(R"([0-9]+,)");
	std::regex case4(R"([0-9]+,[0-9]+)");
	std::regex case5(R"([0-9]+)");

	//std::cout << "case1 " << regex_match(token, case1) << std::endl;
	//std::cout << "case2 " << regex_match(token, case2) << std::endl;
	//std::cout << "case3 " << regex_match(token, case3) << std::endl;
	//std::cout << "case4 " << regex_match(token, case4) << std::endl;
	//std::cout << "case5 " << regex_match(token, case5) << std::endl;

	size_t delimeter_pos = token.find(',');

	if (regex_match(token, case1))
	{
		first_line = -1;
		last_line = -1;
		return dummy_range_check(first_line, last_line);
	}

	if (regex_match(token, case2))
	{
		std::string last = token.substr(delimeter_pos + 1, token.size());
		//std::cout << last << std::endl;

		first_line = -1;
		last_line = dummy_to_int_convertor(last);
		return dummy_range_check(first_line, last_line);
	}

	if (regex_match(token, case3))
	{
		std::string first = token.substr(0, delimeter_pos);
		//std::cout << first << std::endl;

		first_line = dummy_to_int_convertor(first);
		last_line = -1;
		return dummy_range_check(first_line, last_line);
	}

	if (regex_match(token, case4))
	{
		std::string first = token.substr(0, delimeter_pos);
		//std::cout << first << std::endl;
		std::string last = token.substr(delimeter_pos + 1, token.size());
		//std::cout << last << std::endl;

		first_line = dummy_to_int_convertor(first);
		last_line = dummy_to_int_convertor(last);
		return dummy_range_check(first_line, last_line);
	}

	if (regex_match(token, case5))
	{
		//std::cout << token << std::endl;
		first_line = dummy_to_int_convertor(token);
		last_line = dummy_to_int_convertor(token);
		return dummy_range_check(first_line, last_line);
	}

	first_line = -2;
	last_line = -2;
	return dummy_range_check(first_line, last_line);
}

void print_command(std::vector<std::string>& all_lines, int start, int end)
{
	for (size_t i = start - 1; i < size_t(end); i++)
	{
		if (i < all_lines.size())
		{
			std::cout << all_lines[i] << std::endl;
		}
	}
}

void append_command_oneliner(std::vector<std::string>& all_lines, bool& changed, std::string& line, int line_num = -1)
{
	if (line_num == -1)
	{
		all_lines.push_back(line);
		changed = true;
	}
	else
	{
		size_t line_num_help = (size_t)line_num;
		if (line_num_help > all_lines.size())
		{
			for (size_t i = all_lines.size(); i < line_num_help; i++)
			{
				all_lines.push_back("");
			}
		}

		auto iterator = all_lines.begin() + line_num;

		iterator = all_lines.insert(iterator, line) + 1;
	}
}

void append_command(std::vector<std::string>& all_lines, bool& changed, int line_num = -1)
{
	if (line_num == -1)
	{
		std::string line_in;
		std::getline(std::cin, line_in);

		while (line_in != ".")
		{
			all_lines.push_back(line_in);
			changed = true;
			std::getline(std::cin, line_in);
		}
	}
	else
	{
		std::string line_in;
		std::getline(std::cin, line_in);

		size_t line_num_help = (size_t)line_num;
		if (line_num_help > all_lines.size())
		{
			for (size_t i = all_lines.size(); i < line_num_help; i++)
			{
				all_lines.push_back("");
			}
		}

		auto iterator = all_lines.begin() + line_num;

		while (line_in != ".")
		{
			iterator = all_lines.insert(iterator, line_in) + 1;
			changed = true;
			std::getline(std::cin, line_in);
		}
	}
}

void write_command(std::vector<std::string>& all_lines, bool& changed, const std::string& path)
{
	std::ofstream ofs = open_out_stram(path);
	if (!ofs.is_open())
	{
		fails("Not opened output", "main");
	}

	for (size_t i = 0; i < all_lines.size(); i++)
	{
		ofs << all_lines[i] << std::endl;
	}

	changed = false;
	ofs.close();
}

void delete_command(std::vector<std::string>& all_lines, bool& changed, int start, int end)
{
	size_t start2 = (size_t)start - 1;
	size_t end2 = (size_t)end;

	if (end2 > all_lines.size())
	{
		end2 = all_lines.size();
	}

	if (start2 < all_lines.size())
	{
		all_lines.erase(all_lines.begin() + start2, all_lines.begin() + end2);
		changed = true;
	}
}

void change_command_oneliner(std::vector<std::string>& all_lines, bool& changed, std::string& line, int start, int end)
{
	delete_command(all_lines, changed, start, end);
	append_command_oneliner(all_lines, changed, line, start - 1);
}

void change_command(std::vector<std::string>& all_lines, bool& changed, int start, int end)
{
	delete_command(all_lines, changed, start, end);
	append_command(all_lines, changed, start - 1);
}

void read_command(std::vector<std::string>& all_lines, const std::string& path)
{
	bool changed = false;
	while (true)
	{
		std::cout << "* ";

		std::string raw_command;
		std::getline(std::cin, raw_command);
		logger(raw_command);

		std::string l_trimmed_whole = ltrim(raw_command);
		std::string lr_trimmed_whole = trim(raw_command);

		if (lr_trimmed_whole.size() == 0)
		{
			std::cout << "Unsupported command" << std::endl;
			continue;
		}
		else if (lr_trimmed_whole.size() == 1)
		{
			logger("1 letter command");
			if (lr_trimmed_whole == "w")
			{
				write_command(all_lines, changed, path);
			}
			if (lr_trimmed_whole == "q")
			{
				if (changed)
				{
					std::cout << "You have unsaved changes" << std::endl;
					continue;
				}
				else
				{
					return;
				}
			}
			if (lr_trimmed_whole == "a")
			{
				append_command(all_lines, changed);
				continue;
			}
			if (lr_trimmed_whole == "p")
			{
				print_command(all_lines, 1, int(all_lines.size()));
				continue;
			}
			if (lr_trimmed_whole == "d")
			{
				delete_command(all_lines, changed, 1, int(all_lines.size()));
				continue;
			}
			if (lr_trimmed_whole == "c")
			{
				change_command(all_lines, changed, 1, int(all_lines.size()));
				continue;
			}
			std::cout << "Unsupported command" << std::endl;
		}
		else if (lr_trimmed_whole == "q!")
		{
			return;
		}
		else
		{
			logger("multiple letters command");

			std::string command_letter = l_trimmed_whole.substr(0, 2);
			command_letter = trim(command_letter);

			if (!(command_letter == "a" || command_letter == "p" || command_letter == "d" || command_letter == "c"))
			{
				std::cout << "Unsupported command" << std::endl;
				continue;
			}

			l_trimmed_whole.erase(0, 2);
			std::string trimmed_whole_without_command = ltrim(l_trimmed_whole);

			size_t first_space = trimmed_whole_without_command.find(' ');
			std::string other_tokens = "";
			if (first_space == std::string::npos)
			{
				first_space = trimmed_whole_without_command.size();
			}
			else
			{
				other_tokens = trimmed_whole_without_command.substr(first_space + 1, trimmed_whole_without_command.size());
			}

			std::string trimmed_other_tokens = trim(other_tokens);
			std::string first_token = trimmed_whole_without_command.substr(0, first_space);

			if (trimmed_other_tokens == "")
			{
				// Only command and range
				
				// -1 = whole file
				// -2 unsupported range
				int first_line = -1;
				int last_line = -1;

				if (command_letter == "a")
				{
					if (line_number_extractor(first_token, first_line, last_line))
					{
						append_command(all_lines, changed, first_line);
						continue;
					}
					else
					{
						// One-liner error of range
						append_command_oneliner(all_lines, changed, trimmed_whole_without_command);
						continue;
					}
				}
				else if (command_letter == "c")
				{
					if (range_extractor(first_token, first_line, last_line) && first_line != 0)
					{
						if (first_line == -1)
						{
							first_line = 1;
						}

						if (last_line == -1)
						{
							last_line = int(all_lines.size());
						}

						change_command(all_lines, changed, first_line, last_line);
						continue;
					}
					else
					{
						// One-liner error of range
						change_command_oneliner(all_lines, changed, trimmed_whole_without_command, 1, int(all_lines.size()));
						continue;
					}
				}
				else
				{
					if (range_extractor(first_token, first_line, last_line))
					{
						if (first_line == -1)
						{
							first_line = 1;
						}

						if (last_line == -1)
						{
							last_line = int(all_lines.size());
						}
					}
					else
					{
						std::cout << "Invalid range" << std::endl;
						continue;
					}

					if (command_letter == "p")
					{
						print_command(all_lines, first_line, last_line);
						continue;
					}

					if (command_letter == "d")
					{
						if (first_line == 0)
						{
							std::cout << "Invalid range" << std::endl;
							continue;
						}
						delete_command(all_lines, changed, first_line, last_line);
						continue;
					}
				}
				std::cout << "Unsupported command" << std::endl;
			}
		}
	}
}

int main()
{
	//std::string path;
	std::string path{ "Example_2.txt" };

	//std::cin >> path;
	logger(path);

	std::vector<std::string> all_lines;

	if (!read_file(path, all_lines))
	{
		std::ofstream ofs = open_out_stram(path);
		if (!ofs.is_open())
		{
			fails("Not opened output", "main");
		}
		ofs.close();
	} 

	read_command(all_lines, path);

	//for (size_t i = 0; i < all_lines.size(); i++)
	//{
	//	std::cout << all_lines[i] << std::endl;
	//}



	return 0;
}
