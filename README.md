/*
	miko raytracer
	written by markus gnauck (dec2004-mar2005)
	markus@unik.de

	some features:
	- primitives + meshes
	- bounding volumes
	- shadow caching
	- procedural + image textures
	- normal mapping
	- spotlight, area light, skylight
	- monte carlo GI
	- XML scene format support (via TinyXML)

	known bugs/missing import:
	- resolution change influences viewport
	- import of duskshroud scene -> missing hands of figure???
	- no area light import
	- no skylight import
	- no procedural texture import
	- bump amount factor is not imported

	todo:
	- glossy reflections (use gloss factor from material for dimension)
	- reimplement antialiasing
	- octree
	- more clever sampling for gi
	- real aabb test :)
*/