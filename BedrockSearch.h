#pragma once

#include <vector>
#include <map>
#include <tuple>
#include <functional>
#include <iostream>
#include <thread>
#include <wx\wx.h>
#include "BFFGui.h"
#include <atomic>



using BedrockGenFunc = std::function<bool(int, int, int)>;
//using EndSearchFunc = std::function<void(void)>;
using Formation = std::map<std::tuple<int, int, int>, bool>;
using Bounds = std::tuple<int, int, int, int, int, int>;//xmin, xmax, zmin, zmax, ymin, ymax

void searchArea(Bounds bounds, Formation form, BedrockGenFunc func, std::atomic_bool* cont, std::atomic_int* results_pending)
{
	for(int y = std::get<4>(bounds); y <= std::get<5>(bounds); y++)
		for (int x = std::get<0>(bounds); x <= std::get<1>(bounds); x++)
		{
			if (!(*cont)) return;
			for (int z = std::get<2>(bounds); z <= std::get<3>(bounds); z++)
			{

				bool found = true;

				for (const auto& point : form)
				{
					found = point.second == func(std::get<0>(point.first) + x, std::get<1>(point.first) + y, std::get<2>(point.first) + z);
					if (!found)break;
				}



				if (found)
				{
					(*results_pending)++;

					wxCommandEvent* evt = new wxCommandEvent;
					evt->SetId(1006);
					evt->SetEventType(LOG_EVENT);

					if((*results_pending) < 100)
					{ 
						evt->SetString(("X: " + std::to_string(x) + " Z: " + std::to_string(z) + "\n").c_str());
						wxTheApp->QueueEvent(evt);
					}
					else
					{
						evt->SetString(std::string("Too many results, terminating thread...\n").c_str());
						wxTheApp->QueueEvent(evt);
						return;
					}
				}
			}
		}
}

void threadedSearch(Bounds bounds, Formation form, BedrockGenFunc func, int num_threads, std::atomic_bool* cont, std::atomic_int* results_pending)
{
	if (num_threads <= 0)
	{
		return;
	}
	int width = std::abs(std::get<1>(bounds) - std::get<0>(bounds));
	std::vector<std::thread> threads;
	for (int i = 0; i < num_threads - 1; i++)
	{
		threads.emplace_back(searchArea, std::make_tuple(std::get<0>(bounds) + i * (width / num_threads), std::get<0>(bounds) + (i + 1) * (width / num_threads) - 1, std::get<2>(bounds), std::get<3>(bounds), std::get<4>(bounds), std::get<5>(bounds)), form, func, cont, results_pending);
	}
	threads.emplace_back(searchArea, std::make_tuple(std::get<0>(bounds) + (num_threads-1) * (width / num_threads), std::get<1>(bounds), std::get<2>(bounds), std::get<3>(bounds), std::get<4>(bounds), std::get<5>(bounds)), form, func, cont, results_pending);

	for (size_t i = 0; i < threads.size(); i++)
		threads[i].join();
	
	wxCommandEvent* evt = new wxCommandEvent;
	evt->SetId(1007);
	evt->SetString("Done searching\n");
	evt->SetEventType(SEARCH_END_EVENT);
	wxTheApp->QueueEvent(evt);
}