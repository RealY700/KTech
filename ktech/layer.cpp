/*
	KTech, Kaup's C++ 2D terminal game engine library.
	Copyright (C) 2023 Ethan Kaufman (AKA Kaup)

	This file is part of KTech.

	KTech is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	any later version.

	KTech is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with KTech. If not, see <https://www.gnu.org/licenses/>.
*/

#include "ktech.hpp"

int KTech::Layer::AddObject(ID<Object>& object)
{
	engine.memory.objects[object]->parentLayer = id;
	objects.push_back(object);
	return objects.size() - 1;
}

bool KTech::Layer::RemoveObject(const std::string& name)
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (engine.memory.objects[objects[i]]->name == name)
		{
			// engine.memory.objects[objects[i]]->parentLayer = ID<Layer>(0, 0);
			objects.erase(objects.begin() + i);
			return true;
		}
	}
	return false;
}

bool KTech::Layer::RemoveObject(ID<Object>& object)
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i] == object)
		{
			// engine.memory.objects[objects[i]]->parentLayer = ID<Layer>(0, 0);
			objects.erase(objects.begin() + i);
			return true;
		}
	}
	return false;
}

// Currently layers must be forced to be added to maps ASAP, otherwise there would be seg faults, for example, whenever objects try to move (since they can't access engine.collision). 
KTech::Layer::Layer(Engine& engine, ID<Map>& map) : engine(engine)
{
	engine.memory.layers.Add(this);
	engine.memory.maps[map]->AddLayer(id);
}

KTech::Layer::~Layer()
{
	// Reset the `parentLayer` of the objects that have it set to this layer
	IO::Log("<Layer[" + name + "]::~Layer()>", RGBColors::red);
	for (ID<Object>& obj : objects)
		if (engine.memory.objects.Exists(obj))
			engine.memory.objects[obj]->parentLayer = ID<Layer>(0, 0);
	engine.memory.maps[parentMap]->RemoveLayer(id);
}