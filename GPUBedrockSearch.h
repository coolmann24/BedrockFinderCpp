#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

#include <CL\cl.hpp>
#include "BedrockSearch.h"
#include <map>
#include "GPUBedrockGen.h"
#include "PrecomputedRandAdvance.h"
#include <tuple>
#include <string>

extern std::array<int64_t, 256 * 4> A_OW_112;
extern std::array<int64_t, 256 * 4> B_OW_112;
extern std::array<int64_t, 289 * 4> A_OW_113_114;
extern std::array<int64_t, 289 * 4> B_OW_113_114;
extern std::array<int64_t, 289 * 8> A_N_113_114;
extern std::array<int64_t, 289 * 8> B_N_113_114;

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

bool GPUSearch(const std::string& context, const std::string& device, const Formation& form, const Bounds& bounds, int max_num_results)
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
			if (strcmp(gpu.getInfo<CL_DEVICE_NAME>().c_str(), device.c_str()) == 0)//string equals doesnt work here for some reason
			{
				gpu_to_use = gpu;
				valid_device = true;
				break;
			}
		}
		if (valid_device) break;
	}

	if (!valid_device) return false;


	cl::Context cl_context(gpu_to_use);

	std::string get_bedrock_func;
	int64_t* a = nullptr;
	int64_t* b = nullptr;
	int precomp_arr_size = 0;

	if (context == "1.12 Overworld")
	{
		a = A_OW_112.data();
		b = B_OW_112.data();
		precomp_arr_size = A_OW_112.size();
		get_bedrock_func = OW112_114;
	}
	else if (context == "1.13 Overworld")
	{
		a = A_OW_113_114.data();
		b = B_OW_113_114.data();
		precomp_arr_size = A_OW_113_114.size();
		get_bedrock_func = OW113;
	}
	else if (context == "1.14 Overworld")
	{
		a = A_OW_113_114.data();
		b = B_OW_113_114.data();
		precomp_arr_size = A_OW_113_114.size();
		get_bedrock_func = OW112_114;
	}
	else if (context == "1.13 Nether")
	{
		a = A_N_113_114.data();
		b = B_N_113_114.data();
		precomp_arr_size = A_N_113_114.size();
		get_bedrock_func = N113;
	}
	else if (context == "1.14 Nether")
	{
		a = A_N_113_114.data();
		b = B_N_113_114.data();
		precomp_arr_size = A_N_113_114.size();
		get_bedrock_func = N114;
	}

	std::string source_code = utility + get_bedrock_func + kernel;

	cl::Program program(cl_context, source_code);

	auto err = program.build("-cl-std=CL1.2");

	//std::string name = gpu_to_use.getInfo<CL_DEVICE_NAME>();
	//std::string buildlog = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(gpu_to_use);
	//std::cerr << "Build log for " << name << ":" << std::endl
	//	<< buildlog << std::endl;
	
	//return true;

	int form_size = form.size();
	int* arr_form = new int[4*form_size];
	
	int count = 0;
	for (const auto& e : form)
	{
		arr_form[4 * count] = std::get<0>(e.first);
		arr_form[4 * count + 1] = std::get<1>(e.first);
		arr_form[4 * count + 2] = std::get<2>(e.first);
		arr_form[4 * count + 3] = e.second ? 1 : 0;
		count++;
	}

	int results_start_ind = 0;
	int num_results = max_num_results;
	int* results = new int[num_results*3];
	int* coord_offset = new int[3]{ std::get<0>(bounds), std::get<4>(bounds), std::get<2>(bounds) };

	cl::Buffer a_buf(cl_context, CL_MEM_READ_ONLY | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int64_t) * precomp_arr_size, a);
	cl::Buffer b_buf(cl_context, CL_MEM_READ_ONLY | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int64_t) * precomp_arr_size, b);
	cl::Buffer formation_buf(cl_context, CL_MEM_READ_ONLY | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * form_size * 4, arr_form);
	cl::Buffer formation_size_buf(cl_context, CL_MEM_READ_ONLY | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int), &form_size);
	cl::Buffer results_ind_buf(cl_context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int), &results_start_ind);
	cl::Buffer num_results_buf(cl_context, CL_MEM_READ_ONLY | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int), &num_results);
	cl::Buffer results_buf(cl_context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)*num_results*3, results);
	cl::Buffer coord_offsets_buf(cl_context, CL_MEM_READ_ONLY | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)*3, coord_offset);

	cl::Kernel kernel(program, "hasFormation");
	kernel.setArg(0, a_buf);
	kernel.setArg(1, b_buf);
	kernel.setArg(2, formation_buf);
	kernel.setArg(3, formation_size_buf);
	kernel.setArg(4, results_ind_buf);
	kernel.setArg(5, num_results_buf);
	kernel.setArg(6, results_buf);
	kernel.setArg(7, coord_offsets_buf);

	cl::CommandQueue queue(cl_context, gpu_to_use);
	queue.enqueueNDRangeKernel(kernel, cl::NDRange(NULL), cl::NDRange(std::get<1>(bounds) - std::get<0>(bounds) + 1, std::get<5>(bounds) - std::get<4>(bounds) + 1, std::get<3>(bounds) - std::get<2>(bounds) + 1));
	queue.finish();
	queue.enqueueReadBuffer(results_buf, CL_TRUE, 0, sizeof(int) * num_results * 3, results);
	queue.finish();
	queue.enqueueReadBuffer(results_ind_buf, CL_TRUE, 0, sizeof(int), &results_start_ind);
	queue.finish();
	std::string log_result;

	for (int i = 0; i < std::min(num_results, results_start_ind); i++)
	{
		log_result += "X: " + std::to_string(results[i * 3]) + " Z: " + std::to_string(results[i * 3 + 2]) + "\n";
	}

	wxCommandEvent* evt = new wxCommandEvent;
	evt->SetId(1006);
	evt->SetEventType(LOG_EVENT);
	evt->SetString(log_result.c_str());
	wxTheApp->QueueEvent(evt);

	wxCommandEvent* evt2 = new wxCommandEvent;
	evt2->SetId(1007);
	evt2->SetString("Done searching\n");
	evt2->SetEventType(SEARCH_END_EVENT);
	wxTheApp->QueueEvent(evt2);

	return true;
}