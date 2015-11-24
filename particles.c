//http://paulbourke.net/geometry/polygonise/
int triTable[256][16] =
{{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
{3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
{3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
{3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
{9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
{2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
{8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
{4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
{3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
{1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
{4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
{4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
{5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
{2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
{9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
{0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
{2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
{10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
{5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
{5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
{9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
{1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
{10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
{8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
{2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
{7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
{2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
{11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
{5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
{11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
{11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
{9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
{2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
{6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
{3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
{6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
{10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
{6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
{8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
{7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
{3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
{0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
{9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
{8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
{5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
{0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
{6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
{10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
{10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
{8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
{1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
{0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
{10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
{3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
{6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
{9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
{8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
{3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
{6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
{0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
{10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
{10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
{2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
{7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
{7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
{2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
{1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
{11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
{8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
{0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
{7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
{10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
{2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
{6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
{7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
{2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
{10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
{10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
{0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
{7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
{6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
{8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
{9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
{6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
{4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
{10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
{8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
{0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
{1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
{8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
{10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
{4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
{10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
{11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
{9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
{6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
{7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
{3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
{7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
{3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
{6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
{9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
{1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
{4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
{7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
{6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
{3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
{0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
{6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
{0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
{11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
{6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
{5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
{9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
{1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
{1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
{10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
{0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
{5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
{10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
{11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
{9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
{7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
{2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
{8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
{9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
{9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
{1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
{9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
{5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
{0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
{10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
{2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
{0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
{0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
{9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
{5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
{3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
{5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
{8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
{0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
{9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
{1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
{3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
{4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
{9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
{11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
{11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
{2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
{9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
{3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
{1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
{4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
{3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
{0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
{1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}};



// OpenGL 3 conversion 2013.
//gcc particles.c common/*.c common/Linux/MicroGlut.c -lGL -lX11 -lm -o particles -I common -I common/Linux


// Kolla om man kan ladda in lite fler tetraedrar från array typ.. ?  


#include <stdlib.h>


#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#ifdef __APPLE__
// Mac
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// uses framework Cocoa
#else
	#ifdef WIN32
// MS
		#include <windows.h>
		#include <stdio.h>
		#include <GL/glew.h>
		#include <GL/glut.h>
	#else
// Linux
		#include <stdio.h>
		#include <GL/gl.h>
		#include "MicroGlut.h"
//		#include <GL/glut.h>
	#endif
#endif

#include "VectorUtils3.h"
#include "GL_utilities.h"
#include "loadobj.h"
#include "zpr.h"



#define kBallSize 0.3
#define boundRad 0.3
#define cellSize 1.0
// Antalet tetraedrar
#define NO_OBJECTS 1
#define gravity 9.82
#define threshold 1
#define nrCells 8
#define DIM 2

GLuint* vertexArrayObjID, vertexBufferObjID, shader;


GLuint* vertexArrayObjID, vertexBufferObjID, normalBuffer, normalArray, shader;
GLuint* tetraArray, tetraNormalArray, tetraVertexBuffer, tetraNormalBuffer, tetraIndexBuffer;
GLuint* mTriArray, mTriBuffer;



Point3D cam, point;

mat4 projectionMatrix;
mat4 viewMatrix, vm2;
mat4 tmpMatrix; 
const float XMIN = -1.0f, XMAX = 1.0f, YMIN = -1.0f, YMAX=1.0f,  ZMIN = -1.0f, ZMAX=1.0f;

// Marching cubes är inte "på" 
int marchOn = 0; 

// Tetra struts
//*******************************************************************************************
typedef struct
{
	// Tror inte denna behövs..
	GLuint* tetraArr; 
	
	// Tetraederns massa
	GLfloat mass;

	// Om den ska synas eller inte 
	int alive;
	
	// Positioner
	 vec3 pos; 

	// Hastigheten		
	vec3 vel;

} Tetra;

//Marching cude
//******************************************************************************************
typedef struct
{
	// Positioner
	 vec3 pos; 

	//cube Storlek	SKRIV INTE ULFS	
	//float size;

} MCube;

typedef struct 
{

	// inne/ute
	int state;
	
	// antal partiklar
	int nrParts;

}Cell;

// Array med alla tetras som ska ritas upp 
// Glöm inte ändra denna med
Tetra tetras[NO_OBJECTS];

// En array av celler medelst innehåll
Cell cell[nrCells];
int cubeCorners[8];
MCube mc;

//Plane strushish
GLfloat vertices[] = 
{
        // Left bottom triangle
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        // Right top triangle
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
	//back lower triangle
	-1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
	//back upper triangle
	-1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f
};

GLfloat normals[] = 
{
        // Left bottom triangle
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        // Right top triangle
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
	//back lower triangle
	0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
	//back upper triangle
	0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
};


//regular (1,1,1), (1,−1,−1), (−1,1,−1), (−1,−1,1)
GLfloat tetraVertices[] = 
{
        -0.1f, -0.1f, 0.1f,
        0.1f, 0.1f, 0.1f,
        -0.1f, 0.1f, -0.1f,
	0.1f, -0.1f, -0.1f
        
};

//Indices for tetrahedron
GLuint tetraIndicies[] = 
{
	//front	
	0,1,2, 
	//right
	3,1,0, 
	//left
	0,2,3,
	//back
	1,3,2
};

GLfloat mTris[117];


void March()
{
	mc.pos = SetVector(XMIN, YMIN, ZMIN);
	int i;
	// loop through all cells and calculate number of particles
	for(i=0; i < nrCells; i++)
	{ 
		cell[i].nrParts = 0;
		cell[i].state = 0;
		int j;		
		for(j=0; j<NO_OBJECTS; j++)
		{
			// Check if particle is inside this cell
			//OBS! Will now register both cells on cell border
			if(tetras[j].pos.x >= mc.pos.x 
			&& tetras[j].pos.x <= (mc.pos.x + cellSize) 
			&& tetras[j].pos.y >= mc.pos.y 
			&& tetras[j].pos.y <= (mc.pos.y + cellSize)
			&& tetras[j].pos.z >= mc.pos.z 
			&& tetras[j].pos.z <= (mc.pos.z + cellSize))
			{
				// If particle is inside cell add to this cells particle count
				cell[i].nrParts++;	
			}
				
		}
		
		if(cell[i].nrParts >= threshold)
		{
			// If threshold is reached set state to 1!!!!!!!!!!!	
			cell[i].state = 1;
		}
		/*
		Step through cells using the position and check against the borders.
		When XMAX position is used, step up in y direction and reset xpos.
		*/
		mc.pos.x += cellSize;
		if(mc.pos.x == XMAX)
                {
			mc.pos.x = XMIN;
			mc.pos.y += cellSize;

			if(mc.pos.y == YMAX)
                        {
				mc.pos.y = YMIN;
				mc.pos.z += cellSize;
			}
		}
	}
	int m, j, x, y, z, Case;
        y=z=0;
        x=-1;
        vec3 edge[12];
        int count = 0;

	for(j=0; j<nrCells; j++)
        {
                //kommentarer är gay, detta gör vi för att inte alla hörnpunkter ska få värdet 1
                for(m=0; m<8; m++)
                        cubeCorners[m] = 0;
                x++;
                if(x==DIM)
                {
                        y++;
                        x=0;
                }
                if(y==DIM)
                {
                        z++;
                        x=y=0;
                }
                
                //Kolla mot kuber som ligger "vägg i vägg" med vår kub
                
		if(x!=0 && cell[j-1].state == 1)
                {
			cubeCorners[0] = 1;
                        cubeCorners[3] = 8;
                        cubeCorners[4] = 16;
                        cubeCorners[7] = 128;
		}
                if(x!=DIM-1 && cell[j+1].state == 1)
                {
                        cubeCorners[1] = 2;
                        cubeCorners[2] = 4;
                        cubeCorners[5] = 32;
                        cubeCorners[6] = 64;
                }
                if(y!=0 && cell[j-DIM].state == 1){
                        cubeCorners[0] = 1;
                        cubeCorners[1] = 2;
                        cubeCorners[2] = 4;     
                        cubeCorners[3] = 8;
                }
                if(y!=DIM-1 && cell[j+DIM].state == 1)
                {
                        cubeCorners[4] = 16;
                        cubeCorners[5] = 32;
                        cubeCorners[6] = 64;
                        cubeCorners[7] = 128;
                }
                if(z!=0 && cell[j-DIM*DIM].state == 1){
                        cubeCorners[0] = 1;
                        cubeCorners[1] = 2;
                        cubeCorners[4] = 16;
                        cubeCorners[5] = 32;
                }       
                if(z!=DIM-1 && cell[j+DIM*DIM].state == 1)
                {
                        cubeCorners[2] = 4;     
                        cubeCorners[3] = 8;
                        cubeCorners[6] = 64;
                        cubeCorners[7] = 128;
                }
                
                //Kolla kuber som delar en edge med vår kub
                if(x!=0 && y!=0 && cell[j-DIM - 1].state == 1)
                {
                        cubeCorners[0] = 1;
                        cubeCorners[3] = 8;
                }
                if(x!=DIM-1 && y!=0 && cell[j-DIM + 1].state == 1)
                {
                        cubeCorners[1] = 2;
                        cubeCorners[2] = 4;
                }
                if(y!=0 && z!=0 && cell[j-DIM - DIM*DIM].state == 1)
                {
                        cubeCorners[0] = 1;
                        cubeCorners[1] = 2;
                }
                if(y!=0 && z!=DIM-1 && cell[j-DIM + DIM*DIM].state == 1)
                {
                        cubeCorners[2] = 4;
                        cubeCorners[3] = 8;
                }
                if(x!=0 && y!=DIM-1 && cell[j+DIM - 1].state == 1)
                {
                        cubeCorners[4] = 16;
                        cubeCorners[7] = 128;
                }
                if(x!=DIM-1 && y!=DIM-1 && cell[j+DIM + 1].state == 1)
                {
                        cubeCorners[5] = 32;
                        cubeCorners[6] = 64;
                }
                if(y!=DIM-1 && z!=0 && cell[j+DIM - DIM*DIM].state == 1)
                {
                        cubeCorners[4] = 16;
                        cubeCorners[5] = 32;
                }
                if(y!=DIM-1 && z!=DIM-1 && cell[j+DIM + DIM*DIM].state == 1)
                {
                        cubeCorners[6] = 64;
                        cubeCorners[7] = 128;
                }    
                if(x!=0 && z!=0 && cell[j-1 - DIM*DIM].state == 1)
                {
                        cubeCorners[0] = 1;
                        cubeCorners[4] = 16;
                }
                if(x!=DIM-1 && z!=0 && cell[j+1 - DIM*DIM].state == 1)
                {
                        cubeCorners[1] = 2;
                        cubeCorners[5] = 32;
                }
                if(x!=0 && z!=DIM-1 && cell[j-1 + DIM*DIM].state == 1)
                {
                        cubeCorners[7] = 128;
                        cubeCorners[3] = 8;
                }    
                if(x!=DIM-1 && z!=DIM-1 && cell[j+1 + DIM*DIM].state == 1)
                {
                        cubeCorners[6] = 64;
                        cubeCorners[2] = 4;
                }  
 
                //Kollar kuber som delar hörnpunkt med vår kub
                if(x!=0 && y!=0 && z!=0 && cell[j-1 - DIM - DIM*DIM].state == 1)
                {
                        cubeCorners[0] = 1;
                } 
                if(x!=DIM-1 && y!=0 && z!=0 && cell[j+1 - DIM - DIM*DIM].state == 1)
                {
                        cubeCorners[1] = 2;
                }  
                if(x!=0 && y!=0 && z!=DIM-1 && cell[j-1 - DIM + DIM*DIM].state == 1)
                {
                        cubeCorners[3] = 8;
                } 
                if(x!=DIM-1 && y!=0 && z!=DIM-1 && cell[j+1 - DIM + DIM*DIM].state == 1)
                {
                        cubeCorners[2] = 4;
                }
                if(x!=0 && y!=DIM-1 && z!=0 && cell[j-1 + DIM - DIM*DIM].state == 1)
                {
                        cubeCorners[4] = 16;
                } 
                if(x!=DIM-1 && y!=DIM-1 && z!=0 && cell[j+1 + DIM - DIM*DIM].state == 1)
                {
                        cubeCorners[5] = 32;
                }  
                if(x!=0 && y!=DIM-1 && z!=DIM-1 && cell[j-1 + DIM + DIM*DIM].state == 1)
                {
                        cubeCorners[7] = 128;
                } 
                if(x!=DIM-1 && y!=DIM-1 && z!=DIM-1 && cell[j+1 + DIM + DIM*DIM].state == 1)
                {
                        cubeCorners[6] = 64;
                }
                

                int k;
                Case = 0;
                for(k = 0; k<8; k++){
                        Case += cubeCorners[k];
                }
                //vart ligger edgesen i världen
                //botten av cuben
                edge[0]=SetVector(-1.0f+x*cellSize+cellSize/2.0f, -1.0f+y*cellSize, -1.0f+z*cellSize);
                edge[1]=SetVector(-1.0f+(x+1)*cellSize, -1.0f+y*cellSize, -1.0f+z*cellSize+cellSize/2.0f);
                edge[2]=SetVector(-1.0f+x*cellSize+cellSize/2.0f, -1.0f+y*cellSize, -1.0f+(z+1)*cellSize);
                edge[3]=SetVector(-1.0f+x*cellSize, -1.0f+y*cellSize, -1.0f+z*cellSize+cellSize/2.0f);
                //toppen av cuben
                edge[4]=SetVector(-1.0f+x*cellSize+cellSize/2.0f, -1.0f+(y+1)*cellSize, -1.0f+z*cellSize);
                edge[5]=SetVector(-1.0f+(x+1)*cellSize, -1.0f+(y+1)*cellSize, -1.0f+z*cellSize+cellSize/2.0f);
                edge[6]=SetVector(-1.0f+x*cellSize+cellSize/2.0f, -1.0f+(y+1)*cellSize, -1.0f+(z+1)*cellSize);
                edge[7]=SetVector(-1.0f+x*cellSize, -1.0f+(y+1)*cellSize, -1.0f+z*cellSize+cellSize/2.0f);
                //sidorna av cuben
                edge[8]=SetVector(-1.0f+x*cellSize, -1.0f+y*cellSize+cellSize/2.0f, -1.0f+z*cellSize);
                edge[9]=SetVector(-1.0f+(x+1)*cellSize, -1.0f+y*cellSize+cellSize/2.0f, -1.0f+z*cellSize);
                edge[10]=SetVector(-1.0f+(x+1)*cellSize, -1.0f+y*cellSize+cellSize/2.0f, -1.0f+(z+1)*cellSize);
                edge[11]=SetVector(-1.0f+x*cellSize, -1.0f+y*cellSize+cellSize/2.0f, -1.0f+(z+1)*cellSize);            
                
                //hämta vilket case
                int tmp;
                
                for(k = 0; k<16; k++)
                {
                        tmp = triTable[Case][k];
                        //printf("%i, ", tmp);
                        if(tmp != -1)
                        {
                                mTris[count] = edge[tmp].x;
                                count++;
                                mTris[count] = edge[tmp].y;
                                count++;                                
                                mTris[count] = edge[tmp].z;
                                count++;
                        }else{
                                k=15;
                        }
                }
	}
}

void calcBounce(int nr){
	vec3 normal = SetVector(0,0,0);
	if(tetras[nr].pos.x >= XMAX){
		normal = SetVector(-1.0f,0.0f,0.0f);
		tetras[nr].vel = VectorSub(tetras[nr].vel, ScalarMult(normal, 2.0f*DotProduct(tetras[nr].vel, normal)));
	}
		
	if(tetras[nr].pos.x <= XMIN){
		normal = SetVector(1.0f,0.0f,0.0f);
		tetras[nr].vel = VectorSub(tetras[nr].vel, ScalarMult(normal, 2.0f*DotProduct(tetras[nr].vel, normal)));
	}

	if(tetras[nr].pos.y >= YMAX){
		normal = SetVector(0.0f,-1.0f,0.0f);
		tetras[nr].vel = VectorSub(tetras[nr].vel, ScalarMult(normal, 2.0f*DotProduct(tetras[nr].vel, normal)));
	}
	
	if(tetras[nr].pos.y <= YMIN){
		normal = SetVector(0.0f,1.0f,0.0f);
		tetras[nr].vel = VectorSub(tetras[nr].vel, ScalarMult(normal, 2.0f*DotProduct(tetras[nr].vel, normal)));
	}
	
	if(tetras[nr].pos.z >= ZMAX){
		normal = SetVector(0.0f,0.0f,-1.0f);
		tetras[nr].vel = VectorSub(tetras[nr].vel, ScalarMult(normal, 2.0f*DotProduct(tetras[nr].vel, normal)));
	}
	if(tetras[nr].pos.z <= ZMIN){
		normal = SetVector(0.0f,0.0f,1.0f);
		tetras[nr].vel = VectorSub(tetras[nr].vel, ScalarMult(normal, 2.0f*DotProduct(tetras[nr].vel, normal)));
	}
}


void drawTetra(int nr)
{
	calcBounce(nr);
	tetras[nr].pos = VectorAdd(tetras[nr].pos, tetras[nr].vel);
	tmpMatrix = T(tetras[nr].pos.x, tetras[nr].pos.y, tetras[nr].pos.z); // position
	tmpMatrix = Mult(viewMatrix, tmpMatrix);
    	glUniformMatrix4fv(glGetUniformLocation(shader, "modelviewMatrix"), 1, GL_TRUE, tmpMatrix.m);
	
	// Rita ut tetraeder? 
	glBindVertexArray(tetraArray);
	//glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);	
}

//Education is an admirable thing, but it is well to remember from time to time 
//that nothing that is worth knowing can be taught.
// Oscar Wilde

// Uppdatera "fysiken" eller tetraweissarnas positioner
/*void uppdatePos(int nr)
{
	int i;
	for(i = 0; i < NO_OBJECTS; i++)
	{
		// Dålig konstant fall
		//tetras[i].pos.y -= gravity * 0.001; 
		
		// mer korrekt än förra men fel och går sjukt snabbt. 
		tetras[i].pos.y -= gravity * GLUT_ELAPSED_TIME * tetras[i].mass; 	
	}	
}*/


GLfloat tetraNormals[] = 
{
	//front        
	-0.58f, 0.58f, 0.58f,
	//right        
	0.58f, -0.58f, 0.58f,
	//left
        -0.58f, -0.58f, -0.58f,
	//back
	0.58f, 0.58f, -0.58f        
};




//Some cause happiness wherever they go; others whenever they go.
// Oscar Wilde 


// Drawing routine
void Display()
{
	// Uppdatera tetrornas pos och annat smött å gött 

	// Clear framebuffer & zbuffer
	glClearColor(0.1, 0.1, 0.3, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable Z-buffering
	glEnable(GL_DEPTH_TEST);
	// Enable backface culling
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	glUseProgram(shader);

	//viewMatrix = lookAt(0,  3,  3,  0,  0,  0,  0,  1,  0);
	
	glUniformMatrix4fv(glGetUniformLocation(shader, "modelviewMatrix"), 1, GL_TRUE, viewMatrix.m);
	
	
	glBindVertexArray(vertexArrayObjID);
	glBindVertexArray(normalArray);

	// Rita ut planet	
	glDrawArrays(GL_TRIANGLES, 0, 12);// draw objectperspective
	
	// Om marching cubes inte är på
	if(marchOn == 0)
	{
		int i; 
		// Rita ut tetraeder
		for(i = 0; i < NO_OBJECTS; i++) 
		{
			drawTetra(i); 
		}
	}
	glDrawArrays(GL_TRIANGLES, 0, 39);// draw objectperspective
	
	GLfloat ang = 0.001f * glutGet(GLUT_ELAPSED_TIME);
	mat4 rot = ArbRotate(SetVector(1.0, 0.0, 0.0), ang);

	// Skapar en okontrollerbar rotation
	// bortkommenterad för stunden	
	//viewMatrix = Mult(viewMatrix, rot);

	glBindVertexArray(tetraArray);
	glUniformMatrix4fv(glGetUniformLocation(shader, "modelviewMatrix"), 1, GL_TRUE, viewMatrix.m);
        
	if(marchOn == 1);
	{
        	March();
	}

        // VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, mTriBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mTris), mTris, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);							//OBS DETTA SKA ANVÄNDAS. ANNARS BLIR DET SÄMST!!!
	glVertexAttribPointer(glGetAttribLocation(shader, "in_Position"),
	3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindVertexArray(mTriArray);        
        //glDrawArrays(GL_TRIANGLES, 0, 39);// draw objectperspective

	glFlush();
	
	glutSwapBuffers();
}

void OnTimer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(5, &OnTimer, value);
}

void Reshape(int h, int w)
{
	glViewport(0, 0, w, h);
    	GLfloat ratio = (GLfloat) w / (GLfloat) h;
   	projectionMatrix = perspective(90, ratio, 0.1, 1000);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
}

//To love oneself is the beginning of a lifelong romance.
//Oscar Wilde


void Init()
{
	// GL inits
	glClearColor(0.1, 0.1, 0.3, 0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	shader = loadShaders("shader.vert", "shader.frag");
	
	glUseProgram(shader);
	projectionMatrix = perspective(90, 1.0, 0.1, 1000); // It would be silly to upload an uninitialized matrix
	glUniformMatrix4fv(glGetUniformLocation(shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(shader, "modelviewMatrix"), 1, GL_TRUE, viewMatrix.m);
	

	printError("init shader");
	

	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	//Normal buffer
	glGenVertexArrays(1, &normalArray);
	glBindVertexArray(normalArray);	

	// Allocate Vertex Buffer Objects	
	glGenBuffers(1, &vertexBufferObjID);
	glGenBuffers(1, &normalBuffer);
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);							//OBS DETTA SKA ANVÄNDAS. ANNARS BLIR DET SÄMST!!!
	glVertexAttribPointer(glGetAttribLocation(shader, "in_Position"),
	3, GL_FLOAT, GL_FALSE, 0, 0); 

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(glGetAttribLocation(shader, "in_Normal"),
	3, GL_FLOAT, GL_FALSE, 0, 0); 
	
	// Allocate Vertex Buffer Objects

	glGenVertexArrays(1, &tetraArray);
	glBindVertexArray(tetraArray);

	glGenVertexArrays(1, &tetraArray);
	glBindVertexArray(tetraArray);


	glGenBuffers(1, &tetraVertexBuffer);
	glGenBuffers(1, &tetraIndexBuffer);
	glGenBuffers(1, &tetraNormalBuffer);

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, tetraVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tetraVertices), tetraVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);	
	glVertexAttribPointer(glGetAttribLocation(shader, "in_Position"), 3, GL_FLOAT,
	GL_FALSE, 0, 0); 

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, tetraNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tetraNormals), tetraNormals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(glGetAttribLocation(shader, "in_Normal"), 3, GL_FLOAT,
	GL_FALSE, 0,0); 
	
	int i; 
	// Initiera variabler för flera tetror
	for (i = 0; i < NO_OBJECTS; i++)
	{
		
		tetras[i].mass = 0.00001;
		//tetras[i].pos = SetVector(-0.9 + 0.01 * (float)i, -0.9 + 0.008 * (float)i, -0.9 + 0.005 * (float)i);
		//tetras[i].vel = SetVector(0.02f,pow(-1.0f, i)*0.02f,pow(-1.0f, i)*0.001f); 
		tetras[i].pos = SetVector(-0.9 + 0.01 * (float)i, -0.5f, -0.5f);
		tetras[i].vel = SetVector(0.0f,0.0f,0.03f); 	
		
		//tetras[i].pos.y -= gravity * GLUT_ELAPSED_TIME * tetras[i].mass;		
		
	}
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tetraIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tetraIndicies), tetraIndicies, GL_STATIC_DRAW);

        /////////////////////////////////////////////////////////////////////////////////////////////
        // Detta kan bli väldigt fel 
        glGenVertexArrays(1, &mTriArray);
	glBindVertexArray(mTriArray);	

	// Allocate Vertex Buffer Objects	
	glGenBuffers(1, &mTriBuffer);
	
	//viewMatrix = lookAt(0,  3,  3,  0,  0,  0,  0,  1,  0);

	// Så vi kan rotera scenen 
	cam = SetVector(0, 0, 5);
    	point = SetVector(0, 0, 0);
    	zprInit(&viewMatrix, cam, point);
	
}



// Experience is just the name we give to our misstakes.
// Oscar Wilde

// Key listener
void Keyboardfunk(unsigned char key, int x, int y)
{	

	printf("success");
	if(key == 27)
	{
		printf("%s", "Marching cubes on");
		exit(-1);	
	}
	/*switch (key)
	{
		case 'o':
			printf("%s", "Marching cubes on");
			marchOn = 1;			
			break;

		case 'p':
			printf("%s", "Marching cubes off");
			marchOn = 0;
			break;

		case 0x1b:
			exit(0);
	}*/
}


void Idle()
{
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	
	glutInitContextVersion(3, 2);
	glutCreateWindow("Struta Hårt");

	glutDisplayFunc(Display);

	glutReshapeFunc(Reshape);
	glutIdleFunc(Idle);

	// Tangäntbård
	glutKeyboardFunc(Keyboardfunk);
	
	Init();
	
	glutMainLoop();
	exit(0);
}



