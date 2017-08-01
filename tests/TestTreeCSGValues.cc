#include "HyperCubeTree.h"
#include "SimpleSubdivisionScheme.h"
#include "GridDimension.h"
#include "csg.h"
#include "HyperCubeTreeLocatedCursor.h"

#include <iostream>

using hct::Vec3d;
using SubdivisionScheme = hct::SimpleSubdivisionScheme<3>;
using Tree = hct::HyperCubeTree< 3, SubdivisionScheme >;
using TreeCursor = hct::HyperCubeTreeLocatedCursor<Tree>;
std::ostream& operator << (std::ostream& out, Vec3d p) { return p.toStream(out); }

int main()
{
	SubdivisionScheme subdivisions;
	subdivisions.addLevelSubdivision({ 4,4,20 });
	subdivisions.addLevelSubdivision({ 3,3,3 });
	subdivisions.addLevelSubdivision({ 3,3,3 });
	subdivisions.addLevelSubdivision({ 3,3,3 });
	subdivisions.addLevelSubdivision( {3,3,3} );

	Tree tree(subdivisions);
	tree.refine(tree.rootCell());
	size_t nbRootChildren = subdivisions.getLevelSubdivision(0).gridSize();
	for (size_t i = 0; i < nbRootChildren; i++)
	{
		tree.refine(tree.child(tree.rootCell(), i));
	}

	auto sphereA = hct::csg_sphere(Vec3d({ 0.0,0.0,0.0 }), 1.0);
	auto sphereB = hct::csg_sphere(Vec3d({ 0.5,0.5,0.5 }), 0.5);
	auto shape = hct::csg_difference(sphereA, sphereB);

	Vec3d cellSize = Vec3d(1.0);
	std::cout << "level 0 : size = " << cellSize << std::endl;
	for (size_t i = 0; i < subdivisions.getNumberOfLevelSubdivisions(); i++)
	{
		cellSize = cellSize / subdivisions.getLevelSubdivision(i);
		std::cout << "level " << (i + 1) << " : size = " << cellSize << std::endl;
	}

	tree.preorderParseCells(
		[shape, &tree](const TreeCursor& cursor)
		{
			if (tree.isRefinable(cursor.cell()))
			{
				constexpr size_t nVertices = 1 << TreeCursor::D;
				bool allInside = true;
				bool allOutside = true;
				for (size_t i = 0; i < nVertices; i++)
				{
					Vec3d p = cursor.vertexPosition(i).normalize();
					if (shape(p).val > 0.0) { allInside = false; }
					else { allOutside = false; }
				}
				if (!allInside && !allOutside)
				{
					tree.refine(cursor.cell());
				}
			}
		}
		, TreeCursor() );

	hct::TreeLevelArray<double> cellSurfaceDistance;
	tree.addArray(&cellSurfaceDistance);

	hct::TreeLevelArray<size_t> cellLevel;
	tree.addArray(&cellLevel);

	hct::TreeLevelArray<size_t> cellIndex;
	tree.addArray(&cellIndex);

	hct::TreeLevelArray<bool> cellInside;
	tree.addArray(&cellInside);

	tree.preorderParseCells(
		[shape, &cellSurfaceDistance, &cellLevel, &cellIndex, &cellInside](const TreeCursor& cursor)
		{
			hct::HyperCubeTreeCell cell = cursor.cell();
			Vec3d p = cursor.position().addHalfUnit().normalize();
			double surfDist = shape(p).val;
			cellSurfaceDistance[cell] = surfDist;
			cellLevel[cell] = cell.level();
			cellIndex[cell] = cell.index();
			cellInside[cell] = (surfDist <= 0.0);
		}
		, TreeCursor() );

	tree.toStream(std::cout);

	return 0;
}
