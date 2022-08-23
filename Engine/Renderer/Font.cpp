#include "Font.h" 
#include "Core/Logger.h"
#include <SDL_ttf.h> 

namespace neu{
	Font::Font(const std::string& filename, int fontSize){
		
		Load(filename, fontSize);
	}

	Font::~Font(){

		if (m_ttfFont){
			TTF_CloseFont(m_ttfFont);
		}
	}

	bool Font::Create(std::string filename, ...){
		va_list args;
		va_start(args, filename);
		int fontSize = va_arg(args, int);

		va_end(args);

		return Load(filename, fontSize);
	}

	bool Font::Load(const std::string& filename, int fontSize){

		m_ttfFont = TTF_OpenFont(filename.c_str(), fontSize);
		if (m_ttfFont == nullptr){
			LOG("could not open font %s", filename.c_str());
			return false;
		}

		return true;
	}
}

