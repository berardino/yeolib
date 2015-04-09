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
#include <yeo/renderer.hpp>

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
	DAE _dae;
	domCOLLADAProxy* _domProxy;
    const string& _path;


public:
    ColladaScene(const string& path) : _path(path) {
	}

    void Load(yeo::Device<>::Ref device) {
        _domProxy = _dae.open(_path);
        // get vector containing the geometry objects
        std::vector<domGeometry*> geometries = _dae.getDatabase()->typeLookup<domGeometry>();
        std::map<std::string,yeo::Buffer::Ref> buffers;
        for(auto geometry : geometries) {
            auto mesh = geometry->getMesh();
            auto sources = mesh->getSource_array();
            for(int i = 0; i < sources.getCount(); i++) {
                auto source = sources[i];
                auto floats = source->getFloat_array();
                unsigned int numVertex = floats->getCount();
                unsigned int size =  numVertex * sizeof(float);
                auto buffer = device->CreateVertexBuffer();
                buffer->Write(size, floats->getValue().getRawData());
                buffers.insert(std::pair<string,Buffer::Ref>(source->getId(), buffer));

                auto technique = source->getTechnique_common();
                auto accessor = technique->getAccessor();
                auto offset = accessor->getOffset();
                auto layout = device->CreateInputLayout()->
                    Element()
                        .Size(accessor->getStride())
                        .Type(InputElementDesc::Type::FLOAT)
                        .Stride((numVertex/accessor->getCount()) - accessor->getStride())
                    .Add()
                .Build();

                auto vertexInput = mesh->getVertices()->getInput_array();


            }
        }

        std::cout << "File contains " << geometries.size() << " geometry objects." << std::endl;
        

    }

	~ColladaScene() {
	}

};

YEO_API shared_ptr<Scene> YeoLoadScene(const string& path) {
	return shared_ptr<Scene>(new ColladaScene(path));
}
