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

#include <yeo/renderer.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>

#define YEO_DLL_EXPORTS

using namespace yeo;
using namespace std;

static const std::map<Buffer::Usage, GLenum> GL_BUFFER_USAGE_MAP = { {
    Buffer::Usage::STATIC_DRAW, GL_STATIC_DRAW } };

static const std::map<InputElementDesc::Type, GLenum> GL_TYPE_MAP = {
    { InputElementDesc::Type::BYTE, GL_BYTE },
    { InputElementDesc::Type::UBYTE, GL_UNSIGNED_BYTE },
    { InputElementDesc::Type::SHORT, GL_SHORT },
    { InputElementDesc::Type::USHORT, GL_UNSIGNED_SHORT },
    { InputElementDesc::Type::INT, GL_INT },
    { InputElementDesc::Type::UINT, GL_UNSIGNED_INT },
    { InputElementDesc::Type::FLOAT, GL_FLOAT },
    { InputElementDesc::Type::HFLOAT, GL_HALF_FLOAT },
    { InputElementDesc::Type::DOUBLE, GL_DOUBLE }
};

static const std::map<DrawingMode, GLenum> GL_PRIMITIVE_MAP = {
    { DrawingMode::POINTS, GL_POINTS },
    { DrawingMode::LINE_STRIP, GL_LINE_STRIP },
    { DrawingMode::LINE_LOOP, GL_LINE_LOOP },
    { DrawingMode::LINES, GL_LINES },
    { DrawingMode::LINE_STRIP_ADJACENCY, GL_LINE_STRIP_ADJACENCY },
    { DrawingMode::LINES_ADJACENCY, GL_LINES_ADJACENCY },
    { DrawingMode::TRIANGLE_STRIP, GL_TRIANGLE_STRIP },
    { DrawingMode::TRIANGLE_FAN, GL_TRIANGLE_FAN },
    { DrawingMode::TRIANGLES, GL_TRIANGLES },
    { DrawingMode::TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLE_STRIP_ADJACENCY },
    { DrawingMode::TRIANGLES_ADJACENCY, GL_TRIANGLES_ADJACENCY },
    { DrawingMode::PATCHES, GL_PATCHES }
};

static const std::map<ShaderType, GLenum> GL_SHADER_MAP = {
    { ShaderType::COMPUTE, GL_COMPUTE_SHADER },
    { ShaderType::VERTEX, GL_VERTEX_SHADER },
    { ShaderType::TESS_CONTROL, GL_TESS_CONTROL_SHADER },
    { ShaderType::TESS_EVALUATION, GL_TESS_EVALUATION_SHADER },
    { ShaderType::GEOMETRY, GL_GEOMETRY_SHADER },
    { ShaderType::FRAGMENT, GL_FRAGMENT_SHADER }
};

class OpenGLBuffer: public Buffer {
private:
    GLuint _vbo;
    GLenum _target;

public:
    OpenGLBuffer(GLenum target = GL_ARRAY_BUFFER) :
    _target(target) {
        glGenBuffers(1, &_vbo);
    }

    ~OpenGLBuffer() {
        glDeleteBuffers(1, &_vbo);
    }

    void Write(ptrdiff_t size, const void* data, Usage usage) {
        Bind();
        GLenum glUsage = GL_BUFFER_USAGE_MAP.find(usage)->second;
        glBufferData(_target, size, data, glUsage);
        glBindBuffer(_target, 0);
    }

    void Bind() {
        glBindBuffer(_target, _vbo);
    }
};

class OpenGLVertexBuffer: public OpenGLBuffer {
public:
    OpenGLVertexBuffer() :
    OpenGLBuffer(GL_ARRAY_BUFFER) {
    }
};

class OpenGLIndexBuffer: public OpenGLBuffer {
public:
    OpenGLIndexBuffer() :
    OpenGLBuffer(GL_ELEMENT_ARRAY_BUFFER) {
    }
};

class OpenGLInputElementDesc: public InputElementDesc {
protected:
    GLuint _index;
    GLint _size;
    GLenum _type;
    GLsizei _stride;
    const GLvoid * _pointer;
public:
    OpenGLInputElementDesc(GLuint index, GLint size, GLenum type,
                           GLsizei stride, const GLvoid * pointer) :
    _index(index), _size(size), _type(type), _stride(stride), _pointer(
                                                                       pointer) {
    }

    virtual void Apply() const {
        glEnableVertexAttribArray(_index);
    }

    void UnApply() const {
        glDisableVertexAttribArray(_index);
    }
};

class OpenGLFloatInputElementDesc: public OpenGLInputElementDesc {
private:
    GLboolean _normalized;
public:
    OpenGLFloatInputElementDesc(GLuint index, GLint size, GLenum type,
                                GLboolean normalized, GLsizei stride, const GLvoid * pointer) :
    OpenGLInputElementDesc(index, size, type, stride, pointer) {
        _normalized = normalized;
    }

    void Apply() const {
        OpenGLInputElementDesc::Apply();
        glVertexAttribPointer(_index, _size, _type, _normalized, _stride,
                              _pointer);
    }
};

class OpenGLIntegerInputElementDesc: public OpenGLInputElementDesc {
public:
    OpenGLIntegerInputElementDesc(GLuint index, GLint size, GLenum type,
                                  GLsizei stride, const GLvoid * pointer) :
    OpenGLInputElementDesc(index, size, type, stride, pointer) {
    }

    void Apply() const {
        OpenGLInputElementDesc::Apply();
        glVertexAttribIPointer(_index, _size, _type, _stride, _pointer);
    }
};

class OpenGLDoubleInputElementDesc: public OpenGLInputElementDesc {
public:
    OpenGLDoubleInputElementDesc(GLuint index, GLint size, GLenum type,
                                 GLsizei stride, const GLvoid * pointer) :
    OpenGLInputElementDesc(index, size, type, stride, pointer) {
    }

    void Apply() const {
        OpenGLInputElementDesc::Apply();
        glVertexAttribLPointer(_index, _size, _type, _stride, _pointer);
    }
};

class OpenGLInputElementDescBuilder: public InputElementDesc::Builder<
OpenGLInputElementDesc> {
private:
    GLuint _index;
    GLint _size;
    InputElementDesc::Type _type;
    GLsizei _stride;
    const GLvoid * _pointer;
    GLboolean _normalized;
private:
    InputElementDesc::Builder<>& self() {
        return (*(InputElementDesc::Builder<>*) this);
    }
public:
    ~OpenGLInputElementDescBuilder() {
    }

    InputElementDesc::Builder<>& Index(unsigned int value) {
        _index = value;
        return self();
    }

    InputElementDesc::Builder<>& Size(unsigned int value) {
        _size = value;
        return self();
    }

    InputElementDesc::Builder<>& Type(InputElementDesc::Type value) {
        _type = value;
        return self();
    }

    InputElementDesc::Builder<>& Stride(int value) {
        _stride = value;
        return self();
    }

    InputElementDesc::Builder<>& Pointer(void* value) {
        _pointer = value;
        return self();
    }

    InputElementDesc::Builder<>& Normalized(bool value) {
        _normalized = value ? GL_TRUE : GL_FALSE;
        return self();
    }

    const OpenGLInputElementDesc* Build() {
        GLenum glType = GL_TYPE_MAP.find(_type)->second;
        switch (_type) {
                //Integer
            case InputElementDesc::Type::BYTE:
            case InputElementDesc::Type::UBYTE:
            case InputElementDesc::Type::SHORT:
            case InputElementDesc::Type::USHORT:
            case InputElementDesc::Type::INT:
            case InputElementDesc::Type::UINT:
                return new OpenGLIntegerInputElementDesc(_index, _size, glType,
                                                         _stride, _pointer);
                break;
                //Float
            case InputElementDesc::Type::FLOAT:
            case InputElementDesc::Type::HFLOAT:
                return new OpenGLFloatInputElementDesc(_index, _size, glType,
                                                       _normalized, _stride, _pointer);
                break;
                //Double
            case InputElementDesc::Type::DOUBLE:
                return new OpenGLDoubleInputElementDesc(_index, _size, glType,
                                                        _stride, _pointer);
                break;

        }
    }

};

class OpenGLInputLayout: public InputLayout {
private:
    GLuint _vao;
    const std::vector<std::shared_ptr<const OpenGLInputElementDesc>> _elementsDescs;
public:
    OpenGLInputLayout(
                      const std::vector<std::shared_ptr<const OpenGLInputElementDesc>>& elementsDescs) :
    _elementsDescs(elementsDescs) {
        glGenVertexArrays(1, &_vao);
        Bind();
    }

    ~OpenGLInputLayout() {
        glDeleteVertexArrays(1, &_vao);
    }

    void Bind() {
        glBindVertexArray(_vao);
    }

    void Apply() {
        for (auto elementsDesc : _elementsDescs) {
            elementsDesc->Apply();
        }

    }

    void UnApply() {
        for (auto elementsDesc : _elementsDescs) {
            elementsDesc->UnApply();
        }
    }

};

class OpenGLInputLayoutBuilder: public InputLayout::Builder {
private:
    std::vector<std::shared_ptr<const OpenGLInputElementDesc>> _inputElements;
    std::unique_ptr<InputLayout::ElementBuilder<>> _current;
private:
    InputLayout::Builder& self() {
        return (*(InputLayout::Builder*) this);
    }
protected:
    InputLayout::Builder& Add(const OpenGLInputElementDesc* desc) {
        auto p = std::shared_ptr<const OpenGLInputElementDesc>(desc);
        _inputElements.push_back(p);
        return self();
    }
public:
    ~OpenGLInputLayoutBuilder() {
    }

    InputLayout::ElementBuilder<>& Element() {
        InputLayout::ElementBuilder<> *p =
        (InputLayout::ElementBuilder<>*) new OpenGLElementBuilder(this);
        _current = std::unique_ptr<InputLayout::ElementBuilder<>>(p);
        return (*p);
    }

    InputLayout::Ref Build() {
        return shared_ptr<InputLayout>(new OpenGLInputLayout(_inputElements));
    }

    class OpenGLElementBuilder: public virtual InputLayout::ElementBuilder<> {
    private:
        OpenGLInputLayoutBuilder* layoutBuilder;
        OpenGLInputElementDescBuilder elementBuilder;

        ElementBuilder<>& self() {
            return (*(ElementBuilder<>*) this);
        }

    public:
        OpenGLElementBuilder(OpenGLInputLayoutBuilder* layoutBuilder) {
            this->layoutBuilder = layoutBuilder;
        }
        ~OpenGLElementBuilder() {
        }

        ElementBuilder<>& Index(unsigned int value) {
            elementBuilder.Index(value);
            return self();
        }

        ElementBuilder<>& Size(unsigned int value) {
            elementBuilder.Size(value);
            return self();
        }

        ElementBuilder<>& Type(InputElementDesc::Type value) {
            elementBuilder.Type(value);
            return self();
        }

        ElementBuilder<>& Stride(int value) {
            elementBuilder.Stride(value);
            return self();
        }

        ElementBuilder<>& Pointer(void* value) {
            elementBuilder.Pointer(value);
            return self();
        }

        ElementBuilder<>& Normalized(bool value) {
            elementBuilder.Normalized(value);
            return self();
        }

        InputLayout::Builder& Add() {
            return layoutBuilder->Add(elementBuilder.Build());
        }

    };
};

class OpenGLShader : public Shader {
protected:
    GLuint shaderId;
public:
    OpenGLShader(const std::string& shader, GLenum shaderType) {
        shaderId = glCreateShader(shaderType);
        const char * sourcePointer = shader.c_str();
        glShaderSource(shaderId, 1, &sourcePointer, NULL);
        glCompileShader(shaderId);
    }

    ~OpenGLShader() {
        Delete();
    }

    void Delete() {
        if (glIsShader(shaderId)) {
            glDeleteShader(shaderId);
        }
    }

    GLuint GetId() {
        return shaderId;
    }
};

class OpenGLProgram : public Program {
protected:
    GLuint programId;
public:
    OpenGLProgram() {
        programId = glCreateProgram();
    }

    ~OpenGLProgram() {
        glDeleteProgram(programId);
    }

    void Attach(Shader::Ref shader){
        auto glShader = std::dynamic_pointer_cast<OpenGLShader>(shader);
        glAttachShader(programId, glShader->GetId());
    }

    void Detach(Shader::Ref shader){
        auto glShader = std::dynamic_pointer_cast<OpenGLShader>(shader);
        glDetachShader(programId, glShader->GetId());
    }

    void Link() {
        glLinkProgram(programId);
        GLint attachedShaders;
        glGetProgramiv(programId, GL_ATTACHED_SHADERS, &attachedShaders);
        if (attachedShaders == 0) return;

        GLsizei count;
        GLuint shaders[attachedShaders];
        glGetAttachedShaders(programId, attachedShaders, &count, shaders);

        for (int i=0;i<count;i++){
            glDetachShader(programId, shaders[i]);
            glDeleteShader(shaders[i]);
        }
    }

    void Use() {
        glUseProgram(programId);
    }

};

class OpenGLDevice: public Device<OpenGLBuffer, OpenGLInputLayout,
OpenGLInputElementDesc> {
private:
    GLFWwindow* window;

public:
    OpenGLDevice(GLFWwindow* window) {
        this->window = window;
    }

    ~OpenGLDevice() {
        glfwDestroyWindow(window);
    }

    void MakeCurrent() {
        glfwMakeContextCurrent(window);
        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if (GLEW_OK != err) {
            fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        }
        glGetError();
    }

    void Present() {
        glfwSwapBuffers(window);
    }

    int ShouldClose() {
        return glfwWindowShouldClose(window);
    }

    void SetViewport(int x, int y, int width, int height) {
        glViewport(x, y, width, height);
    }

    void GetFramebufferSize(int* width, int* height) {
        glfwGetFramebufferSize(window, width, height);
    }

    void Clear() {
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void SetProjectionMatrix(const float4x4& matrix) {
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(matrix.memptr());
    }

    void SetViewMatrix(const float4x4& matrix) {
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(matrix.memptr());
    }

    string GetVersion() {
        return reinterpret_cast<char const *>(glGetString(GL_VERSION));
    }

    VertexBuffer::Ref CreateVertexBuffer() {
        auto p = (VertexBuffer*) new OpenGLVertexBuffer();
        return shared_ptr<VertexBuffer>(p);
    }

    IndexBuffer::Ref CreateIndexBuffer() {
        auto p = (IndexBuffer*) new OpenGLIndexBuffer();
        return shared_ptr<IndexBuffer>(p);
    }

    std::unique_ptr<InputLayout::Builder> CreateInputLayout() {
        auto p = (InputLayout::Builder*) new OpenGLInputLayoutBuilder();
        return std::unique_ptr<InputLayout::Builder>(p);
    }

    void Draw(DrawingMode mode, unsigned int first, unsigned int count) {
        GLenum _mode = GL_PRIMITIVE_MAP.find(mode)->second;
        glDrawArrays(_mode, first, count);
    }

    void DrawIndexed(DrawingMode mode, unsigned int count) {
        GLenum _mode = GL_PRIMITIVE_MAP.find(mode)->second;
        glDrawElements(_mode, count, GL_UNSIGNED_INT, 0);
    }

    Program::Ref CreateProgram() {
        return std::shared_ptr<Program>(new OpenGLProgram());
    }

    Shader::Ref CreateShader(const std::string& shader, ShaderType type) {
        GLenum shaderType = GL_SHADER_MAP.find(type)->second;
        return std::shared_ptr<Shader>(new OpenGLShader(shader, shaderType));
    }

};

class OpenGLRenderer: public Renderer<OpenGLDevice> {
public:
    ~OpenGLRenderer() {
        glfwTerminate();
    }

    Device<>::Ref CreateDevice() {
        GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL,
                                              NULL);
        Device<>* p = (Device<>*) new OpenGLDevice(window);
        return shared_ptr<Device<>>(p);
    }

    void PollEvents() {
        glfwPollEvents();
    }

    void WaitEvents() {
        glfwWaitEvents();
    }

};

YEO_API shared_ptr<Renderer<>> YeoCreateRenderer(void) {
    if (!glfwInit()) {
        return shared_ptr<Renderer<>>();
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    Renderer<>* p = (Renderer<>*) new OpenGLRenderer();
    return shared_ptr<Renderer<>>(p);
}

