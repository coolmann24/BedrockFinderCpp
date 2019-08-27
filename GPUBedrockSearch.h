#include <CL\cl2.hpp>
#include "BedrockSearch.h"
#include <map>

bool getGPUDevices(std::vector<std::string>& devices)
{
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	if (platforms.size() == 0)return false;

	for (const auto& platform : platforms)
	{
		std::vector<cl::Device> gpus;
		platform.getDevices(CL_DEVICE_TYPE_GPU, &gpus);

		for (const auto& gpu : gpus)
			devices.push_back(gpu.getInfo<CL_DEVICE_NAME>());
	}
	return devices.size() != 0;
		
}

bool GPUSearch(const std::string& context, const std::string& device, const Formation& form, const Bounds& bounds)
{
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	if (platforms.size() == 0)return false;

	cl::Device gpu_to_use;

	bool valid_device = false;
	for (const auto& platform : platforms)
	{
		std::vector<cl::Device> gpus;
		platform.getDevices(CL_DEVICE_TYPE_GPU, &gpus);

		for (const auto& gpu : gpus)
		{
			if (gpu.getInfo<CL_DEVICE_NAME>() == device)
			{
				gpu_to_use = gpu;
				valid_device = true;
				break;
			}
		}
		if (valid_device) break;
	}

	if (!valid_device) return false;
	return true;
	//cl::Context context(gpu_to_use);

	//continue
}