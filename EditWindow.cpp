#include "EditWindow.h"

EditWindow::EditWindow(User::user_info user, bool unlimited_power)
{
	//not really UNLIMITED power, since you won't get to edit people passwords, for obvious reasons
	InitializeObjects(user, unlimited_power);
	PlaceObjects(unlimited_power);
	PaintObjects();
	BindObjects();
}

void EditWindow::InitializeObjects(User::user_info user, bool unlimited_power)
{

}

void EditWindow::PlaceObjects(bool unlimited_power)
{

}

void EditWindow::PaintObjects()
{

}

void EditWindow::BindObjects()
{

}
