#define DEBUG_DRAW_IMPLEMENTATION

#include "debug_draw.hpp"


// ========================================================
// Minimal shaders we need for the debug primitives:
// ========================================================

static const char * linePointVertShaderSrc = "\n"
"#version 150\n"
"\n"
"in vec3 in_Position;\n"
"in vec4 in_ColorPointSize;\n"
"\n"
"out vec4 v_Color;\n"
"uniform mat4 u_MvpMatrix;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position  = u_MvpMatrix * vec4(in_Position, 1.0);\n"
"    gl_PointSize = in_ColorPointSize.w;\n"
"    v_Color      = vec4(in_ColorPointSize.xyz, 1.0);\n"
"}\n";

// ------------------------------------------------------------------

static const char * linePointFragShaderSrc = "\n"
"#version 150\n"
"\n"
"in vec4 v_Color;\n"
"out vec4 out_FragColor;\n"
"\n"
"void main()\n"
"{\n"
"    out_FragColor = v_Color;\n"
"}\n";

// ------------------------------------------------------------------

static const char * textVertShaderSrc = "\n"
"#version 150\n"
"\n"
"in vec2 in_Position;\n"
"in vec2 in_TexCoords;\n"
"in vec3 in_Color;\n"
"\n"
"uniform vec2 u_screenDimensions;\n"
"\n"
"out vec2 v_TexCoords;\n"
"out vec4 v_Color;\n"
"\n"
"void main()\n"
"{\n"
"    // Map to normalized clip coordinates:\n"
"    float x = ((2.0 * (in_Position.x - 0.5)) / u_screenDimensions.x) - 1.0;\n"
"    float y = 1.0 - ((2.0 * (in_Position.y - 0.5)) / u_screenDimensions.y);\n"
"\n"
"    gl_Position = vec4(x, y, 0.0, 1.0);\n"
"    v_TexCoords = in_TexCoords;\n"
"    v_Color     = vec4(in_Color, 1.0);\n"
"}\n";

// ------------------------------------------------------------------

static const char * textFragShaderSrc = "\n"
"#version 150\n"
"\n"
"in vec2 v_TexCoords;\n"
"in vec4 v_Color;\n"
"\n"
"uniform sampler2D u_glyphTexture;\n"
"out vec4 out_FragColor;\n"
"\n"
"void main()\n"
"{\n"
"    out_FragColor = v_Color;\n"
"    out_FragColor.a = texture(u_glyphTexture, v_TexCoords).r;\n"
"}\n";


// ========================================================
// Debug Draw RenderInterface for Core OpenGL:
// ========================================================


class DDRenderInterfaceCoreGL : public dd::RenderInterface {
	void beginDraw() {

	}
	void endDraw() {

	}

	void drawPointList(const dd::DrawVertex * points, int count, bool depthEnabled) {

	}
	void drawLineList(const dd::DrawVertex * lines, int count, bool depthEnabled) {

	}
	void drawGlyphList(const dd::DrawVertex * glyphs, int count, dd::GlyphTextureHandle glyphTex) {

	}

	dd::GlyphTextureHandle createGlyphTexture(int width, int height, const void * pixels) {
		return NULL;
	}
	void destroyGlyphTexture(dd::GlyphTextureHandle glyphTex) {

	}

};