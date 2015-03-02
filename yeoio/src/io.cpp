/**
 This file is part of YeoLibrary.
 
 YeoLibrary is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, version 3.
 
 YeoLibrary is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with YeoLibrary.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <yeo/io.hpp>

#include <FreeImage.h>
#include <dae.h>
#include <dom/domConstants.h>
#include <dom/domCOLLADA.h>
#include <dom/domGeometry.h>
#include <iostream>
#define YEO_DLL_EXPORTS

using namespace yeo;
using namespace std;
using namespace ColladaDOM150;

class ColladaScene: public Scene {
private:
	DAE dae;
	domCOLLADAProxy* domProxy;

public:
	ColladaScene(const string& path) {
		domProxy = dae.open(path);

		// get vector containing the geometry objects
		std::vector<domGeometry*> geometries = dae.getDatabase()->typeLookup<
				domGeometry>();

		std::cout << "File contains " << geometries.size()
				<< " geometry objects." << std::endl;

	}

	~ColladaScene() {
	}

};

YEO_API shared_ptr<Scene> YeoLoadScene(const string& path) {
	return shared_ptr<Scene>(new ColladaScene(path));
}
