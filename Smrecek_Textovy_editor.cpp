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

/// <summary>
/// Function to load the whole file into memory.
/// </summary>
/// <param name="path"></param>
/// <param name="all_lines"></param>
/// <returns></returns>
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

/// <summary>
/// Opens the output stream.
/// </summary>
/// <param name="path"></param>
/// <returns></returns>
std::ofstream open_out_stram(const std::string& path)
{
	std::ofstream ofs{ path };

	return ofs;
}

/// <summary>
/// Function for range check.
/// </summary>
/// <param name="first_line"></param>
/// <param name="last_line"></param>
/// <returns></returns>
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

/// <summary>
/// Convert number to int if possible.
/// </summary>
/// <param name="str_num"></param>
/// <returns></returns>
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

/// <summary>
/// Extract number for append command.
/// </summary>
/// <param name="token"></param>
/// <param name="first_line"></param>
/// <param name="last_line"></param>
/// <returns></returns>
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

/// <summary>
/// Extract possible ranges for multiple commands.
/// </summary>
/// <param name="token"></param>
/// <param name="first_line"></param>
/// <param name="last_line"></param>
/// <returns></returns>
bool range_extractor(const std::string& token, int& first_line, int& last_line)
{
	std::regex case1(R"(,)");
	std::regex case2(R"(,[0-9]+)");
	std::regex case3(R"([0-9]+,)");
	std::regex case4(R"([0-9]+,[0-9]+)");
	std::regex case5(R"([0-9]+)");

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

		first_line = -1;
		last_line = dummy_to_int_convertor(last);
		return dummy_range_check(first_line, last_line);
	}

	if (regex_match(token, case3))
	{
		std::string first = token.substr(0, delimeter_pos);

		first_line = dummy_to_int_convertor(first);
		last_line = -1;
		return dummy_range_check(first_line, last_line);
	}

	if (regex_match(token, case4))
	{
		std::string first = token.substr(0, delimeter_pos);
		std::string last = token.substr(delimeter_pos + 1, token.size());

		first_line = dummy_to_int_convertor(first);
		last_line = dummy_to_int_convertor(last);
		return dummy_range_check(first_line, last_line);
	}

	if (regex_match(token, case5))
	{
		first_line = dummy_to_int_convertor(token);
		last_line = dummy_to_int_convertor(token);
		return dummy_range_check(first_line, last_line);
	}

	first_line = -2;
	last_line = -2;
	return dummy_range_check(first_line, last_line);
}

/// <summary>
/// Command p.
/// </summary>
/// <param name="all_lines"></param>
/// <param name="start"></param>
/// <param name="end"></param>
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

/// <summary>
/// Oneliner version of command a.
/// </summary>
/// <param name="all_lines"></param>
/// <param name="changed"></param>
/// <param name="line"></param>
/// <param name="line_num"></param>
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

/// <summary>
/// Cammand a.
/// </summary>
/// <param name="all_lines"></param>
/// <param name="changed"></param>
/// <param name="line_num"></param>
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

/// <summary>
/// Command w.
/// </summary>
/// <param name="all_lines"></param>
/// <param name="changed"></param>
/// <param name="path"></param>
/// <returns></returns>
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

/// <summary>
/// Command d.
/// </summary>
/// <param name="all_lines"></param>
/// <param name="changed"></param>
/// <param name="start"></param>
/// <param name="end"></param>
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

/// <summary>
/// Oneliner version of command c.
/// </summary>
/// <param name="all_lines"></param>
/// <param name="changed"></param>
/// <param name="line"></param>
/// <param name="start"></param>
/// <param name="end"></param>
void change_command_oneliner(std::vector<std::string>& all_lines, bool& changed, std::string& line, int start, int end)
{
	delete_command(all_lines, changed, start, end);
	append_command_oneliner(all_lines, changed, line, start - 1);
}

/// <summary>
/// Command c.
/// </summary>
/// <param name="all_lines"></param>
/// <param name="changed"></param>
/// <param name="start"></param>
/// <param name="end"></param>
void change_command(std::vector<std::string>& all_lines, bool& changed, int start, int end)
{
	delete_command(all_lines, changed, start, end);
	append_command(all_lines, changed, start - 1);
}

/// <summary>
/// Handler for one letter comands a p d c.
/// </summary>
/// <param name="all_lines"></param>
/// <param name="changed"></param>
/// <param name="lr_trimmed_whole"></param>
/// <returns></returns>
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

/// <summary>
/// Handler for command a.
/// </summary>
/// <param name="all_lines"></param>
/// <param name="changed"></param>
/// <param name="command_and_range_only"></param>
/// <param name="first_token"></param>
/// <param name="other_tokens"></param>
/// <param name="trimmed_whole_without_command"></param>
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

/// <summary>
/// Handler for command c.
/// </summary>
/// <param name="all_lines"></param>
/// <param name="changed"></param>
/// <param name="command_and_range_only"></param>
/// <param name="first_token"></param>
/// <param name="other_tokens"></param>
/// <param name="trimmed_whole_without_command"></param>
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

/// <summary>
/// Handler for commands p d.
/// </summary>
/// <param name="all_lines"></param>
/// <param name="changed"></param>
/// <param name="first_token"></param>
/// <param name="command_letter"></param>
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

/// <summary>
/// Function for reading commands from input. Deciding which command handler call. Handling q and q! commands.
/// </summary>
/// <param name="all_lines"></param>
/// <param name="path"></param>
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

/// <summary>
/// Main function of program. Function reads the file or creates a non-existent file and calls the command processing function. 
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
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

	// Read file. Check if file is read, if not, try to open it for write. If file did not exist, it was created.
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
