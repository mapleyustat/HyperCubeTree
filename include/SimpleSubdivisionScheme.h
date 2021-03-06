#pragma once

#include "GridDimension.h"
#include <vector>
#include <initializer_list>
#include <cstdlib>

namespace hct
{
	template<unsigned int _D>
	class SimpleSubdivisionScheme
	{
		static constexpr unsigned int D = _D;
	public:

		inline void addLevelSubdivision(std::initializer_list<unsigned int> l)
		{
			addLevelSubdivision(GridDimension<D>(l));
		}
		inline void addLevelSubdivision(GridDimension<D> grid)
		{
			m_level_subdivisions.push_back(grid);
		}

		inline size_t getNumberOfLevelSubdivisions() const
		{
			return m_level_subdivisions.size();
		}

		inline GridDimension<D> getLevelSubdivision(size_t level) const
		{
			assert(level < m_level_subdivisions.size());
			return m_level_subdivisions[level];
		}

		template<typename StreamT>
		inline StreamT& toStream(StreamT & out)
		{
			size_t i = 0;
			for (GridDimension<D> g : m_level_subdivisions)
			{
				out << "subdivision " << i << " : "; g.toStream(out); out << '\n';
				++i;
			}
			return out;
		}

		template<typename StreamT>
		inline void fromStream(StreamT & input)
		{
			bool endInput = false;
			while (input && !endInput)
			{
				std::string token;
				input >> token;
				if (token == "end")
				{
					endInput = true;
				}
				else if (token == "grid")
				{
					unsigned int grid[D];
					for (size_t i = 0; i < D; i++) { grid[i] = 0;  input >> grid[i]; }
					addLevelSubdivision(GridDimension<D>(grid));
				}
				else
				{
					std::abort();
				}
			}
		}

	private:
		std::vector< GridDimension<D> > m_level_subdivisions;
	};

}