#include <vector>
#include <map>
#include <tuple>
#include <functional>
#include <iostream>
#include <thread>

enum FormType : bool{
	DEFAULT=false,
	BEDROCK=true
};

using BedrockGenFunc = std::function<bool(int, int, int)>;
using Formation = std::map<std::tuple<int, int, int>, FormType>;
using Bounds = std::tuple<int, int, int, int, int, int>;//xmin, xmax, zmin, zmax, ymin, ymax

void search(Bounds bounds, Formation form, BedrockGenFunc func)
{
	for(int y = std::get<4>(bounds); y <= std::get<5>(bounds); y++)
		for (int x = std::get<0>(bounds); x <= std::get<1>(bounds); x++)
			for (int z = std::get<2>(bounds); z <= std::get<3>(bounds); z++)
			{
				bool found = true;

				for (const auto& point : form)
				{
					found = point.second == func(std::get<0>(point.first) + x, std::get<1>(point.first) + y, std::get<2>(point.first) + z);
					if (!found)break;
				}

				if (found)std::cout << "X: " << x << " Z: " << z << std::endl;
			}
}

void threadedSearch(Bounds bounds, Formation form, BedrockGenFunc func, int num_threads)
{
	if (num_threads <= 0)return;

	int width = std::abs(std::get<1>(bounds) - std::get<0>(bounds));
	std::vector<std::thread> threads;
	for (int i = 0; i < num_threads - 1; i++)
	{
		threads.emplace_back(search, std::make_tuple(std::get<0>(bounds) + i * (width / num_threads), std::get<0>(bounds) + (i + 1) * (width / num_threads) - 1, std::get<2>(bounds), std::get<3>(bounds), std::get<4>(bounds), std::get<5>(bounds)), form, func);
	}
	threads.emplace_back(search, std::make_tuple(std::get<0>(bounds) + (num_threads-1) * (width / num_threads), std::get<1>(bounds), std::get<2>(bounds), std::get<3>(bounds), std::get<4>(bounds), std::get<5>(bounds)), form, func);

	for (size_t i = 0; i < threads.size(); i++)
		threads[i].join();
}