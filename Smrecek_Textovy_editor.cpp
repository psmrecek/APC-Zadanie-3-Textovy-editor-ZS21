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

bool read_file(const std::string& path, std::vector<std::string>& all_lines)
{
	std::ifstream ifs{ path };
	if (!ifs.is_open())
	{
		return false;
	}

	std::string str_line;
	while (std::getline(ifs, str_line))
	{
		all_lines.push_back(str_line);
	}

	if (ifs.fail() && !ifs.eof())
	{
		return false;
	}

	if (ifs.eof())
	{
		return true;
	}
	else
	{
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
	size_t start_convert = (size_t)start - 1;
	for (size_t i = start_convert; i < size_t(end); i++)
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
		changed = true;
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

bool write_command(std::vector<std::string>& all_lines, bool& changed, const std::string& path)
{
	std::ofstream ofs = open_out_stram(path);
	if (!ofs.is_open())
	{
		return false;
	}

	for (size_t i = 0; i < all_lines.size(); i++)
	{
		ofs << all_lines[i] << std::endl;
	}

	changed = false;
	ofs.close();

	return true;
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
		auto it_start = all_lines.begin() + start2;
		auto it_end = all_lines.begin() + end2;
		all_lines.erase(it_start, it_end);
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

bool one_character_commands_handler(std::vector<std::string>& all_lines, bool& changed, std::string& lr_trimmed_whole)
{
	if (lr_trimmed_whole == "a")
	{
		append_command(all_lines, changed);
		return true;
	}
	if (lr_trimmed_whole == "p")
	{
		print_command(all_lines, 1, int(all_lines.size()));
		return true;
	}
	if (lr_trimmed_whole == "d")
	{
		delete_command(all_lines, changed, 1, int(all_lines.size()));
		return true;
	}
	if (lr_trimmed_whole == "c")
	{
		change_command(all_lines, changed, 1, int(all_lines.size()));
		return true;
	}

	return false;
}

void append_handler(std::vector<std::string>& all_lines, bool& changed, bool command_and_range_only, std::string& first_token, std::string& other_tokens, std::string& trimmed_whole_without_command)
{
	// -1 = whole file
	// -2 unsupported range
	int first_line = -1;
	int last_line = -1;

	if (line_number_extractor(first_token, first_line, last_line))
	{
		if (command_and_range_only)
		{
			append_command(all_lines, changed, first_line);
		}
		else
		{
			// One-liner true
			append_command_oneliner(all_lines, changed, other_tokens, first_line);
		}
	}
	else
	{
		// One-liner error of range
		append_command_oneliner(all_lines, changed, trimmed_whole_without_command);
	}
}

void change_handler(std::vector<std::string>& all_lines, bool& changed, bool command_and_range_only, std::string& first_token, std::string& other_tokens, std::string& trimmed_whole_without_command)
{

	// -1 = whole file
	// -2 unsupported range
	int first_line = -1;
	int last_line = -1;

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

		if (command_and_range_only)
		{
			change_command(all_lines, changed, first_line, last_line);
		}
		else
		{
			// One-liner true
			change_command_oneliner(all_lines, changed, other_tokens, first_line, last_line);
		}
	}
	else
	{
		// One-liner error of range
		change_command_oneliner(all_lines, changed, trimmed_whole_without_command, 1, int(all_lines.size()));
	}
}

void print_delete_handler(std::vector<std::string>& all_lines, bool& changed, std::string& first_token, std::string& command_letter)
{
	// -1 = whole file
	// -2 unsupported range
	int first_line = -1;
	int last_line = -1;

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
		return;
	}

	if (command_letter == "p")
	{
		print_command(all_lines, first_line, last_line);
	}

	if (command_letter == "d")
	{
		delete_command(all_lines, changed, first_line, last_line);
	}
}

void read_command(std::vector<std::string>& all_lines, const std::string& path)
{
	bool changed = false;
	while (true)
	{
		std::cout << "* ";

		std::string raw_command;
		std::getline(std::cin, raw_command);

		std::string l_trimmed_whole = ltrim(raw_command);
		std::string lr_trimmed_whole = trim(raw_command);

		if (lr_trimmed_whole.size() == 0)
		{
			std::cout << "Unsupported command" << std::endl;
			continue;
		}
		else if (lr_trimmed_whole.size() == 1)
		{
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

			if (lr_trimmed_whole == "w")
			{
				if(!write_command(all_lines, changed, path))
				{
					std::cout << "Unable to open file" << std::endl;
				}
				continue;
			}

			if (!one_character_commands_handler(all_lines, changed, lr_trimmed_whole))
			{
				std::cout << "Unsupported command" << std::endl;
				continue;
			}
			
		}
		else if (lr_trimmed_whole == "q!")
		{
			return;
		}
		else
		{
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

			std::string full_trimmed_other_tokens = trim(other_tokens);

			// If whitespaces after range shouldnt be ignored, comment following line of code.
			// If whitespaces on the right should be deleted too, change ltrim to trim.
			other_tokens = ltrim(other_tokens);

			std::string first_token = trimmed_whole_without_command.substr(0, first_space);

			bool command_and_range_only = false;
			if (full_trimmed_other_tokens == "")
			{
				command_and_range_only = true;
			}

			if (command_letter == "a")
			{
				append_handler(all_lines, changed, command_and_range_only, first_token, other_tokens, trimmed_whole_without_command);
			}
			else if (command_letter == "c")
			{
				change_handler(all_lines, changed, command_and_range_only, first_token, other_tokens, trimmed_whole_without_command);
			}
			else if (command_and_range_only && (command_letter == "p" || command_letter == "d"))
			{
				print_delete_handler(all_lines, changed, first_token, command_letter);
			}
			else
			{
				std::cout << "Unsupported command" << std::endl;
			}
		}
	}
}

int main(int argc, char* argv[])
{
	//std::string path{ "Example_2.txt" };

	if (argc != 2)
	{
		std::cout << "Invalid number of arguments" << std::endl;
		return 2;
	}

	std::string path{ argv[1] };

	std::vector<std::string> all_lines;

	if (!read_file(path, all_lines))
	{
		std::ofstream ofs = open_out_stram(path);
		if (!ofs.is_open())
		{
			std::cout << "Unable to open file" << std::endl;
			return 1;
		}
		ofs.close();
	} 

	read_command(all_lines, path);

	return 0;
}
