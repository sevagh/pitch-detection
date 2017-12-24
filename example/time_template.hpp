#include <chrono>
#include <cstdlib>
#include <iostream>
#include <functional>
#include <utility>

template <typename TimeT = std::chrono::nanoseconds> struct measure {
	template <typename F, typename... Args>
	static typename TimeT::rep
	execution(int reps, F &&func, Args &&... args)
	{
		double tot = 0.0;

		for (int i = 0; i < reps; ++i) {
			auto start = std::chrono::steady_clock::now();

			std::invoke(std::forward<decltype(func)>(func),
			    std::forward<Args>(args)...);

			auto duration = std::chrono::duration_cast<TimeT>(
			    std::chrono::steady_clock::now() - start);

			tot += duration.count();
		}

		return tot / reps;
	}
};
