
#ifndef __MAP_MANAGER_H__
#define __MAP_MANAGER_H__

#include <map>
#include <string>
#include <iostream>
#include "Map.h"

class MapManager {
public:
	typedef std::map<std::string, Map*> map_type;

	~MapManager() {
		for( map_type::iterator it=maps.begin(); it!=maps.end(); ++it )
			delete it->second;
		maps.clear();
	}

	void addMap( std::string const& name, Map* map ) {

		// find map first
		map_type::iterator it = maps.find( name );

		// add map
		if( it == maps.end() )
			maps[ name ] = map;
		else {
			//maps[ name ] = map;
			std::cout << "ERROR: map <" << name << "> already loaded!" << std::endl;
		}
	}

	Map* getMap( std::string const& name ) {
		
		map_type::iterator it = maps.find( name );
		
		if( it != maps.end() )
			return it->second;

		std::cout << "ERROR: map <" << name << "> not found!" << std::endl;

		return 0;
	}

	bool exists( std::string const& name ) {
		
		if( maps.find( name ) != maps.end() )
			return true;

		return false;
	}

private:
	map_type maps;
};

#endif