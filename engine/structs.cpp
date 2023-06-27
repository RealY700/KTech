#include "engine.hpp"

Engine::RGB::RGB(unsigned char red, unsigned char green, unsigned char blue) : r(red), g(green), b(blue) {}

Engine::RGBA::RGBA(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) : r(red), g(green), b(blue), a(alpha) {}

Engine::Point::Point(long x, long y) : x(x), y(y) {}

Engine::UPoint::UPoint(unsigned long x, unsigned long y) : x(x), y(y) {}

Engine::Cell::Cell(char32_t character, RGB foreRGB, RGB backRGB) : character(character), frgb(foreRGB), brgb(backRGB) {}

Engine::CellA::CellA(char32_t character, RGBA foreRGBA, RGBA backRGBA) : character(character), frgba(foreRGBA), brgba(backRGBA) {}

Engine::TimePoint::TimePoint()
{
	chornoTimePoint = std::chrono::high_resolution_clock::now();
}
void Engine::TimePoint::SetToNow()
{
	chornoTimePoint = std::chrono::high_resolution_clock::now();
}
long Engine::TimePoint::Seconds()
{
	return std::chrono::duration_cast<std::chrono::seconds>(chornoTimePoint - engineStartTP.chornoTimePoint).count();
}
long Engine::TimePoint::Milliseconds()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(chornoTimePoint - engineStartTP.chornoTimePoint).count();
}
long Engine::TimePoint::Microseconds()
{
	return std::chrono::duration_cast<std::chrono::microseconds>(chornoTimePoint - engineStartTP.chornoTimePoint).count();
}
long Engine::TimePoint::Nanoseconds()
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(chornoTimePoint - engineStartTP.chornoTimePoint).count();
}

// Engine::Texture::Texture() {};
void Engine::Texture::Rectangle(UPoint size, CellA value, Point pos) {
	this->pos = pos;
	t.resize(size.y);
	for (size_t y = 0; y < t.size(); y++)
	{
		t[y].resize(size.x, value);
		for (size_t x = 0; x < t[y].size(); x++)
			t[y][x] = value;
	}
}
void Engine::Texture::File(std::string fileName, Point pos) {
	this->pos = pos;
	std::ifstream file(fileName);
	if (!file.is_open())
		return;
	std::string line;

	CellA value;

	for (size_t y = 0; std::getline(file, line); y++)
	{
		// potentially broken if one of the values = 10 ('\n')
		t.resize(t.size() + 1);
		for (size_t x = 0, j = 0; x < line.length(); x++, j++)
		{
			if (j == 9)
				j = 0;
			if (j == 0)
				value.frgba.r = (unsigned char)line[x];
			else if (j == 1)
				value.frgba.g = (unsigned char)line[x];
			else if (j == 2)
				value.frgba.b = (unsigned char)line[x];
			else if (j == 3)
				value.frgba.a = (unsigned char)line[x];
			else if (j == 4)
				value.brgba.r = (unsigned char)line[x];
			else if (j == 5)
				value.brgba.g = (unsigned char)line[x];
			else if (j == 6)
				value.brgba.b = (unsigned char)line[x];
			else if (j == 7)
				value.brgba.a = (unsigned char)line[x];
			else if (j == 8)
			{
				value.character = line[x];
				t[y].push_back(value);
			}
		}
	}
}
void Engine::Texture::Write(std::vector<std::string> stringVector, RGBA frgba, RGBA brgba, Point pos) {
	this->pos = pos;
	t.resize(stringVector.size());
	for (size_t y = 0; y < stringVector.size(); y++)
	{
		t[y].resize(stringVector[y].length());
		for (size_t x = 0; x < stringVector[y].length(); x++)
		{
			t[y][x].character = stringVector[y][x];
			t[y][x].character = stringVector[y][x];
			t[y][x].frgba = frgba;
			t[y][x].brgba = brgba;
		}
	}
}

void Engine::Texture::SetCell(CellA value)
{
	for (unsigned y = 0; y < t.size(); y++)
		for (unsigned x = 0; x < t[y].size(); x++)
			t[y][x] = value;
}
void Engine::Texture::SetForeground(RGBA value)
{
	for (unsigned y = 0; y < t.size(); y++)
		for (unsigned x = 0; x < t[y].size(); x++)
			t[y][x].frgba = value;
}
void Engine::Texture::SetBackground(RGBA value)
{
	for (unsigned y = 0; y < t.size(); y++)
		for (unsigned x = 0; x < t[y].size(); x++)
			t[y][x].brgba = value;
}
void Engine::Texture::SetCharacter(char value)
{
	for (unsigned y = 0; y < t.size(); y++)
		for (unsigned x = 0; x < t[y].size(); x++)
			t[y][x].character = value;
}

Engine::Collider::Collider(UPoint size, Point pos, int type) : size(size), pos(pos), simple(true), type(type) {}
Engine::Collider::Collider(std::vector<std::vector<bool>> collider, Point pos, int type) : c(collider), pos(pos), simple(false), type(type) {}

Engine::Object::Object(Point pos, std::string name) : pos(pos), name(name) {}

Engine::Object::~Object()
{
	if (parentLayer)
		parentLayer->RemoveObject(this);
}