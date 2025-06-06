#pragma once
#include "Renderer.h"

class Texture {
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP; 
public:
	Texture(const std::string& path);
	~Texture();
	void Bind(unsigned int slot) const;
	void Unbind() const;

	void SetFilePath(const std::string& path);

	inline const char* GetFilePath() const { return m_FilePath.c_str(); }
	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline unsigned int GetRendererID() const { return m_RendererID; }
	inline unsigned char* GetLocalBuffer() const { return m_LocalBuffer; }
};