#include "myfs.h"
#include <string.h>
#include <iostream>
#include <math.h>
#include <sstream>
#include <unistd.h>

const char *MyFs::MYFS_MAGIC = "MYFS";

MyFs::MyFs(BlockDeviceSimulator *blkdevsim_):blkdevsim(blkdevsim_) {
	struct myfs_header header;
	blkdevsim->read(0, sizeof(header), (char *)&header);

	if (strncmp(header.magic, MYFS_MAGIC, sizeof(header.magic)) != 0 ||
	    (header.version != CURR_VERSION)) {
		std::cout << "Did not find myfs instance on blkdev" << std::endl;
		std::cout << "Creating..." << std::endl;
		format();
		std::cout << "Finished!" << std::endl;
	}
}



void MyFs::format() {

	// put the header in place
	struct myfs_header header;
	strncpy(header.magic, MYFS_MAGIC, sizeof(header.magic));
	header.version = CURR_VERSION;
	blkdevsim->write(0, sizeof(header), (const char*)&header);

	// TODO: put your format code here
	return;
}

void MyFs::create_file(std::string path_str, bool directory) {
	struct dir_list_entry entry;
	entry.name = path_str;
	entry.is_dir = directory;
	entry.file_size = 0;

	this->list.push_back(entry);
}

std::string MyFs::get_content(std::string path_str) {
	for(int i = 0; i < (int)this->list.size(); i++)
	{
		if(this->list[i].name == path_str)
		{
			return this->list[i].content;
		}
	}

	throw std::runtime_error("No such file or directory");
	return "";
}

void MyFs::set_content(std::string path_str, std::string content) {
	for(int i = 0; i < (int)this->list.size(); i++)
	{
		if(this->list[i].name == path_str)
		{
			this->list[i].content = content + '\26';
			this->list[i].file_size = content.size();
			return;
		}
	}

	throw std::runtime_error("No such file or directory");
}

MyFs::dir_list MyFs::list_dir(std::string path_str) {
	MyFs::dir_list ans;
	for(int i = 0; i < (int)this->list.size(); i++)
	{
		if(strncmp(this->list[i].name.c_str(), path_str.c_str(), path_str.size()))
		{
			ans.push_back(this->list[i]);
		}
	}

	return ans;
}

