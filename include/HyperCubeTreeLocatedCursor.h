#pragma once

#include "HyperCubeTreeCursor.h"
#include "HyperCubeTreeCell.h"
#include "HyperCubeTreeCellPosition.h"

namespace hct
{

/*
A tree traversal cursor adding information about position and size of the traversed cell.
*/
template<typename _Tree>
struct HyperCubeTreeLocatedCursor : public hct::HyperCubeTreeCursor<_Tree>
{
	using SuperClass = hct::HyperCubeTreeCursor<_Tree>;
	using Tree = _Tree;
	static constexpr unsigned int D = Tree::D;
	using VecT = hct::Vec<double, D>;
	using SubdivisionGrid = typename Tree::SubdivisionGrid;
	using GridLocation = typename Tree::GridLocation;

	// initialization constructors
	inline HyperCubeTreeLocatedCursor(hct::HyperCubeTreeCell cell = hct::HyperCubeTreeCell())
		: SuperClass(cell)
		, m_position(0)
		{}

	// recursion constructor
	inline HyperCubeTreeLocatedCursor(const Tree& tree, HyperCubeTreeLocatedCursor parent, SubdivisionGrid grid, GridLocation childLocation)
		: SuperClass(tree, parent, grid, childLocation)
	{
		m_position = parent.m_position.refine(grid) + childLocation;
	}

	HyperCubeTreeCellPosition m_position;
};

}