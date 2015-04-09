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
#ifndef __YEO_RENDERER_H__
#define __YEO_RENDERER_H__
#include <yeo/math.hpp>
#include <memory>
#include <string>

#if defined _WIN32 || defined __CYGWIN__
#define YEO_HELPER_DLL_IMPORT __declspec(dllimport)
#define YEO_HELPER_DLL_EXPORT __declspec(dllexport)
#define YEO_HELPER_DLL_LOCAL
#else
#if __GNUC__ >= 4
#define YEO_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
#define YEO_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
#define YEO_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define YEO_HELPER_DLL_IMPORT
#define YEO_HELPER_DLL_EXPORT
#define YEO_HELPER_DLL_LOCAL
#endif
#endif

#ifdef YEO_DLL
#ifdef YEO_DLL_EXPORTS
#define YEO_API YEO_HELPER_DLL_EXPORT
#else
#define YEO_API YEO_HELPER_DLL_IMPORT
#endif
#define YEO_LOCAL YEO_HELPER_DLL_LOCAL
#else
#define YEO_API
#define YEO_LOCAL
#endif // YEO_DLL

namespace yeo {

class Buffer {
public:
	using Ref = std::shared_ptr<Buffer>;
	enum Usage {
		STATIC_DRAW
	};
	virtual ~Buffer() {
	}
	;
	virtual void Write(unsigned int size, const void* data, Usage usage =
			STATIC_DRAW) = 0;
	virtual void Bind() = 0;
};

class VertexBuffer: public Buffer {
public:
	using Ref = std::shared_ptr<VertexBuffer>;
	virtual ~VertexBuffer() {
	}
	;
};

class IndexBuffer: public Buffer {
public:
	using Ref = std::shared_ptr<IndexBuffer>;
	virtual ~IndexBuffer() {
	}
	;
};

class InputElementDesc {
public:
	enum Type {
		//Integer
		BYTE,
		UBYTE,
		SHORT,
		USHORT,
		INT,
		UINT,

		//Float
		FLOAT,
		HFLOAT,

		//Double
		DOUBLE
	};
	using Ref = std::shared_ptr<InputElementDesc>;
	virtual ~InputElementDesc() {
	};

	template<typename T = InputElementDesc>
	class Builder {
	public:
		virtual ~Builder() {
		}
		;
		virtual Builder<>& Index(unsigned int value) = 0;
		virtual Builder<>& Size(unsigned int value) = 0;
		virtual Builder<>& Type(InputElementDesc::Type value) = 0;
		virtual Builder<>& Stride(int value) = 0;
		virtual Builder<>& Pointer(void* value) = 0;
		virtual Builder<>& Normalized(bool value) = 0;
		virtual const T* Build() = 0;
	};

};

class InputLayout {
public:
	using Ref = std::shared_ptr<InputLayout>;
	virtual ~InputLayout() {
	}
	;
	virtual void Apply() = 0;
	virtual void UnApply() = 0;

	template<typename T = InputElementDesc> class ElementBuilder;

	class Builder {
	public:
		virtual ~Builder() {
		}
		;
		virtual ElementBuilder<>& Element() = 0;
		virtual Ref Build() = 0;
	};

	template<typename T>
	class ElementBuilder {
	public:
		virtual ~ElementBuilder() {
		}
		;
		virtual ElementBuilder<>& Index(unsigned int value) = 0;
		virtual ElementBuilder<>& Size(unsigned int value) = 0;
		virtual ElementBuilder<>& Type(InputElementDesc::Type value) = 0;
		virtual ElementBuilder<>& Stride(int value) = 0;
		virtual ElementBuilder<>& Pointer(void* value) = 0;
		virtual ElementBuilder<>& Normalized(bool value) = 0;
		virtual InputLayout::Builder& Add() = 0;
	};

};

enum ShaderType {
	COMPUTE,
    VERTEX,
    TESS_CONTROL,
    TESS_EVALUATION,
    GEOMETRY,
    FRAGMENT
};

class Shader {
public:
    using Ref = std::shared_ptr<Shader>;
    virtual ~Shader() {};
};

class Program {
public:
    using Ref = std::shared_ptr<Program>;
    virtual ~Program() {};
    virtual void Attach(Shader::Ref shader) = 0;
    virtual void Detach(Shader::Ref shader) = 0;
    virtual void Link() = 0;
    virtual void Use() = 0;
};

enum DrawingMode {
    POINTS,
    LINE_STRIP,
    LINE_LOOP,
    LINES,
    LINE_STRIP_ADJACENCY,
    LINES_ADJACENCY,
    TRIANGLE_STRIP,
    TRIANGLE_FAN,
    TRIANGLES,
    TRIANGLE_STRIP_ADJACENCY,
    TRIANGLES_ADJACENCY,
    PATCHES
};


template<typename B = Buffer, typename I = InputLayout,
		typename E = InputElementDesc>
class Device {
public:
	using Ref = std::shared_ptr<Device<B,I,E>>;

	virtual ~Device() {};
	virtual void MakeCurrent() = 0;
	virtual void Present() = 0;
	virtual int ShouldClose() = 0;
	virtual void Clear() = 0;
	virtual std::string GetVersion() = 0;
	virtual void SetViewport(int x, int y, int width, int height) = 0;
	virtual void GetFramebufferSize(int* width, int* height) = 0;
	virtual void SetProjectionMatrix(const float4x4& matrix) = 0;
	virtual void SetViewMatrix(const float4x4& matrix) = 0;
	virtual VertexBuffer::Ref CreateVertexBuffer() = 0;
	virtual IndexBuffer::Ref CreateIndexBuffer() = 0;
	virtual std::unique_ptr<InputLayout::Builder> CreateInputLayout() = 0;
	virtual void Draw(DrawingMode mode, unsigned int first, unsigned int count) = 0;
	virtual void DrawIndexed(DrawingMode mode, unsigned int count) = 0;
    virtual Program::Ref CreateProgram() = 0;
    virtual Shader::Ref CreateShader(const std::string& shader, ShaderType type) = 0;
};

template<typename D = Device<>>
class Renderer {
public:
	using Ref = std::shared_ptr<Renderer<D>>;

	virtual ~Renderer() {
	}
	;
	virtual Device<>::Ref CreateDevice() = 0;
	virtual void PollEvents() = 0;
	virtual void WaitEvents() = 0;
};
};

YEO_API yeo::Renderer<>::Ref YeoCreateRenderer(void);

#endif //__YEO_RENDERER_H__
