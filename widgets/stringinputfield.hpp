#include "widget.hpp"

#define Characters_Lower 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'
#define Characters_Upper 'Q','W','E','R','T','Y','U','I','O','P','A','S','D','F','G','H','J','K','L','Z','X','C','V','B','N','M'
#define Characters_Digits '1','2','3','4','5','6','7','8','9','0'
#define Characters_Special '`','~','!','@','#','$','%','^','&','*','(',')','-','_','=','+','[','{',']','}','\\','|',';',':','\'','"',',','<','.','>','/','?'
#define Characters_All Characters_Lower, Characters_Upper, Characters_Digits, Characters_Special

class StringInputField : public Widget
{
public:
	std::string string = "";

	std::function<void()> OnInsert;

	Engine::RGBA unselectedRGBA, selectedRGBA;

private:
	uint16_t currentChar = 0;
	uint16_t maxChars;

	void InternalInsert()
	{
		if (selected)
		{
			if (Engine::Input::Is(Engine::Input::K::backspace) || Engine::Input::Is(Engine::Input::K::delete_))
			{
				if (currentChar == 0)
					return;

				currentChar--;
				string.pop_back();

				obj.textures[0].t[0][currentChar].c = ' ';
			}
			else if (currentChar == maxChars)
				return;
			else
			{
				obj.textures[0].t[0][currentChar].c = Engine::Input::input.at(0);
				string.push_back(Engine::Input::input.at(0));
				currentChar++;
			}
			if (OnInsert)
				OnInsert();
		}
	}

public:
	virtual void Select()
	{
		for (size_t i = 0; i < obj.textures.size(); i++)
			obj.textures[i].SetForeground(selectedRGBA);
		selected = true;
	}

	virtual void Deselect()
	{
		for (size_t i = 0; i < obj.textures.size(); i++)
			obj.textures[i].SetForeground(unselectedRGBA);
		selected = false;
	}

	void ChangeValue(std::string newString)
	{
		string.resize(newString.length() > maxChars ? maxChars : newString.length());
		for (size_t x = 0; x < maxChars; x++)
		{
			if (x < string.length())
			{
				string[x] = newString[x];
				obj.textures[0].t[0][x].c = newString[x];
			}
			else
				obj.textures[0].t[0][x].c = ' ';
		}
		currentChar = string.length();
	}

	StringInputField(Engine::Layer* layer = 0,
		std::function<void()> OnInsert = 0,
		std::vector<char> allowedCharacters = {},
		Engine::Point pos = { 0, 0 },
		const std::string& text = "Value = ",
		unsigned int maxChars = 8,
		const std::string& defaultString = "String",
		bool withFrame = false,
		Engine::RGBA unselectedRGBA = { 150, 150, 150, 255 },
		Engine::RGBA selectedRGBA = { 255, 255, 255, 255 })
		: OnInsert(OnInsert), maxChars(maxChars), unselectedRGBA(unselectedRGBA), selectedRGBA(selectedRGBA), string(defaultString)
	{
		obj.pos = pos;

		// Texture
		if (withFrame)
		{
			obj.textures.resize(10);
			// input
			obj.textures[0].Rectangle(Engine::UPoint(maxChars, 1), Engine::CellA(' ', unselectedRGBA), Engine::Point(1 + text.length(), 1));
			for (size_t i = 0; i < string.length() && i < obj.textures[0].t[0].size(); i++)
			{
				obj.textures[0].t[0][i].c = string[i];
				currentChar++;
			}
			// text
			obj.textures[1].Write({text}, unselectedRGBA, Engine::RGBA(0, 0, 0, 0), Engine::Point(1, 1));
			// up-left corner
			obj.textures[2].Simple(Engine::UPoint(1, 1), Engine::CellA('#', unselectedRGBA), Engine::Point(0, 0));
			// up-right corner
			obj.textures[3].Simple(Engine::UPoint(1, 1), Engine::CellA('#', unselectedRGBA), Engine::Point(1 + text.length() + maxChars, 0));
			// bottom-left corner
			obj.textures[4].Simple(Engine::UPoint(1, 1), Engine::CellA('#', unselectedRGBA), Engine::Point(0, 2));
			// bottom-right corner
			obj.textures[5].Simple(Engine::UPoint(1, 1), Engine::CellA('#', unselectedRGBA), Engine::Point(1 + text.length() + maxChars, 2));
			// up frame
			obj.textures[6].Simple(Engine::UPoint(text.length() + maxChars, 1), Engine::CellA('-', unselectedRGBA), Engine::Point(1, 0));
			// left frame
			obj.textures[7].Simple(Engine::UPoint(1, 1), Engine::CellA('|', unselectedRGBA), Engine::Point(0, 1));
			// bottom frame
			obj.textures[8].Simple(Engine::UPoint(text.length() + maxChars, 1), Engine::CellA('-', unselectedRGBA), Engine::Point(1, 2));
			// right frame
			obj.textures[9].Simple(Engine::UPoint(1, 1), Engine::CellA('|', unselectedRGBA), Engine::Point(1 + text.length() + maxChars, 1));
		}
		else
		{
			obj.textures.resize(2);
			// input
			obj.textures[0].Rectangle(Engine::UPoint(maxChars, 1), Engine::CellA(' ', unselectedRGBA), Engine::Point(1 + text.length(), 1));
			for (size_t i = 0; i < string.length() && i < obj.textures[0].t[0].size(); i++)
			{
				obj.textures[0].t[0][i].c = string[i];
				currentChar++;
			}
			// text
			obj.textures[1].Write({text}, unselectedRGBA, Engine::RGBA(0, 0, 0, 0), Engine::Point(1, 1));
		}
		
		// Input handlers
		for (size_t i = 0; i < allowedCharacters.size(); i++)
			Engine::Input::RegisterHandler({allowedCharacters[i]}, std::bind(&StringInputField::InternalInsert, this), true);
		Engine::Input::RegisterHandler(Engine::Input::K::delete_, std::bind(&StringInputField::InternalInsert, this), true);
		Engine::Input::RegisterHandler(Engine::Input::K::backspace, std::bind(&StringInputField::InternalInsert, this), true);
		
		// Add object
		layer->AddObject(&obj);
	}
};